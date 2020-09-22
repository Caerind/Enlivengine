#include <Enlivengine/Graphics/AnimationController.hpp>

#include <Enlivengine/System/Hash.hpp>
#include <Enlivengine/System/Profiler.hpp>

namespace en
{

AnimationController::AnimationController()
	: mAnimationStateMachine()
	, mAnimationStateMachineDirtyIndex(0)
	, mStateIndex(U32_Max)
	, mClipIndex(U32_Max)
	, mClipFrameIndex(U32_Max)
	, mFrameIndex(U32_Max)
	, mTimeAccumulator()
{
}

bool AnimationController::SetAnimationStateMachine(AnimationStateMachinePtr animationStateMachine)
{
	mAnimationStateMachine = animationStateMachine;
	mAnimationStateMachineDirtyIndex = 0;

	mStateIndex = U32_Max;
	mClipIndex = U32_Max;
	mClipFrameIndex = U32_Max;
	mFrameIndex = U32_Max;
	mTimeAccumulator = Time::Zero();
	mParameters.clear();

	if (!IsStateMachineValid())
	{
		return false;
	}

	const AnimationStateMachine& stateMachine = mAnimationStateMachine.Get();
	mAnimationStateMachineDirtyIndex = stateMachine.GetDirtyIndex();
	
	// Load indices
	{
		const Animation& animation = stateMachine.GetAnimation().Get();

		if (stateMachine.GetDefaultStateIndex() >= stateMachine.GetStateCount())
		{
			return false;
		}
		mStateIndex = stateMachine.GetDefaultStateIndex();
		const AnimationStateMachine::State& state = stateMachine.GetState(mStateIndex);

		if (state.GetClipIndex() >= animation.GetClipCount())
		{
			return false;
		}
		mClipIndex = GetClipIndexFromState(state); // Return the clip index of the state on loading, will be changed on the first update
		const Animation::Clip& clip = animation.GetClip(mClipIndex);

		if (clip.GetFrameCount() == 0)
		{
			return false;
		}
		mClipFrameIndex = 0;

		if (clip.GetFrameIndex(mClipFrameIndex) >= animation.GetFrameCount())
		{
			return false;
		}
		mFrameIndex = clip.GetFrameIndex(mClipFrameIndex);
	}

	// Load parameters
	{
		const U32 parameterCount = stateMachine.GetParameterCount();
		mParameters.reserve(parameterCount);
		for (U32 i = 0; i < parameterCount; ++i)
		{
			mParameters.push_back(stateMachine.GetParameter(i));
		}
	}

	return true;
}

bool AnimationController::IsStateMachineValid() const
{
	const AnimationStateMachine* stateMachine = mAnimationStateMachine.GetPtr();
	if (stateMachine == nullptr)
	{
		return false;
	}
	const Animation* animation = stateMachine->GetAnimation().GetPtr();
	if (animation == nullptr)
	{
		return false;
	}
	if (stateMachine->GetStateCount() == 0 
		|| animation->GetClipCount() == 0
		|| animation->GetFrameCount() == 0)
	{
		return false;
	}
	return true;
}

bool AnimationController::IsStateMachineDirty() const
{
	if (!IsStateMachineValid())
	{
		return true;
	}
	return mAnimationStateMachineDirtyIndex < mAnimationStateMachine.Get().GetDirtyIndex();
}

void AnimationController::Recompute()
{
	ENLIVE_PROFILE_FUNCTION();

	// TODO : Improve if needed/possible
	SetAnimationStateMachine(mAnimationStateMachine);
}

bool AnimationController::HasParameter(const std::string& name) const
{
	return HasParameter(Hash::SlowHash(name.c_str()));
}

bool AnimationController::HasParameter(U32 hashedName) const
{
	return GetParameterIndexByName(hashedName) < GetParameterCount();
}

void AnimationController::SetParameterBoolean(const std::string& name, bool value)
{
	SetParameterBoolean(Hash::SlowHash(name.c_str()), value);
}

void AnimationController::SetParameterBoolean(U32 hashedName, bool value)
{
	const U32 parameterIndex = GetParameterIndexByName(hashedName);
	enAssert(parameterIndex < GetParameterCount());
	enAssert(mParameters[parameterIndex].GetType() == AnimationStateMachine::Parameter::Type::Boolean);
	mParameters[parameterIndex].SetBooleanValue(value);
}

void AnimationController::SetParameterFloat(const std::string& name, F32 value)
{
	SetParameterFloat(Hash::SlowHash(name.c_str()), value);
}

void AnimationController::SetParameterFloat(U32 hashedName, F32 value)
{
	const U32 parameterIndex = GetParameterIndexByName(hashedName);
	enAssert(parameterIndex < GetParameterCount());
	enAssert(mParameters[parameterIndex].GetType() == AnimationStateMachine::Parameter::Type::Float);
	mParameters[parameterIndex].SetFloatValue(value);
}

void AnimationController::SetParameterInteger(const std::string& name, I32 value)
{
	SetParameterInteger(Hash::SlowHash(name.c_str()), value);
}

void AnimationController::SetParameterInteger(U32 hashedName, I32 value)
{
	const U32 parameterIndex = GetParameterIndexByName(hashedName);
	enAssert(parameterIndex < GetParameterCount());
	enAssert(mParameters[parameterIndex].GetType() == AnimationStateMachine::Parameter::Type::Integer);
	mParameters[parameterIndex].SetIntegerValue(value);
}

void AnimationController::SetParameterTrigger(const std::string& name)
{
	SetParameterTrigger(Hash::SlowHash(name.c_str()));
}

void AnimationController::SetParameterTrigger(U32 hashedName)
{
	const U32 parameterIndex = GetParameterIndexByName(hashedName);
	enAssert(parameterIndex < GetParameterCount());
	enAssert(mParameters[parameterIndex].GetType() == AnimationStateMachine::Parameter::Type::Trigger);
	mParameters[parameterIndex].SetTriggerValue(true);
}

void AnimationController::ResetParameterTrigger(const std::string& name)
{
	ResetParameterTrigger(Hash::SlowHash(name.c_str()));
}

void AnimationController::ResetParameterTrigger(U32 hashedName)
{
	const U32 parameterIndex = GetParameterIndexByName(hashedName);
	enAssert(parameterIndex < GetParameterCount());
	enAssert(mParameters[parameterIndex].GetType() == AnimationStateMachine::Parameter::Type::Trigger);
	mParameters[parameterIndex].SetTriggerValue(false);
}

bool AnimationController::GetParameterBoolean(const std::string& name) const
{
	return GetParameterBoolean(Hash::SlowHash(name.c_str()));
}

bool AnimationController::GetParameterBoolean(U32 hashedName) const
{
	const U32 parameterIndex = GetParameterIndexByName(hashedName);
	enAssert(parameterIndex < GetParameterCount());
	enAssert(mParameters[parameterIndex].GetType() == AnimationStateMachine::Parameter::Type::Boolean);
	return mParameters[parameterIndex].GetBooleanValue();
}

F32 AnimationController::GetParameterFloat(const std::string& name) const
{
	return GetParameterFloat(Hash::SlowHash(name.c_str()));
}

F32 AnimationController::GetParameterFloat(U32 hashedName) const
{
	const U32 parameterIndex = GetParameterIndexByName(hashedName);
	enAssert(parameterIndex < GetParameterCount());
	enAssert(mParameters[parameterIndex].GetType() == AnimationStateMachine::Parameter::Type::Float);
	return mParameters[parameterIndex].GetFloatValue();
}

I32 AnimationController::GetParameterInteger(const std::string& name) const
{
	return GetParameterInteger(Hash::SlowHash(name.c_str()));
}

I32 AnimationController::GetParameterInteger(U32 hashedName) const
{
	const U32 parameterIndex = GetParameterIndexByName(hashedName);
	enAssert(parameterIndex < GetParameterCount());
	enAssert(mParameters[parameterIndex].GetType() == AnimationStateMachine::Parameter::Type::Integer);
	return mParameters[parameterIndex].GetIntegerValue();
}

bool AnimationController::GetParameterTrigger(const std::string& name) const
{
	return GetParameterTrigger(Hash::SlowHash(name.c_str()));
}

bool AnimationController::GetParameterTrigger(U32 hashedName) const
{
	const U32 parameterIndex = GetParameterIndexByName(hashedName);
	enAssert(parameterIndex < GetParameterCount());
	enAssert(mParameters[parameterIndex].GetType() == AnimationStateMachine::Parameter::Type::Trigger);
	return mParameters[parameterIndex].GetTriggerValue();
}

U32 AnimationController::GetParameterIndexByName(const std::string& name) const
{
	return GetParameterIndexByName(Hash::SlowHash(name.c_str()));
}

U32 AnimationController::GetParameterIndexByName(U32 hashedName) const
{
	const U32 count = GetParameterCount();
	for (U32 i = 0; i < count; ++i)
	{
		if (mParameters[i].GetHashedName() == hashedName)
		{
			return i;
		}
	}
	return U32_Max;
}

U32 AnimationController::GetParameterCount() const
{
	return static_cast<U32>(mParameters.size());
}

const AnimationStateMachine::Parameter& AnimationController::GetParameter(U32 index) const
{
	enAssert(index < GetParameterCount());
	return mParameters[index];
}

void AnimationController::Update(const Time& dt)
{
	ENLIVE_PROFILE_FUNCTION();

	if (!IsStateMachineValid())
	{
		return;
	}
	if (IsStateMachineDirty())
	{
		Recompute();
	}

	const AnimationStateMachine& stateMachine = mAnimationStateMachine.Get();
	const Animation& animation = stateMachine.GetAnimation().Get();

	const AnimationStateMachine::State& state = stateMachine.GetState(mStateIndex);
	mClipIndex = GetClipIndexFromState(state);

	const F32 speedScale = state.GetSpeedScale();
	if (speedScale != 1.0f)
	{
		mTimeAccumulator += Time::Seconds(dt.AsSeconds() * state.GetSpeedScale());
	}
	else
	{
		mTimeAccumulator += dt;
	}

	const Time& duration = animation.GetFrame(mFrameIndex).GetDuration();
	const bool frameEnded = mTimeAccumulator >= duration;
	const bool lastFrame = (mClipFrameIndex == animation.GetClip(mClipIndex).GetFrameCount() - 1);
	const bool lastFrameEnded = frameEnded && lastFrame;

	// Try to change state
	bool stateChanged = false;
	if (!state.GetExitOnlyAtEnd() || lastFrameEnded)
	{
		for (U32 i = state.GetFirstOutputTransitionIndex(); i < stateMachine.GetTransitionCount(); ++i)
		{
			const AnimationStateMachine::Transition& transition = stateMachine.GetTransition(i);
			if (transition.GetFromState() != mStateIndex || stateChanged)
			{
				break;
			}
			if (!transition.GetExitOnlyAtEnd() || lastFrameEnded)
			{
				if (CanUseTransition(transition, lastFrameEnded))
				{
					stateChanged = true;
					mStateIndex = transition.GetToState();
					mClipIndex = GetClipIndexFromState(stateMachine.GetState(mStateIndex));
					const Animation::Clip& clip = animation.GetClip(mClipIndex);
					enAssert(clip.GetFrameCount() > 0);
					mClipFrameIndex = 0;
					mFrameIndex = clip.GetFrameIndex(mClipFrameIndex);
					mTimeAccumulator = Time::Zero();
				}
			}
		}
	}

	// Update frame in the current clip
	if (!stateChanged && frameEnded)
	{
		const Animation::Clip& clip = animation.GetClip(mClipIndex);
		mClipFrameIndex = (mClipFrameIndex >= clip.GetFrameCount() - 1) ? 0 : mClipFrameIndex + 1;
		mFrameIndex = clip.GetFrameIndex(mClipFrameIndex);
		mTimeAccumulator = Time::Zero();
	}

	// Reset Triggers
	const U32 parameterCount = GetParameterCount();
	for (U32 i = 0; i < parameterCount; ++i)
	{
		if (mParameters[i].GetType() == AnimationStateMachine::Parameter::Type::Trigger)
		{
			mParameters[i].SetTriggerValue(false);
		}
	}
}

bool AnimationController::AreIndicesValid() const
{
	enAssert(IsStateMachineValid());

	const AnimationStateMachine* stateMachine = mAnimationStateMachine.GetPtr();
	if (stateMachine == nullptr)
	{
		return false;
	}
	const Animation* animation = stateMachine->GetAnimation().GetPtr();
	if (animation == nullptr)
	{
		return false;
	}

	if (mStateIndex >= stateMachine->GetStateCount())
	{
		return false;
	}
	if (mClipIndex >= animation->GetClipCount())
	{
		return false;
	}
	if (mClipFrameIndex >= animation->GetClip(mClipIndex).GetFrameCount())
	{
		return false;
	}
	if (mFrameIndex >= animation->GetFrameCount())
	{
		return false;
	}
	return true;
}

U32 AnimationController::GetStateIndex() const
{
	return mStateIndex;
}

U32 AnimationController::GetClipIndex() const
{
	return mClipIndex;
}

U32 AnimationController::GetClipFrameIndex() const
{
	return mClipFrameIndex;
}

U32 AnimationController::GetFrameIndex() const
{
	return mFrameIndex;
}

U32 AnimationController::GetClipIndexFromState(const AnimationStateMachine::State& state)
{
	ENLIVE_PROFILE_FUNCTION();

	if (state.HasBlendStateInfo() && mParameters.size() > 0)
	{
		const AnimationStateMachine::State::BlendStateInfo& blendState = *(state.GetBlendStateInfo());
		const U32 dimension = blendState.GetDimension();
		const U32 motionCount = blendState.GetMotionCount();
		if (dimension > 0 && motionCount > 0)
		{
			U32 bestMotion = 0;
			F32 bestDistanceSqr = 999999999.9f;
			for (U32 i = 0; i < motionCount; ++i)
			{
				F32 distanceSqr = 0.0f;
				for (U32 j = 0; j < dimension; ++j)
				{
					const F32 mValue = blendState.GetMotion(i).GetValue(j);
					const AnimationStateMachine::Parameter& parameter = mParameters[blendState.GetParameter(j)];
					F32 pValue = 0.0f;
					if (parameter.GetType() == AnimationStateMachine::Parameter::Type::Float)
					{
						pValue = parameter.GetFloatValue();
					}
					else if (parameter.GetType() == AnimationStateMachine::Parameter::Type::Integer)
					{
						pValue = static_cast<F32>(parameter.GetIntegerValue());
					}
					else
					{
						enAssert(false);
					}
					const F32 delta = mValue - pValue;
					distanceSqr += (delta * delta);
				}

				if (distanceSqr < bestDistanceSqr)
				{
					bestMotion = i;
					bestDistanceSqr = distanceSqr;
				}
			}

			return blendState.GetMotion(bestMotion).GetClipIndex();
		}
	}
	return state.GetClipIndex();
}

bool AnimationController::CanUseTransition(const AnimationStateMachine::Transition& transition, bool lastFrameEnded)
{
	enAssert(mAnimationStateMachine.IsValid());
	const AnimationStateMachine& stateMachine = mAnimationStateMachine.Get();
	enAssert(!IsStateMachineDirty());

	if (!lastFrameEnded && transition.GetExitOnlyAtEnd())
	{
		return false;
	}

	const U32 conditionCount = transition.GetConditionCount();
	for (U32 i = 0; i < conditionCount; ++i)
	{
		const U32 conditionIndex = transition.GetCondition(i);
		enAssert(conditionIndex < stateMachine.GetConditionCount());
		const AnimationStateMachine::Condition& condition = stateMachine.GetCondition(conditionIndex);
		enAssert(condition.GetParameterIndex() < GetParameterCount());
		const AnimationStateMachine::Parameter& parameter = GetParameter(condition.GetParameterIndex());
		if (!condition.Evaluate(parameter))
		{
			return false;
		}
	}

	return true;
}

} // namespace en
