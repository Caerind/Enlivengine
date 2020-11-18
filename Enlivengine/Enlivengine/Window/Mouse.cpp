#include <Enlivengine/Window/Mouse.hpp>

#include <SDL.h>

#include <Enlivengine/Utils/Assert.hpp>

namespace en
{

void Mouse::Refresh()
{
	Mouse& mouse = GetInstance();

	int pX, pY;
	mouse.mPreviousButtonMask = mouse.mButtonMask;
	if (IsRelativeMode())
	{
		mouse.mButtonMask = SDL_GetRelativeMouseState(&pX, &pY);
		const I32 x = static_cast<I32>(pX);
		const I32 y = static_cast<I32>(pY);
		mouse.mMouseMovement.x = x;
		mouse.mMouseMovement.y = y;
	}
	else
	{
		mouse.mButtonMask = SDL_GetMouseState(&pX, &pY);
		const I32 x = static_cast<I32>(pX);
		const I32 y = static_cast<I32>(pY);
		mouse.mMouseMovement.x = x - mouse.mPosition.x;
		mouse.mMouseMovement.y = y - mouse.mPosition.y;
		mouse.mPosition.x = x;
		mouse.mPosition.y = y;
	}
	mouse.mWheel = 0;
	mouse.mHorizontalWheel = 0;
}

void Mouse::HandleEvent(const SDL_Event& event)
{
	Mouse& mouse = GetInstance();
	if (event.type == SDL_MOUSEMOTION)
	{
	}
	else if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		mouse.mButtonMask |= event.button.button;
	}
	else if (event.type == SDL_MOUSEBUTTONUP)
	{
		mouse.mButtonMask &= ~(event.button.button);
	}
	else if (event.type == SDL_MOUSEWHEEL)
	{
		if (event.wheel.x > 0) mouse.mHorizontalWheel += 1;
		if (event.wheel.x < 0) mouse.mHorizontalWheel -= 1;
		if (event.wheel.y > 0) mouse.mWheel += 1;
		if (event.wheel.y < 0) mouse.mWheel -= 1;
	}
}

void Mouse::SetPositionGlobal(const Vector2i& mousePos)
{
	SDL_WarpMouseGlobal(static_cast<int>(mousePos.x), static_cast<int>(mousePos.y));
}

Vector2i Mouse::GetPositionGlobal()
{
	int x, y;
	SDL_GetGlobalMouseState(&x, &y);
	return Vector2i(static_cast<I32>(x), static_cast<I32>(y));
}

void Mouse::SetPositionCurrentWindow(const Vector2i& mousePos)
{
	SDL_WarpMouseInWindow(nullptr, static_cast<int>(mousePos.x), static_cast<int>(mousePos.y));
	GetInstance().mPosition = mousePos;
}

Vector2i Mouse::GetPositionCurrentWindow()
{
	// TODO : Is this more precise ?
	/*
	int x, y;
	SDL_GetMouseState(&x, &y);
	return Vector2i(x, y);
	*/

	return GetInstance().mPosition;
}

bool Mouse::HasMouseMoved()
{
	Mouse& mouse = GetInstance();
	return mouse.mMouseMovement.x != 0 || mouse.mMouseMovement.y != 0;
}

Vector2i Mouse::GetMouseMovement()
{
	return GetInstance().mMouseMovement;
}

bool Mouse::HasWheelMoved()
{
	return GetInstance().mWheel != 0;
}

I32 Mouse::GetWheel()
{
	return GetInstance().mWheel;
}

bool Mouse::HasHorizontalWheelMoved()
{
	return GetInstance().mHorizontalWheel != 0;
}

I32 Mouse::GetHorizontalWheel()
{
	return GetInstance().mHorizontalWheel;
}

Window* Mouse::GetCurrentWindow()
{
	SDL_Window* window = SDL_GetMouseFocus();
	if (window != nullptr)
	{
		return Window::GetWindowFromSDLWindow(window);
	}
	else
	{
		return nullptr;
	}
}

