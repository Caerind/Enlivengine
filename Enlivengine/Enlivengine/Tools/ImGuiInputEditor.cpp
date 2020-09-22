#include <Enlivengine/Tools/ImGuiInputEditor.hpp>

#if defined(ENLIVE_ENABLE_IMGUI)

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <Enlivengine/Tools/ImGuiHelper.hpp>
#include <Enlivengine/System/Assert.hpp>
#include <Enlivengine/Application/Application.hpp>
#include <Enlivengine/Application/ActionSystem.hpp>

namespace en
{

ImGuiInputEditor::ImGuiInputEditor()
	: ImGuiTool()
{
}

ImGuiInputEditor::~ImGuiInputEditor()
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

void ImGuiInputEditor::Display()
{
	static constexpr U32 kBufferSize{ 256 };

	static const char* actionInputTypeNames[] = { 
		"Variable", 
		"Function", 
		"Event", 
		"Key",
		"Mouse",
		"JoystickConnect",
		"JoystickButton",
		"JoystickAxis",
		"And",
		"Or",
		"Not"
	};
	static_assert(IM_ARRAYSIZE(actionInputTypeNames) == static_cast<int>(ActionInputType::Count));

	static const char* actionTypeNames[] = {
		"Hold",
		"Pressed",
		"Released"
	};
	static_assert(IM_ARRAYSIZE(actionTypeNames) == static_cast<int>(ActionType::Count));
	
	static const char* actionInputLogicalOperatorNames[] = {
		"And",
		"Or",
		"Not"
	};
	static_assert(IM_ARRAYSIZE(actionInputLogicalOperatorNames) == static_cast<int>(ActionInputLogicalOperator::Count));

	static const char* mouseButtonNames[] = {
		"Left",
		"Right",
		"Middle",
		"XButton1",
		"XButton2"
	};
	static_assert(IM_ARRAYSIZE(mouseButtonNames) == static_cast<int>(sf::Mouse::Button::ButtonCount));

	static bool keysSet = false;
	static std::vector<const char*> keyNames;
	if (!keysSet)
	{
		const U32 keyCount = static_cast<U32>(sf::Keyboard::Key::KeyCount);
		keyNames.reserve(keyCount);
		for (U32 i = 0; i < keyCount; ++i)
		{
			keyNames.push_back(sfKeyToString(static_cast<sf::Keyboard::Key>(i)));
		}
		keysSet = true;
	}
	enAssert(keyNames.size() == static_cast<std::size_t>(sf::Keyboard::Key::KeyCount));

	ActionSystem& actionSystem = Application::GetInstance().GetActionSystem();

	// New action input
	{
		static char newActionInputName[kBufferSize];
		static int newActionInputType = 0;
		static int newActionInputActionType = 0;
		static int newActionInputKey = 0;
		static int newActionInputButton = 0;
		static int newActionInputAIndex = 0;
		static int newActionInputBIndex = 0;

		ImGui::InputText("Name##NewActionInput", newActionInputName, kBufferSize);
		ImGui::Combo("Type##NewActionInput", &newActionInputType, actionInputTypeNames, IM_ARRAYSIZE(actionInputTypeNames));

		const ActionInputType actionInputType = static_cast<ActionInputType>(newActionInputType);
		switch (actionInputType)
		{
		case ActionInputType::Variable: ImGui::Text("Not configurable yet"); /* TODO_Someday */ break;
		case ActionInputType::Function: ImGui::Text("Not configurable yet"); /* TODO_Someday */ break;
		case ActionInputType::Event: ImGui::Text("Not configurable yet"); /* TODO_Someday */ break;
		case ActionInputType::Key: 
		{
			ImGui::Combo("ActionType##NewActionInput", &newActionInputActionType, actionTypeNames, IM_ARRAYSIZE(actionTypeNames));
			ImGui::Combo("Key##NewActionInput", &newActionInputKey, keyNames.data(), static_cast<int>(keyNames.size()));
		} break;
		case ActionInputType::Mouse: 
		{
			ImGui::Combo("ActionType##NewActionInput", &newActionInputActionType, actionTypeNames, IM_ARRAYSIZE(actionTypeNames));
			ImGui::Combo("Button##NewActionInput", &newActionInputButton, mouseButtonNames, IM_ARRAYSIZE(mouseButtonNames));
		} break;
		case ActionInputType::And:
		case ActionInputType::Or: 
		{
			const U32 inputCount = actionSystem.GetInputCount();
			if (inputCount < 2)
			{
				break;
			}
			std::vector<const char*> inputNames;
			inputNames.reserve(inputCount);
			for (U32 i = 0; i < inputCount; ++i)
			{
				inputNames.push_back(actionSystem.GetInputByIndex(i)->GetName().c_str());
			}

			ImGui::Combo("InputA##NewActionInput", &newActionInputAIndex, inputNames.data(), static_cast<int>(inputNames.size()));
			ImGui::Combo("InputB##NewActionInput", &newActionInputBIndex, inputNames.data(), static_cast<int>(inputNames.size()));
		} break;
		case ActionInputType::Not:
		{
			const U32 inputCount = actionSystem.GetInputCount();
			if (inputCount < 1)
			{
				break;
			}
			std::vector<const char*> inputNames;
			inputNames.reserve(inputCount);
			for (U32 i = 0; i < inputCount; ++i)
			{
				inputNames.push_back(actionSystem.GetInputByIndex(i)->GetName().c_str());
			}

			ImGui::Combo("Input##NewActionInput", &newActionInputAIndex, inputNames.data(), static_cast<int>(inputNames.size()));
		} break;
		default: enAssert(false); break;
		}

		bool validNewInput = true;

		const U32 nameLength = static_cast<U32>(strlen(newActionInputName));
		if (nameLength <= 0 || nameLength >= 255)
		{
			validNewInput = false;
		}
		if (actionInputType == ActionInputType::Variable || actionInputType == ActionInputType::Function || actionInputType == ActionInputType::Event)
		{
			validNewInput = false;
		}
		if (actionInputType == ActionInputType::And || actionInputType == ActionInputType::Or)
		{
			if (actionSystem.GetInputCount() < 2 || newActionInputAIndex == newActionInputBIndex)
			{
				validNewInput = false;
			}
		}
		if (actionInputType == ActionInputType::Not)
		{
			if (actionSystem.GetInputCount() < 1)
			{
				validNewInput = false;
			}
		}

		if (validNewInput)
		{
			if (ImGui::Button("Create##NewActionInput"))
			{
				switch (actionInputType)
				{
				case ActionInputType::Variable: enAssert(false); break;
				case ActionInputType::Function: enAssert(false); break;
				case ActionInputType::Event: enAssert(false); break;
				case ActionInputType::Key: actionSystem.AddInputKey(std::string(newActionInputName), static_cast<sf::Keyboard::Key>(newActionInputKey), static_cast<ActionType>(newActionInputActionType)); break;
				case ActionInputType::Mouse: actionSystem.AddInputMouse(std::string(newActionInputName), static_cast<sf::Mouse::Button>(newActionInputButton), static_cast<ActionType>(newActionInputActionType)); break;
				case ActionInputType::And:
				case ActionInputType::Or:
				{
					const U32 inputAID = actionSystem.GetInputByIndex(newActionInputAIndex)->GetID();
					const U32 inputBID = actionSystem.GetInputByIndex(newActionInputBIndex)->GetID();
					if (actionInputType == ActionInputType::And)
					{
						actionSystem.AddInputAnd(std::string(newActionInputName), inputAID, inputBID);
					}
					else
					{
						actionSystem.AddInputOr(std::string(newActionInputName), inputAID, inputBID);
					}
				} break;
				case ActionInputType::Not:
				{
					const U32 inputAID = actionSystem.GetInputByIndex(newActionInputAIndex)->GetID();
					actionSystem.AddInputNot(std::string(newActionInputName), inputAID);
				} break;
				default: enAssert(false); break;
				}

				
#ifdef ENLIVE_COMPILER_MSVC
				strcpy_s(newActionInputName, "");
#else
				strcpy(newActionInputName, "");
#endif // ENLIVE_COMPILER_MSVC
			}
		}
		else
		{
			ImGui::DisabledButton("Create##NewActionInput");
		}
	}

	ImGui::Separator();


	// Input list
	{
		ImGui::Text("Input list:");
	
		ImGui::Indent();
		ImGui::Separator();
		U32 inputCount = actionSystem.GetInputCount();
		for (U32 i = 0; i < inputCount; i++)
		{
			ActionInput* actionInput = actionSystem.GetInputByIndexNonConst(i);
			if (actionInput == nullptr)
				continue;

			ImGui::PushID(static_cast<int>(i));

			ImGui::Text("%s", actionInput->GetName().c_str());
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("ID:%d", actionInput->GetID());
			}
			ImGui::SameLine(); 
			ImGui::Text(" : %s", actionInputTypeNames[static_cast<U32>(actionInput->GetInputType())]);
			if (actionInput->IsActive())
			{
				ImGui::SameLine();
				ImGui::Text(ICON_FA_COG);
			}

			switch (actionInput->GetInputType())
			{
			case ActionInputType::Variable: ImGui::Text("Not configurable yet"); /* TODO_Someday */ break;
			case ActionInputType::Function: ImGui::Text("Not configurable yet"); /* TODO_Someday */ break;
			case ActionInputType::Event: ImGui::Text("Not configurable yet"); /* TODO_Someday */ break;
			case ActionInputType::Key: 
			{
				if (ActionInputKey* actionInputKey = static_cast<ActionInputKey*>(actionInput))
				{
					int actionType = static_cast<int>(actionInputKey->GetType());
					if (ImGui::Combo("ActionType##ActionInput", &actionType, actionTypeNames, IM_ARRAYSIZE(actionTypeNames)))
					{
						actionInputKey->SetActionType(static_cast<ActionType>(actionType));
					}

					int actionKey = static_cast<int>(actionInputKey->GetKey());
					if (ImGui::Combo("Key##ActionInput", &actionKey, keyNames.data(), static_cast<int>(keyNames.size())))
					{
						actionInputKey->SetKey(static_cast<sf::Keyboard::Key>(actionKey));
					}
				}
				else
				{
					enAssert(false);
				}
			} break;
			case ActionInputType::Mouse:
			{
				if (ActionInputMouse* actionInputMouse = static_cast<ActionInputMouse*>(actionInput))
				{
					int actionType = static_cast<int>(actionInputMouse->GetType());
					if (ImGui::Combo("ActionType##ActionInput", &actionType, actionTypeNames, IM_ARRAYSIZE(actionTypeNames)))
					{
						actionInputMouse->SetActionType(static_cast<ActionType>(actionType));
					}

					int actionButton = static_cast<int>(actionInputMouse->GetButton());
					if (ImGui::Combo("Button##ActionInput", &actionButton, mouseButtonNames, IM_ARRAYSIZE(mouseButtonNames)))
					{
						actionInputMouse->SetButton(static_cast<sf::Mouse::Button>(actionButton));
					}
				}
				else
				{
					enAssert(false);
				}
			} break;
			case ActionInputType::JoystickConnect:
			{

			} break;
			case ActionInputType::JoystickButton:
			{

			} break;
			case ActionInputType::JoystickAxis:
			{

			} break;
			case ActionInputType::And:
			case ActionInputType::Or:
			case ActionInputType::Not:
			{
				if (ActionInputLogical* actionInputLogical = static_cast<ActionInputLogical*>(actionInput))
				{
					std::vector<const char*> inputNames;
					inputNames.reserve(inputCount);
					for (U32 j = 0; j < inputCount; ++j)
					{
						inputNames.push_back(actionSystem.GetInputByIndex(j)->GetName().c_str());
					}

					int inputAIndex = static_cast<int>(actionSystem.GetInputIndexFromID(actionInputLogical->GetInputAID()));
					if (ImGui::Combo("InputA##ActionInput", &inputAIndex, inputNames.data(), static_cast<int>(inputNames.size())))
					{
						if (inputAIndex != static_cast<int>(i))
						{
							actionInputLogical->SetInputAID(actionSystem.GetInputByIndex(static_cast<U32>(inputAIndex))->GetID());
							actionSystem.FlagPriorityAsDirty();
						}
					}

					if (actionInput->GetInputType() != ActionInputType::Not)
					{
						int inputBIndex = static_cast<int>(actionSystem.GetInputIndexFromID(actionInputLogical->GetInputBID()));
						if (ImGui::Combo("InputB##ActionInput", &inputBIndex, inputNames.data(), static_cast<int>(inputNames.size())))
						{
							if (inputBIndex != static_cast<int>(i))
							{
								actionInputLogical->SetInputBID(actionSystem.GetInputByIndex(static_cast<U32>(inputBIndex))->GetID());
								actionSystem.FlagPriorityAsDirty();
							}
						}
					}
				}
				else
				{
					enAssert(false);
				}
			} break;
                        default: enAssert(false); break;
			}

			if (ImGui::Button("Remove"))
			{
				actionSystem.RemoveInputByIndex(i);
				i--;
				inputCount--;
			}

			ImGui::PopID();
			ImGui::Separator();
		}
		ImGui::Unindent();
	}

}

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
