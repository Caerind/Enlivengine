#pragma once

#include <SDL_events.h>
#include <SDL_scancode.h>

#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Utils/TypeInfo.hpp>

namespace en
{

class Keyboard
{
public:
	enum class Key
	{
		Unknown = 0,
		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		Num0,
		Num1,
		Num2,
		Num3,
		Num4,
		Num5,
		Num6,
		Num7,
		Num8,
		Num9,
		Return,
		Escape,
		Backspace,
		Tab,
		Space,
		LCtrl,
		LShift,
		LAlt,
		LSystem,
		RCtrl,
		RShift,
		RAlt,
		RSystem,
		Application,
		Menu,
		LBracket,
		RBracket,
		Semicolon,
		Comma,
		Period,
		Slash,
		Backslash,
		Tilde,
		Equal,
		Hyphen,
		PageUp,
		PageDown,
		End,
		Home,
		Insert,
		Delete,
		Left,
		Right,
		Up,
		Down,
		Pause,
		Numpad0,
		Numpad1,
		Numpad2,
		Numpad3,
		Numpad4,
		Numpad5,
		Numpad6,
		Numpad7,
		Numpad8,
		Numpad9,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		F13,
		F14,
		F15,

		Count
	};

	enum Modifier
	{
		None = 0,
		Alt = 0x01,
		Control = 0x02,
		Shift = 0x04,
		System = 0x08
	};

	static bool IsHold(Key key);
	static bool IsPressed(Key key);
	static bool IsReleased(Key key);

	static bool AreModifiersHold(U32 modifiers);
	static bool IsAltHold();
	static bool IsControlHold();
	static bool IsShiftHold();
	static bool IsSystemHold();

	static const char* GetInputCharacters();
	static U32 GetInputCharacterCount();

	static const char* GetKeyName(Key key);

private:
	static Key GetKeyFromSDLKey(SDL_Scancode scancode);

private:
	friend class EventSystem;
	static void Refresh();
	static void HandleEvent(const SDL_Event& event);

private:
	static Keyboard& GetInstance();

	Keyboard();

	// NonCopyable & NonMovable
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;
	Keyboard(Keyboard&&) = delete;
	Keyboard& operator=(Keyboard&&) = delete;

	static constexpr U32 kKeyCount = static_cast<U32>(Key::Count);
	static constexpr U32 kMaxInputCharacters = 32;

	enum class KeyState
	{
		None = 0,
		Hold = 0x01,
		Pressed = 0x02,
		Released = 0x04
	};
	U8 mKeyStates[kKeyCount];
	U32 mModifiers;
	U32 mInputCharacterCount;
	char mInputCharacters[kMaxInputCharacters];
};

} // namespace en

ENLIVE_DEFINE_ENUM_INFO(en::Keyboard::Key)