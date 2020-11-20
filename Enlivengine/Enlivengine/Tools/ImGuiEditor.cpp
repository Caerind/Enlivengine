#include <Enlivengine/Tools/ImGuiEditor.hpp>

#if defined(ENLIVE_ENABLE_IMGUI) && defined(ENLIVE_TOOL)

#include <ImGuizmo/ImGuizmo.h>

#include <Enlivengine/Core/Universe.hpp>
#include <Enlivengine/Core/World.hpp>
#include <Enlivengine/Core/Entity.hpp>
#include <Enlivengine/Core/TransformComponent.hpp>

#include <Enlivengine/Meta/ObjectEditor.hpp>

namespace en
{

ImGuiEditor::ImGuiEditor()
	: ImGuiTool()
	, mFramebuffer()
	, mCamera()
	, mEditCamera(false)
	, mGizmoOperation(GizmoOperation::Translate)
{
	mCamera.SetFramebuffer(&mFramebuffer);
}

ImGuiToolTab ImGuiEditor::GetTab() const
{
	return ImGuiToolTab::Engine;
}

const char* ImGuiEditor::GetName() const
{
	return "Editor";
}

const char* ImGuiEditor::GetSaveName() const
{
	return "Editor";
}

int ImGuiEditor::GetWindowFlags() const
{
	return ImGuiWindowFlags_MenuBar;
}

void ImGuiEditor::Display()
{
	if (World* world = Universe::GetInstance().GetCurrentWorld())
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
		ImGuizmo::SetOrthographic(mCamera.GetProjection() == Camera::ProjectionMode::Orthographic);

		ImGuizmo::OPERATION gizmoOperation = ImGuizmo::TRANSLATE;
		switch (mGizmoOperation)
		{
		case GizmoOperation::Translate: gizmoOperation = ImGuizmo::TRANSLATE; break;
		case GizmoOperation::Rotate: gizmoOperation = ImGuizmo::ROTATE; break;
		case GizmoOperation::Scale: gizmoOperation = ImGuizmo::SCALE; break;
		default: enAssert(false); break;
		}

		const auto& selectedEntities = world->GetSelectedEntities();
		for (const auto& enttEntity : selectedEntities)
		{
			Entity entity(*world, enttEntity);
			if (entity.IsValid() && entity.Has<TransformComponent>())
			{
				TransformComponent& transform = entity.Get<TransformComponent>();
				float* mtxData = const_cast<float*>(transform.GetLocalMatrix().GetData());
				ImGuizmo::Manipulate(
					mCamera.GetViewMatrix().GetData(),
					mCamera.GetProjectionMatrix().GetData(),
					gizmoOperation,
					ImGuizmo::LOCAL,
					mtxData
				);
				if (ImGuizmo::IsUsing())
				{
					transform.MarkGlobalMatrixAsDirty();
				}
			}
		}
	}

	if (ImGui::BeginMenuBar())
	{
#ifdef ENLIVE_DEBUG
		ImGui::PushItemWidth(130);
		ObjectEditor::ImGuiEditor(mGizmoOperation, "");
		ImGui::PopItemWidth();
#endif // ENLIVE_DEBUG

		if (ImGui::SmallButton("Camera"))
		{
			mEditCamera = true;
		}

		ImGui::EndMenuBar();
	}

	if (mEditCamera)
	{
		if (ImGui::Begin("Editor - Camera", &mEditCamera))
		{
			ObjectEditor::ImGuiEditor(mCamera, "");
			ImGui::End();
		}
		else
		{
			mEditCamera = false;
		}
	}
	
	ImVec2 windowSize = ImGui::GetWindowSize();
	{
		ImVec2 vMin = ImGui::GetWindowContentRegionMin();
		ImVec2 vMax = ImGui::GetWindowContentRegionMax();
		vMin.x += ImGui::GetWindowPos().x;
		vMin.y += ImGui::GetWindowPos().y;
		vMax.x += ImGui::GetWindowPos().x;
		vMax.y += ImGui::GetWindowPos().y;
		windowSize = ImVec2(vMax.x - vMin.x, vMax.y - vMin.y);
	}
	const Vector2u imWindowSize = Vector2u(static_cast<U32>(windowSize.x), static_cast<U32>(windowSize.y));
	if (imWindowSize != mFramebuffer.GetSize())
	{
		mFramebuffer.Resize(imWindowSize);
	}
	ImGui::Image(mFramebuffer.GetTexture(), windowSize);
}

Framebuffer* ImGuiEditor::GetFramebuffer()
{
	return &GetInstance().mFramebuffer;
}

Camera& ImGuiEditor::GetCamera()
{
	return GetInstance().mCamera;
}

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI && defined(ENLIVE_TOOL)