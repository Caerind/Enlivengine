#pragma once

#include <Enlivengine/Application/ResourceManager.hpp>
#include <Enlivengine/Graphics/Animation.hpp>

namespace en
{

class AnimationStateMachine : public Resource<AnimationStateMachine>
{
public:
	class State
	{
	public:
        State(const std::string& name, U32 clipIndex);

        void SetName(const std::string& name);
        const std::string& GetName() const { return mName; }
        U32 GetHashedName() const { return mHashedName; }

        void SetClipIndex(U32 clipIndex) { mClipIndex = clipIndex; }
        U32 GetClipIndex() const { return mClipIndex; }

        void SetSpeedScale(F32 speedScale) { mSpeedScale = speedScale; }
        F32 GetSpeedScale() const { return mSpeedScale; }

        void SetExitOnlyAtEnd(bool value) { mExitOnlyAtEnd = value; }
        bool GetExitOnlyAtEnd() const { return mExitOnlyAtEnd; }

		void SetFirstOutputTransitionIndex(U32 firstOutputTransitionIndex) { mFirstOutputTransitionIndex = firstOutputTransitionIndex; }
		U32 GetFirstOutputTransitionIndex() const { return mFirstOutputTransitionIndex; }

		class BlendStateInfo
		{
		public:
			class Motion
			{
			public:
				Motion(U32 clipIndex, U32 dimensions);

				U32 GetClipIndex() const { return mClipIndex; }
				U32 GetValueCount() const { return static_cast<U32>(mValues.size()); }

				void SetValue(U32 dimensionIndex, F32 value);
				F32 GetValue(U32 dimensionIndex) const;

			private:
				U32 mClipIndex;
				std::vector<F32> mValues;
			};

			BlendStateInfo(U32 dimension);

			U32 GetDimension() const { return static_cast<U32>(mParameters.size()); }
			void SetParameter(U32 dimensionIndex, U32 parameterIndex);
			U32 GetParameter(U32 dimensionIndex) const;

			U32 GetMotionCount() const { return static_cast<U32>(mMotions.size()); }
			U32 AddMotion(U32 clipIndex);
			const Motion& GetMotion(U32 motionIndex) const;
			void SetMotionValue(U32 motionIndex, U32 dimensionIndex, F32 value);
			void RemoveMotion(U32 motionIndex);
			void ClearMotions();

		private:
			std::vector<U32> mParameters;
			std::vector<Motion> mMotions;
		};
		void CreateBlendStateInfo(U32 dimension);
		void RemoveBlendStateInfo();
		BlendStateInfo* GetBlendStateInfo() { return mBlendStateInfo; }
		const BlendStateInfo* GetBlendStateInfo() const { return mBlendStateInfo; }
		bool HasBlendStateInfo() const { return mBlendStateInfo != nullptr; }

    private:
		std::string mName;
		U32 mHashedName;
		U32 mClipIndex;
		F32 mSpeedScale;
		bool mExitOnlyAtEnd;
		U32 mFirstOutputTransitionIndex;
		BlendStateInfo* mBlendStateInfo;
	};

	class Parameter
	{
    public:
        enum class Type
        {
            Boolean,
            Float,
            Integer,
            Trigger,
			Count
        };

		Parameter(const std::string& name, Type type);

        void SetName(const std::string& name);
		const std::string& GetName() const { return mName; }
		U32 GetHashedName() const { return mHashedName; }

		void SetType(Type type);
        Type GetType() const { return mType; }

		void SetBooleanValue(bool value);
		void SetFloatValue(F32 value);
        void SetIntegerValue(I32 value);
        void SetTriggerValue(bool value);
		bool GetBooleanValue() const;
		F32 GetFloatValue() const;
		I32 GetIntegerValue() const;
		bool GetTriggerValue() const;

	private:
		std::string mName;
		U32 mHashedName;
        Type mType;
		union
		{
			bool bValue;
			F32 fValue;
			I32 iValue;
		} mValue;
	};

	class Condition
	{
	public:
		enum class Operator
		{
			Equal,
			NotEqual,
			Less,
			LessEq,
			Greater,
			GreaterEq
		};

        Condition(U32 parameterIndex);

		void SetParameterIndex(U32 parameterIndex) { mParameterIndex = parameterIndex; }
		U32 GetParameterIndex() const { return mParameterIndex; }

        void SetOperator(Operator op) { mOperator = op; }
		Operator GetOperator() const { return mOperator; }

        void SetOperandBoolean(bool value) { mOperand.bValue = value; }
        void SetOperandFloat(F32 value) { mOperand.fValue = value; }
        void SetOperandInteger(I32 value) { mOperand.iValue = value; }
        bool GetOperandBoolean() const { return mOperand.bValue; }
		F32 GetOperandFloat() const { return mOperand.fValue; }
		I32 GetOperandInteger() const { return mOperand.iValue; }

		bool Evaluate(const AnimationStateMachine::Parameter& parameter) const;
		bool EvaluateBoolean(bool value) const;
		bool EvaluateFloat(F32 value) const;
		bool EvaluateInteger(I32 value) const;
		bool EvaluateTrigger(bool value) const;

	private:
		U32 mParameterIndex;
		Operator mOperator;
		union
		{
			bool bValue;
			F32 fValue;
			I32 iValue;
		} mOperand;
	};

	class Transition
	{
	public:
        Transition(U32 fromState, U32 toState);

        void SetFromState(U32 fromState) { mFromState = fromState; }
        U32 GetFromState() const { return mFromState; }

