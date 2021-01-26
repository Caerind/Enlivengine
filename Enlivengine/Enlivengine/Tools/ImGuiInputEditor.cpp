#include <Enlivengine/Tools/ImGuiInputEditor.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <Enlivengine/Window/EventSystem.hpp>
#include <Enlivengine/Window/Controller.hpp>
#include <Enlivengine/Resources/PathManager.hpp>
#include <Enlivengine/Utils/XmlSerializer.hpp>
#include <Enlivengine/Tools/ImGuiObjectEditor.hpp>

namespace en
{

ImGuiInputEditor::ImGuiInputEditor()
	: ImGuiTool()
{
}

ImGuiToolTab ImGuiInputEditor::GetTab() const
{
	return ImGuiToolTab::Main;
}

const char* ImGuiInputEditor::GetName() const
{
	return ICON_FA_GAMEPAD " InputEditor";
}

const char* ImGuiInputEditor::GetSaveName() const
{
	return "InputEditor";
}

void ImGuiInputEditor::Initialize()
{
	LoadInputsFromFile();
}

void ImGuiInputEditor::Display()
{
	bool modified = false;

	if (ImGui::CollapsingHeader("Buttons"))
	{
		ImGui::Indent();
		if (Buttons())
		{
			modified = true;
		}
		ImGui::Unindent();
	}
	if (ImGui::CollapsingHeader("Axes"))
	{
		ImGui::Indent();
		if (Axes())
		{
			modified = true;
		}
		ImGui::Unindent();
	}

	if (modified)
	{
		SaveInputsToFile();
	}
}

bool ImGuiInputEditor::Buttons()
{
	bool modified = false;

	static constexpr U32 kBufferSize{ 256 };
	static char newButtonName[kBufferSize] = "";
	static EventSystem::EventButton newButton;

	ImGui::InputText("Name##NewButton", newButtonName, kBufferSize);
	
	ImGuiObjectEditor objectEditor;
	GenericEdit(objectEditor, "New button", newButton);

	// Capturing system
	{
		static bool capturing = false;
		static bool capturingFrameOne = false;
		if (capturing && !capturingFrameOne)
		{
			ImGui::DisabledButton("Capturing...");

			const EventSystem::EventButton& buttonCapture = EventSystem::GetLastButton();
			switch (buttonCapture.type)
			{
				case EventSystem::EventButton::Type::KeyboardKey:
				{
					if (Keyboard::IsPressed(static_cast<Keyboard::Key>(buttonCapture.buttonIdentifier)) && Keyboard::AreModifiersHold(buttonCapture.extraInfo))
					{
						newButton = buttonCapture;
						capturing = false;
					}
				} break;
				case EventSystem::EventButton::Type::MouseButton:
				{
					if (Mouse::IsPressed(static_cast<Mouse::Button>(buttonCapture.buttonIdentifier)))
					{
						newButton = buttonCapture;
						capturing = false;
					}
				} break;
				case EventSystem::EventButton::Type::JoystickButton:
				{
					if (Controller::IsButtonPressed(buttonCapture.extraInfo, buttonCapture.buttonIdentifier))
					{
						newButton = buttonCapture;
						capturing = false;
					}
				} break;
			}
		}
		else
		{
			if (ImGui::Button("Capture"))
			{
				capturing = true;
				capturingFrameOne = true;
			}
		}
		if (capturingFrameOne)
		{
			capturingFrameOne = false;
		}
	}

	bool validNewInput = true;

	const U32 nameLength = static_cast<U32>(strlen(newButtonName));
	if (nameLength <= 0 || nameLength > kBufferSize)
	{
		validNewInput = false;
	}

	if (validNewInput)
	{
		if (ImGui::Button("Create##NewButton"))
		{
			EventSystem::AddButton(newButtonName, newButton.type, newButton.buttonIdentifier, newButton.extraInfo, newButton.action);

			modified = true;

#ifdef ENLIVE_COMPILER_MSVC
			strcpy_s(newButtonName, "");
#else
			strcpy(newButtonName, "");
#endif // ENLIVE_COMPILER_MSVC
			newButton = EventSystem::EventButton();
		}
	}
	else
	{
		ImGui::DisabledButton("Create##NewButton");
	}

	ImGui::Separator();

	for (U32 i = 0; i < EventSystem::GetButtonCount(); )
	{
		EventSystem::EventButton& button = const_cast<EventSystem::EventButton&>(EventSystem::GetButton(i));

		ImGui::PushID(i);
		bool queryRemoval = false;
		if (ImGui::Button("-"))
		{
			queryRemoval = true;
		}
		ImGui::SameLine();

		if (GenericEdit(objectEditor, button.name.c_str(), button))
		{
			modified = true;
		}

		if (queryRemoval)
		{
			EventSystem::RemoveButtonAtIndex(i);
			modified = true;
		}
		else
		{
			++i;
		}
		ImGui::PopID();
	}

	return modified;
}

bool ImGuiInputEditor::Axes()
{
	bool modified = false;

	static constexpr U32 kBufferSize{ 256 };
	static char newAxisName[kBufferSize] = "";
	static EventSystem::EventAxis newAxis;

	ImGui::InputText("Name##NewAxis", newAxisName, kBufferSize);

	ImGuiObjectEditor objectEditor;
	GenericEdit(objectEditor, "New axis", newAxis);

	bool validNewInput = true;

	const U32 nameLength = static_cast<U32>(strlen(newAxisName));
	if (nameLength <= 0 || nameLength >= 255)
	{
		validNewInput = false;
	}

	if (validNewInput)
	{
		if (ImGui::Button("Create##NewAxis"))
		{
			EventSystem::AddAxis(newAxisName, newAxis.type, newAxis.axisIdentifier, newAxis.extraInfo);

			modified = true;

#ifdef ENLIVE_COMPILER_MSVC
			strcpy_s(newAxisName, "");
#else
			strcpy(newAxisName, "");
#endif // ENLIVE_COMPILER_MSVC
			newAxis = EventSystem::EventAxis();
		}
	}
	else
	{
		ImGui::DisabledButton("Create##NewAxis");
	}

	ImGui::Separator();

	for (U32 i = 0; i < EventSystem::GetAxisCount(); )
	{
		EventSystem::EventAxis& axis = const_cast<EventSystem::EventAxis&>(EventSystem::GetAxis(i));

		ImGui::PushID(i);
		bool queryRemoval = false;
		if (ImGui::Button("-"))
		{
			queryRemoval = true;
		}
		ImGui::SameLine();

		if (GenericEdit(objectEditor, axis.name.c_str(), axis))
		{
			modified = true;
		}

		if (queryRemoval)
		{
			EventSystem::RemoveAxisAtIndex(i);
			modified = true;
		}
		else
		{
			++i;
		}
		ImGui::PopID();
	}

	return modified;
}

bool ImGuiInputEditor::LoadInputsFromFile()
{
	const std::filesystem::path path = std::string(PathManager::GetAssetsPath() + "inputs.data");
	if (std::filesystem::exists(path))
	{
		XmlSerializer xml;
		if (!xml.Open(path.string(), Serializer::Mode::Read))
		{
			return false;
		}

		std::vector<EventSystem::EventButton> buttons;
		if (GenericSerialization(xml, "Buttons", buttons))
		{
			for (const EventSystem::EventButton& button : buttons)
			{
				EventSystem::AddButton(button.name.c_str(), button.type, button.buttonIdentifier, button.extraInfo, button.action);
			}
		}

		std::vector<EventSystem::EventAxis> axes;
		if (GenericSerialization(xml, "Axes", axes))
		{
			for (const EventSystem::EventAxis& axis : axes)
			{
				EventSystem::AddAxis(axis.name.c_str(), axis.type, axis.axisIdentifier, axis.extraInfo);
			}
		}

		return true;
	}
	else
	{
		return SaveInputsToFile();
	}
}

bool ImGuiInputEditor::SaveInputsToFile()
{
	const std::filesystem::path path = std::string(PathManager::GetAssetsPath() + "inputs.data");

	XmlSerializer xml;
	if (xml.Open(path.string(), Serializer::Mode::Write))
	{
		GenericSerialization(xml, "Buttons", EventSystem::GetButtons());
		GenericSerialization(xml, "Axes", EventSystem::GetAxes());
		return xml.Close();
	}
	else
	{
		enLogError(LogChannel::Tools, "Can't save Worlds");
		return false;
	}
}

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
