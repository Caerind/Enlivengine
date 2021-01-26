#include <Enlivengine/Window/EventSystem.hpp>

#include <Enlivengine/Utils/Assert.hpp>
#include <Enlivengine/SDL/SDLWrapper.hpp>

#include <Enlivengine/Window/Window.hpp>
#include <Enlivengine/Window/Controller.hpp>

#include <Enlivengine/Utils/Serializer.hpp>

#ifdef ENLIVE_ENABLE_IMGUI
#include <imgui/imgui.h>
#include <IconFontCppHeaders/IconsFontAwesome5.h>
#endif // ENLIVE_ENABLE_IMGUI

namespace en
{

bool EventSystem::EventButton::Edit(ObjectEditor& objectEditor, const char* _name)
{
	if (objectEditor.BeginClass(_name, TypeInfo<EventSystem::EventButton>::GetName(), TypeInfo<EventSystem::EventButton>::GetHash()))
	{
		bool ret = false;

#ifdef ENLIVE_ENABLE_IMGUI
		if (objectEditor.IsImGuiEditor())
		{
			if (EventSystem::HasButton(hash))
			{
				ImGui::Text("Active: %s", EventSystem::IsButtonActive(hash) ? ICON_FA_BELL : ICON_FA_BELL_SLASH);
			}
		}
#endif // ENLIVE_ENABLE_IMGUI   

		ret = GenericEdit(objectEditor, "Type", type) || ret;
		ret = GenericEdit(objectEditor, "Action", action) || ret;

		switch (type)
		{
		case EventSystem::EventButton::Type::KeyboardKey:
		{
			Keyboard::Key key = static_cast<Keyboard::Key>(buttonIdentifier);
			if (GenericEdit(objectEditor, "Key", key))
			{
				buttonIdentifier = static_cast<U32>(key);
				ret = true;
			}

			bool alt = (extraInfo & Keyboard::Modifier::Alt) > 0;
			bool control = (extraInfo & Keyboard::Modifier::Control) > 0;
			bool shift = (extraInfo & Keyboard::Modifier::Shift) > 0;
			bool system = (extraInfo & Keyboard::Modifier::System) > 0;

			ret = GenericEdit(objectEditor, "Alt", alt) || ret;
#ifdef ENLIVE_ENABLE_IMGUI
			if (objectEditor.IsImGuiEditor())
			{
				ImGui::SameLine();
			}
#endif // ENLIVE_ENABLE_IMGUI 
			ret = GenericEdit(objectEditor, "Ctrl", control) || ret;

			ret = GenericEdit(objectEditor, "Shift", shift) || ret;
#ifdef ENLIVE_ENABLE_IMGUI
			if (objectEditor.IsImGuiEditor())
			{
				ImGui::SameLine();
			}
#endif // ENLIVE_ENABLE_IMGUI 
			ret = GenericEdit(objectEditor, "System", system) || ret;

			if (ret)
			{
				extraInfo = 0 |
					((alt) ? static_cast<U32>(Keyboard::Modifier::Alt) : 0) |
					((control) ? static_cast<U32>(Keyboard::Modifier::Control) : 0) |
					((shift) ? static_cast<U32>(Keyboard::Modifier::Shift) : 0) |
					((system) ? static_cast<U32>(Keyboard::Modifier::System) : 0);
			}

		} break;
		case EventSystem::EventButton::Type::MouseButton:
		{
			Mouse::Button mbutton = static_cast<Mouse::Button>(buttonIdentifier);
			if (GenericEdit(objectEditor, "Button", mbutton))
			{
				buttonIdentifier = static_cast<U32>(mbutton);
				ret = true;
			}
		} break;
		case EventSystem::EventButton::Type::JoystickButton:
		{
			ret = GenericEdit(objectEditor, "Button", buttonIdentifier) || ret;
			ret = GenericEdit(objectEditor, "Controller", extraInfo) || ret;
		} break;
		}

		objectEditor.EndClass();
		return ret;
	}
	else
	{
		return false;
	}
}

bool EventSystem::EventAxis::Edit(ObjectEditor& objectEditor, const char* _name)
{
	if (objectEditor.BeginClass(_name, TypeInfo<EventSystem::EventAxis>::GetName(), TypeInfo<EventSystem::EventAxis>::GetHash()))
	{
		bool ret = false;

#ifdef ENLIVE_ENABLE_IMGUI
		if (objectEditor.IsImGuiEditor())
		{
			if (EventSystem::HasAxis(hash))
			{
				ImGui::Text("Value: %f", EventSystem::GetAxisValue(hash));
			}
		}
#endif // ENLIVE_ENABLE_IMGUI   

		ret = GenericEdit(objectEditor, "Type", type) || ret;

		if (type == EventSystem::EventAxis::Type::JoystickAxis
			|| type == EventSystem::EventAxis::Type::JoystickBallX
			|| type == EventSystem::EventAxis::Type::JoystickBallY)
		{
			ret = GenericEdit(objectEditor, "Axis", axisIdentifier) || ret;
			ret = GenericEdit(objectEditor, "Controller", extraInfo) || ret;
		}

		objectEditor.EndClass();
		return ret;
	}
	else
	{
		return false;
	}
}

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
	const U32 hash = HashFct(name);
	if (HasButton(hash))
	{
		return hash;
	}

