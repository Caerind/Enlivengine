#pragma once

#include <Enlivengine/Graphics/Animation.hpp>
#include <Enlivengine/Graphics/AnimationStateMachine.hpp>

namespace en
{

class AnimationController
{
public:
	AnimationController();

	bool SetAnimationStateMachine(AnimationStateMachinePtr animationStateMachine);
	bool IsStateMachineValid() const;
	bool IsStateMachineDirty() const;
	void Recompute();

	bool HasParameter(const std::string& name) const;
	bool HasParameter(U32 hashedName) const;
	void SetParameterBoolean(const std::string& name, bool value);
	void SetParameterBoolean(U32 hashedName, bool value);
	void SetParameterFloat(const std::string& name, F32 value);
	void SetParameterFloat(U32 hashedName, F32 value);
	void SetParameterInteger(const std::string& name, I32 value);
	void SetParameterInteger(U32 hashedName, I32 value);
	void SetParameterTrigger(const std::string& name);
	void SetParameterTrigger(U32 hashedName);
	void ResetParameterTrigger(const std::string& name);
	void ResetParameterTrigger(U32 hashedName);
	bool GetParameterBoolean(const std::string& name) const;
	bool GetParameterBoolean(U32 hashedName) const;
	F32 GetParameterFloat(const std::string& name) const;
	F32 GetParameterFloat(U32 hashedName) const;
	I32 GetParameterInteger(const std::string& name) const;
	I32 GetParameterInteger(U32 hashedName) const;
	bool GetParameterTrigger(const std::string& name) const;
	bool GetParameterTrigger(U32 hashedName) const;
	U32 GetParameterIndexByName(const std::string& name) const;
	U32 GetParameterIndexByName(U32 hashedName) const;
	U32 GetParameterCount() const;
	const AnimationStateMachine::Parameter& GetParameter(U32 index) const;

	void Update(const Time& dt);

	bool AreIndicesValid() const;
	U32 GetStateIndex() const;
	U32 GetClipIndex() const;
	U32 GetClipFrameIndex() const;
	U32 GetFrameIndex() const;

private:
	U32 GetClipIndexFromState(const AnimationStateMachine::State& state);
	bool CanUseTransition(const AnimationStateMachine::Transition& transition, bool lastFrameEnded);

private:
	AnimationStateMachinePtr mAnimationStateMachine;
	U32 mAnimationStateMachineDirtyIndex;

	U32 mStateIndex;
	U32 mClipIndex;
	U32 mClipFrameIndex;
	U32 mFrameIndex;
	Time mTimeAccumulator;

	std::vector<AnimationStateMachine::Parameter> mParameters;
};

} // namespace en
