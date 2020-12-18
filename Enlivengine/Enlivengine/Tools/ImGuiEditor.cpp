#include <Enlivengine/Tools/ImGuiEditor.hpp>

#if defined(ENLIVE_ENABLE_IMGUI) && defined(ENLIVE_TOOL)

#include <ImGuizmo/ImGuizmo.h>

#include <Enlivengine/Core/Universe.hpp>
#include <Enlivengine/Core/World.hpp>
#include <Enlivengine/Core/Entity.hpp>
#include <Enlivengine/Core/TransformComponent.hpp>

#include <Enlivengine/Window/Mouse.hpp>

#include <Enlivengine/Meta/ObjectEditor.hpp>

namespace en
{

ImGuiEditor::ImGuiEditor()
	: ImGuiTool()
	, mFramebuffer()
	, mViewRect()
	, mViewVisible(false)
	, mCamera()
	, mEditCamera(false)
	, mGizmoOperation(GizmoOperation::Translate)
{
	mFramebuffer.Create(Vector2u(840, 600), true);
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
	const ImVec2 vMin = ImGui::GetWindowContentRegionMin();
	const ImVec2 vMax = ImGui::GetWindowContentRegionMax();
	const ImVec2 windowPos = ImGui::GetWindowPos();
	mViewRect.SetMin(Vector2f(vMin.x + windowPos.x, vMin.y + windowPos.y));
	mViewRect.SetMax(Vector2f(vMax.x + windowPos.x, vMax.y + windowPos.y));
	const Vector2f windowSize = mViewRect.GetSize();

	if (World* world = Universe::GetInstance().GetCurrentWorld())
	{
		ImGuizmo::SetRect(mViewRect.GetMin().x, mViewRect.GetMin().y, windowSize.x, windowSize.y);
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
	
	const Vector2u uWindowSize = Vector2u(windowSize);
	if (uWindowSize != mFramebuffer.GetSize())
	{
		mFramebuffer.Resize(uWindowSize);
	}
	ImGui::Image(mFramebuffer.GetTexture(), ImVec2(windowSize.x, windowSize.y));

	mViewVisible = ImGui::IsItemVisible(); // TODO : Not really working...
}

Framebuffer* ImGuiEditor::GetFramebuffer()
{
	return &GetInstance().mFramebuffer;
}

Vector2i ImGuiEditor::GetMouseScreenCoordinates()
{
	return Mouse::GetPositionCurrentWindow() - Vector2i(GetInstance().mViewRect.GetMin());
}

bool ImGuiEditor::IsMouseInView()
{
	return GetInstance().mViewRect.Contains(Vector2f(Mouse::GetPositionCurrentWindow()));
}

bool ImGuiEditor::IsViewVisible()
{
	return GetInstance().IsVisible() && GetInstance().mViewVisible;
}

Camera& ImGuiEditor::GetCamera()
{
	return GetInstance().mCamera;
}

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI && defined(ENLIVE_TOOL)