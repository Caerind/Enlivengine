#include <Enlivengine/Tools/ImGuiInputEditor.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <imgui/imgui.h>

#include <Enlivengine/Window/EventSystem.hpp>

#include <Enlivengine/Meta/MetaSpecialization.hpp>
#include <Enlivengine/Meta/ObjectEditor.hpp>

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

void ImGuiInputEditor::Display()
{
	if (ImGui::CollapsingHeader("Buttons"))
	{
		ImGui::Indent();
		Buttons();
		ImGui::Unindent();
	}
	if (ImGui::CollapsingHeader("Axes"))
	{
		ImGui::Indent();
		Axes();
		ImGui::Unindent();
	}
}

void ImGuiInputEditor::Buttons()
{
	static constexpr U32 kBufferSize{ 256 };
	static char newButtonName[kBufferSize] = "";
	static EventSystem::EventButton newButton;

	ImGui::InputText("Name##NewButton", newButtonName, kBufferSize);
	ObjectEditor::ImGuiEditor(newButton, "New button");

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
	if (nameLength <= 0 || nameLength >= 255)
	{
		validNewInput = false;
	}

	if (validNewInput)
	{
		if (ImGui::Button("Create##NewButton"))
		{
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

	const U32 eventButtonCount = EventSystem::GetButtonCount();
	for (U32 i = 0; i < eventButtonCount; ++i)
	{
		EventSystem::EventButton& button = const_cast<EventSystem::EventButton&>(EventSystem::GetButton(i));

		ObjectEditor::ImGuiEditor(button, button.GetName().c_str());
	}
}

void ImGuiInputEditor::Axes()
{
	static constexpr U32 kBufferSize{ 256 };
	static char newAxisName[kBufferSize] = "";
	static EventSystem::EventAxis newAxis;

	ImGui::InputText("Name##NewAxis", newAxisName, kBufferSize);
	ObjectEditor::ImGuiEditor(newAxis, "New axis");

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

	const U32 eventAxisCount = EventSystem::GetAxisCount();
	for (U32 i = 0; i < eventAxisCount; ++i)
	{
		EventSystem::EventAxis& axis = const_cast<EventSystem::EventAxis&>(EventSystem::GetAxis(i));

		ObjectEditor::ImGuiEditor(axis, axis.GetName().c_str());
	}
}

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
