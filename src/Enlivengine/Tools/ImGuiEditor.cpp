#include <Enlivengine/Tools/ImGuiEditor.hpp>

#if defined(ENLIVE_ENABLE_IMGUI) && defined(ENLIVE_TOOL)

#include <Enlivengine/Core/Universe.hpp>
#include <Enlivengine/Core/World.hpp>
#include <Enlivengine/Core/Entity.hpp>
#include <Enlivengine/Core/TransformComponent.hpp>

#include <Enlivengine/Math/Math.hpp>
#include <Enlivengine/Window/Mouse.hpp>
#include <Enlivengine/Window/Keyboard.hpp>

#include <Enlivengine/Tools/ImGuiObjectEditor.hpp>
#include <Enlivengine/Engine/WorldFileManager.hpp>

namespace en
{

ImGuiEditor::ImGuiEditor()
	: ImGuiTool()
	, mRenderTarget()
	, mViewRect()
	, mViewVisible(false)
	, mView()
	, mUseMainView(true)
	, mEditConfig(false)
	, mShowManipulator(true)
	, mShowDebug(true)
	, mGizmoOperation(GizmoOperation::Translate)
	, mStatus(GameStatus::Stopped)
{
	mRenderTarget.Create({ 840, 600 });
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
	ImGuiObjectEditor objectEditor;
	World* world = Universe::GetCurrentWorld();

	const ImVec2 vMin = ImGui::GetWindowContentRegionMin();
	const ImVec2 vMax = ImGui::GetWindowContentRegionMax();
	const ImVec2 windowPos = ImGui::GetWindowPos();
	mViewRect.SetMin(Vector2f(vMin.x + windowPos.x, vMin.y + windowPos.y));
	mViewRect.SetMax(Vector2f(vMax.x + windowPos.x, vMax.y + windowPos.y));
	const Vector2f windowSize = mViewRect.GetSize();

    // TODO : ImGuizmo
    /*
	if (IsUsingEditorView())
	{
		ImGuizmo::SetRect(mViewRect.GetMin().x, mViewRect.GetMin().y, windowSize.x, windowSize.y);
		ImGuizmo::SetOrthographic(true);

		if (world != nullptr)
		{
			ImGuizmo::OPERATION gizmoOperation = ImGuizmo::TRANSLATE;
			switch (mGizmoOperation)
			{
			case GizmoOperation::Translate: gizmoOperation = ImGuizmo::TRANSLATE; break;
			case GizmoOperation::Rotate: gizmoOperation = ImGuizmo::ROTATE; break;
			case GizmoOperation::Scale: gizmoOperation = ImGuizmo::SCALE; break;
			default: enAssert(false); break;
			}

			ImGuizmo::MODE gizmoMode = ImGuizmo::LOCAL;
			switch (mGizmoMode)
			{
			case GizmoMode::Local: gizmoMode = ImGuizmo::LOCAL; break;
			case GizmoMode::Global: gizmoMode = ImGuizmo::WORLD; break;
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
						gizmoMode,
						mtxData
					);

					Matrix4f parentMtx = Matrix4f::Identity();
					if (transform.HasParent())
					{
						Entity parentEntity = transform.GetParent().Get();
						if (parentEntity.IsValid())
						{
							enAssert(parentEntity.Has<TransformComponent>());
							parentMtx = parentEntity.Get<TransformComponent>().GetGlobalMatrix();
						}
					}
          
					// TODO : DebugDraw Transform
					//world->GetDebugDraw().DrawTransform(parentMtx);
					if (ImGuizmo::IsUsing())
					{
						const Matrix4f result = Matrix4f::Identity().Set(mtxData) * parentMtx.Inversed();
						transform.SetTransform(result.GetTranslation(), result.GetRotation(), result.GetScale());
					}
				}
			}
        }
    }
    */

	if (ImGui::BeginMenuBar())
	{
		if (world != nullptr)
		{
			// If was OneFrame, now the frame is done so pause
			if (mStatus == GameStatus::OneFrame)
			{
				mStatus = GameStatus::Paused;
			}

			if (IsPlaying())
			{
				if (ImGui::SmallButton(ICON_FA_PAUSE))
				{
					mStatus = GameStatus::Paused;
				}
				if (ImGui::SmallButton(ICON_FA_STOP))
				{
					StopGame();
					world = Universe::GetCurrentWorld();
				}
			}
			else if (IsPaused())
			{
				if (ImGui::SmallButton(ICON_FA_PLAY))
				{
					mStatus = GameStatus::Playing;
				}
				if (ImGui::SmallButton(ICON_FA_STOP))
				{
					StopGame();
					world = Universe::GetCurrentWorld();
				}
				if (ImGui::SmallButton("+1Frame"))
				{
					mStatus = GameStatus::OneFrame;
				}
			}
			else if (IsStopped())
			{
				if (ImGui::SmallButton(ICON_FA_PLAY))
				{
					if (StartGame())
					{
						world = Universe::GetCurrentWorld();
					}
					else
					{
						enAssert(false);
					}
				}
			}

			// Button to switch from editor cam / game cam in during test
			if (mStatus != GameStatus::Stopped)
			{
				if (mUseMainView)
				{
					if (ImGui::SmallButton("EditorCam"))
					{
						mUseMainView = false;
					}
				}
				else
				{
					if (ImGui::SmallButton("GameCam"))
					{
						mUseMainView = true;
					}
				}

			}

			if (!IsUsingEditorView())
			{
				GenericEdit(objectEditor, "Debug", mShowDebug);
			}
		}

		if (IsUsingEditorView())
		{
			if (ImGui::SmallButton("Config"))
			{
				mEditConfig = !mEditConfig;
			}
			
			ImGui::PushItemWidth(100);
			GenericEdit(objectEditor, "##GizmoOperation", mGizmoOperation);
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("Gizmo operation");
			}
			ImGui::PopItemWidth();

			ImGui::PushItemWidth(80);
			GenericEdit(objectEditor, "##GizmoMode", mGizmoMode);
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("Gizmo mode");
			}
			ImGui::PopItemWidth();
		}

