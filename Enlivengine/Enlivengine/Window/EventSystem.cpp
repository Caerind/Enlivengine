#include <Enlivengine/Window/EventSystem.hpp>

#include <Enlivengine/Utils/Assert.hpp>
#include <Enlivengine/SDL/SDLWrapper.hpp>

namespace en
{

void EventSystem::Update()
{
	EventSystem& eventSystem = GetInstance();

	Mouse::Refresh();
	Keyboard::Refresh();
	Controller::Refresh();

	SDL_Event event;
	while (SDLWrapper::PollEvent(event))
	{
		eventSystem.HandleEvent(event);
	}

	for (EventButton& eventButton : eventSystem.mButtons)
	{
		switch (eventButton.type)
		{
		case EventButton::Type::KeyboardKey:
		{
			switch (eventButton.action)
			{
			case EventButton::ActionType::None:
			{
				eventButton.active = false;
				break;
			}
			case EventButton::ActionType::Hold:
			{
				eventButton.active = Keyboard::IsHold(static_cast<Keyboard::Key>(eventButton.buttonIdentifier));
				if (eventButton.active && eventButton.extraInfo > 0)
				{
					eventButton.active = Keyboard::AreModifiersHold(eventButton.extraInfo);
				}
				break;
			}
			case EventButton::ActionType::Pressed:
			{
				eventButton.active = Keyboard::IsPressed(static_cast<Keyboard::Key>(eventButton.buttonIdentifier));
				if (eventButton.active && eventButton.extraInfo > 0)
				{
					eventButton.active = Keyboard::AreModifiersHold(eventButton.extraInfo);
				}
				break;
			}
			case EventButton::ActionType::Released:
			{
				eventButton.active = Keyboard::IsReleased(static_cast<Keyboard::Key>(eventButton.buttonIdentifier));
				break;
			}
			default: enAssert(false); break;
			}
			break;
		}
		case EventButton::Type::MouseButton:
		{
			switch (eventButton.action)
			{
			case EventButton::ActionType::None:
			{
				eventButton.active = false;
				break;
			}
			case EventButton::ActionType::Hold:
			{
				eventButton.active = Mouse::IsHold(static_cast<Mouse::Button>(eventButton.buttonIdentifier));
				break;
			}
			case EventButton::ActionType::Pressed:
			{
				eventButton.active = Mouse::IsPressed(static_cast<Mouse::Button>(eventButton.buttonIdentifier));
				break;
			}
			case EventButton::ActionType::Released:
			{
				eventButton.active = Mouse::IsReleased(static_cast<Mouse::Button>(eventButton.buttonIdentifier));
				break;
			}
			default: enAssert(false); break;
			}
			break;
		}
		case EventButton::Type::JoystickButton:
		{
			switch (eventButton.action)
			{
			case EventButton::ActionType::None:
			{
				eventButton.active = false;
				break;
			}
			case EventButton::ActionType::Hold:
			{
				eventButton.active = Controller::IsButtonHold(eventButton.extraInfo, eventButton.buttonIdentifier);
				break;
			}
			case EventButton::ActionType::Pressed:
			{
				eventButton.active = Controller::IsButtonPressed(eventButton.extraInfo, eventButton.buttonIdentifier);
				break;
			}
			case EventButton::ActionType::Released:
			{
				eventButton.active = Controller::IsButtonReleased(eventButton.extraInfo, eventButton.buttonIdentifier);
				break;
			}
			default: enAssert(false); break;
			}
			break;
		}
		default: enAssert(false); break;
		}
	}

	for (EventAxis& eventAxis : eventSystem.mAxes)
	{
		switch (eventAxis.type)
		{
		case EventAxis::Type::MouseMovementX:
		{
			I32 mvt = Mouse::GetMouseMovement().x;
			eventAxis.active = mvt != 0;
			eventAxis.value = static_cast<F32>(mvt);
			break;
		}
		case EventAxis::Type::MouseMovementY:
		{
			I32 mvt = Mouse::GetMouseMovement().y;
			eventAxis.active = mvt != 0;
			eventAxis.value = static_cast<F32>(mvt);
			break;
		}
		case EventAxis::Type::MouseWheelX:
		{
			I32 mvt = Mouse::GetHorizontalWheel();
			eventAxis.active = mvt != 0;
			eventAxis.value = static_cast<F32>(mvt);
			break;
		}
		case EventAxis::Type::MouseWheelY:
		{
			I32 mvt = Mouse::GetWheel();
			eventAxis.active = mvt != 0;
			eventAxis.value = static_cast<F32>(mvt);
			break;
		}
		case EventAxis::Type::JoystickAxis:
		{
			eventAxis.active = Controller::HasAxisMoved(eventAxis.extraInfo, eventAxis.axisIdentifier);
			eventAxis.value = Controller::GetAxis(eventAxis.extraInfo, eventAxis.axisIdentifier);
			break;
		}
		case EventAxis::Type::JoystickBallX:
		{
			eventAxis.active = Controller::HasAxisMoved(eventAxis.extraInfo, eventAxis.axisIdentifier);
			eventAxis.value = Controller::GetBallVector(eventAxis.extraInfo, eventAxis.axisIdentifier).x;
			break;
		}
		case EventAxis::Type::JoystickBallY:
		{
			eventAxis.active = Controller::HasAxisMoved(eventAxis.extraInfo, eventAxis.axisIdentifier);
			eventAxis.value = Controller::GetBallVector(eventAxis.extraInfo, eventAxis.axisIdentifier).y;
			break;
		}
		default: enAssert(false); break;
		}
	}
}

U32 EventSystem::AddButton(const char* name, EventButton::Type type, U32 buttonIdentifier, U32 extraInfo, EventButton::ActionType action)
{
	EventButton eventButton;
#ifdef ENGINE_DEBUG
	eventButton.name = name;
#endif // ENGINE_DEBUG
	eventButton.hash = HashFct(name);
	eventButton.active = false;
	eventButton.action = action;
	eventButton.type = type;
	eventButton.buttonIdentifier = buttonIdentifier;
	eventButton.extraInfo = extraInfo;
	GetInstance().mButtons.push_back(eventButton);
	return eventButton.hash;
}

U32 EventSystem::AddKeyButton(const char* name, Keyboard::Key key, EventButton::ActionType action, U32 modifiers /*= static_cast<U32>(Keyboard::Modifier::None)*/)
{
	return AddButton(name, EventButton::Type::KeyboardKey, static_cast<U32>(key), modifiers, action);
}

U32 EventSystem::AddMouseButton(const char* name, Mouse::Button mouseButton, EventButton::ActionType action)
{
	return AddButton(name, EventButton::Type::MouseButton, static_cast<U32>(mouseButton), 0, action);
}

U32 EventSystem::AddJoystickButton(const char* name, U32 controllerId, U32 buttonIndex, EventButton::ActionType action)
{
	return AddButton(name, EventButton::Type::JoystickButton, buttonIndex, controllerId, action);
}

bool EventSystem::IsButtonActive(const char* name)
{
	return IsButtonActive(HashFct(name));
}

bool EventSystem::IsButtonActive(U32 hash)
{
	EventSystem& eventSystem = GetInstance();
	const U32 buttonCount = static_cast<U32>(eventSystem.mButtons.size());
	for (U32 i = 0; i < buttonCount; ++i)
	{
		if (eventSystem.mButtons[i].hash == hash)
		{
			return eventSystem.mButtons[i].active;
		}
	}
	return false;
}

U32 EventSystem::GetButtonCount()
{
	return static_cast<U32>(GetInstance().mButtons.size());
}

void EventSystem::RemoveButtonAtIndex(U32 index)
{
	EventSystem& eventSystem = GetInstance();
	eventSystem.mButtons.erase(eventSystem.mButtons.begin() + index);
}

void EventSystem::RemoveButton(const char* name)
{
	RemoveButton(HashFct(name));
}

void EventSystem::RemoveButton(U32 hash)
{
	EventSystem& eventSystem = GetInstance();
	const U32 buttonCount = static_cast<U32>(eventSystem.mButtons.size());
	for (U32 i = 0; i < buttonCount; ++i)
	{
		if (eventSystem.mButtons[i].hash == hash)
		{
			eventSystem.mButtons.erase(eventSystem.mButtons.begin() + i);
			return;
		}
	}
}

U32 EventSystem::AddAxis(const char* name, EventAxis::Type type, U32 axisIdentifier /*= 0*/, U32 extraInfo /*= 0*/)
{
	EventAxis eventAxis;
#ifdef ENGINE_DEBUG
	eventAxis.name = name;
#endif // ENGINE_DEBUG
	eventAxis.hash = HashFct(name);
	eventAxis.active = false;
	eventAxis.value = 0.0f;
	eventAxis.type = type;
	eventAxis.axisIdentifier = axisIdentifier;
	eventAxis.extraInfo = extraInfo;
	GetInstance().mAxes.push_back(eventAxis);
	return eventAxis.hash;
}

bool EventSystem::IsAxisActive(const char* name)
{
	return IsAxisActive(HashFct(name));
}

bool EventSystem::IsAxisActive(U32 hash)
{
	EventSystem& eventSystem = GetInstance();
	const U32 axisCount = static_cast<U32>(eventSystem.mAxes.size());
	for (U32 i = 0; i < axisCount; ++i)
	{
		if (eventSystem.mAxes[i].hash == hash)
		{
			return eventSystem.mAxes[i].active;
		}
	}
	return false;
}

F32 EventSystem::GetAxisValue(const char* name)
{
	return GetAxisValue(HashFct(name));
}

F32 EventSystem::GetAxisValue(U32 hash)
{
	EventSystem& eventSystem = GetInstance();
	const U32 axisCount = static_cast<U32>(eventSystem.mAxes.size());
	for (U32 i = 0; i < axisCount; ++i)
	{
		if (eventSystem.mAxes[i].hash == hash)
		{
			return eventSystem.mAxes[i].value;
		}
	}
	return false;
}

U32 EventSystem::GetAxisCount()
{
	return static_cast<U32>(GetInstance().mAxes.size());
}

void EventSystem::RemoveAxisAtIndex(U32 index)
{
	EventSystem& eventSystem = GetInstance();
	eventSystem.mAxes.erase(eventSystem.mAxes.begin() + index);
}

void EventSystem::RemoveAxis(const char* name)
{
	RemoveAxis(HashFct(name));
}

void EventSystem::RemoveAxis(U32 hash)
{
	EventSystem& eventSystem = GetInstance();
	const U32 axisCount = static_cast<U32>(eventSystem.mAxes.size());
	for (U32 i = 0; i < axisCount; ++i)
	{
		if (eventSystem.mAxes[i].hash == hash)
		{
			eventSystem.mAxes.erase(eventSystem.mAxes.begin() + i);
			return;
		}
	}
}

U32 EventSystem::HashFct(const char* name)
{
	if (name != nullptr)
	{
		U32 h = 89;
		while (*name != 0)
		{
			h = h * 33 + (*name++);
		}
		return h;
	}
	else
	{
		return 0;
	}
}

EventSystem& EventSystem::GetInstance()
{
	static EventSystem instance;
	return instance;
}

EventSystem::EventSystem()
	: mButtons()
{
}

void EventSystem::HandleEvent(const SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_MOUSEMOTION:
	case SDL_MOUSEWHEEL:
	case SDL_MOUSEBUTTONDOWN:
	{
		Mouse::HandleEvent(event);
		break;
	}
	case SDL_KEYDOWN:
	case SDL_KEYUP:
	case SDL_TEXTINPUT:
	{
		Keyboard::HandleEvent(event);
		break;
	}
	case SDL_JOYAXISMOTION:
	case SDL_JOYBUTTONDOWN:
	case SDL_JOYBUTTONUP:
	case SDL_JOYHATMOTION:
	case SDL_JOYBALLMOTION:
	{
		Controller::HandleEvent(event);
		break;
	}
	case SDL_QUIT:
	{
		for (U32 i = 0; i < Window::sWindowCount; ++i)
		{
			if (Window* window = Window::sWindows[i])
			{
				window->Close();
			}
		}
		break;
	}
	case SDL_WINDOWEVENT:
	{
		if (Window* window = Window::GetWindowFromSDLWindowID(event.window.windowID))
		{
			switch (event.window.event)
			{
			case SDL_WINDOWEVENT_CLOSE:
			{
				window->Close();
				break;
			}
			case SDL_WINDOWEVENT_RESIZED:
			case SDL_WINDOWEVENT_SIZE_CHANGED:
			{
				window->OnResized(window, static_cast<U32>(event.window.data1), static_cast<U32>(event.window.data2));
				break;
			}
			}
		}
		break;
	}
	}
}

} // namespace en