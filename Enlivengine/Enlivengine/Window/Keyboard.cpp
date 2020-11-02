#include <Enlivengine/Window/Keyboard.hpp>

#ifdef ENLIVE_MODULE_WINDOW

#include <Enlivengine/Utils/Assert.hpp>

namespace en
{

void Keyboard::Refresh()
{
	Keyboard& keyboard = GetInstance();

	const U8 clearEvents = static_cast<U8>(KeyState::Pressed) | static_cast<U8>(KeyState::Released);
	for (U32 i = 0; i < kKeyCount; ++i)
	{
		keyboard.mKeyStates[i] &= ~clearEvents;
	}
	keyboard.mInputCharacters[0] = '\0';
	keyboard.mInputCharacterCount = 0;
}

void Keyboard::HandleEvent(const SDL_Event& event)
{
	Keyboard& keyboard = GetInstance();
	const bool keyDown = event.type == SDL_KEYDOWN;
	const bool keyUp = event.type == SDL_KEYUP;
	if (keyDown || keyUp)
	{
		const Key key = GetKeyFromSDLKey(event.key.keysym.scancode);
		if (keyDown)
		{
			keyboard.mKeyStates[static_cast<U32>(key)] = (static_cast<U8>(KeyState::Hold) | static_cast<U8>(KeyState::Pressed));
		}
		else
		{
			keyboard.mKeyStates[static_cast<U32>(key)] = static_cast<U8>(KeyState::Released);
		}

		U8 modifier = 0;
		if (key == Key::LAlt || key == Key::RAlt)
		{
			modifier = static_cast<U8>(Modifier::Alt);
		}
		else if (key == Key::LCtrl || key == Key::RCtrl)
		{
			modifier = static_cast<U8>(Modifier::Control);
		}
		else if (key == Key::LShift || key == Key::RShift)
		{
			modifier = static_cast<U8>(Modifier::Shift);
		}
		else if (key == Key::LSystem || key == Key::RSystem)
		{
			modifier = static_cast<U8>(Modifier::System);
		}

		if (modifier > 0)
		{
			if (keyDown)
			{
				keyboard.mModifiers |= modifier;
			}
			else // keyUp
			{
				keyboard.mModifiers &= ~modifier;
			}
		}
	}
	else if (event.type == SDL_TEXTINPUT)
	{
		const U32 inputLength = static_cast<U32>(strlen(event.text.text));
		if (keyboard.mInputCharacterCount + inputLength < kMaxInputCharacters)
		{
#ifdef ENLIVE_COMPILER_MSVC
			strcat_s(keyboard.mInputCharacters, event.text.text);
#else
			strcat(keyboard.mInputCharacters, event.text.text);
#endif // ENLIVE_COMPILER_MSVC
			keyboard.mInputCharacterCount += inputLength;
		}
		else
		{
			// Maximum amount of input characters reached
			// Increase Keyboard::kMaxInputCharacters or use less input characters
			enAssert(false);
		}
	}
}

bool Keyboard::IsHold(Key key)
{
	return (GetInstance().mKeyStates[static_cast<U32>(key)] & static_cast<U8>(KeyState::Hold)) > 0;
}

bool Keyboard::IsPressed(Key key)
{
	return (GetInstance().mKeyStates[static_cast<U32>(key)] & static_cast<U8>(KeyState::Pressed)) > 0;
}

bool Keyboard::IsReleased(Key key)
{
	return (GetInstance().mKeyStates[static_cast<U32>(key)] & static_cast<U8>(KeyState::Released)) > 0;
}

bool Keyboard::AreModifiersHold(U32 modifiers)
{
	return (GetInstance().mModifiers & modifiers) > 0;
}

bool Keyboard::IsAltHold()
{
	return (GetInstance().mModifiers & static_cast<U32>(Modifier::Alt)) > 0;
}

bool Keyboard::IsControlHold()
{
	return (GetInstance().mModifiers & static_cast<U32>(Modifier::Control)) > 0;
}

bool Keyboard::IsShiftHold()
{
	return (GetInstance().mModifiers & static_cast<U32>(Modifier::Shift)) > 0;
}

bool Keyboard::IsSystemHold()
{
	return (GetInstance().mModifiers & static_cast<U32>(Modifier::System)) > 0;
}

const char* Keyboard::GetInputCharacters()
{
	return GetInstance().mInputCharacters;
}

U32 Keyboard::GetInputCharacterCount()
{
	return GetInstance().mInputCharacterCount;
}

const char* Keyboard::GetKeyName(Key key)
{
#define KeyValueToKeyName(key) case Key::key: return #key; break;
	switch (key)
	{
		KeyValueToKeyName(A);
		KeyValueToKeyName(B);
		KeyValueToKeyName(C);
		KeyValueToKeyName(D);
		KeyValueToKeyName(E);
		KeyValueToKeyName(F);
		KeyValueToKeyName(G);
		KeyValueToKeyName(H);
		KeyValueToKeyName(I);
		KeyValueToKeyName(J);
		KeyValueToKeyName(K);
		KeyValueToKeyName(L);
		KeyValueToKeyName(M);
		KeyValueToKeyName(N);
		KeyValueToKeyName(O);
		KeyValueToKeyName(P);
		KeyValueToKeyName(Q);
		KeyValueToKeyName(R);
		KeyValueToKeyName(S);
		KeyValueToKeyName(T);
		KeyValueToKeyName(U);
		KeyValueToKeyName(V);
		KeyValueToKeyName(W);
		KeyValueToKeyName(X);
		KeyValueToKeyName(Y);
		KeyValueToKeyName(Z);
		KeyValueToKeyName(Num0);
		KeyValueToKeyName(Num1);
		KeyValueToKeyName(Num2);
		KeyValueToKeyName(Num3);
		KeyValueToKeyName(Num4);
		KeyValueToKeyName(Num5);
		KeyValueToKeyName(Num6);
		KeyValueToKeyName(Num7);
		KeyValueToKeyName(Num8);
		KeyValueToKeyName(Num9);
		KeyValueToKeyName(Return);
		KeyValueToKeyName(Escape);
		KeyValueToKeyName(Backspace);
		KeyValueToKeyName(Tab);
		KeyValueToKeyName(Space);
		KeyValueToKeyName(LCtrl);
		KeyValueToKeyName(LShift);
		KeyValueToKeyName(LAlt);
		KeyValueToKeyName(LSystem);
		KeyValueToKeyName(RCtrl);
		KeyValueToKeyName(RShift);
		KeyValueToKeyName(RAlt);
		KeyValueToKeyName(RSystem);
		KeyValueToKeyName(Menu);
		KeyValueToKeyName(LBracket);
		KeyValueToKeyName(RBracket);
		KeyValueToKeyName(Semicolon);
		KeyValueToKeyName(Comma);
		KeyValueToKeyName(Period);
		KeyValueToKeyName(Slash);
		KeyValueToKeyName(Backslash);
		KeyValueToKeyName(Tilde);
		KeyValueToKeyName(Equal);
		KeyValueToKeyName(Hyphen);
		KeyValueToKeyName(PageUp);
		KeyValueToKeyName(PageDown);
		KeyValueToKeyName(End);
		KeyValueToKeyName(Home);
		KeyValueToKeyName(Insert);
		KeyValueToKeyName(Delete);
		KeyValueToKeyName(Left);
		KeyValueToKeyName(Right);
		KeyValueToKeyName(Up);
		KeyValueToKeyName(Down);
		KeyValueToKeyName(Pause);
		KeyValueToKeyName(F1);
		KeyValueToKeyName(F2);
		KeyValueToKeyName(F3);
		KeyValueToKeyName(F4);
		KeyValueToKeyName(F5);
		KeyValueToKeyName(F6);
		KeyValueToKeyName(F7);
		KeyValueToKeyName(F8);
		KeyValueToKeyName(F9);
		KeyValueToKeyName(F10);
		KeyValueToKeyName(F11);
		KeyValueToKeyName(F12);
		default: break;
	}
#undef KeyValueToKeyName
	return "Unknown";
}

Keyboard::Key Keyboard::GetKeyFromSDLKey(SDL_Scancode scancode)
{
	switch (scancode)
	{
    case SDL_SCANCODE_A: return Key::A; break;
    case SDL_SCANCODE_B: return Key::B; break;
    case SDL_SCANCODE_C: return Key::C; break;
    case SDL_SCANCODE_D: return Key::D; break;
    case SDL_SCANCODE_E: return Key::E; break;
    case SDL_SCANCODE_F: return Key::F; break;
    case SDL_SCANCODE_G: return Key::G; break;
    case SDL_SCANCODE_H: return Key::H; break;
    case SDL_SCANCODE_I: return Key::I; break;
    case SDL_SCANCODE_J: return Key::J; break;
    case SDL_SCANCODE_K: return Key::K; break;
    case SDL_SCANCODE_L: return Key::L; break;
    case SDL_SCANCODE_M: return Key::M; break;
    case SDL_SCANCODE_N: return Key::N; break;
    case SDL_SCANCODE_O: return Key::O; break;
    case SDL_SCANCODE_P: return Key::P; break;
    case SDL_SCANCODE_Q: return Key::Q; break;
    case SDL_SCANCODE_R: return Key::R; break;
    case SDL_SCANCODE_S: return Key::S; break;
    case SDL_SCANCODE_T: return Key::T; break;
    case SDL_SCANCODE_U: return Key::U; break;
    case SDL_SCANCODE_V: return Key::V; break;
    case SDL_SCANCODE_W: return Key::W; break;
    case SDL_SCANCODE_X: return Key::X; break;
    case SDL_SCANCODE_Y: return Key::Y; break;
    case SDL_SCANCODE_Z: return Key::Z; break;
    case SDL_SCANCODE_0: return Key::Num0; break;
    case SDL_SCANCODE_1: return Key::Num1; break;
    case SDL_SCANCODE_2: return Key::Num2; break;
    case SDL_SCANCODE_3: return Key::Num3; break;
    case SDL_SCANCODE_4: return Key::Num4; break;
    case SDL_SCANCODE_5: return Key::Num5; break;
    case SDL_SCANCODE_6: return Key::Num6; break;
    case SDL_SCANCODE_7: return Key::Num7; break;
	case SDL_SCANCODE_8: return Key::Num8; break;
	case SDL_SCANCODE_9: return Key::Num9; break;
	case SDL_SCANCODE_RETURN: return Key::Return; break;
	case SDL_SCANCODE_ESCAPE: return Key::Escape; break;
	case SDL_SCANCODE_BACKSPACE: return Key::Backspace; break;
	case SDL_SCANCODE_TAB: return Key::Tab; break;
	case SDL_SCANCODE_SPACE: return Key::Space; break;
	case SDL_SCANCODE_LCTRL: return Key::LCtrl; break;
	case SDL_SCANCODE_LSHIFT: return Key::LShift; break;
	case SDL_SCANCODE_LALT: return Key::LAlt; break;
	case SDL_SCANCODE_LGUI: return Key::LSystem; break;
	case SDL_SCANCODE_RCTRL: return Key::RCtrl; break;
	case SDL_SCANCODE_RSHIFT: return Key::RShift; break;
	case SDL_SCANCODE_RALT: return Key::RAlt; break;
	case SDL_SCANCODE_RGUI: return Key::RSystem; break;
	case SDL_SCANCODE_MENU: return Key::Menu; break;
	case SDL_SCANCODE_LEFTBRACKET: return Key::LBracket; break;
	case SDL_SCANCODE_RIGHTBRACKET: return Key::RBracket; break;
	case SDL_SCANCODE_SEMICOLON: return Key::Semicolon; break;
	case SDL_SCANCODE_COMMA: return Key::Comma; break;
	case SDL_SCANCODE_PERIOD: return Key::Period; break;
	case SDL_SCANCODE_SLASH: return Key::Slash; break;
	case SDL_SCANCODE_BACKSLASH: return Key::Backslash; break;
	case SDL_SCANCODE_GRAVE: return Key::Tilde; break;
	case SDL_SCANCODE_EQUALS: return Key::Equal; break;
	case SDL_SCANCODE_MINUS: return Key::Hyphen; break;
	case SDL_SCANCODE_PAGEUP: return Key::PageUp; break;
	case SDL_SCANCODE_PAGEDOWN: return Key::PageDown; break;
	case SDL_SCANCODE_END: return Key::End; break;
	case SDL_SCANCODE_HOME: return Key::Home; break;
	case SDL_SCANCODE_INSERT: return Key::Insert; break;
	case SDL_SCANCODE_DELETE: return Key::Delete; break;
	case SDL_SCANCODE_LEFT: return Key::Left; break;
	case SDL_SCANCODE_RIGHT: return Key::Right; break;
	case SDL_SCANCODE_UP: return Key::Up; break;
	case SDL_SCANCODE_DOWN: return Key::Down; break;
	case SDL_SCANCODE_PAUSE: return Key::Pause; break;
	case SDL_SCANCODE_F1: return Key::F1; break;
	case SDL_SCANCODE_F2: return Key::F2; break;
	case SDL_SCANCODE_F3: return Key::F3; break;
	case SDL_SCANCODE_F4: return Key::F4; break;
	case SDL_SCANCODE_F5: return Key::F5; break;
	case SDL_SCANCODE_F6: return Key::F6; break;
	case SDL_SCANCODE_F7: return Key::F7; break;
	case SDL_SCANCODE_F8: return Key::F8; break;
	case SDL_SCANCODE_F9: return Key::F9; break;
	case SDL_SCANCODE_F10: return Key::F10; break;
	case SDL_SCANCODE_F11: return Key::F11; break;
	case SDL_SCANCODE_F12: return Key::F12; break;
    default: break;
	}
    return Key::Unknown;
}

Keyboard& Keyboard::GetInstance()
{
	static Keyboard instance;
	return instance;
}

Keyboard::Keyboard()
	: mKeyStates()
	, mModifiers(0)
	, mInputCharacters()
	, mInputCharacterCount()
{
}

} // namespace en

#endif // ENLIVE_MODULE_WINDOW