bool Mouse::IsHold(Button button)
{
	Mouse& mouse = GetInstance();
	switch (button)
	{
	case Button::Left: return (SDL_BUTTON_LMASK & mouse.mButtonMask) > 0; break;
	case Button::Middle: return (SDL_BUTTON_MMASK & mouse.mButtonMask) > 0; break;
	case Button::Right: return (SDL_BUTTON_RMASK & mouse.mButtonMask) > 0; break;
	case Button::X1: return (SDL_BUTTON_X1MASK & mouse.mButtonMask) > 0; break;
	case Button::X2: return (SDL_BUTTON_X2MASK & mouse.mButtonMask) > 0; break;
	default: enAssert(false); break;
	}
	return false;
}

bool Mouse::IsPressed(Button button)
{
	Mouse& mouse = GetInstance();
	switch (button)
	{
	case Button::Left: return (SDL_BUTTON_LMASK & mouse.mButtonMask) > 0 && (SDL_BUTTON_LMASK & mouse.mPreviousButtonMask) == 0; break;
	case Button::Middle: return (SDL_BUTTON_MMASK & mouse.mButtonMask) > 0 && (SDL_BUTTON_MMASK & mouse.mPreviousButtonMask) == 0; break;
	case Button::Right: return (SDL_BUTTON_RMASK & mouse.mButtonMask) > 0 && (SDL_BUTTON_RMASK & mouse.mPreviousButtonMask) == 0; break;
	case Button::X1: return (SDL_BUTTON_X1MASK & mouse.mButtonMask) > 0 && (SDL_BUTTON_X1MASK & mouse.mPreviousButtonMask) == 0; break;
	case Button::X2: return (SDL_BUTTON_X2MASK & mouse.mButtonMask) > 0 && (SDL_BUTTON_X2MASK & mouse.mPreviousButtonMask) == 0; break;
	default: enAssert(false); break;
	}
	return false;
}

bool Mouse::IsReleased(Button button)
{
	Mouse& mouse = GetInstance();
	switch (button)
	{
	case Button::Left: return (SDL_BUTTON_LMASK & mouse.mButtonMask) == 0 && (SDL_BUTTON_LMASK & mouse.mPreviousButtonMask) > 0; break;
	case Button::Middle: return (SDL_BUTTON_MMASK & mouse.mButtonMask) == 0 && (SDL_BUTTON_MMASK & mouse.mPreviousButtonMask) > 0; break;
	case Button::Right: return (SDL_BUTTON_RMASK & mouse.mButtonMask) == 0 && (SDL_BUTTON_RMASK & mouse.mPreviousButtonMask) > 0; break;
	case Button::X1: return (SDL_BUTTON_X1MASK & mouse.mButtonMask) == 0 && (SDL_BUTTON_X1MASK & mouse.mPreviousButtonMask) > 0; break;
	case Button::X2: return (SDL_BUTTON_X2MASK & mouse.mButtonMask) == 0 && (SDL_BUTTON_X2MASK & mouse.mPreviousButtonMask) > 0; break;
	default: enAssert(false); break;
	}
	return false;
}

bool Mouse::IsRelativeMode()
{
	return SDL_GetRelativeMouseMode() == SDL_TRUE;
}

void Mouse::SetRelativeMode(bool relativeMode)
{
	if (IsRelativeMode() != relativeMode)
	{
		if (relativeMode)
		{
			SDL_SetRelativeMouseMode(SDL_TRUE);
			int pX, pY;
			SDL_GetRelativeMouseState(&pX, &pY);
		}
		else
		{
			SDL_SetRelativeMouseMode(SDL_FALSE);
		}
	}
}

void Mouse::EnableRelativeMode()
{
	SetRelativeMode(true);
}

void Mouse::DisableRelativeMode()
{
	SetRelativeMode(false);
}

bool Mouse::IsCursorVisible()
{
	return SDL_ShowCursor(SDL_QUERY) == SDL_ENABLE;
}

void Mouse::SetCursorVisible(bool visible)
{
	SDL_ShowCursor(visible ? SDL_ENABLE : SDL_DISABLE);
}

void Mouse::ShowCursor()
{
	SDL_ShowCursor(SDL_ENABLE);
}

void Mouse::HideCursor()
{
	SDL_ShowCursor(SDL_DISABLE);
}

Mouse& Mouse::GetInstance()
{
	static Mouse instance;
	return instance;
}

Mouse::Mouse()
	: mPosition(0, 0)
	, mMouseMovement(0, 0)
	, mPreviousButtonMask(0)
	, mButtonMask(0)
	, mWheel(0)
	, mHorizontalWheel(0)
{
}

} // namespace en