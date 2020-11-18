#include <Enlivengine/Window/Controller.hpp>

namespace en
{

U32 Controller::GetJoystickCount()
{
	return static_cast<U32>(SDL_NumJoysticks());
}

U32 Controller::GetHapticCount()
{
	return static_cast<U32>(SDL_NumHaptics());
}

void Controller::SetThreshold(U32 threshold)
{
	GetInstance().mThreshold = threshold;
}

U32 Controller::GetThreshold()
{
	return GetInstance().mThreshold;
}

void Controller::Refresh()
{
	Controller& controller = GetInstance();

	static constexpr U8 clearEvents = static_cast<U8>(Joystick::State::Pressed) | static_cast<U8>(Joystick::State::Released);

	const U32 joystickCount = static_cast<U32>(controller.mJoysticks.size());
	for (U32 i = 0; i < joystickCount; ++i)
	{
		const U32 axisCount = static_cast<U32>(controller.mJoysticks[i].axes.size());
		for (U32 j = 0; j < axisCount; ++j)
		{
			controller.mJoysticks[i].axes[j].moved = false;
		}
		const U32 ballCount = static_cast<U32>(controller.mJoysticks[i].balls.size());
		for (U32 j = 0; j < ballCount; ++j)
		{
			controller.mJoysticks[i].balls[j].moved = false;
		}
		const U32 buttonCount = static_cast<U32>(controller.mJoysticks[i].buttons.size());
		for (U32 j = 0; j < buttonCount; ++j)
		{
			controller.mJoysticks[i].buttons[j] &= ~clearEvents;
		}
		const U32 hatCount = static_cast<U32>(controller.mJoysticks[i].hats.size());
		for (U32 j = 0; j < hatCount; ++j)
		{
			controller.mJoysticks[i].hats[j].state &= ~clearEvents;
		}
	}
}

void Controller::HandleEvent(const SDL_Event& event)
{
	const bool buttonUp = event.type == SDL_JOYBUTTONUP;
	const bool buttonDown = event.type == SDL_JOYBUTTONDOWN;
	if (event.type == SDL_JOYAXISMOTION)
	{
		if (Joystick* joystick = GetJoystickFromJoystickID(event.jaxis.which))
		{
			if (event.jaxis.axis < static_cast<U32>(joystick->axes.size()))
			{
				joystick->axes[event.jaxis.axis].value = event.jaxis.value;
				joystick->axes[event.jaxis.axis].moved = true;
			}
		}
	}
	else if (buttonUp || buttonDown)
	{
		if (Joystick* joystick = GetJoystickFromJoystickID(event.jbutton.which))
		{
			if (event.jbutton.button < static_cast<U32>(joystick->buttons.size()))
			{
				if (buttonDown)
				{
					joystick->buttons[event.jbutton.button] = static_cast<U8>(Joystick::State::Pressed) | static_cast<U8>(Joystick::State::Hold);
				}
				else
				{
					joystick->buttons[event.jbutton.button] = static_cast<U8>(Joystick::State::Released);
				}
			}
		}
	}
	else if (event.type == SDL_JOYHATMOTION)
	{
		if (Joystick* joystick = GetJoystickFromJoystickID(event.jhat.which))
		{
			if (event.jhat.hat < static_cast<U32>(joystick->hats.size()))
			{
				joystick->hats[event.jhat.hat].previousValue = joystick->hats[event.jhat.hat].value;
				joystick->hats[event.jhat.hat].value = static_cast<HatValue>(event.jhat.value);
				if (event.jhat.value == 0)
				{
					joystick->hats[event.jhat.hat].state = static_cast<U8>(Joystick::State::Released);
				}
				else
				{
					joystick->hats[event.jhat.hat].state = static_cast<U8>(Joystick::State::Pressed) | static_cast<U8>(Joystick::State::Hold);
				}
			}
		}
	}
	else if (event.type == SDL_JOYBALLMOTION)
	{
		if (Joystick* joystick = GetJoystickFromJoystickID(event.jball.which))
		{
			if (event.jball.ball < static_cast<U32>(joystick->balls.size()))
			{
				joystick->balls[event.jball.ball].deltaX = event.jball.xrel;
				joystick->balls[event.jball.ball].deltaY = event.jball.yrel;
				joystick->balls[event.jball.ball].moved = true;
			}
		}
	}
}

bool Controller::IsValid(U32 controllerId)
{
	if (Joystick* joystick = GetJoystickFromControllerID(controllerId))
	{
		return joystick->joystick != nullptr;
	}
	return false;
}

const char* Controller::GetName(U32 controllerId)
{
	if (Joystick* joystick = GetJoystickFromControllerID(controllerId))
	{
		if (SDL_Joystick* joy = joystick->joystick)
		{
			return SDL_JoystickName(joy);
		}
	}
	return "";
}

U32 Controller::GetAxisCount(U32 controllerId)
{
	if (Joystick* joystick = GetJoystickFromControllerID(controllerId))
	{
		if (SDL_Joystick* joy = joystick->joystick)
		{
			return SDL_JoystickNumAxes(joy);
		}
	}
	return 0;
}

U32 Controller::GetBallCount(U32 controllerId)
{
	if (Joystick* joystick = GetJoystickFromControllerID(controllerId))
	{
		if (SDL_Joystick* joy = joystick->joystick)
		{
			return SDL_JoystickNumBalls(joy);
		}
	}
	return 0;
}

U32 Controller::GetButtonCount(U32 controllerId)
{
	if (Joystick* joystick = GetJoystickFromControllerID(controllerId))
	{
		if (SDL_Joystick* joy = joystick->joystick)
		{
			return SDL_JoystickNumButtons(joy);
		}
	}
	return 0;
}

U32 Controller::GetHatCount(U32 controllerId)
{
	if (Joystick* joystick = GetJoystickFromControllerID(controllerId))
	{
		if (SDL_Joystick* joy = joystick->joystick)
		{
			return SDL_JoystickNumHats(joy);
		}
	}
	return 0;
}

bool Controller::HasAxisMoved(U32 controllerId, U32 axisIndex)
{
	if (Joystick* joystick = GetJoystickFromControllerID(controllerId))
	{
		if (axisIndex < static_cast<U32>(joystick->axes.size()))
		{
			return joystick->axes[axisIndex].moved;
		}
	}
	return false;
}

F32 Controller::GetAxis(U32 controllerId, U32 axisIndex)
{
	if (Joystick* joystick = GetJoystickFromControllerID(controllerId))
	{
		if (axisIndex < static_cast<U32>(joystick->axes.size()))
		{
			return NormalizeSignedInt(joystick->axes[axisIndex].value);
		}
	}
	return 0.0f;
}

bool Controller::HasBallMoved(U32 controllerId, U32 ballIndex)
{
	if (Joystick* joystick = GetJoystickFromControllerID(controllerId))
	{
		if (ballIndex < static_cast<U32>(joystick->balls.size()))
		{
			return joystick->balls[ballIndex].moved;
		}
	}
	return false;
}

Vector2f Controller::GetBallVector(U32 controllerId, U32 ballIndex)
{
	Vector2f delta;
	if (Joystick* joystick = GetJoystickFromControllerID(controllerId))
	{
		if (ballIndex < static_cast<U32>(joystick->balls.size()))
		{
			delta.x = NormalizeSignedInt(joystick->balls[ballIndex].deltaX);
			delta.y = NormalizeSignedInt(joystick->balls[ballIndex].deltaY); // TODO : Ensures Y is up
		}
	}
	return delta;
}

bool Controller::IsButtonHold(U32 controllerId, U32 buttonIndex)
{
	if (Joystick* joystick = GetJoystickFromControllerID(controllerId))
	{
		if (buttonIndex < static_cast<U32>(joystick->buttons.size()))
		{
			return (joystick->buttons[buttonIndex] & static_cast<U8>(Joystick::State::Hold)) > 0;
		}
	}
	return false;
}

bool Controller::IsButtonPressed(U32 controllerId, U32 buttonIndex)
{
	if (Joystick* joystick = GetJoystickFromControllerID(controllerId))
	{
		if (buttonIndex < static_cast<U32>(joystick->buttons.size()))
		{
			return (joystick->buttons[buttonIndex] & static_cast<U8>(Joystick::State::Pressed)) > 0;
		}
	}
	return false;
}

bool Controller::IsButtonReleased(U32 controllerId, U32 buttonIndex)
{
	if (Joystick* joystick = GetJoystickFromControllerID(controllerId))
	{
		if (buttonIndex < static_cast<U32>(joystick->buttons.size()))
		{
			return (joystick->buttons[buttonIndex] & static_cast<U8>(Joystick::State::Released)) > 0;
		}
	}
	return false;
}

bool Controller::IsHatHold(U32 controllerId, U32 hatIndex, HatValue value)
{
	if (Joystick* joystick = GetJoystickFromControllerID(controllerId))
	{
		if (hatIndex < static_cast<U32>(joystick->hats.size()))
		{
			if (joystick->hats[hatIndex].value == value)
			{
				return (joystick->hats[hatIndex].state & static_cast<U8>(Joystick::State::Hold)) > 0;
			}
		}
	}
	return false;
}

bool Controller::IsHatPressed(U32 controllerId, U32 hatIndex, HatValue value)
{
	if (Joystick* joystick = GetJoystickFromControllerID(controllerId))
	{
		if (hatIndex < static_cast<U32>(joystick->hats.size()))
		{
			if (joystick->hats[hatIndex].value == value)
			{
				return (joystick->hats[hatIndex].state & static_cast<U8>(Joystick::State::Pressed)) > 0;
			}
		}
	}
	return false;
}

bool Controller::IsHatReleased(U32 controllerId, U32 hatIndex, HatValue value)
{
	if (Joystick* joystick = GetJoystickFromControllerID(controllerId))
	{
		if (hatIndex < static_cast<U32>(joystick->hats.size()))
		{
			if (joystick->hats[hatIndex].previousValue == value)
			{
				return (joystick->hats[hatIndex].state & static_cast<U8>(Joystick::State::Released)) > 0;
			}
		}
	}
	return false;
}

Vector2f Controller::GetHatVector(U32 controllerId, U32 hatIndex)
{
	Vector2f hatVector;
	if (Joystick* joystick = GetJoystickFromControllerID(controllerId))
	{
		if (hatIndex < static_cast<U32>(joystick->hats.size()))
		{
			const HatValue hatValue = joystick->hats[hatIndex].value;
			const U32 value = static_cast<U32>(hatValue);
			if ((value & static_cast<U32>(HatValue::Left)) > 0)
			{
				hatVector.x -= 1.0f;
			}
			if ((value & static_cast<U32>(HatValue::Right)) > 0)
			{
				hatVector.x += 1.0f;
			}
			if ((value & static_cast<U32>(HatValue::Up)) > 0)
			{
				hatVector.y += 1.0f;
			}
			if ((value & static_cast<U32>(HatValue::Down)) > 0)
			{
				hatVector.y -= 1.0f;
			}
		}
	}
	return hatVector;
}

bool Controller::IsHaptic(U32 controllerId)
{
	if (Joystick* joystick = GetJoystickFromControllerID(controllerId))
	{
		if (SDL_Joystick* joy = joystick->joystick)
		{
			return SDL_JoystickIsHaptic(joy) && joystick->haptic != nullptr;
		}
	}
	return false;
}

bool Controller::Rumble(U32 controllerId, F32 strength, U32 durationMs)
{
	if (Joystick* joystick = GetJoystickFromControllerID(controllerId))
	{
		if (SDL_Haptic* haptic = joystick->haptic)
		{
			return SDL_HapticRumblePlay(haptic, strength, durationMs) >= 0;
		}
	}
	return false;
}

Controller& Controller::GetInstance()
{
	static Controller instance;
	return instance;
}

Controller::Controller()
	: mJoysticks()
{
	// Use the SDL event loop : Recommended by documentation
	SDL_JoystickEventState(SDL_ENABLE);

	mThreshold = 3200;

	const U32 joystickCount = GetJoystickCount();
	mJoysticks.reserve(joystickCount);
	for (U32 i = 0; i < joystickCount; ++i)
	{
		Joystick joystick;
		joystick.joystick = SDL_JoystickOpen(static_cast<int>(i));
		if (joystick.joystick != nullptr)
		{
			joystick.axes.resize(SDL_JoystickNumAxes(joystick.joystick));
			joystick.balls.resize(SDL_JoystickNumBalls(joystick.joystick));
			joystick.buttons.resize(SDL_JoystickNumButtons(joystick.joystick), 0);
			joystick.hats.resize(SDL_JoystickNumHats(joystick.joystick));

			if (SDL_JoystickIsHaptic(joystick.joystick))
			{
				joystick.haptic = SDL_HapticOpenFromJoystick(joystick.joystick);
				if (joystick.haptic != nullptr)
				{
					SDL_HapticRumbleInit(joystick.haptic);
				}
			}
		}
		mJoysticks.push_back(joystick);
	}
}

Controller::Joystick::Joystick()
	: joystick(nullptr)
	, haptic(nullptr)
{
}

Controller::Joystick* Controller::GetJoystickFromControllerID(U32 controllerId)
{
	Controller& controller = GetInstance();
	if (controllerId < static_cast<U32>(controller.mJoysticks.size()))
	{
		return &controller.mJoysticks[controllerId];
	}
	return nullptr;
}

Controller::Joystick* Controller::GetJoystickFromJoystickID(SDL_JoystickID joystickId)
{
	Controller& controller = GetInstance();
	const U32 joystickCount = static_cast<U32>(controller.mJoysticks.size());
	for (U32 i = 0; i < joystickCount; ++i)
	{
		if (SDL_Joystick* joystick = controller.mJoysticks[i].joystick)
		{
			if (SDL_JoystickInstanceID(joystick) == joystickId)
			{
				return &controller.mJoysticks[i];
			}
		}
	}
	return nullptr;
}

F32 Controller::NormalizeSignedInt(I16 value)
{
	const I16 threshold = static_cast<I16>(GetInstance().mThreshold);
	F32 length = 1.0f;
	if (value >= 0)
	{
		value -= threshold;
		if (value < 0.0f)
		{
			return 0.0f;
		}
		length = static_cast<F32>(32767 - threshold);
	}
	else
	{
		value += threshold;
		if (value > 0.0f)
		{
			return 0.0f;
		}
		length = static_cast<F32>(32768 - threshold);
	}
	return static_cast<F32>(value) / length;
}

} // namespace en