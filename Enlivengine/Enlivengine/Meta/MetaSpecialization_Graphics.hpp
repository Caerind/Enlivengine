#pragma once

#include <Enlivengine/Utils/String.hpp>
#include <Enlivengine/Meta/TypeInfo.hpp>
#include <Enlivengine/Meta/Meta.hpp>
#include <Enlivengine/Meta/MetaTraits.hpp>
#include <Enlivengine/Meta/DataFile.hpp>
#include <Enlivengine/Meta/ObjectEditor.hpp>

#include <Enlivengine/Graphics/Sprite.hpp>
#include <Enlivengine/Graphics/Camera.hpp>

//////////////////////////////////////////////////////////////////
// en::Sprite
//////////////////////////////////////////////////////////////////
ENLIVE_META_CLASS_BEGIN(en::Sprite)
	ENLIVE_META_CLASS_MEMBER("texture", &en::Sprite::GetTexture, &en::Sprite::SetTexture),
	ENLIVE_META_CLASS_MEMBER("textureRect", &en::Sprite::GetTextureRect, &en::Sprite::SetTextureRect)
ENLIVE_META_CLASS_END()

//////////////////////////////////////////////////////////////////
// en::AABB
//////////////////////////////////////////////////////////////////
ENLIVE_META_CLASS_BEGIN(en::Camera)
ENLIVE_META_CLASS_END()

#ifdef ENLIVE_ENABLE_IMGUI
template <>
struct HasCustomEditor<en::Camera>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(en::Camera& object, const char* name)
	{
		bool modified = false;
		if (ImGui::CollapsingHeader(name))
		{
			ImGui::Indent();

			auto projection = object.GetProjection();
			if (en::ObjectEditor::ImGuiEditor(projection, "Projection"))
			{
				object.SetProjection(projection);
				modified = true;
			}

			en::F32 nearPlane = object.GetNearPlane();
			if (en::ObjectEditor::ImGuiEditor(nearPlane, "NearPlane"))
			{
				object.SetNearPlane(nearPlane);
				modified = true;
			}
			en::F32 farPlane = object.GetFarPlane();
			if (en::ObjectEditor::ImGuiEditor(farPlane, "FarPlane"))
			{
				object.SetFarPlane(farPlane);
				modified = true;
			}

			if (object.GetProjection() == en::Camera::ProjectionMode::Perspective)
			{
				en::F32 fov = object.GetFOV();
				if (en::ObjectEditor::ImGuiEditor(fov, "FOV"))
				{
					object.SetFOV(fov);
					modified = true;
				}
				en::F32 aspect = object.GetAspect();
				if (en::ObjectEditor::ImGuiEditor(aspect, "Aspect"))
				{
					object.SetAspect(aspect);
					modified = true;
				}
			}
			else
			{
				en::F32 left = object.GetLeft();
				if (en::ObjectEditor::ImGuiEditor(left, "Left"))
				{
					object.SetLeft(left);
					modified = true;
				}
				en::F32 right = object.GetRight();
				if (en::ObjectEditor::ImGuiEditor(right, "Right"))
				{
					object.SetRight(right);
					modified = true;
				}
				en::F32 top = object.GetTop();
				if (en::ObjectEditor::ImGuiEditor(top, "Top"))
				{
					object.SetTop(top);
					modified = true;
				}
				en::F32 bottom = object.GetBottom();
				if (en::ObjectEditor::ImGuiEditor(bottom, "Bottom"))
				{
					object.SetBottom(bottom);
					modified = true;
				}
			}

			en::Color clearColor = object.GetClearColor();
			if (en::ObjectEditor::ImGuiEditor(clearColor, "ClearColor"))
			{
				object.SetClearColor(clearColor);
				modified = true;
			}

			en::Rectf viewport = object.GetViewport();
			if (en::ObjectEditor::ImGuiEditor(viewport, "Viewport"))
			{
				object.SetViewport(viewport);
				modified = true;
			}

			// TODO : Framebuffer

			ImGui::Unindent();
		}
		return modified;
	}
};
#endif // ENLIVE_ENABLE_IMGUI