		ImGui::EndMenuBar();
	}

	if (IsUsingEditorView())
	{
		if (mEditConfig)
		{
			if (ImGui::Begin("Editor - Config", &mEditConfig))
			{
				GenericEdit(objectEditor, "View settings", mView);
				GenericEdit(objectEditor, "Show Manipulator", mShowManipulator);
				GenericEdit(objectEditor, "Show Debug", mShowDebug);
				ImGui::End();
			}
			else
			{
				mEditConfig = false;
			}
		}

		// ViewManipulator
		if (world != nullptr)
		{
			if (mShowManipulator)
			{
				// TODO
				/*
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
				*/
			}

			UpdateView();
		}
	}
	
	if (windowSize.x > 0 && windowSize.y > 0)
	{
		const Vector2u uWindowSize = Vector2u(windowSize);
		if (uWindowSize != mRenderTarget.GetSize())
		{
			mRenderTarget.Resize(uWindowSize);
		}

		// TODO
		//ImGui::Image(mFramebuffer.GetTexture(), ImVec2(windowSize.x, windowSize.y));

		mViewVisible = ImGui::IsItemVisible(); // TODO : Not really working...
	}
	else
	{
		mViewVisible = false;
	}

	if (mStatus != GameStatus::Stopped && mUseMainView && world == nullptr)
	{
		mUseMainView = false;
	}
}

