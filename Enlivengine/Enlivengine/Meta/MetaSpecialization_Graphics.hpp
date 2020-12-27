#pragma once

#include <Enlivengine/Utils/String.hpp>
#include <Enlivengine/Meta/TypeInfo.hpp>
#include <Enlivengine/Meta/Meta.hpp>
#include <Enlivengine/Meta/MetaTraits.hpp>
#include <Enlivengine/Meta/DataFile.hpp>
#include <Enlivengine/Meta/ObjectEditor.hpp>

#include <Enlivengine/Graphics/Sprite.hpp>
#include <Enlivengine/Graphics/Tilemap.hpp>
#include <Enlivengine/Graphics/Camera.hpp>

//////////////////////////////////////////////////////////////////
// en::Sprite
//////////////////////////////////////////////////////////////////
ENLIVE_META_CLASS_BEGIN(en::Sprite)
	ENLIVE_META_CLASS_MEMBER("texture", &en::Sprite::GetTexture, &en::Sprite::SetTexture),
	ENLIVE_META_CLASS_MEMBER("textureRect", &en::Sprite::GetTextureRect, &en::Sprite::SetTextureRect)
ENLIVE_META_CLASS_END()

//////////////////////////////////////////////////////////////////
// en::Tilemap
//////////////////////////////////////////////////////////////////
ENLIVE_META_CLASS_BEGIN(en::Tilemap)
	ENLIVE_META_CLASS_MEMBER("size", &en::Tilemap::GetSize, &en::Tilemap::SetSize)
ENLIVE_META_CLASS_END()

//////////////////////////////////////////////////////////////////
// en::Camera
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
			}
			else
			{
				en::F32 size = object.GetSize();
				if (en::ObjectEditor::ImGuiEditor(size, "Size"))
				{
					object.SetSize(size);
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
