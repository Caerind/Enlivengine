#pragma once

#include <functional>
#include <string>
#include <vector>

#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/System/Singleton.hpp>
#include <Enlivengine/System/NonCopyable.hpp>
#include <Enlivengine/System/Meta.hpp>

#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Joystick.hpp>

namespace en
{

enum class ActionInputType : U32
{
    Variable,
    Function,
    Event,
    Key,
    Mouse,
	JoystickConnect,
	JoystickButton,
	JoystickAxis,
    And,
    Or,
    Not,

	Count
};

enum class ActionType : U32
{
    Hold,
    Pressed,
	Released,

	Count
};

enum class ActionInputLogicalOperator : U32
{
    And,
    Or,
	Not,

	Count
};

class ActionSystem;

class ActionInput : private NonCopyable
{
public:
    ActionInput(const std::string& name);
    virtual ~ActionInput() {};

    virtual ActionInputType GetInputType() const = 0;
    virtual bool IsLogicalOperator() const;

    const std::string& GetName() const;
    U32 GetID() const;
    bool IsActive() const;

    virtual bool IsCurrentlyActive(ActionSystem* system) const = 0;

protected:
    std::string mName;
    U32 mID;
    bool mActive;

private:
    friend class ActionSystem;
    void SetActive(bool active);
    virtual U32 GetPriorityLevel() const;
};

class ActionInputVariable : public ActionInput
{
public:
	ActionInputVariable(const std::string& name, bool* variable);

	ActionInputType GetInputType() const override;
	bool IsCurrentlyActive(ActionSystem* system) const override;

private:
	bool* mVariable;
};

class ActionInputFunction : public ActionInput
{
public:
	using FuncType = std::function<bool()>;

	ActionInputFunction(const std::string& name, FuncType function);

	ActionInputType GetInputType() const override;
	bool IsCurrentlyActive(ActionSystem* system) const override;

private:
	FuncType mFunction;
};

class ActionInputEvent : public ActionInput
{
public:
	using FuncType = std::function<bool(const sf::Event& event)>;

	ActionInputEvent(const std::string& name, FuncType eventValidator);

	ActionInputType GetInputType() const override;
	bool IsCurrentlyActive(ActionSystem* system) const override;

private:
	FuncType mEventValidator;
};

class ActionInputKey : public ActionInput
{
public:
	enum class KeyCombination
	{
		None = 0,
		Ctrl = 1 << 0,
		Alt = 1 << 1,
		Shift = 1 << 2,
		System = 1 << 3
	};

	ActionInputKey(const std::string& name, sf::Keyboard::Key key, ActionType actionType = ActionType::Pressed, U32 keyCombination = static_cast<U32>(KeyCombination::None));

	ActionInputType GetInputType() const override;
	bool IsCurrentlyActive(ActionSystem* system) const override;

	sf::Keyboard::Key GetKey() const;
	ActionType GetType() const;
	U32 GetKeyCombination() const;

	void SetKey(sf::Keyboard::Key key);
	void SetActionType(ActionType actionType);
	void SetKeyCombination(U32 keyCombination);

private:
	sf::Keyboard::Key mKey;
	ActionType mActionType;
	U32 mKeyCombination;
};

class ActionInputMouse : public ActionInput
{
public:
	ActionInputMouse(const std::string& name, sf::Mouse::Button button, ActionType actionType = ActionType::Pressed);

	ActionInputType GetInputType() const override;
	bool IsCurrentlyActive(ActionSystem* system) const override;

	sf::Mouse::Button GetButton() const;
	ActionType GetType() const;

	void SetButton(sf::Mouse::Button button);
	void SetActionType(ActionType actionType);

private:
	sf::Mouse::Button mButton;
	ActionType mActionType;
};

class ActionInputJoystickConnect : public ActionInput
{
public:
	ActionInputJoystickConnect(const std::string& name, U32 joystickID, ActionType actionType = ActionType::Pressed);

	ActionInputType GetInputType() const override;
	bool IsCurrentlyActive(ActionSystem* system) const override;

	U32 GetJoystickID() const;
	ActionType GetType() const;

	void SetJoystickID(U32 joystickID);
	void SetActionType(ActionType actionType);

private:
	U32 mJoystickID;
	ActionType mActionType;
};

class ActionInputJoystickButton : public ActionInput
{
public:
	ActionInputJoystickButton(const std::string& name, U32 joystickID, U32 buttonID, ActionType actionType = ActionType::Pressed);

	ActionInputType GetInputType() const override;
	bool IsCurrentlyActive(ActionSystem* system) const override;

