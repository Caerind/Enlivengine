#include <Enlivengine/Tools/ImGuiEditor.hpp>

#if defined(ENLIVE_ENABLE_IMGUI) && defined(ENLIVE_TOOL)

#include <ImGuizmo/ImGuizmo.h>

#include <Enlivengine/Core/Universe.hpp>
#include <Enlivengine/Core/World.hpp>
#include <Enlivengine/Core/Entity.hpp>
#include <Enlivengine/Core/TransformComponent.hpp>

#include <Enlivengine/Math/Math.hpp>
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
	, mEditConfig(false)
	, mShowManipulator(true)
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

	ImGuizmo::SetRect(mViewRect.GetMin().x, mViewRect.GetMin().y, windowSize.x, windowSize.y);
	ImGuizmo::SetOrthographic(mCamera.GetProjection() == Camera::ProjectionMode::Orthographic);

	if (World* world = Universe::GetInstance().GetCurrentWorld())
	{

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
		if (ImGui::SmallButton("Config"))
		{
			mEditConfig = true;
		}

#ifdef ENLIVE_DEBUG
		ImGui::PushItemWidth(130);
		ObjectEditor::ImGuiEditor(mGizmoOperation, "");
		ImGui::PopItemWidth();
#endif // ENLIVE_DEBUG

		ImGui::EndMenuBar();
	}

	if (mEditConfig)
	{
		if (ImGui::Begin("Editor - Config", &mEditConfig))
		{
			ObjectEditor::ImGuiEditor(mCamera, "Camera Settings");
			ImGui::Checkbox("Show Manipulator", &mShowManipulator);
			ImGui::End();
		}
		else
		{
			mEditConfig = false;
		}
	}
	
	const Vector2u uWindowSize = Vector2u(windowSize);
	if (uWindowSize != mFramebuffer.GetSize())
	{
		mFramebuffer.Resize(uWindowSize);
	}
	ImGui::Image(mFramebuffer.GetTexture(), ImVec2(windowSize.x, windowSize.y));

	mViewVisible = ImGui::IsItemVisible(); // TODO : Not really working...

	// ViewManipulator
	if (mShowManipulator)
	{
		float manipulatorSize = 100;
		float viewMtx[16];
		float viewMtxR[16];
		std::memcpy(viewMtx, mCamera.GetViewMatrix().GetData(), sizeof(float) * 16);
		std::memcpy(viewMtxR, mCamera.GetViewMatrix().GetData(), sizeof(float) * 16);
		ImGuizmo::ViewManipulate(viewMtx, 8.0f, ImVec2(mViewRect.GetMin().x + mViewRect.GetSize().x - manipulatorSize, mViewRect.GetMin().y), ImVec2(manipulatorSize, manipulatorSize), 0x10101010);
		if (std::memcmp(viewMtx, viewMtxR, sizeof(float) * 16) != 0)
		{
			const Matrix4f iMtx = Matrix4f::Identity().Set(viewMtx).Inversed();
			const Matrix3f rot = Matrix3f(-1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f) * iMtx.GetRotation(); // Don't exactly know why, but it seems we need to negate X too...

			mCamera.SetPosition(iMtx.GetTranslation());
			mCamera.SetRotation(rot);
		}
	}
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