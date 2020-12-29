#include <Enlivengine/Tools/ImGuiEditor.hpp>

#if defined(ENLIVE_ENABLE_IMGUI) && defined(ENLIVE_TOOL)

#include <ImGuizmo/ImGuizmo.h>

#include <Enlivengine/Core/Engine.hpp>
#include <Enlivengine/Core/World.hpp>
#include <Enlivengine/Core/Entity.hpp>
#include <Enlivengine/Core/TransformComponent.hpp>

#include <Enlivengine/Math/Math.hpp>
#include <Enlivengine/Window/Mouse.hpp>
#include <Enlivengine/Window/Keyboard.hpp>

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
	mCamera.InitializePerspective(80.0f);
	mCamera.InitializeView(Vector3f(0.0f, 0.8f, 0.0f), Matrix3f::Identity());

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

	if (World* world = Engine::GetCurrentWorld())
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
		if (selectedEntities.size() == 1)
		{
			Entity entity(*world, selectedEntities[0]);
			if (entity.IsValid() && entity.Has<TransformComponent>())
			{
				TransformComponent& transform = entity.Get<TransformComponent>();
				float mtxData[16];
				std::memcpy(mtxData, transform.GetGlobalMatrix().GetData(), sizeof(float) * 16);
				ImGuizmo::Manipulate(
					mCamera.GetViewMatrix().GetData(),
					mCamera.GetProjectionMatrix().GetData(),
					gizmoOperation,
					ImGuizmo::LOCAL,
					mtxData
				);

				Matrix4f parentMtx = transform.HasParent() ? transform.GetParent().Get<TransformComponent>().GetGlobalMatrix() : Matrix4f::Identity();
				world->GetDebugDraw().DrawTransform(parentMtx);
				if (ImGuizmo::IsUsing())
				{
					const Matrix4f result = Matrix4f::Identity().Set(mtxData) * parentMtx.Inversed();
					transform.SetTransform(result.GetTranslation(), result.GetRotation(), result.GetScale());
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
	if (uWindowSize != mFramebuffer.GetSize() && uWindowSize.x > 0 && uWindowSize.y > 0)
	{
		mFramebuffer.Resize(uWindowSize);
	}
	if (uWindowSize.x > 0 && uWindowSize.y > 0)
	{
		ImGui::Image(mFramebuffer.GetTexture(), ImVec2(windowSize.x, windowSize.y));

		mViewVisible = ImGui::IsItemVisible(); // TODO : Not really working...
	}
	else
	{
		mViewVisible = false;
	}

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

void ImGuiEditor::UpdateCamera(Time dt)
{
	ImGuiEditor& editor = GetInstance();

	if (Keyboard::IsAltHold())
	{
		Mouse::SetRelativeMode(true);

		F32 forward = 0.0f;
		F32 left = 0.0;
		if (Keyboard::IsHold(Keyboard::Key::W)) forward += 1.0f;
		if (Keyboard::IsHold(Keyboard::Key::S)) forward -= 1.0f;
		if (Keyboard::IsHold(Keyboard::Key::A)) left += 1.0f;
		if (Keyboard::IsHold(Keyboard::Key::D)) left -= 1.0f;
		const F32 deltaYaw = Mouse::GetMouseMovement().x * 0.25f;
		const F32 deltaPitch = Mouse::GetMouseMovement().y * 0.15f;
		const F32 dtSeconds = dt.AsSeconds();

		Vector3f direction = editor.mCamera.GetRotation().GetForward();

		// Movement
		if (forward != 0.0f || left != 0.0f)
		{
			Vector3f mvtUnit = direction;
			mvtUnit.y = 0.0f;
			mvtUnit.Normalize();

			Vector3f movement;
			movement += 3.0f * forward * mvtUnit * dtSeconds;
			movement -= 3.0f * left * mvtUnit.CrossProduct(ENLIVE_DEFAULT_UP) * dtSeconds;
			editor.mCamera.Move(movement);
		}

		// Rotation
		if (deltaYaw != 0.0f || deltaPitch != 0.0f)
		{
			if (!Math::Equals(deltaYaw, 0.0f))
			{
				editor.mCamera.Rotate(Matrix3f::RotationY(100.0f * dtSeconds * deltaYaw));
			}
			if (!Math::Equals(deltaPitch, 0.0f))
			{
				//editor.mCamera.Rotate(Quaternionf(100.0f * dtSeconds * deltaPitch, direction.CrossProduct(ENLIVE_DEFAULT_UP)));
			}
		}
	}
	else
	{
		Mouse::SetRelativeMode(false);
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
	return IsViewVisible() && GetInstance().mViewRect.Contains(Vector2f(Mouse::GetPositionCurrentWindow()));
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