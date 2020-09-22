#include <Enlivengine/Tools/ImGuiAnimationEditor.hpp>

#if defined(ENLIVE_ENABLE_IMGUI)

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <Enlivengine/Tools/ImGuiHelper.hpp>

namespace en
{

ImGuiAnimationEditor::ImGuiAnimationEditor()
	: ImGuiTool()
	, mStateMachine()
	, mEditorContext(nullptr)
    , mDisplayPreviews(true)
{
}

ImGuiAnimationEditor::~ImGuiAnimationEditor()
{
	// We won't be able to save here as the resource might have 
	// been freed before by the ResourceManager.
	// We can't neither test the IsInitialized as it requires the 
	// ResourceManager to be valid and we can't be sure about it.

	mStateMachine = AnimationStateMachinePtr();
	if (mEditorContext != nullptr)
	{
		ax::NodeEditor::DestroyEditor(mEditorContext);
	}
}

ImGuiToolTab ImGuiAnimationEditor::GetTab() const
{
	return ImGuiToolTab::Main;
}

const char* ImGuiAnimationEditor::GetName() const
{
	return ICON_FA_RUNNING " AnimationEditor";
}

void ImGuiAnimationEditor::Display()
{
	if (IsInitialized() && mStateMachine.IsValid())
	{
		AnimationStateMachine& stateMachine = mStateMachine.Get();
		ax::NodeEditor::SetCurrentEditor(mEditorContext);

		ImGui::Columns(2, "animEditorColumns");

		LeftPanel(stateMachine);
		ImGui::NextColumn();

		NodeEditor(stateMachine);
		ImGui::NextColumn();

		ImGui::Columns(1);

		ax::NodeEditor::SetCurrentEditor(nullptr);
	}
	else
	{
		AnimationStateMachineList();
	}
}

bool ImGuiAnimationEditor::Initialize(AnimationStateMachinePtr stateMachinePtr)
{
	if (IsInitialized())
	{
		Uninitialize();
	}

	if (stateMachinePtr.IsValid())
	{
		AnimationStateMachine& stateMachine = stateMachinePtr.Get();
		if (!stateMachine.GetAnimation().IsValid())
		{
			return false;
		}

		if (!mAnimationController.SetAnimationStateMachine(stateMachinePtr))
		{
			return false;
		}

		mStateMachine = stateMachinePtr;

		ax::NodeEditor::Config config;
		config.SettingsFile = "AnimationEditor.json";
		mEditorContext = ax::NodeEditor::CreateEditor(&config);

		mVisible = true;
		AskForFocus();

		return true;
	}

	return false;
}

void ImGuiAnimationEditor::Uninitialize()
{
	if (IsInitialized())
	{
		// TODO : Save ?

		mStateMachine = AnimationStateMachinePtr();
		ax::NodeEditor::DestroyEditor(mEditorContext);

		mVisible = false;
	}
}

bool ImGuiAnimationEditor::IsInitialized() const
{
	return mStateMachine.IsValid() && mEditorContext != nullptr;
}

int ImGuiAnimationEditor::GetStateID(const AnimationStateMachine::State& state, const AnimationStateMachine& stateMachine)
{
	ENLIVE_UNUSED(stateMachine);
	return static_cast<int>(state.GetHashedName());
}

int ImGuiAnimationEditor::GetTransitionID(const AnimationStateMachine::Transition& transition, const AnimationStateMachine& stateMachine)
{
	const U32 fromStateHash = stateMachine.GetState(transition.GetFromState()).GetHashedName();
	const U32 toStateHash = stateMachine.GetState(transition.GetToState()).GetHashedName();
	const U32 transitionCount = stateMachine.GetTransitionCount();
	U32 transitionIndex = 0;
	for (U32 i = 0; i < transitionCount; ++i)
	{
		if (&transition == &(stateMachine.GetTransition(i)))
		{
			transitionIndex = i;
		}
	}
	return static_cast<int>(fromStateHash + toStateHash + transitionIndex);
}

int ImGuiAnimationEditor::GetTransitionInputID(const AnimationStateMachine::Transition& transition, const AnimationStateMachine& stateMachine)
{
	const U32 toStateHash = stateMachine.GetState(transition.GetToState()).GetHashedName();
	const U32 transitionCount = stateMachine.GetTransitionCount();
	U32 transitionIndex = 0;
	for (U32 i = 0; i < transitionCount; ++i)
	{
		if (&transition == &(stateMachine.GetTransition(i)))
		{
			transitionIndex = i;
		}
	}
	return static_cast<int>(toStateHash + transitionIndex);
}

int ImGuiAnimationEditor::GetTransitionOutputID(const AnimationStateMachine::Transition& transition, const AnimationStateMachine& stateMachine)
{
	const U32 fromStateHash = stateMachine.GetState(transition.GetFromState()).GetHashedName();
	const U32 transitionCount = stateMachine.GetTransitionCount();
	U32 transitionIndex = 0;
	for (U32 i = 0; i < transitionCount; ++i)
	{
		if (&transition == &(stateMachine.GetTransition(i)))
		{
			transitionIndex = i;
		}
	}
	return static_cast<int>(fromStateHash + transitionIndex);
}

void ImGuiAnimationEditor::LeftPanel(AnimationStateMachine& stateMachine)
{
	static int initialLeftColSize = true;
	if (initialLeftColSize)
	{
		ImGui::SetColumnWidth(0, 240.0f);
        initialLeftColSize = false;
	}

    StateMachine(stateMachine);
	Selection(stateMachine);
	NewState(stateMachine);
	NewTransition(stateMachine);
	NewParameter(stateMachine);
	ParametersList(stateMachine); 
	//Debug(stateMachine);
	Preview(stateMachine);
}

void ImGuiAnimationEditor::StateMachine(AnimationStateMachine& stateMachine)
{
    if (ImGui::Button("Save"))
    {
        if (!stateMachine.SaveToFile(stateMachine.GetFilename()))
        {
            enLogError(en::LogChannel::Animation, "Can't save file {}", stateMachine.GetFilename().c_str());
        }
    }

    ImGui::Checkbox("Show clips", &mDisplayPreviews);
}

void ImGuiAnimationEditor::Selection(AnimationStateMachine& stateMachine)
{
	const U32 selected = static_cast<U32>(ax::NodeEditor::GetSelectedObjectCount());
	std::vector<ax::NodeEditor::NodeId> selectedNodes;
	std::vector<ax::NodeEditor::LinkId> selectedLinks;
	selectedNodes.resize(selected);
	selectedLinks.resize(selected);
	const U32 nodeCount = static_cast<U32>(ax::NodeEditor::GetSelectedNodes(selectedNodes.data(), static_cast<int>(selectedNodes.size())));
	const U32 linkCount = static_cast<U32>(ax::NodeEditor::GetSelectedLinks(selectedLinks.data(), static_cast<int>(selectedLinks.size())));
	if (nodeCount >= 1)
	{
		SelectedNode(stateMachine, selectedNodes[0]);
	}
	else if (linkCount >= 1)
	{
		SelectedLink(stateMachine, selectedLinks[0]);
	}
	else
	{
		ImGui::CollapsingHeader("Empty selection", ImGuiTreeNodeFlags_Leaf);
	}
}

void ImGuiAnimationEditor::SelectedNode(AnimationStateMachine& stateMachine, ax::NodeEditor::NodeId node)
{
	ImGui::CollapsingHeader("Selected State", ImGuiTreeNodeFlags_Leaf);
	ImGui::Indent();

	const int nodeID = static_cast<int>(reinterpret_cast<std::uintptr_t>(node.AsPointer()));

	U32 stateIndex = stateMachine.GetStateCount();
	for (U32 i = 0; i < stateIndex; ++i)
	{
		if (GetStateID(stateMachine.GetState(i), stateMachine) == nodeID)
		{
			stateIndex = i;
		}
	}
	if (stateIndex >= stateMachine.GetStateCount())
	{
		ImGui::Unindent();
		ax::NodeEditor::ClearSelection();
		return;
	}

	if (ImGui::Button(ICON_FA_BAN " Remove state"))
	{
		stateMachine.RemoveState(stateIndex);
		ax::NodeEditor::ClearSelection();
		ImGui::Unindent();
		return;
	}

	const AnimationStateMachine::State& state = stateMachine.GetState(stateIndex);

	ImGui::PushID(stateIndex);

    ImGui::Text("%s", state.GetName().c_str());

    static U32 clipFrameIndex = 0;
	static Time accumulator;
	const Animation& animation = stateMachine.GetAnimation().Get();
    ImGui::PreviewAnimationClip(animation, 100.0f, state.GetClipIndex(), clipFrameIndex, accumulator, state.GetSpeedScale());

	float speedScale = static_cast<float>(state.GetSpeedScale());
	ImGui::PushItemWidth(90.0f);
	if (ImGui::InputFloat("SpeedScale##SelectedNode", &speedScale))
	{
		stateMachine.SetStateSpeedScale(stateIndex, static_cast<F32>(speedScale));
	}
	ImGui::PopItemWidth();

    bool isDefaultState = (stateIndex == stateMachine.GetDefaultStateIndex());
    if (ImGui::Checkbox("DefaultState##SelectedNode", &isDefaultState))
    {
        if (isDefaultState)
        {
            stateMachine.SetDefaultStateIndex(stateIndex);
        }
        else
        {
            stateMachine.SetDefaultStateIndex(stateMachine.GetStateCount());
        }
    }

	ImGui::Spacing();
	if (state.HasBlendStateInfo())
	{
		const AnimationStateMachine::State::BlendStateInfo* blendStateInfo = state.GetBlendStateInfo();
		if (ImGui::Button(ICON_FA_BAN " Remove BlendState"))
		{
			stateMachine.RemoveBlendStateFromState(stateIndex);
		}
		if (state.HasBlendStateInfo() && ImGui::CollapsingHeader("BlendState"))
		{
			// Parameters
			std::vector<const char*> parameterNames;
			std::vector<U32> parameterIndexes;
			const U32 parameterCount = stateMachine.GetParameterCount();
			for (U32 i = 0; i < parameterCount; ++i)
			{
				const AnimationStateMachine::Parameter& parameter = stateMachine.GetParameter(i);
				if (parameter.GetType() == AnimationStateMachine::Parameter::Type::Float || parameter.GetType() == AnimationStateMachine::Parameter::Type::Integer)
				{
					parameterNames.push_back(parameter.GetName().c_str());
					parameterIndexes.push_back(i);
				}
			}
			const U32 dimensionCount = blendStateInfo->GetDimension();
			for (U32 dimension = 0; dimension < dimensionCount; ++dimension)
			{
				int currentParameter = -1;
				for (std::size_t j = 0; j < parameterIndexes.size(); ++j)
				{
					if (parameterIndexes[j] == blendStateInfo->GetParameter(dimension))
					{
						currentParameter = static_cast<int>(j);
					}
				}
				if (currentParameter < 0)
				{
					// TODO : Default parameter
				}
				ImGui::PushID(1000 + dimension);
				ImGui::Text("%d", dimension);
				ImGui::SameLine();
				ImGui::PushItemWidth(100.0f);
				if (ImGui::Combo("##ComboParameterDimension", &currentParameter, parameterNames.data(), static_cast<int>(parameterNames.size())))
				{
					stateMachine.SetBlendStateParameter(stateIndex, dimension, parameterIndexes[currentParameter]);
				}
				ImGui::PopItemWidth();
				ImGui::PopID();
			}

			ImGui::Spacing();

			struct AnimInfo
			{
				U32 clipFrameIndex = 0;
				Time accumulator;
			};
			static std::vector<AnimInfo> animInfos;
			if (static_cast<U32>(animInfos.size()) != blendStateInfo->GetMotionCount())
			{
				animInfos.resize(blendStateInfo->GetMotionCount());
			}
			for (U32 motionIndex = 0; motionIndex < blendStateInfo->GetMotionCount(); ++motionIndex)
			{
				ImGui::PreviewAnimationClip(stateMachine.GetAnimation().Get(), 30.0f, blendStateInfo->GetMotion(motionIndex).GetClipIndex(), animInfos[motionIndex].clipFrameIndex, animInfos[motionIndex].accumulator, state.GetSpeedScale());
				ImGui::Indent();
				for (U32 valueIndex = 0; valueIndex < dimensionCount; ++valueIndex)
				{
					ImGui::PushID(2000 + motionIndex + valueIndex);
					float value = blendStateInfo->GetMotion(motionIndex).GetValue(valueIndex);
					ImGui::Text("%d", valueIndex);
					ImGui::SameLine();
					if (ImGui::InputFloat("##ValueMotionDimension", &value))
					{
						stateMachine.SetBlendStateMotionValue(stateIndex, motionIndex, valueIndex, value);
					}
					ImGui::PopID();
				}
				ImGui::Unindent();
			}

			ImGui::Spacing();

			static AnimInfo animNewMotion{ state.GetClipIndex(), Time::Zero() };
			static int animClipNewMotion = 0;
			ImGui::PreviewAnimationClip(stateMachine.GetAnimation().Get(), 30.0f, static_cast<U32>(animClipNewMotion), animNewMotion.clipFrameIndex, animNewMotion.accumulator, state.GetSpeedScale());
			ImGui::PushItemWidth(90.0f);
			if (ImGui::InputInt("", &animClipNewMotion))
			{
				if (animClipNewMotion < 0) animClipNewMotion = 0;
				//  TODO : Check out of bounds
			}
			ImGui::PopItemWidth();
			ImGui::SameLine();
			if (ImGui::Button("Add Motion"))
			{
				stateMachine.AddBlendStateMotion(stateIndex, static_cast<U32>(animClipNewMotion));
				animClipNewMotion = state.GetClipIndex();
			}
		}
	}
	else
	{
		static int dimensions = 1;
		ImGui::PushItemWidth(60.0f);
		ImGui::InputInt("D##SelectedNodeDimensionInput", &dimensions);
		ImGui::PopItemWidth();
		if (dimensions < 1) dimensions = 1;
		ImGui::SameLine();
		if (ImGui::Button("Add BlendState"))
		{
			stateMachine.AddBlendStateToState(stateIndex, static_cast<U32>(dimensions));
		}
	}

	ImGui::PopID();

	ImGui::Unindent();
}

void ImGuiAnimationEditor::SelectedLink(AnimationStateMachine& stateMachine, ax::NodeEditor::LinkId link)
{
	ImGui::CollapsingHeader("Selected Transition", ImGuiTreeNodeFlags_Leaf);
	ImGui::Indent();

	const int linkID = static_cast<int>(reinterpret_cast<std::uintptr_t>(link.AsPointer()));

	U32 transitionIndex = stateMachine.GetTransitionCount();
	for (U32 i = 0; i < transitionIndex; ++i)
	{
		if (GetTransitionID(stateMachine.GetTransition(i), stateMachine) == linkID)
		{
			transitionIndex = i;
		}
	}
	if (transitionIndex >= stateMachine.GetTransitionCount())
	{
		ImGui::Unindent();
		ax::NodeEditor::ClearSelection();
		return;
	}

	if (ImGui::Button(ICON_FA_BAN " Remove transition"))
	{
		stateMachine.RemoveTransition(transitionIndex);
		ax::NodeEditor::ClearSelection();
		ImGui::Unindent();
		return;
	}

	const AnimationStateMachine::Transition& transition = stateMachine.GetTransition(transitionIndex);

	ImGui::Text("From: %s", stateMachine.GetState(transition.GetFromState()).GetName().c_str());
	ImGui::Text("To: %s", stateMachine.GetState(transition.GetToState()).GetName().c_str());

	bool exitOnlyAtEnd = transition.GetExitOnlyAtEnd();
	if (ImGui::Checkbox("ExitOnlyAtEnd##SelectedLink", &exitOnlyAtEnd))
	{
		stateMachine.SetTransitionExitOnlyAtEnd(transitionIndex, exitOnlyAtEnd);
	}

	if (stateMachine.GetParameterCount() > 0)
	{
		ImGui::Text("Conditions:");
		ImGui::SameLine();
		if (ImGui::Button("New condition"))
		{
			constexpr U32 defaultParameterIndex = 0;
			U32 conditionIndex = stateMachine.AddCondition(defaultParameterIndex);
			stateMachine.AddConditionToTransition(transitionIndex, conditionIndex);
			stateMachine.SetConditionOperator(conditionIndex, AnimationStateMachine::Condition::Operator::Equal);
			switch (stateMachine.GetParameter(defaultParameterIndex).GetType())
			{
			case AnimationStateMachine::Parameter::Type::Boolean: stateMachine.SetConditionOperandBoolean(conditionIndex, true); break;
			case AnimationStateMachine::Parameter::Type::Float: stateMachine.SetConditionOperandFloat(conditionIndex, 0.0f); break;
			case AnimationStateMachine::Parameter::Type::Integer: stateMachine.SetConditionOperandInteger(conditionIndex, 0); break;
			default: break;
			}
		}

		std::vector<const char*> paramNames;
		paramNames.reserve(stateMachine.GetParameterCount());
		for (U32 parameterIndex = 0; parameterIndex < stateMachine.GetParameterCount(); ++parameterIndex)
		{
			paramNames.push_back(stateMachine.GetParameter(parameterIndex).GetName().c_str());
		}

		static const char* boolOperators[] = { "=", "!=" };
		static const char* boolValues[] = { "False", "True" };
		static const char* operators[] = { "=", "!=", "<", "<=", ">", ">=" };

		ImGui::Indent();
		for (U32 conditionIndexInTransition = 0; conditionIndexInTransition < transition.GetConditionCount(); ++conditionIndexInTransition)
		{
			ImGui::PushID(conditionIndexInTransition);

			U32 conditionIndex = transition.GetCondition(conditionIndexInTransition);
			const AnimationStateMachine::Condition& condition = stateMachine.GetCondition(conditionIndex);

			ImGui::Text("-");
			ImGui::SameLine();

			ImGui::PushItemWidth(120.0f);
			int parameterIndex = static_cast<int>(condition.GetParameterIndex());
			if (ImGui::Combo("##CondEditParam", &parameterIndex, paramNames.data(), static_cast<int>(paramNames.size())))
			{
				stateMachine.SetConditionParameter(conditionIndex, static_cast<U32>(parameterIndex));
			}
			ImGui::PopItemWidth();

			bool valid = true;
			ImGui::SameLine();
			ImGui::Text(ICON_FA_BAN);
			if (ImGui::IsItemClicked())
			{
				stateMachine.RemoveConditionFromTransition(transitionIndex, conditionIndex);
				valid = false;
			}

			if (valid)
			{
				ImGui::Indent();
				AnimationStateMachine::Parameter::Type type = stateMachine.GetParameter(condition.GetParameterIndex()).GetType();
				switch (type)
				{
				case AnimationStateMachine::Parameter::Type::Boolean:
				{
					ImGui::PushItemWidth(40.0f);
					int operatorIndex;
					AnimationStateMachine::Condition::Operator operatorValue = condition.GetOperator();
					if (operatorValue == AnimationStateMachine::Condition::Operator::NotEqual
						|| operatorValue == AnimationStateMachine::Condition::Operator::Greater
						|| operatorValue == AnimationStateMachine::Condition::Operator::Less)
					{
						operatorIndex = 1;
					}
					else
					{
						operatorIndex = 0;
					}
					if (ImGui::Combo("##ConditionBoolOperatorInput", &operatorIndex, boolOperators, IM_ARRAYSIZE(boolOperators)))
					{
						stateMachine.SetConditionOperator(conditionIndex, (operatorIndex == 0)
							? AnimationStateMachine::Condition::Operator::Equal
							: AnimationStateMachine::Condition::Operator::NotEqual);
					}
					ImGui::PopItemWidth();

					ImGui::SameLine();

					ImGui::PushItemWidth(80.0f);
					int boolValueIndex = (int)condition.GetOperandBoolean();
					if (ImGui::Combo("##ConditionBoolValueInput", &boolValueIndex, boolValues, IM_ARRAYSIZE(boolValues)))
					{
						stateMachine.SetConditionOperandBoolean(conditionIndex, (bool)boolValueIndex);
					}
					ImGui::PopItemWidth();
				} break;
				case AnimationStateMachine::Parameter::Type::Float:
				{
					ImGui::PushItemWidth(40.0f);
					int operatorIndex = static_cast<int>(condition.GetOperator());
					if (ImGui::Combo("##ConditionFloatOperatorInput", &operatorIndex, operators, IM_ARRAYSIZE(operators)))
					{
						stateMachine.SetConditionOperator(conditionIndex, static_cast<AnimationStateMachine::Condition::Operator>(operatorIndex));
					}
					ImGui::PopItemWidth();

					ImGui::SameLine();

					ImGui::PushItemWidth(80.0f);
					float floatValue = (float)condition.GetOperandFloat();
					if (ImGui::InputFloat("##ConditionFloatValueInput", &floatValue))
					{
						stateMachine.SetConditionOperandFloat(conditionIndex, (F32)floatValue);
					}
					ImGui::PopItemWidth();
				} break;
				case AnimationStateMachine::Parameter::Type::Integer:
				{
					ImGui::PushItemWidth(40.0f);
					int operatorIndex = static_cast<int>(condition.GetOperator());
					if (ImGui::Combo("##ConditionIntegerOperatorInput", &operatorIndex, operators, IM_ARRAYSIZE(operators)))
					{
						stateMachine.SetConditionOperator(conditionIndex, static_cast<AnimationStateMachine::Condition::Operator>(operatorIndex));
					}
					ImGui::PopItemWidth();

					ImGui::SameLine();

					ImGui::PushItemWidth(80.0f);
					int intValue = (int)condition.GetOperandInteger();
					if (ImGui::InputInt("##ConditionIntegerValueInput", &intValue))
					{
						stateMachine.SetConditionOperandInteger(conditionIndex, (I32)intValue);
					}
					ImGui::PopItemWidth();
				} break;
				case AnimationStateMachine::Parameter::Type::Trigger: break;
				default: enAssert(false); break;
				}
				ImGui::Unindent();
			}

			ImGui::PopID();
		}
		ImGui::Unindent();
	}

	ImGui::Unindent();
}

void ImGuiAnimationEditor::NewState(AnimationStateMachine& stateMachine)
{
	if (ImGui::CollapsingHeader("New state"))
	{
		ImGui::Indent();

		bool validNewState = true;
		ImGui::InputText("Name##NewState", mStateInputTextBuffer, kBufferSize);
		if (strlen(mStateInputTextBuffer) <= 0)
		{
			validNewState = false;
		}

		static int stateClipIndex = 0;

		if (stateMachine.GetAnimation().IsValid() && stateMachine.GetAnimation().Get().GetClipCount() > 0)
		{
			const Animation& animation = stateMachine.GetAnimation().Get();
			ImGui::InputInt("##NewStateClipIndex", &stateClipIndex);
			if (stateClipIndex >= static_cast<int>(animation.GetClipCount()))
			{
				stateClipIndex = 0;
			}
			if (stateClipIndex < 0)
			{
				stateClipIndex = static_cast<int>(animation.GetClipCount() - 1);
			}

			const Animation::Clip& clip = animation.GetClip(static_cast<U32>(stateClipIndex));
			if (clip.GetFrameCount() > 0)
			{
				static Time accumulator;
				static U32 clipFrameIndex = 0;
                ImGui::PreviewAnimationClip(animation, 100.0f, static_cast<U32>(stateClipIndex), clipFrameIndex, accumulator, 1.0f);
			}
			else
			{
				ImGui::TextColored(Color::Orange.toImGuiColor(), "Invalid clip");
				validNewState = false;
			}
		}
		else
		{
			validNewState = false;
		}
		if (validNewState && (stateClipIndex >= static_cast<int>(stateMachine.GetAnimation().Get().GetClipCount()) || stateClipIndex < 0))
		{
			validNewState = false;
		}

		if (validNewState)
		{
			if (ImGui::Button("Add##NewState"))
			{
				stateMachine.AddState(std::string(mStateInputTextBuffer), stateClipIndex);
#ifdef ENLIVE_COMPILER_MSVC
				strcpy_s(mStateInputTextBuffer, "");
#else
				strcpy(mStateInputTextBuffer, "");
#endif // ENLIVE_COMPILER_MSVC
			}
		}
		else
		{
			ImGui::DisabledButton("Add##NewStateDisabled");
		}

		ImGui::Unindent();
	}
}

void ImGuiAnimationEditor::NewTransition(AnimationStateMachine& stateMachine)
{
	if (ImGui::CollapsingHeader("New transition") && stateMachine.GetStateCount() > 0)
	{
		std::vector<const char*> stateNames;
		stateNames.reserve(stateMachine.GetStateCount());
		for (U32 stateIndex = 0; stateIndex < stateMachine.GetStateCount(); ++stateIndex)
		{
			stateNames.push_back(stateMachine.GetState(stateIndex).GetName().c_str());
		}

		const int stateCount = static_cast<int>(stateMachine.GetStateCount());

		static int fromStateIndex = 0;
		if (fromStateIndex >= stateCount)
		{
			fromStateIndex = 0;
		}

		static int toStateIndex = 0;
		if (toStateIndex >= stateCount)
		{
			toStateIndex = 0;
		}

		ImGui::Combo("FromState##NewTransition", &fromStateIndex, stateNames.data(), stateCount);
		ImGui::Combo("ToState##NewTransition", &toStateIndex, stateNames.data(), stateCount);

		bool transitionValid = true;
		if (fromStateIndex < 0 || fromStateIndex >= stateCount)
		{
			transitionValid = false;
		}
		if (toStateIndex < 0 || toStateIndex >= stateCount)
		{
			transitionValid = false;
		}
		if (fromStateIndex == toStateIndex)
		{
			transitionValid = false;
		}

		if (transitionValid)
		{
			if (ImGui::Button("Add##NewTransition"))
			{
				stateMachine.AddTransition(static_cast<U32>(fromStateIndex), static_cast<U32>(toStateIndex));
				fromStateIndex = 0;
				toStateIndex = 0;
			}
		}
		else
		{
			ImGui::DisabledButton("Add##NewTransitionDisabled");
		}
	}
}

void ImGuiAnimationEditor::NewParameter(AnimationStateMachine& stateMachine)
{
	if (ImGui::CollapsingHeader("New parameter"))
	{
		ImGui::Indent();

		bool validNewParameter = true;

		// Name
		ImGui::InputText("Name##NewParameterNameInput", mParameterInputTextBuffer, kBufferSize);
		if (strlen(mParameterInputTextBuffer) <= 0)
		{
			validNewParameter = false;
		}

		static int parameterType = 0;
		static bool boolValue = true;
		static F32 floatValue = 0.0f;
		static I32 intValue = 0;

		// Type
		static const char* paramTypes[] = { "Boolean", "Float", "Integer", "Trigger" };
		static_assert(IM_ARRAYSIZE(paramTypes) == static_cast<int>(AnimationStateMachine::Parameter::Type::Count));
		int previousType = parameterType;
		if (ImGui::Combo("Type##NewParameterTypeInput", &parameterType, paramTypes, IM_ARRAYSIZE(paramTypes)))
		{
			if (previousType != parameterType)
			{
				boolValue = true;
				floatValue = 0.0f;
				intValue = 0;
			}
		}

		// Value
		ImGui::Indent();
		switch (parameterType)
		{
		case 0: // Boolean
		{
			static const char* boolValues[] = { "False", "True" };
			int boolValueIndex = (int)boolValue;
			if (ImGui::Combo("##NewParameterBoolParameterInput", &boolValueIndex, boolValues, IM_ARRAYSIZE(boolValues)))
			{
				boolValue = (bool)boolValueIndex;
			}
		} break;
		case 1: // Float
			ImGui::InputFloat("##NewParameterFloatParameterInput", &floatValue); 
			break;
		case 2: // Integer
			ImGui::InputInt("##NewParameterIntParameterInput", &intValue);
			break;
		case 3: // Trigger
			break;
		default: 
			break;
		}
		ImGui::Unindent();

		if (validNewParameter)
		{
			if (ImGui::Button("Add##NewParameterButton"))
			{
				const U32 parameterIndex = stateMachine.AddParameter(std::string(mParameterInputTextBuffer), static_cast<AnimationStateMachine::Parameter::Type>(parameterType));
				switch (parameterType)
				{
				case 0: stateMachine.SetParameterBoolean(parameterIndex, boolValue); break;
				case 1: stateMachine.SetParameterFloat(parameterIndex, floatValue); break;
				case 2: stateMachine.SetParameterInteger(parameterIndex, intValue); break;
				case 3: break;
				default: break;
				}
				parameterType = 0;
				boolValue = true;
				floatValue = 0.0f;
				intValue = 0;
#ifdef ENLIVE_COMPILER_MSVC
				strcpy_s(mParameterInputTextBuffer, "");
#else
				strcpy(mParameterInputTextBuffer, "");
#endif // ENLIVE_COMPILER_MSVC
			}
		}
		else
		{
			ImGui::DisabledButton("Add##NewParameterDisabled");
		}

		ImGui::Unindent();
	}
}

void ImGuiAnimationEditor::ParametersList(AnimationStateMachine& stateMachine)
{
    if (ImGui::CollapsingHeader("Parameters list"))
    {
        ImGui::Indent();

        U32 parameterCount = stateMachine.GetParameterCount();
        for (U32 parameterIndex = 0; parameterIndex < parameterCount; ++parameterIndex)
        {
            const AnimationStateMachine::Parameter& parameter = stateMachine.GetParameter(parameterIndex);
            ImGui::PushID(parameterIndex);

            ImGui::Text("-");
            ImGui::SameLine();

            ImVec4 color;
            switch (parameter.GetType())
            {
            case AnimationStateMachine::Parameter::Type::Boolean: color = Color::Lime.toImGuiColor(); break;
            case AnimationStateMachine::Parameter::Type::Float: color = Color::Salmon.toImGuiColor(); break;
            case AnimationStateMachine::Parameter::Type::Integer: color = Color::Cyan.toImGuiColor(); break;
            case AnimationStateMachine::Parameter::Type::Trigger: color = Color::Yellow.toImGuiColor(); break;
            default: break;
            }
            ImGui::TextColored(color, "%s", parameter.GetName().c_str());

            if (parameter.GetType() < AnimationStateMachine::Parameter::Type::Trigger)
            {
                ImGui::SameLine();

                ImGui::PushItemWidth(80.0f);
                switch (parameter.GetType())
                {
                case AnimationStateMachine::Parameter::Type::Boolean: // Boolean
                {
                    static const char* boolValues[] = { "False", "True" };
                    int boolValueIndex = (int)parameter.GetBooleanValue();
                    if (ImGui::Combo("##ParametersListBoolParametersListInput", &boolValueIndex, boolValues, IM_ARRAYSIZE(boolValues)))
                    {
                        stateMachine.SetParameterBoolean(parameterIndex, (bool)boolValueIndex);
                    }
                } break;
                case AnimationStateMachine::Parameter::Type::Float: // Float
                {
                    float floatValue = parameter.GetFloatValue();
                    if (ImGui::InputFloat("##ParametersListFloatParametersListInput", &floatValue))
                    {
                        stateMachine.SetParameterFloat(parameterIndex, (F32)floatValue);
                    }
                } break;
                case AnimationStateMachine::Parameter::Type::Integer: // Integer
                {
                    int intValue = parameter.GetIntegerValue();
                    if (ImGui::InputInt("##ParametersListIntParametersListInput", &intValue))
                    {
                        stateMachine.SetParameterInteger(parameterIndex, (I32)intValue);
                    }
                } break;
                default:
                    break;
                }
                ImGui::PopItemWidth();
            }

            ImGui::SameLine();
            ImGui::Text(ICON_FA_BAN);
            if (ImGui::IsItemClicked())
            {
                stateMachine.RemoveParameter(parameterIndex);
                parameterCount--;
                parameterIndex--;
            }
            ImGui::PopID();
        }

        ImGui::Unindent();
    }
}

void ImGuiAnimationEditor::Debug(AnimationStateMachine& stateMachine)
{
	if (ImGui::CollapsingHeader("Debug"))
	{
		ImGui::Text("%d states", stateMachine.GetStateCount());
		if (ImGui::IsItemHovered() && stateMachine.GetStateCount() > 0)
		{
			ImGui::BeginTooltip();
			const U32 stateCount = stateMachine.GetStateCount();
			for (U32 i = 0; i < stateCount; ++i)
			{
				const AnimationStateMachine::State& state = stateMachine.GetState(i);
				ImGui::Text("%s", state.GetName().c_str());
			}
			ImGui::EndTooltip();
		}

		ImGui::Text("%d parameters", stateMachine.GetParameterCount());
		if (ImGui::IsItemHovered() && stateMachine.GetParameterCount() > 0)
		{
			ImGui::BeginTooltip();
			const U32 parameterCount = stateMachine.GetParameterCount();
			for (U32 i = 0; i < parameterCount; ++i)
			{
				const AnimationStateMachine::Parameter& parameter = stateMachine.GetParameter(i);
				ImGui::Text("%s", parameter.GetName().c_str());
			}
			ImGui::EndTooltip();
		}

		ImGui::Text("%d transitions", stateMachine.GetTransitionCount());
		if (ImGui::IsItemHovered() && stateMachine.GetTransitionCount() > 0)
		{
			ImGui::BeginTooltip();
			const U32 transitionCount = stateMachine.GetTransitionCount();
			for (U32 i = 0; i < transitionCount; ++i)
			{
				const AnimationStateMachine::Transition& transition = stateMachine.GetTransition(i);
				ImGui::Text("From: %s", stateMachine.GetState(transition.GetFromState()).GetName().c_str());
				ImGui::Text("To: %s", stateMachine.GetState(transition.GetToState()).GetName().c_str());

				const U32 conditionCount = transition.GetConditionCount();
				for (U32 j = 0; j < conditionCount; ++j)
				{
					ImGui::Text("%d ", transition.GetCondition(j));
					if (j < conditionCount - 1)
					{
						ImGui::SameLine();
					}
				}

				if (i < transitionCount - 1)
				{
					ImGui::Separator();
				}
			}
			ImGui::EndTooltip();
		}

		ImGui::Text("%d conditions", stateMachine.GetConditionCount());
		if (ImGui::IsItemHovered() && stateMachine.GetConditionCount() > 0)
		{
			ImGui::BeginTooltip();
			const U32 conditionCount = stateMachine.GetConditionCount();
			for (U32 i = 0; i < conditionCount; ++i)
			{
				const AnimationStateMachine::Condition& condition = stateMachine.GetCondition(i);
				ImGui::Text("On: %s", stateMachine.GetParameter(condition.GetParameterIndex()).GetName().c_str());
			}
			ImGui::EndTooltip();
		}
	}
}

void ImGuiAnimationEditor::Preview(AnimationStateMachine& stateMachine)
{
	if (ImGui::CollapsingHeader("Preview"))
	{
		ImGui::Indent();

		// Parameters
		if (mAnimationController.IsStateMachineValid())
		{
			U32 parameterCount = mAnimationController.GetParameterCount();
			for (U32 parameterIndex = 0; parameterIndex < parameterCount; ++parameterIndex)
			{
				const AnimationStateMachine::Parameter& parameter = mAnimationController.GetParameter(parameterIndex);
				ImGui::PushID(parameterIndex);

				ImGui::Text("-");
				ImGui::SameLine();

				ImVec4 color;
				switch (parameter.GetType())
				{
				case AnimationStateMachine::Parameter::Type::Boolean: color = Color::Lime.toImGuiColor(); break;
				case AnimationStateMachine::Parameter::Type::Float: color = Color::Salmon.toImGuiColor(); break;
				case AnimationStateMachine::Parameter::Type::Integer: color = Color::Cyan.toImGuiColor(); break;
				case AnimationStateMachine::Parameter::Type::Trigger: color = Color::Yellow.toImGuiColor(); break;
				default: break;
				}
				ImGui::TextColored(color, "%s", parameter.GetName().c_str());

				ImGui::SameLine();

				ImGui::PushItemWidth(80.0f);
				switch (parameter.GetType())
				{
				case AnimationStateMachine::Parameter::Type::Boolean: // Boolean
				{
					static const char* boolValues[] = { "False", "True" };
					int boolValueIndex = (int)parameter.GetBooleanValue();
					if (ImGui::Combo("##PreviewBoolParametersListInput", &boolValueIndex, boolValues, IM_ARRAYSIZE(boolValues)))
					{
						mAnimationController.SetParameterBoolean(parameter.GetHashedName(), (bool)boolValueIndex);
					}
				} break;
				case AnimationStateMachine::Parameter::Type::Float: // Float
				{
					float floatValue = parameter.GetFloatValue();
					if (ImGui::InputFloat("##PreviewFloatParametersListInput", &floatValue))
					{
						mAnimationController.SetParameterFloat(parameter.GetHashedName(), (F32)floatValue);
					}
				} break;
				case AnimationStateMachine::Parameter::Type::Integer: // Integer
				{
					int intValue = parameter.GetIntegerValue();
					if (ImGui::InputInt("##PreviewIntParametersListInput", &intValue))
					{
						mAnimationController.SetParameterInteger(parameter.GetHashedName(), (I32)intValue);
					}
				} break;
				case AnimationStateMachine::Parameter::Type::Trigger: // Trigger
				{
					if (ImGui::Button("X##PreviewTriggerParametersListInput"))
					{
						mAnimationController.SetParameterTrigger(parameter.GetHashedName());
					}
				} break;
				default:
					break;
				}
				ImGui::PopItemWidth();

				ImGui::PopID();
			}
		}

		// Update
		mAnimationController.Update(Time::Seconds(ImGui::GetIO().DeltaTime));

		// Render
		if (mAnimationController.IsStateMachineValid() && mAnimationController.AreIndicesValid())
		{
			const Animation& animation = stateMachine.GetAnimation().Get();
			const Texture& texture = animation.GetTexture().Get();
			const U32 frameIndex = mAnimationController.GetFrameIndex();
			const Rectu textureRect = animation.GetFrame(frameIndex).GetRect();

			ImGui::PreviewTexture(texture, textureRect, 100.0f, true);
		}

		ImGui::Unindent();
	}
}

void ImGuiAnimationEditor::NodeEditor(AnimationStateMachine& stateMachine)
{
	static constexpr float paddingX = 12.0f;
	static constexpr float paddingY = 6.0f;
	static constexpr float rounding = 4.0f;
	static constexpr float pinHalfSize = 0.1f;
	static constexpr float pinPaddingY = 6.0f;
	static constexpr float pinSpacing = 2.0f;

	ax::NodeEditor::Begin(stateMachine.GetIdentifier().c_str(), ImVec2(0.0f, 0.0f));

    ax::NodeEditor::PushStyleColor(ax::NodeEditor::StyleColor_NodeBg, ImColor(128, 128, 128, 250));
    ax::NodeEditor::PushStyleColor(ax::NodeEditor::StyleColor_NodeBorder, ImColor(32, 32, 32, 250));
    ax::NodeEditor::PushStyleVar(ax::NodeEditor::StyleVar_NodePadding, ImVec4(paddingX, paddingY, paddingX, paddingY));
    ax::NodeEditor::PushStyleVar(ax::NodeEditor::StyleVar_NodeRounding, rounding);
    ax::NodeEditor::PushStyleVar(ax::NodeEditor::StyleVar_NodeBorderWidth, 2.0f);
    ax::NodeEditor::PushStyleVar(ax::NodeEditor::StyleVar_LinkStrength, 0.0f);
    ax::NodeEditor::PushStyleVar(ax::NodeEditor::StyleVar_PivotSize, ImVec2(0.0f, 0.0f));
    ax::NodeEditor::PushStyleVar(ax::NodeEditor::StyleVar_PinCorners, 4);
	ax::NodeEditor::PushStyleVar(ax::NodeEditor::StyleVar_PinRadius, 0.0f);

	bool rectsDirty = false;
	static std::vector<ImRect> lastFrameRects;
	if (static_cast<U32>(lastFrameRects.size()) != stateMachine.GetStateCount())
	{
		lastFrameRects.resize(stateMachine.GetStateCount());
		rectsDirty = true;
	}

	const U32 stateCount = stateMachine.GetStateCount();
	for (U32 stateIndex = 0; stateIndex < stateCount; ++stateIndex)
	{
		const AnimationStateMachine::State& state = stateMachine.GetState(stateIndex);

        ax::NodeEditor::BeginNode(GetStateID(state, stateMachine));

        ImRect defaultIconRect;
        const bool isDefaultState = (stateMachine.GetDefaultStateIndex() == stateIndex);
        if (isDefaultState)
        {
            ImGui::Text(ICON_FA_SIGN_IN_ALT);
            defaultIconRect = ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
            ImGui::SameLine();
        }

        ImGui::Text("%s", state.GetName().c_str());
        ImRect nodeRect = ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
        if (isDefaultState)
        {
            nodeRect.Add(defaultIconRect.GetTL());
            nodeRect.Add(defaultIconRect.GetBR());
        }

        if (stateMachine.GetAnimation().IsValid() && mDisplayPreviews)
        {
            struct AnimInfo
            {
                U32 clipFrameIndex = 0;
                Time accumulator;
            };
            static std::vector<AnimInfo> animInfos;
            if (stateCount != static_cast<U32>(animInfos.size()))
            {
                animInfos.resize(stateCount);
            }
            
            static constexpr F32 previewSize = 30.0f;
            if (nodeRect.GetSize().x > previewSize)
            {
                const F32 size = nodeRect.GetSize().x - previewSize;
                ImGui::InvisibleButton("", ImVec2(size * 0.5f, previewSize * 0.5f));
                nodeRect.Add(ImGui::GetItemRectMin());
                nodeRect.Add(ImGui::GetItemRectMax());
                ImGui::SameLine();
            }

            ImGui::PreviewAnimationClip(stateMachine.GetAnimation().Get(), previewSize, state.GetClipIndex(), animInfos[stateIndex].clipFrameIndex, animInfos[stateIndex].accumulator, state.GetSpeedScale());
            nodeRect.Add(ImGui::GetItemRectMin());
            nodeRect.Add(ImGui::GetItemRectMax());
        }

		nodeRect.Min.x -= paddingX;
		nodeRect.Min.y -= paddingY;
		nodeRect.Max.x += paddingX;
		nodeRect.Max.y += paddingY;
		const U32 pinCount = stateMachine.GetTransitionFromStateCount(stateIndex) + stateMachine.GetTransitionToStateCount(stateIndex);
		nodeRect.Max.y += pinPaddingY * pinCount;
		if (pinCount > 0)
		{
			nodeRect.Max.y -= pinSpacing * (pinCount - 1);
		}
		lastFrameRects[stateIndex] = nodeRect;
		//ImGui::GetWindowDrawList()->AddRect(nodeRect.Min, nodeRect.Max, IM_COL32(0, 255, 0, 255));

		std::vector<PinInfo> inputPositions;
		std::vector<PinInfo> outputPositions;
		if (!rectsDirty)
		{
			GetStatePinPositions(stateMachine, stateIndex, lastFrameRects, inputPositions, outputPositions);
		}

		const U32 inputCount = static_cast<U32>(inputPositions.size());
		for (U32 input = 0; input < inputCount; ++input)
		{
			const ImVec2& pos = inputPositions[input].pos;
			const ImVec2 posMin = ImVec2(pos.x - pinHalfSize, pos.y - pinHalfSize);
			const ImVec2 posMax = ImVec2(pos.x + pinHalfSize, pos.y + pinHalfSize);

			ax::NodeEditor::PushStyleVar(ax::NodeEditor::StyleVar_PinArrowSize, 10.0f);
			ax::NodeEditor::PushStyleVar(ax::NodeEditor::StyleVar_PinArrowWidth, 10.0f);
			ax::NodeEditor::BeginPin(inputPositions[input].id, ax::NodeEditor::PinKind::Input);
			ax::NodeEditor::PinRect(posMin, posMax);
			ax::NodeEditor::PinPivotRect(posMin, posMax);
			ax::NodeEditor::EndPin();
			ax::NodeEditor::PopStyleVar(2);

			//ImGui::GetWindowDrawList()->AddRect(posMin, posMax, IM_COL32(0, 0, 255, 255));
		}
		const U32 outputCount = static_cast<U32>(outputPositions.size());
		for (U32 output = 0; output < outputCount; ++output)
		{
			const ImVec2& pos = outputPositions[output].pos;
			const ImVec2 posMin = ImVec2(pos.x - pinHalfSize, pos.y - pinHalfSize);
			const ImVec2 posMax = ImVec2(pos.x + pinHalfSize, pos.y + pinHalfSize);

			ax::NodeEditor::BeginPin(outputPositions[output].id, ax::NodeEditor::PinKind::Output);
			ax::NodeEditor::PinRect(posMin, posMax);
			ax::NodeEditor::PinPivotRect(posMin, posMax);
			ax::NodeEditor::EndPin();

			//ImGui::GetWindowDrawList()->AddRect(posMin, posMax, IM_COL32(0, 0, 255, 255));
		}

        ax::NodeEditor::EndNode();
    }

    ax::NodeEditor::PopStyleVar(7);
    ax::NodeEditor::PopStyleColor(2);
	
	if (!rectsDirty)
	{
		const U32 transitionCount = stateMachine.GetTransitionCount();
		for (U32 i = 0; i < transitionCount; ++i)
		{
			const AnimationStateMachine::Transition& transition = stateMachine.GetTransition(i);
			ax::NodeEditor::Link(GetTransitionID(transition, stateMachine),
				GetTransitionInputID(transition, stateMachine),
				GetTransitionOutputID(transition, stateMachine),
				Color::Lime.toImGuiColor(),
				2.0f
			);
		}
	}

	/*
    if (ax::NodeEditor::BeginCreate())
    {
        ax::NodeEditor::PinId inputPinId, outputPinId;
        if (ax::NodeEditor::QueryNewLink(&inputPinId, &outputPinId))
        {
            if (inputPinId && outputPinId)
            {
                if (ax::NodeEditor::AcceptNewItem())
                {
                    // TODO : Get State Indexes
                    // TODO : Add New Transition
                    // TODO : Select the created transition
                }
            }
        }
    }
    ax::NodeEditor::EndCreate();
	*/

	ax::NodeEditor::End();
}

void ImGuiAnimationEditor::GetStatePinPositions(const AnimationStateMachine& stateMachine, const U32 stateIndex, const std::vector<ImRect>& stateRects, std::vector<PinInfo>& inputPins, std::vector<PinInfo>& outputPins)
{
	struct StateTransitionInfo
	{
		std::vector<U32> inputs;
		std::vector<U32> outputs;
	};
	std::vector<StateTransitionInfo> withState;
	withState.resize(stateMachine.GetStateCount());
	const U32 transitionCount = stateMachine.GetTransitionCount();
	for (U32 transitionIndex = 0; transitionIndex < transitionCount; ++transitionIndex)
	{
		const AnimationStateMachine::Transition& transition = stateMachine.GetTransition(transitionIndex);
		if (transition.GetToState() == stateIndex) // Input
		{
			withState[transition.GetFromState()].inputs.push_back(transitionIndex);
		}
		if (transition.GetFromState() == stateIndex) // Output
		{
			withState[transition.GetToState()].outputs.push_back(transitionIndex);
		}
	}
	enAssert(withState[stateIndex].inputs.size() + withState[stateIndex].outputs.size() == 0);

	const U32 stateCount = stateMachine.GetStateCount();
	for (U32 otherStateIndex = 0; otherStateIndex < stateCount; ++otherStateIndex)
	{
		StateTransitionInfo& sti = withState[otherStateIndex];
		const U32 transitions = static_cast<U32>(sti.inputs.size() + sti.outputs.size());
		if (transitions > 0 && otherStateIndex != stateIndex)
		{
			const ImVec2 centerThis = stateRects[stateIndex].GetCenter();
			const ImVec2 centerOther = stateRects[otherStateIndex].GetCenter();
			const ImVec2 delta = ImVec2(centerOther.x - centerThis.x, centerOther.y - centerThis.y);
			const F32 deltaLength = Math::Sqrt(delta.x * delta.x + delta.y * delta.y);
			if (deltaLength)
			{
				const ImVec2 deltaNormalized = ImVec2(delta.x / deltaLength, delta.y / deltaLength);
				const ImVec2 cross = ImVec2(deltaNormalized.y, -deltaNormalized.x);
				static constexpr F32 diff = 10.0f;
				const F32 factor = ((transitions % 2) == 0) ? diff * ((transitions * 0.5f) - 0.5f) : diff * (transitions - 1) * 0.5f;
				const ImVec2 startPos = ImVec2(centerThis.x - cross.x * factor, centerThis.y - cross.y * factor);
				const ImVec2 startPosOther = ImVec2(centerOther.x - cross.x * factor, centerOther.y - cross.y * factor);

				const bool useCorrectedPosition = !stateRects[stateIndex].Overlaps(stateRects[otherStateIndex]);

				std::sort(sti.inputs.begin(), sti.inputs.end());
				inputPins.reserve(sti.inputs.size());
				const U32 stiInputCount = static_cast<U32>(sti.inputs.size());
				for (U32 stindex = 0; stindex < stiInputCount; ++stindex)
				{
					PinInfo pinInfo;
					pinInfo.id = GetTransitionInputID(stateMachine.GetTransition(sti.inputs[stindex]), stateMachine);
					const ImVec2 pinPos = ImVec2(stindex * diff * cross.x + startPos.x, stindex * diff * cross.y + startPos.y);
					if (useCorrectedPosition)
					{
						const ImVec2 otherPinPos = ImVec2(stindex * diff * cross.x + startPosOther.x, stindex * diff * cross.y + startPosOther.y);
						pinInfo.pos = GetCorrectedPinPosition(pinPos, otherPinPos, stateRects[stateIndex]);
					}
					else
					{
						pinInfo.pos = pinPos;
					}
					inputPins.emplace_back(pinInfo);
				}

				std::sort(sti.outputs.begin(), sti.outputs.end(), [](U32 a, U32 b) { return a > b; }); // Reverse to match the inputs on other side
				outputPins.reserve(sti.outputs.size());
				const U32 stiOutputCount = static_cast<U32>(sti.outputs.size());
				for (U32 stindex = 0; stindex < stiOutputCount; ++stindex)
				{
					PinInfo pinInfo;
					pinInfo.id = GetTransitionOutputID(stateMachine.GetTransition(sti.outputs[stindex]), stateMachine);
					const ImVec2 pinPos = ImVec2((stiInputCount + stindex) * diff * cross.x + startPos.x, (stiInputCount + stindex) * diff * cross.y + startPos.y);
					if (useCorrectedPosition)
					{
						const ImVec2 otherPinPos = ImVec2((stiInputCount + stindex) * diff * cross.x + startPosOther.x, (stiInputCount + stindex) * diff * cross.y + startPosOther.y);
						pinInfo.pos = GetCorrectedPinPosition(pinPos, otherPinPos, stateRects[stateIndex]);
					}
					else
					{
						pinInfo.pos = pinPos;
					}
					outputPins.emplace_back(pinInfo);
				}
			}
		}
	}
}

ImVec2 ImGuiAnimationEditor::GetCorrectedPinPosition(const ImVec2& pinPos, const ImVec2& otherPinPos, const ImRect& pinRect)
{
	const F32 deltaExpand = 2.0f;
	ImRect largerRect = pinRect;
	largerRect.Expand(deltaExpand);

	bool validIntersections[4];
	ImVec2 intersections[4];
	validIntersections[0] = GetLineIntersection(pinPos, otherPinPos, pinRect.GetTL(), pinRect.GetTR(), intersections[0]);
	validIntersections[1] = GetLineIntersection(pinPos, otherPinPos, pinRect.GetTR(), pinRect.GetBR(), intersections[1]);
	validIntersections[2] = GetLineIntersection(pinPos, otherPinPos, pinRect.GetBR(), pinRect.GetBL(), intersections[2]);
	validIntersections[3] = GetLineIntersection(pinPos, otherPinPos, pinRect.GetBL(), pinRect.GetTL(), intersections[3]);
	U32 bestIndex = 4;
	F32 bestDistanceSqr = 9999999999999.9f;
	for (U32 i = 0; i < 4; ++i)
	{
		if (validIntersections[i] && largerRect.Contains(intersections[i]))
		{
			const ImVec2 delta = ImVec2(otherPinPos.x - intersections[i].x, otherPinPos.y - intersections[i].y);
			const F32 distanceSqr = (delta.x * delta.x + delta.y * delta.y);
			if (distanceSqr < bestDistanceSqr)
			{
				bestDistanceSqr = distanceSqr;
				bestIndex = i;
			}
		}
	}
	if (bestIndex < 4)
	{
		return intersections[bestIndex];
	}
	return pinPos;
}

bool ImGuiAnimationEditor::GetLineIntersection(const ImVec2& lineA1, const ImVec2& lineA2, const ImVec2& lineB1, const ImVec2& lineB2, ImVec2& intersection)
{
	const float A1 = lineA2.y - lineA1.y;
	const float B1 = lineA1.x - lineA2.x;
	const float A2 = lineB2.y - lineB1.y;
	const float B2 = lineB1.x - lineB2.x;
	const float delta = A1 * B2 - A2 * B1;
	if (delta == 0.0f)
	{
		return false;
	}
	const float invdelta = 1.0f / delta;
	const float C2 = A2 * lineB1.x + B2 * lineB1.y;
	const float C1 = A1 * lineA1.x + B1 * lineA1.y;
	intersection = ImVec2((B2 * C1 - B1 * C2) * invdelta, (A1 * C2 - A2 * C1) * invdelta);
	return true;
}

void ImGuiAnimationEditor::AnimationStateMachineList()
{
	// TODO : AnimationStateMachine list
	ImGui::Text("In future update, we will have the list of \nall the AnimationStateMachine here\n\nBut unfortunatelly, you will have to wait\nand use the ResourceBrowser...\n\nSorry for the incovenience.");
}

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
