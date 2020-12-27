#pragma once

#include <vector>
#include <string>

#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Window/Keyboard.hpp>
#include <Enlivengine/Window/Mouse.hpp>
#include <Enlivengine/Window/Controller.hpp>

namespace en
{

class EventSystem
{
public:
	class EventBase
	{
	public:
#ifdef ENLIVE_DEBUG
		std::string name{};
#endif // ENLIVE_DEBUG
		U32 hash{ 0 };
	protected:
		friend class EventSystem;
		bool active{ false };
	};

	class EventButton : public EventBase
	{
	public:
		enum class Type
		{
			KeyboardKey,
			MouseButton,
			JoystickButton
		};
		enum class ActionType
		{
			None,
			Pressed,
			Released,
			Hold
		};
		Type type{ Type::KeyboardKey };
		ActionType action{ ActionType::None };
		U32 buttonIdentifier{ 0 };
		U32 extraInfo{ 0 };
	private:
		friend class EventSystem;
	};

	class EventAxis : public EventBase
	{
	public:
		enum class Type
		{
			MouseMovementX,
			MouseMovementY,
			MouseWheelX,
			MouseWheelY,
			JoystickAxis,
			JoystickBallX,
			JoystickBallY
		};
		Type type{ Type::JoystickAxis };
		U32 axisIdentifier{ 0 };
		U32 extraInfo{ 0 };
		F32 GetValue() const { return value; }
	private:
		friend class EventSystem;
		F32 value{ 0.0f };
	};

public:
	static void Update();

	static U32 AddButton(const char* name, EventButton::Type type, U32 buttonIdentifier, U32 extraInfo, EventButton::ActionType action);
	static U32 AddKeyButton(const char* name, Keyboard::Key key, EventButton::ActionType action, U32 modifiers = static_cast<U32>(Keyboard::Modifier::None));
	static U32 AddMouseButton(const char* name, Mouse::Button mouseButton, EventButton::ActionType action);
	static U32 AddJoystickButton(const char* name, U32 controllerId, U32 buttonIndex, EventButton::ActionType action);
	static bool HasButton(const char* name);
	static bool HasButton(U32 hash);
	static bool IsButtonActive(const char* name);
	static bool IsButtonActive(U32 hash);
	static U32 GetButtonCount();
	static void RemoveButtonAtIndex(U32 index);
	static void RemoveButton(const char* name);
	static void RemoveButton(U32 hash);
	static const EventButton& GetButton(U32 index);
	static const std::vector<EventButton>& GetButtons();
	static const EventButton& GetLastButton();

	static U32 AddAxis(const char* name, EventAxis::Type type, U32 axisIdentifier = 0, U32 extraInfo = 0); 
	static bool HasAxis(const char* name);
	static bool HasAxis(U32 hash);
	static bool IsAxisActive(const char* name);
	static bool IsAxisActive(U32 hash);
	static F32 GetAxisValue(const char* name);
	static F32 GetAxisValue(U32 hash);
	static U32 GetAxisCount();
	static void RemoveAxisAtIndex(U32 index);
	static void RemoveAxis(const char* name);
	static void RemoveAxis(U32 hash);
	static const EventAxis& GetAxis(U32 index);
	static const std::vector<EventAxis>& GetAxes();

private:
	friend class Controller;
	friend class Keyboard;
	friend class Mouse;
	static void SetLastButton(EventButton::Type type, U32 buttonIdentifier, U32 extraInfo = 0);
	
	static U32 HashFct(const char* name);

	static EventSystem& GetInstance();

	EventSystem();
	void HandleEvent(const SDL_Event& event);

	// NonCopyable & NonMovable
	EventSystem(const EventSystem&) = delete;
	EventSystem& operator=(const EventSystem&) = delete;
	EventSystem(Mouse&&) = delete;
	EventSystem& operator=(EventSystem&&) = delete;

	std::vector<EventButton> mButtons;
	std::vector<EventAxis> mAxes;

	EventButton mLastButton;
};

} // namespace en