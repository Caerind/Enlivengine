#pragma once

#include <Enlivengine/Utils/String.hpp>
#include <Enlivengine/Meta/TypeInfo.hpp>
#include <Enlivengine/Meta/Meta.hpp>
#include <Enlivengine/Meta/MetaTraits.hpp>
#include <Enlivengine/Meta/DataFile.hpp>
#include <Enlivengine/Meta/ObjectEditor.hpp>

#include <Enlivengine/Window/Keyboard.hpp>
#include <Enlivengine/Window/Mouse.hpp>
#include <Enlivengine/Window/EventSystem.hpp>

//////////////////////////////////////////////////////////////////
// en::Keyboard::Key
//////////////////////////////////////////////////////////////////
ENLIVE_DEFINE_TYPE_INFO(en::Keyboard::Key)

//////////////////////////////////////////////////////////////////
// en::Mouse::Button
//////////////////////////////////////////////////////////////////
ENLIVE_DEFINE_TYPE_INFO(en::Mouse::Button)

//////////////////////////////////////////////////////////////////
// en::EventSystem::EventButton::Type
//////////////////////////////////////////////////////////////////
ENLIVE_DEFINE_TYPE_INFO(en::EventSystem::EventButton::Type)

//////////////////////////////////////////////////////////////////
// en::EventSystem::EventButton::ActionType
//////////////////////////////////////////////////////////////////
ENLIVE_DEFINE_TYPE_INFO(en::EventSystem::EventButton::ActionType)

//////////////////////////////////////////////////////////////////
// en::EventSystem::EventButton
//////////////////////////////////////////////////////////////////
ENLIVE_META_CLASS_BEGIN(en::EventSystem::EventButton)
	ENLIVE_META_CLASS_MEMBER("name", &en::EventSystem::EventButton::name),
	ENLIVE_META_CLASS_MEMBER("type", &en::EventSystem::EventButton::type),
	ENLIVE_META_CLASS_MEMBER("action", &en::EventSystem::EventButton::action),
	ENLIVE_META_CLASS_MEMBER("buttonIdentifier", &en::EventSystem::EventButton::buttonIdentifier),
	ENLIVE_META_CLASS_MEMBER("extraInfo", &en::EventSystem::EventButton::extraInfo)
ENLIVE_META_CLASS_END()

#ifdef ENLIVE_ENABLE_IMGUI
template <>
struct HasCustomEditor<en::EventSystem::EventButton>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(en::EventSystem::EventButton& object, const char* name)
	{
		bool modified = false;
		if (ImGui::CollapsingHeader(name))
		{
			ImGui::Indent();

			if (en::EventSystem::HasButton(object.hash))
			{
				ImGui::Text("Active : %s", en::EventSystem::IsButtonActive(object.hash) ? ICON_FA_BELL : ICON_FA_BELL_SLASH);
			}

			if (en::ObjectEditor::ImGuiEditor(object.type, "Type"))
			{
				modified = true;
			}
			if (en::ObjectEditor::ImGuiEditor(object.action, "Action"))
			{
				modified = true;
			}

			switch (object.type)
			{
			case en::EventSystem::EventButton::Type::KeyboardKey:
			{
				en::Keyboard::Key key = static_cast<en::Keyboard::Key>(object.buttonIdentifier);
				if (en::ObjectEditor::ImGuiEditor(key, "Key"))
				{
					object.buttonIdentifier = static_cast<en::U32>(key);
					modified = true;
				}

				bool alt = (object.extraInfo & en::Keyboard::Modifier::Alt) > 0;
				bool control = (object.extraInfo & en::Keyboard::Modifier::Control) > 0;
				bool shift = (object.extraInfo & en::Keyboard::Modifier::Shift) > 0;
				bool system = (object.extraInfo & en::Keyboard::Modifier::System) > 0;

				en::ObjectEditor::ImGuiEditor(alt, "Alt");
				ImGui::SameLine(); 
				en::ObjectEditor::ImGuiEditor(control, "Ctrl");

				en::ObjectEditor::ImGuiEditor(shift, "Shift");
				ImGui::SameLine();
				en::ObjectEditor::ImGuiEditor(system, "System");

				object.extraInfo = 0 |
					((alt) ? static_cast<en::U32>(en::Keyboard::Modifier::Alt) : 0) |
					((control) ? static_cast<en::U32>(en::Keyboard::Modifier::Control) : 0) |
					((shift) ? static_cast<en::U32>(en::Keyboard::Modifier::Shift) : 0) |
					((system) ? static_cast<en::U32>(en::Keyboard::Modifier::System) : 0);

			} break;
			case en::EventSystem::EventButton::Type::MouseButton:
			{
				en::Mouse::Button mbutton = static_cast<en::Mouse::Button>(object.buttonIdentifier);
				if (en::ObjectEditor::ImGuiEditor(mbutton, "Button"))
				{
					object.buttonIdentifier = static_cast<en::U32>(mbutton);
					modified = true;
				}
			} break;
			case en::EventSystem::EventButton::Type::JoystickButton:
			{
				if (en::ObjectEditor::ImGuiEditor(object.buttonIdentifier, "Button"))
				{
					modified = true;
				}
				if (en::ObjectEditor::ImGuiEditor(object.extraInfo, "Controller"))
				{
					modified = true;
				}
			} break;
			}

			ImGui::Unindent();
		}
		return modified;
	}
};
#endif // ENLIVE_ENABLE_IMGUI

//////////////////////////////////////////////////////////////////
// en::EventSystem::EventAxis::Type
//////////////////////////////////////////////////////////////////
ENLIVE_DEFINE_TYPE_INFO(en::EventSystem::EventAxis::Type)

//////////////////////////////////////////////////////////////////
// en::EventSystem::EventAxis
//////////////////////////////////////////////////////////////////
ENLIVE_META_CLASS_BEGIN(en::EventSystem::EventAxis)
	ENLIVE_META_CLASS_MEMBER("name", &en::EventSystem::EventAxis::name),
	ENLIVE_META_CLASS_MEMBER("type", &en::EventSystem::EventAxis::type),
	ENLIVE_META_CLASS_MEMBER("axisIdentifier", &en::EventSystem::EventAxis::axisIdentifier),
	ENLIVE_META_CLASS_MEMBER("extraInfo", &en::EventSystem::EventAxis::extraInfo)
ENLIVE_META_CLASS_END()

#ifdef ENLIVE_ENABLE_IMGUI
template <>
struct HasCustomEditor<en::EventSystem::EventAxis>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(en::EventSystem::EventAxis& object, const char* name)
	{
		bool modified = false;
		if (ImGui::CollapsingHeader(name))
		{
			ImGui::Indent();

			if (en::EventSystem::HasAxis(object.hash))
			{
				ImGui::Text("Value : %f", en::EventSystem::GetAxisValue(object.hash));
			}

			if (en::ObjectEditor::ImGuiEditor(object.type, "Type"))
			{
				modified = true;
			}

			if (object.type == en::EventSystem::EventAxis::Type::JoystickAxis
				|| object.type == en::EventSystem::EventAxis::Type::JoystickBallX
				|| object.type == en::EventSystem::EventAxis::Type::JoystickBallY)
			{
				if (en::ObjectEditor::ImGuiEditor(object.axisIdentifier, "Axis"))
				{
					modified = true;
				}

				if (en::ObjectEditor::ImGuiEditor(object.extraInfo, "Controller"))
				{
					modified = true;
				}
			}

			ImGui::Unindent();
		}
		return modified;
	}
};
#endif // ENLIVE_ENABLE_IMGUI