	U32 GetJoystickID() const;
	U32 GetButtonID() const;
	ActionType GetType() const;

	void SetJoystickID(U32 joystickID);
	void SetButtonID(U32 buttonID);
	void SetActionType(ActionType actionType);

private:
	U32 mJoystickID;
	U32 mButtonID;
	ActionType mActionType;
};

class ActionInputJoysticAxis : public ActionInput
{
public:
	ActionInputJoysticAxis(const std::string& name, U32 joystickID, sf::Joystick::Axis axis);

	ActionInputType GetInputType() const override;
	bool IsCurrentlyActive(ActionSystem* system) const override;

	U32 GetJoystickID() const;
	sf::Joystick::Axis GetAxis() const;

	void SetJoystickID(U32 joystickID);
	void SetAxis(sf::Joystick::Axis axis);

private:
	U32 mJoystickID;
	sf::Joystick::Axis mAxis;
};

class ActionInputLogical : public ActionInput
{
public:
	ActionInputLogical(const std::string& name, ActionInputLogicalOperator logic, U32 inputAID, U32 inputBID = U32_Max);

	ActionInputType GetInputType() const override;
	bool IsLogicalOperator() const override;
	bool IsCurrentlyActive(ActionSystem* system) const override;
	U32 GetPriorityLevel() const override;

	ActionInputLogicalOperator GetLogicalOperator() const;
	U32 GetInputAID() const;
	U32 GetInputBID() const;

	void SetInputAID(U32 inputID);
	void SetInputBID(U32 inputID);

private:
	friend class ActionSystem;
	U32 GetInputAIndex() const;
	U32 GetInputBIndex() const;
	void SetInputAIndex(U32 inputAIndex);
	void SetInputBIndex(U32 inputBIndex);
	void SetPriorityLevel(U32 priorityLevel);

private:
	ActionInputLogicalOperator mLogicOperator;
	U32 mInputAID;
	U32 mInputBID;
	U32 mInputAIndex;
	U32 mInputBIndex;
	U32 mPriorityLevel;
};

class ActionSystem : private NonCopyable
{
public:
    ActionSystem();

    void Update();

    bool IsInputActive(const std::string& inputName) const;
    bool IsInputActive(U32 inputID) const;

    bool IsInputExisting(const std::string& inputName) const;
    bool IsInputExisting(U32 inputID) const;

    void AddInputVariable(const std::string& name, bool* variable);
	void AddInputFunction(const std::string& name, ActionInputFunction::FuncType function);
	void AddInputEvent(const std::string& name, ActionInputEvent::FuncType eventValidator);
	void AddInputKey(const std::string& name, sf::Keyboard::Key key, ActionType actionType = ActionType::Pressed, U32 keyCombination = static_cast<U32>(ActionInputKey::KeyCombination::None));
	void AddInputMouse(const std::string& name, sf::Mouse::Button button, ActionType actionType = ActionType::Pressed);
	void AddInputJoystickConnect(const std::string& name, U32 joystickID, ActionType actionType = ActionType::Pressed);
	void AddInputJoystickButton(const std::string& name, U32 joystickID, U32 buttonID, ActionType actionType = ActionType::Pressed);
	void AddInputJoystickAxis(const std::string& name, U32 joystickID, sf::Joystick::Axis axis);
	void AddInputAnd(const std::string& name, U32 inputAID, U32 inputBID);
	void AddInputOr(const std::string& name, U32 inputAID, U32 inputBID);
	void AddInputNot(const std::string& name, U32 inputAID);

    U32 GetInputCount() const;
    const ActionInput* GetInputByIndex(U32 index) const;
    const ActionInput* GetInputByName(const std::string& inputName) const;
    const ActionInput* GetInputByID(U32 inputID) const;
	void RemoveInputByIndex(U32 index);
	U32 GetInputIndexFromName(const std::string& inputName) const;
	U32 GetInputIndexFromID(U32 inputID) const;
    void ClearInputs();

    void AddEvent(const sf::Event& event);
    U32 GetEventCount() const;
    const sf::Event& GetEvent(U32 index) const;
    void ClearEvents();

private:
	friend class ImGuiInputEditor;
	ActionInput* GetInputByIndexNonConst(U32 index);
	void FlagPriorityAsDirty();

private:
    std::vector<sf::Event> mEvents;
    std::vector<ActionInput*> mInputs;
    bool mDirty;

    void AddInput_Internal(ActionInput* input);
    void Update_Internal();
    U32 GetMaxPriority_Internal(U32 inputAID, U32 inputBID) const;
};

} // namespace en