#include <Enlivengine/Window/Window.hpp>

#ifdef ENLIVE_MODULE_WINDOW

#include <Enlivengine/Utils/Assert.hpp>

namespace en
{

U32 Window::sWindowCount = 0;
Window* Window::sWindows[kMaxWindows];

Window::Window()
    : mWindow(nullptr)
    , mShouldClose(false)
{
    RegisterWindow(this);
}

Window::~Window()
{
    Close();
    UnregisterWindow(this);
}

bool Window::Create(const char* name, I32 width, I32 height, U32 flags /*= SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE*/)
{
    mWindow = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);
    ResetShouldClose();
    return mWindow != nullptr;
}

void Window::Destroy()
{
	if (mWindow != nullptr)
	{
		SDL_DestroyWindow(mWindow);
		mWindow = nullptr;
	}
}

bool Window::IsValid() const
{
	return mWindow != nullptr;
}

void Window::Close()
{
    mShouldClose = true;
}

void Window::ResetShouldClose()
{
	mShouldClose = false;
}

bool Window::ShouldClose() const
{
    return mShouldClose;
}

void Window::SetVisible(bool visible)
{
    if (mWindow != nullptr)
    {
        if (!visible)
        {
            SDL_HideWindow(mWindow);
        }
        else
        {
            SDL_ShowWindow(mWindow);
        }
    }
}

bool Window::IsVisible() const
{
    return (mWindow != nullptr) ? (SDL_GetWindowFlags(mWindow) & SDL_WINDOW_SHOWN) > 0 : false;
}

void Window::Minimize()
{
    if (mWindow != nullptr)
    {
        SDL_MinimizeWindow(mWindow);
    }
}

void Window::Maximize()
{
    if (mWindow != nullptr)
    {
        SDL_MaximizeWindow(mWindow);
    }
}

bool Window::IsMinimized() const
{
    return (mWindow != nullptr) ? (SDL_GetWindowFlags(mWindow) & SDL_WINDOW_MINIMIZED) > 0 : false;
}

bool Window::IsMaximized() const
{
    return (mWindow != nullptr) ? (SDL_GetWindowFlags(mWindow) & SDL_WINDOW_MAXIMIZED) > 0 : false;
}

void Window::SetSize(I32 width, I32 height)
{
    if (mWindow != nullptr)
    {
        SDL_SetWindowSize(mWindow, width, height);
    }
}

I32 Window::GetWidth() const
{
    if (mWindow != nullptr)
    {
        I32 width, dummy;
        SDL_GetWindowSize(mWindow, &width, &dummy);
        return width;
    }
    else
    {
        return 0;
    }
}

I32 Window::GetHeight() const
{
    if (mWindow != nullptr)
    {
        I32 dummy, height;
        SDL_GetWindowSize(mWindow, &dummy, &height);
        return height;
    }
    else
    {
        return 0;
    }
}

void Window::GetSize(I32& width, I32& height)
{
    if (mWindow != nullptr)
    {
        SDL_GetWindowSize(mWindow, &width, &height);
    }
    else
    {
        width = 0;
        height = 0;
    }
}

void Window::SetTitle(const char* title)
{
    if (mWindow != nullptr)
    {
        SDL_SetWindowTitle(mWindow, title);
    }
}

const char* Window::GetTitle() const
{
    return (mWindow != nullptr) ? SDL_GetWindowTitle(mWindow) : "";
}

U32 Window::GetFlags() const
{
    return (mWindow != nullptr) ? SDL_GetWindowFlags(mWindow) : 0;
}

void Window::RegisterWindow(Window* window)
{
	if (sWindowCount < kMaxWindows)
	{
		sWindows[sWindowCount] = window;
		sWindowCount++;
	}
	else
	{
		// Maximum amount of windows reached
		// Increase Window::kMaxWindows or use less windows
		enAssert(false);
	}
}

void Window::UnregisterWindow(Window* window)
{
	for (U32 i = 0; i < sWindowCount; ++i)
	{
		if (sWindows[i] == window)
		{
			sWindowCount--;
			for (U32 j = i; j < sWindowCount; ++j)
			{
				sWindows[j] = sWindows[j + 1];
			}
			return;
		}
	}
	// Not found ?
    enAssert(false);
}

Window* Window::GetWindowFromSDL(SDL_Window* sdlWindow)
{
    if (sdlWindow == nullptr)
    {
        return nullptr;
    }
    else
    {
        for (U32 i = 0; i < sWindowCount; ++i)
        {
            if (sWindows[i] != nullptr && sWindows[i]->mWindow == sdlWindow)
            {
                return sWindows[i];
            }
        }
        // SDL_Window created outside the Window class from the engine
        enAssert(false);
        return nullptr;
    }
}

} // namespace en

#endif // ENLIVE_MODULE_WINDOW