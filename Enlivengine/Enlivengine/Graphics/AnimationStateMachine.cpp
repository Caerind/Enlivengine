#include <Enlivengine/Graphics/AnimationStateMachine.hpp>

#include <Enlivengine/System/ParserXml.hpp>

namespace en
{

AnimationStateMachine::State::State(const std::string& name, U32 clipIndex)
	: mName(name)
	, mHashedName(Hash::CRC32(name.c_str()))
	, mClipIndex(clipIndex)
	, mSpeedScale(1.0f)
	, mExitOnlyAtEnd(false)
	, mFirstOutputTransitionIndex(U32_Max)
	, mBlendStateInfo(nullptr)
{
}

void AnimationStateMachine::State::SetName(const std::string& name)
{
    const U32 hashedName = Hash::CRC32(name.c_str());
    if (mHashedName != hashedName)
    {
        mName = name;
        mHashedName = hashedName;
    }
}

void AnimationStateMachine::State::CreateBlendStateInfo(U32 dimension)
{
	if (HasBlendStateInfo())
	{
		RemoveBlendStateInfo();
	}
	mBlendStateInfo = new BlendStateInfo(dimension);
}

void AnimationStateMachine::State::RemoveBlendStateInfo()
{
	if (HasBlendStateInfo())
	{
		delete mBlendStateInfo;
		mBlendStateInfo = nullptr;
	}
}

AnimationStateMachine::State::BlendStateInfo::Motion::Motion(U32 clipIndex, U32 dimensions)
{
	mClipIndex = clipIndex;
	mValues.resize(dimensions);
}

void AnimationStateMachine::State::BlendStateInfo::Motion::SetValue(U32 dimensionIndex, F32 value)
{
	assert(dimensionIndex < GetValueCount());
	mValues[dimensionIndex] = value;
}

F32 AnimationStateMachine::State::BlendStateInfo::Motion::GetValue(U32 dimensionIndex) const
{
	assert(dimensionIndex < GetValueCount());
	return mValues[dimensionIndex];
}

AnimationStateMachine::State::BlendStateInfo::BlendStateInfo(U32 dimension)
{
	mParameters.resize(dimension);
}

void AnimationStateMachine::State::BlendStateInfo::SetParameter(U32 dimensionIndex, U32 parameterIndex)
{
	assert(dimensionIndex < GetDimension());
	mParameters[dimensionIndex] = parameterIndex;
}

U32 AnimationStateMachine::State::BlendStateInfo::GetParameter(U32 dimensionIndex) const
{
	assert(dimensionIndex < GetDimension());
	return mParameters[dimensionIndex];
}

U32 AnimationStateMachine::State::BlendStateInfo::AddMotion(U32 clipIndex)
{
	mMotions.emplace_back(clipIndex, GetDimension());
	return static_cast<U32>(GetMotionCount() - 1);
}

const AnimationStateMachine::State::BlendStateInfo::Motion& AnimationStateMachine::State::BlendStateInfo::GetMotion(U32 motionIndex) const
{
	assert(motionIndex < GetMotionCount());
	return mMotions[motionIndex];
}

void AnimationStateMachine::State::BlendStateInfo::SetMotionValue(U32 motionIndex, U32 dimensionIndex, F32 value)
{
	assert(motionIndex < GetMotionCount());
	mMotions[motionIndex].SetValue(dimensionIndex, value);
}

void AnimationStateMachine::State::BlendStateInfo::RemoveMotion(U32 motionIndex)
{
	assert(motionIndex < GetMotionCount());
	mMotions.erase(mMotions.begin() + motionIndex);
}

void AnimationStateMachine::State::BlendStateInfo::ClearMotions()
{
	mMotions.clear();
}

AnimationStateMachine::Parameter::Parameter(const std::string& name, Parameter::Type type)
{
    SetName(name);
	SetType(type);
}

void AnimationStateMachine::Parameter::SetName(const std::string& name)
{
    const U32 hashedName = Hash::CRC32(name.c_str());
    if (mHashedName != hashedName)
    {
        mName = name;
        mHashedName = hashedName;
    }
}

void AnimationStateMachine::Parameter::SetType(Parameter::Type type)
{
	assert(type < Parameter::Type::Count);
	if (mType != type)
	{
		mType = type;

		switch (type)
		{
        case Parameter::Type::Boolean: SetBooleanValue(false); break;
        case Parameter::Type::Float: SetFloatValue(0.0f); break;
        case Parameter::Type::Integer: SetIntegerValue(0); break;
        case Parameter::Type::Trigger: SetTriggerValue(false); break;
		default: assert(false); break;
		}
	}
}

void AnimationStateMachine::Parameter::SetBooleanValue(bool value)
{
	assert(mType == Parameter::Type::Boolean);
	mValue.bValue = value;
}

void AnimationStateMachine::Parameter::SetFloatValue(F32 value)
{
	assert(mType == Parameter::Type::Float);
	mValue.fValue = value;
}

void AnimationStateMachine::Parameter::SetIntegerValue(I32 value)
{
	assert(mType == Parameter::Type::Integer);
	mValue.iValue = value;
}

void AnimationStateMachine::Parameter::SetTriggerValue(bool value)
{
	assert(mType == Parameter::Type::Trigger);
	mValue.bValue = value;
}

bool AnimationStateMachine::Parameter::GetBooleanValue() const
{
	assert(mType == Parameter::Type::Boolean);
	return mValue.bValue;
}

F32 AnimationStateMachine::Parameter::GetFloatValue() const
{
	assert(mType == Parameter::Type::Float);
	return mValue.fValue;
}

I32 AnimationStateMachine::Parameter::GetIntegerValue() const
{
	assert(mType == Parameter::Type::Integer);
	return mValue.iValue;
}

bool AnimationStateMachine::Parameter::GetTriggerValue() const
{
	assert(mType == Parameter::Type::Trigger);
	return mValue.bValue;
}

AnimationStateMachine::Condition::Condition(U32 parameterIndex)
{
	mParameterIndex = parameterIndex;
    mOperator = Operator::Equal;
    mOperand.bValue = false;
}

bool AnimationStateMachine::Condition::Evaluate(const AnimationStateMachine::Parameter& parameter) const
{
	switch (parameter.GetType())
	{
	case Parameter::Type::Boolean: return EvaluateBoolean(parameter.GetBooleanValue());
	case Parameter::Type::Float: return EvaluateFloat(parameter.GetFloatValue());
	case Parameter::Type::Integer: return EvaluateInteger(parameter.GetIntegerValue());
	case Parameter::Type::Trigger: return EvaluateTrigger(parameter.GetTriggerValue());
        default: assert(false); break;
	}
	return false;
}

bool AnimationStateMachine::Condition::EvaluateBoolean(bool value) const
{
	const bool bValue = GetOperandBoolean();
	switch (mOperator)
	{
	case Operator::Equal:
	case Operator::LessEq:
	case Operator::GreaterEq: return value == bValue;
	case Operator::NotEqual: 
	case Operator::Less:
	case Operator::Greater: return value != bValue;
	}
	return false;
}

bool AnimationStateMachine::Condition::EvaluateFloat(F32 value) const
{
	const F32 fValue = GetOperandFloat();
	switch (mOperator)
	{
	case Operator::Equal: return value == fValue;
	case Operator::NotEqual: return value != fValue;
	case Operator::Less: return value < fValue;
	case Operator::LessEq: return value <= fValue;
	case Operator::Greater: return value > fValue;
	case Operator::GreaterEq: return value >= fValue;
	}
	return false;
}

bool AnimationStateMachine::Condition::EvaluateInteger(I32 value) const
{
	const I32 iValue = GetOperandInteger();
	switch (mOperator)
	{
	case Operator::Equal: return value == iValue;
	case Operator::NotEqual: return value != iValue;
	case Operator::Less: return value < iValue;
	case Operator::LessEq: return value <= iValue;
	case Operator::Greater: return value > iValue;
	case Operator::GreaterEq: return value >= iValue;
	}
	return false;
}

bool AnimationStateMachine::Condition::EvaluateTrigger(bool value) const
{
	return value;
}

AnimationStateMachine::Transition::Transition(U32 fromState, U32 toState)
	: mFromState(fromState)
	, mToState(toState)
	, mExitOnlyAtEnd(true)
{
}

U32 AnimationStateMachine::Transition::GetConditionCount() const
{
	return static_cast<U32>(mConditions.size());
}

U32 AnimationStateMachine::Transition::GetCondition(U32 conditionIndexInTransition) const
{
	assert(conditionIndexInTransition < GetConditionCount());
	return mConditions[conditionIndexInTransition];
}

void AnimationStateMachine::Transition::SetCondition(U32 conditionIndexInTransition, U32 conditionIndex)
{
	assert(conditionIndexInTransition < GetConditionCount());
	mConditions[conditionIndexInTransition] = conditionIndex;
}

bool AnimationStateMachine::Transition::HasCondition(U32 conditionIndex) const
{
	const U32 conditionCount = GetConditionCount();
	for (U32 i = 0; i < conditionCount; ++i)
	{
		if (mConditions[i] == conditionIndex)
		{
			return true;
		}
	}
	return false;
}

U32 AnimationStateMachine::Transition::AddCondition(U32 conditionIndex)
{
	for (U32 conditionI : mConditions)
	{
		if (conditionI == conditionIndex)
		{
			return conditionI;
		}
	}
	mConditions.push_back(conditionIndex);
	return GetConditionCount() - 1;
}

void AnimationStateMachine::Transition::RemoveCondition(U32 conditionIndex)
{
	const U32 conditionCount = GetConditionCount();
	for (U32 i = 0; i < conditionCount; ++i)
	{
		if (mConditions[i] == conditionIndex)
		{
			mConditions.erase(mConditions.begin() + i);
			return;
		}
	}
}

void AnimationStateMachine::Transition::ClearConditions()
{
	mConditions.clear();
}

AnimationStateMachine::AnimationStateMachine()
    : mDefaultStateIndex(0)
	, mDirtyIndex(0)
{
}

bool AnimationStateMachine::LoadFromFile(const std::string& filename)
{
	Clear();

	mDirtyIndex++;

    std::string str = filename.substr(0, filename.size() - std::string("astm").size());
    str += "json";
    mAnimation = ResourceManager::GetInstance().GetFromFilename<Animation>(str);
    if (!mAnimation.IsValid())
    {
        return false;
    }

    ParserXml xml;
    if (!xml.loadFromFile(filename))
    {
        return false;
    }
    if (!xml.readNode("AnimationStateMachine"))
    {
        return false;
    }

    std::string animationIdentifier;
    xml.getAttribute("animation", animationIdentifier);
    // TODO : Maybe we can do something with it ?

    U32 stateCount = 0;
    U32 parameterCount = 0;
    U32 conditionCount = 0;
    U32 transitionCount = 0;

    // States
    if (xml.readNode("States"))
    {
        xml.getAttribute("count", stateCount);
        if (xml.readNode("State"))
        {
            do
            {
                std::string name = "";
                xml.getAttribute("name", name);
                U32 clipIndex = 0;
                xml.getAttribute("clipIndex", clipIndex);
                F32 speedScale = 1.0f;
                xml.getAttribute("speedScale", speedScale);
                I32 exitOnlyAtEnd = static_cast<I32>(true);
                xml.getAttribute("exitOnlyAtEnd", exitOnlyAtEnd);
				U32 firstOutputTransition = U32_Max;
				xml.getAttribute("firstOutputTransition", firstOutputTransition);

                State state(name, clipIndex);
                state.SetSpeedScale(speedScale);
                state.SetExitOnlyAtEnd(static_cast<bool>(exitOnlyAtEnd));
				state.SetFirstOutputTransitionIndex(firstOutputTransition);

				// BlendState
				if (xml.readNode("BlendState"))
				{
					U32 dimension = 1;
					xml.getAttribute("dimension", dimension);
					if (dimension >= 1)
					{
						state.CreateBlendStateInfo(dimension);

						if (xml.readNode("Parameters"))
						{
							if (xml.readNode("Parameter"))
							{
								do
								{
									U32 parameterIndexInBlendState = 0;
									xml.getAttribute("index", parameterIndexInBlendState);
									U32 parameterIndex = 0;
									xml.getAttribute("parameterIndex", parameterIndex);
									state.GetBlendStateInfo()->SetParameter(parameterIndexInBlendState, parameterIndex);

								} while (xml.nextSibling("Parameter"));
								xml.closeNode();
							}
							xml.closeNode();
						}

						if (xml.readNode("Motions"))
						{
							U32 motionCount = 0;
							xml.getAttribute("count", motionCount);
							if (xml.readNode("Motion"))
							{
								do
								{
									U32 motionClipIndex;
									xml.getAttribute("clipIndex", motionClipIndex);
									U32 motionValueCount;
									xml.getAttribute("values", motionValueCount);

									const U32 motionIndex = state.GetBlendStateInfo()->AddMotion(motionClipIndex);

									if (xml.readNode("Value"))
									{
										do
										{
											U32 motionValueDimension = 0;
											xml.getAttribute("index", motionValueDimension);
											F32 motionValue = 0.0f;
											xml.getAttribute("value", motionValue);
											state.GetBlendStateInfo()->SetMotionValue(motionIndex, motionValueDimension, motionValue);

										} while (xml.nextSibling("Value"));
										xml.closeNode();
									}

								} while (xml.nextSibling("Motion"));
								xml.closeNode();
							}
							xml.closeNode();
						}

					}

					xml.closeNode();
				}

                mStates.push_back(state);

            } while (xml.nextSibling("State"));
            xml.closeNode();
        }
        xml.closeNode();
    }

    // Parameters
    if (xml.readNode("Parameters"))
    {
        xml.getAttribute("count", parameterCount);
        if (xml.readNode("Parameter"))
        {
            do
            {
                std::string name = "";
                xml.getAttribute("name", name);
                U32 typeIndex = 0;
                xml.getAttribute("type", typeIndex);
                Parameter::Type type = static_cast<Parameter::Type>(typeIndex);

                Parameter parameter(name, type);
                switch (type)
                {
                case Parameter::Type::Boolean:
                {
                    I32 defaultValue = static_cast<I32>(false);
                    xml.getAttribute("defaultValue", defaultValue);
                    parameter.SetBooleanValue(static_cast<bool>(defaultValue));
                } break;
                case Parameter::Type::Float:
                {
                    F32 defaultValue = 0.0f;
                    xml.getAttribute("defaultValue", defaultValue);
                    parameter.SetFloatValue(defaultValue);
                } break;
                case Parameter::Type::Integer:
                {
                    I32 defaultValue = 0;
                    xml.getAttribute("defaultValue", defaultValue);
                    parameter.SetIntegerValue(defaultValue);
                } break;
                case Parameter::Type::Trigger:
                {
                    I32 defaultValue = static_cast<I32>(false);
                    xml.getAttribute("defaultValue", defaultValue);
                    parameter.SetTriggerValue(static_cast<bool>(defaultValue));
                } break;
                default: break;
                }

                mParameters.push_back(parameter);

            } while (xml.nextSibling("Parameter"));
            xml.closeNode();
        }
        xml.closeNode();
    }

    // Conditions
    if (xml.readNode("Conditions"))
    {
        xml.getAttribute("count", conditionCount);
        if (xml.readNode("Condition"))
        {
            do
            {
                U32 parameterIndex = 0;
                xml.getAttribute("parameterIndex", parameterIndex);
                U32 operatorIndex = 0;
                xml.getAttribute("operator", operatorIndex);
                Condition::Operator operatorType = static_cast<Condition::Operator>(operatorIndex);

                Condition condition(parameterIndex);
                condition.SetOperator(operatorType);
                switch (GetParameter(parameterIndex).GetType())
                {
                case Parameter::Type::Boolean:
                {
                    I32 operandValue = static_cast<I32>(false);
                    xml.getAttribute("operandValue", operandValue);
                    condition.SetOperandBoolean(static_cast<bool>(operandValue));
                } break;
                case Parameter::Type::Float:
                {
                    F32 operandValue = 0.0f;
                    xml.getAttribute("operandValue", operandValue);
                    condition.SetOperandFloat(operandValue);
                } break;
                case Parameter::Type::Integer:
                {
                    I32 defaultValue = 0;
                    xml.getAttribute("operandValue", defaultValue);
                    condition.SetOperandInteger(defaultValue);
                } break;
                case Parameter::Type::Trigger: break;
                default: break;
                }

                mConditions.push_back(condition);

            } while (xml.nextSibling("Condition"));
            xml.closeNode();
        }
        xml.closeNode();
    }

    // Transitions
    if (xml.readNode("Transitions"))
    {
        xml.getAttribute("count", transitionCount);
        if (xml.readNode("Transition"))
        {
            do
            {
                U32 fromState = 0;
                xml.getAttribute("fromState", fromState);
                U32 toState = 0;
                xml.getAttribute("toState", toState);
                I32 exitOnlyAtEnd = static_cast<I32>(true);
                xml.getAttribute("exitOnlyAtEnd", exitOnlyAtEnd);
                U32 conditionInTransitionCount = 0;
                xml.getAttribute("conditionCount", conditionInTransitionCount);

                Transition transition(fromState, toState);
                transition.SetExitOnlyAtEnd(static_cast<bool>(exitOnlyAtEnd));

                if (xml.readNode("Condition"))
                {
                    do 
                    {
                        U32 conditionIndex = 0;
                        xml.getAttribute("conditionIndex", conditionIndex);

                        transition.AddCondition(conditionIndex);

                    } while (xml.nextSibling("Condition"));
                    xml.closeNode();
                }

                mTransitions.push_back(transition);

            } while (xml.nextSibling("Transition"));
            xml.closeNode();
        }
        xml.closeNode();
    }

    xml.closeNode();

    return true;
}

bool AnimationStateMachine::SaveToFile(const std::string& filename)
{
	Precompute();

    ParserXml xml;
    xml.newFile();

    if (!xml.createChild("AnimationStateMachine"))
    {
        return false;
    }

    if (mAnimation.IsValid())
    {
        xml.setAttribute("animation", mAnimation.Get().GetIdentifier());
    }
    else
    {
        xml.setAttribute("animation", "");
    }

    const U32 stateCount = GetStateCount();
    const U32 parameterCount = GetParameterCount();
    const U32 conditionCount = GetConditionCount();
    const U32 transitionCount = GetTransitionCount();

    // States
    if (xml.createChild("States"))
    {
        xml.setAttribute("count", stateCount);
        xml.setAttribute("defaultIndex", mDefaultStateIndex);
        for (U32 i = 0; i < stateCount; ++i)
        {
            if (!xml.createChild("State"))
            {
                continue;
            }
            const State& state = GetState(i);
            xml.setAttribute("name", state.GetName());
            xml.setAttribute("clipIndex", state.GetClipIndex());
            xml.setAttribute("speedScale", state.GetSpeedScale());
            xml.setAttribute("exitOnlyAtEnd", state.GetExitOnlyAtEnd());
			xml.setAttribute("firstOutputTransition", state.GetFirstOutputTransitionIndex());

            if (state.HasBlendStateInfo())
            {
                if (!xml.createChild("BlendState"))
                {
                    continue;
                }
                const State::BlendStateInfo* blendState = state.GetBlendStateInfo();
                xml.setAttribute("dimension", blendState->GetDimension());

				if (!xml.createChild("Parameters"))
				{
					continue;
				}
				xml.setAttribute("count", blendState->GetDimension());
				for (U32 j = 0; j < blendState->GetDimension(); ++j)
				{
					if (!xml.createChild("Parameter"))
					{
						continue;
					}
					xml.setAttribute("index", j);
					xml.setAttribute("parameterIndex", blendState->GetParameter(j));
					xml.closeNode();
				}
				xml.closeNode(); // Parameters

				if (!xml.createChild("Motions"))
				{
					continue;
				}
				xml.setAttribute("count", blendState->GetMotionCount());
				for (U32 j = 0; j < blendState->GetMotionCount(); ++j)
				{
					if (!xml.createChild("Motion"))
					{
						continue;
					}
					xml.setAttribute("index", j);
					xml.setAttribute("clipIndex", blendState->GetMotion(j).GetClipIndex());
					xml.setAttribute("values", blendState->GetMotion(j).GetValueCount());
					for (U32 k = 0; k < blendState->GetMotion(j).GetValueCount(); ++k)
					{
						if (!xml.createChild("Value"))
						{
							continue;
						}
						xml.setAttribute("index", k);
						xml.setAttribute("value", blendState->GetMotion(j).GetValue(k));
						xml.closeNode();
					}
					xml.closeNode();
				}
				xml.closeNode(); // Motions

                xml.closeNode(); // BlendState
            }

            xml.closeNode();
        }
        xml.closeNode();
    }

    // Parameters
    if (xml.createChild("Parameters"))
    {
        xml.setAttribute("count", parameterCount);
        for (U32 i = 0; i < parameterCount; ++i)
        {
            if (!xml.createChild("Parameter"))
            {
                continue;
            }
            const Parameter& parameter = GetParameter(i);
            xml.setAttribute("name", parameter.GetName());
            xml.setAttribute("type", static_cast<I32>(parameter.GetType()));
            switch (parameter.GetType())
            {
            case Parameter::Type::Boolean: xml.setAttribute("defaultValue", parameter.GetBooleanValue()); break;
            case Parameter::Type::Float: xml.setAttribute("defaultValue", parameter.GetFloatValue()); break;
            case Parameter::Type::Integer: xml.setAttribute("defaultValue", parameter.GetIntegerValue()); break;
            case Parameter::Type::Trigger: xml.setAttribute("defaultValue", parameter.GetTriggerValue()); break;
            default: break;
            }

            xml.closeNode();
        }
        xml.closeNode();
    }

    // Conditions
    if (xml.createChild("Conditions"))
    {
        xml.setAttribute("count", conditionCount);
        for (U32 i = 0; i < conditionCount; ++i)
        {
            if (!xml.createChild("Condition"))
            {
                continue;
            }
            const Condition& condition = GetCondition(i);
            xml.setAttribute("parameterIndex", condition.GetParameterIndex());
            xml.setAttribute("operator", static_cast<I32>(condition.GetOperator()));
            switch (GetParameter(condition.GetParameterIndex()).GetType())
            {
            case Parameter::Type::Boolean: xml.setAttribute("operandValue", condition.GetOperandBoolean()); break;
            case Parameter::Type::Float: xml.setAttribute("operandValue", condition.GetOperandFloat()); break;
            case Parameter::Type::Integer: xml.setAttribute("operandValue", condition.GetOperandInteger()); break;
            default: break;
            }

            xml.closeNode();
        }
        xml.closeNode();
    }

    // Transitions
    if (xml.createChild("Transitions"))
    {
        xml.setAttribute("count", transitionCount);
        for (U32 i = 0; i < transitionCount; ++i)
        {
            if (!xml.createChild("Transition"))
            {
                continue;
            }
            const Transition& transition = GetTransition(i);
            xml.setAttribute("fromState", transition.GetFromState());
            xml.setAttribute("toState", transition.GetToState());
            xml.setAttribute("exitOnlyAtEnd", transition.GetExitOnlyAtEnd());
            const U32 conditionCountInTransition = transition.GetConditionCount();
            xml.setAttribute("conditionCount", conditionCountInTransition);
            if (transition.GetConditionCount() > 0)
            {
                for (U32 j = 0; j < conditionCountInTransition; ++j)
                {
                    if (!xml.createChild("Condition"))
                    {
                        continue;
                    }
                    xml.setAttribute("conditionIndex", transition.GetCondition(j));
                    xml.closeNode();
                }
            }
            xml.closeNode();
        }   
        xml.closeNode();
    }

    if (!xml.saveToFile(filename))
    {
        return false;
    }

    return true;
}

void AnimationStateMachine::Precompute()
{
	mDirtyIndex++; // TODO : Only increment if really changed something

	// Fallback default state
	if (mDefaultStateIndex >= GetStateCount())
	{
		mDefaultStateIndex = 0;
	}

	// Sort transitions by FromState for faster iterations in Controller
	std::sort(mTransitions.begin(), mTransitions.end(), [](const Transition& a, const Transition& b)
	{
		if (a.GetFromState() != b.GetFromState())
		{
			return a.GetFromState() < b.GetFromState();
		}
		return a.GetToState() < b.GetToState();
	});

	// Save if state can only exit at end or not depending on its transitions
	const U32 stateCount = GetStateCount();
	const U32 transitionCount = GetTransitionCount();
	for (U32 stateIndex = 0; stateIndex < stateCount; ++stateIndex)
	{
		mStates[stateIndex].SetExitOnlyAtEnd(true);
		mStates[stateIndex].SetFirstOutputTransitionIndex(U32_Max);
		for (U32 transitionIndex = 0; transitionIndex < transitionCount && mStates[stateIndex].GetExitOnlyAtEnd() && mTransitions[transitionIndex].GetFromState() <= stateIndex; ++transitionIndex)
		{
			if (mTransitions[transitionIndex].GetFromState() == stateIndex)
			{
				mStates[stateIndex].SetFirstOutputTransitionIndex(transitionIndex);
				if (!mTransitions[transitionIndex].GetExitOnlyAtEnd())
				{
					mStates[stateIndex].SetExitOnlyAtEnd(false);
				}
			}
		}
	}

	// TODO : Remove conditions duplicates
}

void AnimationStateMachine::Clear()
{
	mDirtyIndex++;

	mParameters.clear();
	mConditions.clear();
	mTransitions.clear();
	mStates.clear();
}

void AnimationStateMachine::SetAnimation(AnimationPtr animation)
{
	if (mAnimation != animation)
	{
		mAnimation = animation;
		mDirtyIndex++;
	}
}

const AnimationPtr& AnimationStateMachine::GetAnimation() const
{
    return mAnimation;
}

U32 AnimationStateMachine::AddState(const std::string& name, U32 clipIndex)
{
	mDirtyIndex++;
	assert(mAnimation.IsValid());
	assert(clipIndex < mAnimation.Get().GetClipCount());
	mStates.emplace_back(name, clipIndex);
	return GetStateCount() - 1;
}

void AnimationStateMachine::RemoveState(U32 index)
{
	mDirtyIndex++;
	assert(index < GetStateCount());

	// Remove all transitions from/to this state
	U32 transitionCount = GetTransitionCount();
	for (U32 i = 0; i < transitionCount; )
	{
		Transition& transtion = mTransitions[i];
		const U32 fromState = transtion.GetFromState();
		const U32 toState = transtion.GetToState();
		if (fromState == index || toState == index)
		{
			RemoveTransition(i);
			--transitionCount;
		}
		else
		{
			if (fromState > index)
			{
				transtion.SetFromState(fromState - 1);
			}
			if (toState > index)
			{
				transtion.SetToState(toState - 1);
			}
			++i;
		}
	}

	mStates.erase(mStates.begin() + index);
}

void AnimationStateMachine::ClearStates()
{
	mDirtyIndex++;
	mStates.clear();
	ClearTransitions();
}

void AnimationStateMachine::SetStateName(U32 index, const std::string& name)
{
	mDirtyIndex++;
	assert(index < GetStateCount());
	mStates[index].SetName(name);
}

void AnimationStateMachine::SetStateClipIndex(U32 index, U32 clipIndex)
{
	mDirtyIndex++;
	assert(index < GetStateCount());
	assert(mAnimation.IsValid());
	assert(clipIndex < mAnimation.Get().GetClipCount()); // TODO : Check anim is valid
	mStates[index].SetClipIndex(clipIndex);
}

void AnimationStateMachine::SetStateSpeedScale(U32 index, F32 speedScale)
{
	mDirtyIndex++;
	assert(index < GetStateCount());
	mStates[index].SetSpeedScale(speedScale);
}

U32 AnimationStateMachine::GetStateCount() const
{
	return static_cast<U32>(mStates.size());
}

const AnimationStateMachine::State& AnimationStateMachine::GetState(U32 index) const
{
	assert(index < GetStateCount());
    return mStates[index];
}

U32 AnimationStateMachine::GetStateIndexByName(const std::string& name) const
{
	return GetStateIndexByName(Hash::CRC32(name.c_str()));
}

U32 AnimationStateMachine::GetStateIndexByName(U32 hashedName) const
{
	const U32 stateCount = GetStateCount();
	for (U32 i = 0; i < stateCount; ++i)
	{
		if (mStates[i].GetHashedName() == hashedName)
		{
			return i;
		}
	}
	return U32_Max;
}

void AnimationStateMachine::AddBlendStateToState(U32 stateIndex, U32 dimension)
{
	mDirtyIndex++;
	assert(stateIndex < GetStateCount());
	assert(!mStates[stateIndex].HasBlendStateInfo());
	mStates[stateIndex].CreateBlendStateInfo(dimension);
}

void AnimationStateMachine::RemoveBlendStateFromState(U32 stateIndex)
{
	mDirtyIndex++;
	assert(stateIndex < GetStateCount());
	assert(mStates[stateIndex].HasBlendStateInfo());
	mStates[stateIndex].RemoveBlendStateInfo();
}

void AnimationStateMachine::SetBlendStateParameter(U32 stateIndex, U32 dimensionIndex, U32 parameterIndex)
{
	mDirtyIndex++;
	assert(stateIndex < GetStateCount());
	assert(mStates[stateIndex].HasBlendStateInfo());
	assert(dimensionIndex < mStates[stateIndex].GetBlendStateInfo()->GetDimension());
	assert(parameterIndex < GetParameterCount());
	assert(GetParameter(parameterIndex).GetType() == Parameter::Type::Float || GetParameter(parameterIndex).GetType() == Parameter::Type::Integer);
	mStates[stateIndex].GetBlendStateInfo()->SetParameter(dimensionIndex, parameterIndex);
}

U32 AnimationStateMachine::AddBlendStateMotion(U32 stateIndex, U32 clipIndex)
{
	mDirtyIndex++;
	assert(stateIndex < GetStateCount());
	assert(mStates[stateIndex].HasBlendStateInfo());
	assert(clipIndex < mAnimation.Get().GetClipCount()); // TODO : Check anim is valid
	return mStates[stateIndex].GetBlendStateInfo()->AddMotion(clipIndex);
}

void AnimationStateMachine::SetBlendStateMotionValue(U32 stateIndex, U32 motionIndex, U32 dimensionIndex, F32 value)
{
	mDirtyIndex++;
	assert(stateIndex < GetStateCount());
	assert(mStates[stateIndex].HasBlendStateInfo());
	assert(motionIndex < mStates[stateIndex].GetBlendStateInfo()->GetMotionCount());
	assert(dimensionIndex < mStates[stateIndex].GetBlendStateInfo()->GetDimension());
	mStates[stateIndex].GetBlendStateInfo()->SetMotionValue(motionIndex, dimensionIndex, value);
}

void AnimationStateMachine::RemoveBlendStateMotion(U32 stateIndex, U32 motionIndex)
{
	mDirtyIndex++;
	assert(stateIndex < GetStateCount());
	assert(mStates[stateIndex].HasBlendStateInfo());
	assert(motionIndex < mStates[stateIndex].GetBlendStateInfo()->GetMotionCount());
	mStates[stateIndex].GetBlendStateInfo()->RemoveMotion(motionIndex);
}

void AnimationStateMachine::ClearBlendStateMotions(U32 stateIndex)
{
	mDirtyIndex++;
	assert(stateIndex < GetStateCount());
	assert(mStates[stateIndex].HasBlendStateInfo());
	mStates[stateIndex].GetBlendStateInfo()->ClearMotions();
}

U32 AnimationStateMachine::AddParameter(const std::string& name, Parameter::Type type)
{
	mDirtyIndex++;
	assert(type < Parameter::Type::Count);
	mParameters.emplace_back(name, type);
	return GetParameterCount() - 1;
}

void AnimationStateMachine::RemoveParameter(U32 index)
{
	mDirtyIndex++;
	assert(index < GetParameterCount());
	mParameters.erase(mParameters.begin() + index);
	
	U32 conditionCount = GetConditionCount();
	for (U32 i = 0; i < conditionCount; )
	{
		if (mConditions[i].GetParameterIndex() == index)
		{
			RemoveCondition(i);
			--conditionCount;
		}
		else
		{
			++i;
		}
	}
}

void AnimationStateMachine::ClearParameters()
{
	mDirtyIndex++;
	mParameters.clear();
	ClearConditions();
}

void AnimationStateMachine::SetParameterName(U32 index, const std::string& name)
{
	mDirtyIndex++;
	assert(index < GetParameterCount());
	mParameters[index].SetName(name);
}

void AnimationStateMachine::SetParameterType(U32 index, AnimationStateMachine::Parameter::Type type)
{
	mDirtyIndex++;
	assert(index < GetParameterCount());
	assert(type < Parameter::Type::Count);
	mParameters[index].SetType(type);
}

void AnimationStateMachine::SetParameterBoolean(U32 index, bool value)
{
	mDirtyIndex++;
	assert(index < GetParameterCount());
	assert(mParameters[index].GetType() == Parameter::Type::Boolean);
	mParameters[index].SetBooleanValue(value);
}

void AnimationStateMachine::SetParameterFloat(U32 index, F32 value)
{
	mDirtyIndex++;
	assert(index < GetParameterCount());
	assert(mParameters[index].GetType() == Parameter::Type::Float);
	mParameters[index].SetFloatValue(value);
}

void AnimationStateMachine::SetParameterInteger(U32 index, I32 value)
{
	mDirtyIndex++;
	assert(index < GetParameterCount());
	assert(mParameters[index].GetType() == Parameter::Type::Integer);
	mParameters[index].SetIntegerValue(value);
}

U32 AnimationStateMachine::GetParameterCount() const
{
	return static_cast<U32>(mParameters.size());
}

const AnimationStateMachine::Parameter& AnimationStateMachine::GetParameter(U32 index) const
{
	assert(index < GetParameterCount());
    return mParameters[index];
}

U32 AnimationStateMachine::GetParameterIndexByName(const std::string& name) const
{
	return GetParameterIndexByName(Hash::CRC32(name.c_str()));
}

U32 AnimationStateMachine::GetParameterIndexByName(U32 hashedName) const
{
	const U32 parameterCount = GetParameterCount();
	for (U32 i = 0; i < parameterCount; ++i)
	{
		if (mParameters[i].GetHashedName() == hashedName)
		{
			return i;
		}
	}
	return U32_Max;
}

U32 AnimationStateMachine::AddCondition(U32 parameterIndex)
{
	mDirtyIndex++;
    assert(parameterIndex < GetParameterCount());
    mConditions.emplace_back(parameterIndex);
    return GetConditionCount() - 1;
}

void AnimationStateMachine::RemoveCondition(U32 index)
{
	mDirtyIndex++;
	assert(index < GetConditionCount());
	mConditions.erase(mConditions.begin() + index);

	// We should go through every transition to 
	//  - Remove the condition
	//  - Reduce superior indexes by one
	const U32 transitionCount = GetTransitionCount();
	for (U32 i = 0; i < transitionCount; ++i)
	{
		if (mTransitions[i].HasCondition(index))
		{
			mTransitions[i].RemoveCondition(index);
		}
		const U32 conditionCountInTransition = mTransitions[i].GetConditionCount();
		for (U32 j = 0; j < conditionCountInTransition; ++j)
		{
			U32 conditionIndex = mTransitions[i].GetCondition(j);
			if (conditionIndex > index)
			{
				mTransitions[i].SetCondition(j, conditionIndex - 1);
			}
		}
	}
}

void AnimationStateMachine::ClearConditions()
{
	mDirtyIndex++;
	mConditions.clear();
	const U32 transitionCount = GetTransitionCount();
	for (U32 i = 0; i < transitionCount; ++i)
	{
		mTransitions[i].ClearConditions();
	}
}

void AnimationStateMachine::SetConditionParameter(U32 index, U32 parameterIndex)
{
	mDirtyIndex++;
    assert(index < GetConditionCount());
	assert(parameterIndex < GetParameterCount());
	assert(mParameters[parameterIndex].GetType() < Parameter::Type::Count);

    mConditions[index].SetParameterIndex(parameterIndex);

	Parameter::Type type = mParameters[parameterIndex].GetType();
    switch (type)
    {
    case Parameter::Type::Boolean: mConditions[index].SetOperandBoolean(false); break;
    case Parameter::Type::Float: mConditions[index].SetOperandFloat(0.0f); break;
    case Parameter::Type::Integer: mConditions[index].SetOperandInteger(0); break;
    case Parameter::Type::Trigger: /* Nothing */ break;
    default: assert(false); break;
    }
}

void AnimationStateMachine::SetConditionOperator(U32 index, AnimationStateMachine::Condition::Operator operat)
{
	mDirtyIndex++;
    assert(index < GetConditionCount());

    // TODO : Error, assert or no effect if not matching the parameter type properly ? (Trigger/Boolean)
#ifdef ENLIVE_ENABLE_ASSERT
    const U32 parameterIndex = mConditions[index].GetParameterIndex();
    assert(parameterIndex < GetParameterCount());
    const Parameter::Type parameterType = mParameters[parameterIndex].GetType();
    assert(parameterType != Parameter::Type::Trigger);
    if (parameterType == Parameter::Type::Boolean)
    {
        assert(operat == Condition::Operator::Equal || operat == Condition::Operator::NotEqual);
    }
#endif // ENLIVE_ENABLE_ASSERT

    mConditions[index].SetOperator(operat);
}

void AnimationStateMachine::SetConditionOperandBoolean(U32 index, bool operand)
{
	mDirtyIndex++;
#ifdef ENLIVE_ENABLE_ASSERT
    assert(index < GetConditionCount());
    const U32 parameterIndex = mConditions[index].GetParameterIndex();
    assert(parameterIndex < GetParameterCount());
    const Parameter::Type parameterType = mParameters[parameterIndex].GetType();
    assert(parameterType == Parameter::Type::Boolean);
#endif // ENLIVE_ENABLE_ASSERT

    mConditions[index].SetOperandBoolean(operand);
}

void AnimationStateMachine::SetConditionOperandFloat(U32 index, F32 operand)
{
	mDirtyIndex++;
#ifdef ENLIVE_ENABLE_ASSERT
    assert(index < GetConditionCount());
    const U32 parameterIndex = mConditions[index].GetParameterIndex();
    assert(parameterIndex < GetParameterCount());
    const Parameter::Type parameterType = mParameters[parameterIndex].GetType();
    assert(parameterType == Parameter::Type::Float);
#endif // ENLIVE_ENABLE_ASSERT

    mConditions[index].SetOperandFloat(operand);
}

void AnimationStateMachine::SetConditionOperandInteger(U32 index, I32 operand)
{
	mDirtyIndex++;
#ifdef ENLIVE_ENABLE_ASSERT
    assert(index < GetConditionCount());
    const U32 parameterIndex = mConditions[index].GetParameterIndex();
	assert(parameterIndex < GetParameterCount());
	const Parameter::Type parameterType = mParameters[parameterIndex].GetType();
    assert(parameterType == Parameter::Type::Integer);
#endif // ENLIVE_ENABLE_ASSERT

    mConditions[index].SetOperandInteger(operand);
}

U32 AnimationStateMachine::GetConditionCount() const
{
    return static_cast<U32>(mConditions.size());
}

const AnimationStateMachine::Condition& AnimationStateMachine::GetCondition(U32 index) const
{
    assert(index < GetConditionCount());
    return mConditions[index];
}

U32 AnimationStateMachine::AddTransition(U32 fromState, U32 toState)
{
	mDirtyIndex++;
    mTransitions.emplace_back(fromState, toState);
    return GetTransitionCount() - 1;
}

void AnimationStateMachine::RemoveTransition(U32 index)
{
	mDirtyIndex++;
	assert(index < GetTransitionCount());

	const U32 transitionCount = GetTransitionCount();

	// If a condition is only in this transition, remove it
	U32 conditionCountInTransition = mTransitions[index].GetConditionCount();
	for (U32 i = 0; i < conditionCountInTransition; )
	{
		U32 conditionIndex = mTransitions[index].GetCondition(i);
		bool conditionFound = false;
		for (U32 j = 0; j < transitionCount; ++j)
		{
			if (j != index)
			{
				if (mTransitions[j].HasCondition(conditionIndex))
				{
					conditionFound = true;
				}
			}
		}
		if (!conditionFound)
		{
			RemoveCondition(conditionIndex);
			--conditionCountInTransition;
		}
		else
		{
			++i;
		}
	}

	mTransitions.erase(mTransitions.begin() + index);
}

void AnimationStateMachine::ClearTransitions()
{
	mDirtyIndex++;
	mTransitions.clear();
	ClearConditions();
}

void AnimationStateMachine::SetTransitionFromState(U32 index, U32 fromState)
{
	mDirtyIndex++;
    assert(index < GetTransitionCount());
    assert(fromState < GetStateCount());
    mTransitions[index].SetFromState(fromState);
}

void AnimationStateMachine::SetTransitionToState(U32 index, U32 toState)
{
	mDirtyIndex++;
    assert(index < GetTransitionCount());
    assert(toState < GetStateCount());
    mTransitions[index].SetToState(toState);
}

void AnimationStateMachine::SetTransitionExitOnlyAtEnd(U32 index, bool exitOnlyAtEnd)
{
	mDirtyIndex++;
	assert(index < GetTransitionCount());
	mTransitions[index].SetExitOnlyAtEnd(exitOnlyAtEnd);
}

void AnimationStateMachine::AddConditionToTransition(U32 transitionIndex, U32 conditionIndex)
{
	mDirtyIndex++;
    assert(transitionIndex < GetTransitionCount());
    assert(conditionIndex < GetConditionCount());
    mTransitions[transitionIndex].AddCondition(conditionIndex);
}

void AnimationStateMachine::RemoveConditionFromTransition(U32 transitionIndex, U32 conditionIndex)
{
	mDirtyIndex++;
    assert(transitionIndex < GetTransitionCount());
    assert(conditionIndex < GetConditionCount());
	mTransitions[transitionIndex].RemoveCondition(conditionIndex);

	// Remove condition if not used elsewhere anymore
	const U32 transitionCount = GetTransitionCount();
	for (U32 i = 0; i < transitionCount; ++i)
	{
		if (mTransitions[i].HasCondition(conditionIndex))
		{
			return;
		}
	}
	RemoveCondition(conditionIndex);
}

void AnimationStateMachine::ClearConditionsFromTransition(U32 transitionIndex)
{
	mDirtyIndex++;
    assert(transitionIndex < GetTransitionCount());
	mTransitions[transitionIndex].ClearConditions();
}

U32 AnimationStateMachine::GetTransitionCount() const
{
    return static_cast<U32>(mTransitions.size());
}

const AnimationStateMachine::Transition& AnimationStateMachine::GetTransition(U32 index) const
{
    assert(index < GetTransitionCount());
    return mTransitions[index];
}

U32 AnimationStateMachine::GetTransitionFromStateCount(U32 stateIndex) const
{
    U32 count = 0;
    const U32 transitionCount = GetTransitionCount();
    for (U32 i = 0; i < transitionCount; ++i)
    {
        if (mTransitions[i].GetFromState() == stateIndex)
        {
            ++count;
        }
    }
    return count;
}

U32 AnimationStateMachine::GetTransitionToStateCount(U32 stateIndex) const
{
    U32 count = 0;
    const U32 transitionCount = GetTransitionCount();
    for (U32 i = 0; i < transitionCount; ++i)
    {
        if (mTransitions[i].GetToState() == stateIndex)
        {
            ++count;
        }
    }
    return count;
}

void AnimationStateMachine::SetDefaultStateIndex(U32 stateIndex)
{
	if (mDefaultStateIndex != stateIndex)
	{
		mDefaultStateIndex = stateIndex;
		mDirtyIndex++;
	}
}

U32 AnimationStateMachine::GetDefaultStateIndex() const
{
    return mDefaultStateIndex;
}

} // namespace en