	EventButton eventButton;
	eventButton.name = name;
	eventButton.hash = hash;
	eventButton.active = false;
	eventButton.action = action;
	eventButton.type = type;
	eventButton.buttonIdentifier = buttonIdentifier;
	eventButton.extraInfo = extraInfo;
	GetInstance().mButtons.push_back(eventButton);
	return hash;
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

bool EventSystem::HasButton(const char* name)
{
	return IsButtonActive(HashFct(name));
}

bool EventSystem::HasButton(U32 hash)
{
	EventSystem& eventSystem = GetInstance();
	const U32 buttonCount = static_cast<U32>(eventSystem.mButtons.size());
	for (U32 i = 0; i < buttonCount; ++i)
	{
		if (eventSystem.mButtons[i].hash == hash)
		{
			return true;
		}
	}
	return false;
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

const EventSystem::EventButton& EventSystem::GetButton(U32 index)
{
	enAssert(index < GetButtonCount());
	return GetInstance().mButtons[index];
}

const std::vector<en::EventSystem::EventButton>& EventSystem::GetButtons()
{
	return GetInstance().mButtons;
}

const EventSystem::EventButton& EventSystem::GetLastButton()
{
	return GetInstance().mLastButton;
}

U32 EventSystem::AddAxis(const char* name, EventAxis::Type type, U32 axisIdentifier /*= 0*/, U32 extraInfo /*= 0*/)
{
	const U32 hash = HashFct(name);
	if (HasAxis(hash))
	{
		return hash;
	}

	EventAxis eventAxis;
	eventAxis.name = name;
	eventAxis.hash = hash;
	eventAxis.active = false;
	eventAxis.value = 0.0f;
	eventAxis.type = type;
	eventAxis.axisIdentifier = axisIdentifier;
	eventAxis.extraInfo = extraInfo;
	GetInstance().mAxes.push_back(eventAxis);
	return hash;
}

bool EventSystem::HasAxis(const char* name)
{
	return HasAxis(HashFct(name));
}

bool EventSystem::HasAxis(U32 hash)
{
	EventSystem& eventSystem = GetInstance();
	const U32 axisCount = static_cast<U32>(eventSystem.mAxes.size());
	for (U32 i = 0; i < axisCount; ++i)
	{
		if (eventSystem.mAxes[i].hash == hash)
		{
			return true;
		}
	}
	return false;
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

const EventSystem::EventAxis& EventSystem::GetAxis(U32 index)
{
	enAssert(index < GetButtonCount());
	return GetInstance().mAxes[index];
}

const std::vector<en::EventSystem::EventAxis>& EventSystem::GetAxes()
{
	return GetInstance().mAxes;
}

void EventSystem::SetLastButton(EventButton::Type type, U32 buttonIdentifier, U32 extraInfo /*= 0*/)
{
	auto& system = GetInstance();
	system.mLastButton.type = type;
	system.mLastButton.action = EventButton::ActionType::Pressed;
	system.mLastButton.buttonIdentifier = buttonIdentifier;
	system.mLastButton.extraInfo = extraInfo;
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