void ImGuiEditor::UpdateView()
{
	// TODO : UpdateView mvt
	/*
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
		const F32 dtSeconds = Time::GetDeltaTime().AsSeconds();

		Vector3f direction = mCamera.GetRotation().GetForward();

		// Movement
		if (forward != 0.0f || left != 0.0f)
		{
			Vector3f mvtUnit = direction;
			mvtUnit.y = 0.0f;
			mvtUnit.Normalize();

			Vector3f movement;
			movement += 3.0f * forward * mvtUnit * dtSeconds;
			movement -= 3.0f * left * mvtUnit.CrossProduct(ENLIVE_DEFAULT_UP) * dtSeconds;
			mView.Move(movement);
		}

		// Rotation
		if (deltaYaw != 0.0f || deltaPitch != 0.0f)
		{
			if (!Math::Equals(deltaYaw, 0.0f))
			{
				mView.Rotate(Matrix3f::RotationY(100.0f * dtSeconds * deltaYaw));
			}
			if (!Math::Equals(deltaPitch, 0.0f))
			{
				mView.Rotate(Quaternionf(100.0f * dtSeconds * deltaPitch, direction.CrossProduct(ENLIVE_DEFAULT_UP)));
			}
		}
	}
	else
	{
		Mouse::SetRelativeMode(false);
	}
	*/
}

RenderTarget& ImGuiEditor::GetRenderTarget()
{
	return GetInstance().mRenderTarget;
}

Vector2i ImGuiEditor::GetMouseScreenCoordinates()
{
	return Mouse::GetPosition() - Vector2i(GetInstance().mViewRect.GetMin());
}

bool ImGuiEditor::IsMouseInView()
{
	return IsViewVisible() && GetInstance().mViewRect.Contains(Vector2f(Mouse::GetPosition()));
}

bool ImGuiEditor::IsViewVisible()
{
	return GetInstance().IsVisible() && GetInstance().mViewVisible;
}

bool ImGuiEditor::IsShowingManipulator()
{
	return GetInstance().mShowManipulator;
}

bool ImGuiEditor::IsShowingDebug()
{
	return GetInstance().mShowDebug;
}

bool ImGuiEditor::IsUsingEditorView()
{
	auto& instance = GetInstance();
	if (instance.mStatus != GameStatus::Stopped && instance.mUseMainView)
	{
		return false;
	}
	else
	{
		return true;
	}
}

View* ImGuiEditor::GetView()
{
	auto& instance = GetInstance();
	if (instance.mStatus != GameStatus::Stopped && instance.mUseMainView)
	{
		return nullptr;
	}
	else
	{
		return &instance.mView;
	}
}

bool ImGuiEditor::IsPlaying()
{
	auto& instance = GetInstance();
	return instance.mStatus == GameStatus::Playing || instance.mStatus == GameStatus::OneFrame;
}

bool ImGuiEditor::IsPaused()
{
	return GetInstance().mStatus == GameStatus::Paused;
}

bool ImGuiEditor::IsStopped()
{
	return GetInstance().mStatus == GameStatus::Stopped;
}

bool ImGuiEditor::StartGame()
{
	enAssert(IsStopped());

	if (World* world = Universe::GetCurrentWorld())
	{
		const std::string worldName = world->GetName();

		WorldFileManager::UnloadCurrentWorld();

		// Reload the same world
		if (WorldFileManager::LoadWorld(worldName))
		{
			GetInstance().mStatus = GameStatus::Playing;
			return true;
		}
		else
		{
			enAssert(false); // Oups...
			enLogFatal(LogChannel::Core, "Can't load world {}", worldName.c_str());
		}
	}
	else
	{
		enLogWarning(LogChannel::Core, "No loaded world, can't start the game");
	}

	return false;
}

bool ImGuiEditor::StopGame()
{
	enAssert(!IsStopped());

	World* world = Universe::GetCurrentWorld();
	enAssert(world != nullptr);

	const std::string worldName = world->GetName();

	if (WorldFileManager::LoadWorld(worldName))
	{
		GetInstance().mStatus = GameStatus::Stopped;
		return true;
	}
	else
	{
		enAssert(false); // Oups...
		enLogFatal(LogChannel::Core, "Can't load world {}", worldName.c_str());
	}

	return false;
}

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI && defined(ENLIVE_TOOL)