        void SetToState(U32 toState) { mToState = toState; }
		U32 GetToState() const { return mToState; }

		void SetExitOnlyAtEnd(bool value) { mExitOnlyAtEnd = value; }
		bool GetExitOnlyAtEnd() const { return mExitOnlyAtEnd; }

		U32 GetConditionCount() const;
		U32 GetCondition(U32 conditionIndexInTransition) const;
		void SetCondition(U32 conditionIndexInTransition, U32 conditionIndex);
		bool HasCondition(U32 conditionIndex) const;
        U32 AddCondition(U32 conditionIndex);
        void RemoveCondition(U32 conditionIndex);
		void ClearConditions();

    private:
		U32 mFromState;
		U32 mToState;
		bool mExitOnlyAtEnd;
		std::vector<U32> mConditions;
	};

public:
	AnimationStateMachine();

	// IO
	bool LoadFromFile(const std::string& filename);
	bool SaveToFile(const std::string& filename);
    void Precompute();
	void Clear();

	// Animation
	void SetAnimation(AnimationPtr animation);
	const AnimationPtr& GetAnimation() const;

	// States
	U32 AddState(const std::string& name, U32 clipIndex);
	void RemoveState(U32 index);
	void ClearStates();
	void SetStateName(U32 index, const std::string& name);
	void SetStateClipIndex(U32 index, U32 clipIndex);
	void SetStateSpeedScale(U32 index, F32 speedScale);
	U32 GetStateCount() const;
	const AnimationStateMachine::State& GetState(U32 index) const;
	U32 GetStateIndexByName(const std::string& name) const;
	U32 GetStateIndexByName(U32 hashedName) const;

	// States - BlendState
	void AddBlendStateToState(U32 stateIndex, U32 dimension);
	void RemoveBlendStateFromState(U32 stateIndex);
	void SetBlendStateParameter(U32 stateIndex, U32 dimensionIndex, U32 parameterIndex);
	U32 AddBlendStateMotion(U32 stateIndex, U32 clipIndex);
	void SetBlendStateMotionValue(U32 stateIndex, U32 motionIndex, U32 dimensionIndex, F32 value);
	void RemoveBlendStateMotion(U32 stateIndex, U32 motionIndex);
	void ClearBlendStateMotions(U32 stateIndex);

	// Parameters
	U32 AddParameter(const std::string& name, Parameter::Type type);
	void RemoveParameter(U32 index);
	void ClearParameters();
	void SetParameterName(U32 index, const std::string& name);
	void SetParameterType(U32 index, Parameter::Type type);
	void SetParameterBoolean(U32 index, bool value);
	void SetParameterFloat(U32 index, F32 value);
	void SetParameterInteger(U32 index, I32 value);
	U32 GetParameterCount() const;
	const AnimationStateMachine::Parameter& GetParameter(U32 index) const;
	U32 GetParameterIndexByName(const std::string& name) const;
	U32 GetParameterIndexByName(U32 hashedName) const;

    // Conditions
    U32 AddCondition(U32 parameterIndex);
    void RemoveCondition(U32 index);
    void ClearConditions();
    void SetConditionParameter(U32 index, U32 parameterIndex);
    void SetConditionOperator(U32 index, Condition::Operator operat);
    void SetConditionOperandBoolean(U32 index, bool operand);
    void SetConditionOperandFloat(U32 index, F32 operand);
    void SetConditionOperandInteger(U32 index, I32 operand);
	U32 GetConditionCount() const;
	const AnimationStateMachine::Condition& GetCondition(U32 index) const;

    // Transitions
    U32 AddTransition(U32 fromState, U32 toState);
    void RemoveTransition(U32 index);
    void ClearTransitions();
    void SetTransitionFromState(U32 index, U32 fromState);
	void SetTransitionToState(U32 index, U32 toState);
	void SetTransitionExitOnlyAtEnd(U32 index, bool exitOnlyAtEnd);
    void AddConditionToTransition(U32 transitionIndex, U32 conditionIndex);
    void RemoveConditionFromTransition(U32 transitionIndex, U32 conditionIndex);
    void ClearConditionsFromTransition(U32 transitionIndex);
	U32 GetTransitionCount() const;
    const AnimationStateMachine::Transition& GetTransition(U32 index) const;
    U32 GetTransitionFromStateCount(U32 stateIndex) const;
    U32 GetTransitionToStateCount(U32 stateIndex) const;

    void SetDefaultStateIndex(U32 stateIndex);
    U32 GetDefaultStateIndex() const;

	U32 GetDirtyIndex() const { return mDirtyIndex; }

private:
	AnimationPtr mAnimation;
	std::vector<State> mStates;
	std::vector<Parameter> mParameters;
	std::vector<Condition> mConditions;
	std::vector<Transition> mTransitions;
    U32 mDefaultStateIndex;
	U32 mDirtyIndex; // Transient
};

using AnimationStateMachinePtr = ResourcePtr<AnimationStateMachine>;

class AnimationStateMachineLoader
{
public:
	AnimationStateMachineLoader() = delete;
	~AnimationStateMachineLoader() = delete;

	static ResourceLoader<AnimationStateMachine> FromFile(const std::string& filename)
	{
		return ResourceLoader<AnimationStateMachine>([&filename](AnimationStateMachine& r)
		{
			const bool result = r.LoadFromFile(filename);
			r.mFilename = (result) ? filename : "";
			return result;
		});
	}
};

} // namespace en
