#pragma once

#include <vector>

#include <SDL_events.h>
#include <SDL_joystick.h>
#include <SDL_haptic.h>

#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Math/Vector2.hpp>

namespace en
{

class Controller
{
public:
	static U32 GetJoystickCount();
	static U32 GetHapticCount();
	static void SetThreshold(U32 threshold);
	static U32 GetThreshold();

	static bool IsValid(U32 controllerId);
	static const char* GetName(U32 controllerId);
	static U32 GetAxisCount(U32 controllerId);
	static U32 GetBallCount(U32 controllerId);
	static U32 GetButtonCount(U32 controllerId);
	static U32 GetHatCount(U32 controllerId);

	// Axes
	static bool HasAxisMoved(U32 controllerId, U32 axisIndex);
	static F32 GetAxis(U32 controllerId, U32 axisIndex);

	// Balls
	static bool HasBallMoved(U32 controllerId, U32 ballIndex);
	static Vector2f GetBallVector(U32 controllerId, U32 ballIndex); // Not normalized, Y up
	
	// Buttons
	static bool IsButtonHold(U32 controllerId, U32 buttonIndex);
	static bool IsButtonPressed(U32 controllerId, U32 buttonIndex);
	static bool IsButtonReleased(U32 controllerId, U32 buttonIndex);

	// Hats
	enum class HatValue
	{
		Centered = 0,
		Up = 0x01,
		Right = 0x02,
		Down = 0x04,
		Left = 0x08,
		RightUp = (Right | Up),
		RightDown = (Right | Down),
		LeftUp = (Left | Up),
		LeftDown = (Left | Down)
	};
	static bool IsHatHold(U32 controllerId, U32 hatIndex, HatValue value);
	static bool IsHatPressed(U32 controllerId, U32 hatIndex, HatValue value);
	static bool IsHatReleased(U32 controllerId, U32 hatIndex, HatValue value);
	static Vector2f GetHatVector(U32 controllerId, U32 hatIndex); // Not normalized, Y up

	// Haptic
	static bool IsHaptic(U32 controllerId);
	static bool Rumble(U32 controllerId, F32 strength, U32 durationMs);

private:
	friend class EventSystem;
	static void Refresh();
	static void HandleEvent(const SDL_Event& event);

private:
	static Controller& GetInstance();

	Controller();

	struct Joystick
	{
		Joystick();

		SDL_Joystick* joystick;
		SDL_Haptic* haptic;

		enum class State
		{
			None = 0,
			Hold = 0x01,
			Pressed = 0x02,
			Released = 0x04
		};

		struct AxisData
		{
			I16 value{ 0 };
			bool moved{ false };
		};
		std::vector<AxisData> axes;

		struct BallData
		{
			I16 deltaX{ 0 };
			I16 deltaY{ 0 };
			bool moved{ false };
		};
		std::vector<BallData> balls;

		std::vector<U8> buttons;

		struct HatData
		{
			HatValue value{ HatValue::Centered };
			HatValue previousValue{ HatValue::Centered };
			U8 state{ 0 };
		};
		std::vector<HatData> hats;
	};

	std::vector<Joystick> mJoysticks;
	U32 mThreshold;

	static Joystick* GetJoystickFromControllerID(U32 controllerId);
	static Joystick* GetJoystickFromJoystickID(SDL_JoystickID joystickId);

	static F32 NormalizeSignedInt(I16 value);
};

} // namespace en