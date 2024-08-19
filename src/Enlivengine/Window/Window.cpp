#include <Enlivengine/Window/Window.hpp>

#include <Enlivengine/Utils/Assert.hpp>

namespace en
{

Window::Window()
    : mWindow()
{
    enAssert(!IsValid());
}

Window::~Window()
{
    Close();
}

bool Window::Create(const char* name)
{
    if (IsValid())
    {
        SetTitle(name);
        return true;
    }
    
    sf::VideoMode videoMode = sf::VideoMode::getDesktopMode();
    mWindow.create(videoMode, name);
    mTitle = name;
    mVisible = true;
    return true;
}

void Window::Destroy()
{
    if (IsValid())
        mWindow.close();
}

bool Window::IsValid() const
{
    return mWindow.isOpen();
}

void Window::Close()
{
    mShouldClose = true;
    OnShouldClose(this);
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
    /*if (mWindow != nullptr)
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
    */
    mWindow.setVisible(visible);
    mVisible = visible;
}

bool Window::IsVisible() const
{
    /*return (mWindow != nullptr) ? (SDL_GetWindowFlags(mWindow) & SDL_WINDOW_SHOWN) > 0 : false;*/
    return mVisible;
}

/*
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
*/

void Window::SetSize(const Vector2u& size)
{
    /*if (mWindow != nullptr)
    {
        SDL_SetWindowSize(mWindow, static_cast<I32>(size.x), static_cast<I32>(size.y));
    }*/
    mWindow.setSize(sf::Vector2u(size.x, size.y));
}

Vector2u Window::GetSize() const
{
    /*if (mWindow != nullptr)
    {
        I32 w, h;
        SDL_GetWindowSize(mWindow, &w, &h);
        return Vector2u(static_cast<U32>(w), static_cast<U32>(h));
    }
    else
    {
        return Vector2u(0, 0);
    }
    */
    const auto size = mWindow.getSize();
    return Vector2u(size.x, size.y);
}

void Window::SetTitle(const char* title)
{
    /*if (mWindow != nullptr)
    {
        SDL_SetWindowTitle(mWindow, title);
    }*/
    mTitle = title;
    mWindow.setTitle(mTitle.c_str());
}

const char* Window::GetTitle() const
{
    /*return (mWindow != nullptr) ? SDL_GetWindowTitle(mWindow) : "";*/
    return mTitle.c_str();
}

/*
bool Window::IsGrabbing() const
{
    return (mWindow != nullptr) ? SDL_GetWindowGrab(mWindow) == SDL_TRUE : false;
}

void Window::SetGrab(bool grab)
{
    if (mWindow != nullptr)
    {
        SDL_SetWindowGrab(mWindow, grab ? SDL_TRUE : SDL_FALSE);
    }
}

void Window::Grab()
{
    if (mWindow)
    {
        SDL_SetWindowGrab(mWindow, SDL_TRUE);
    }
}

void Window::Ungrab()
{
    if (mWindow)
    {
        SDL_SetWindowGrab(mWindow, SDL_FALSE);
    }
}

U32 Window::GetID() const
{
    return (mWindow != nullptr) ? SDL_GetWindowID(mWindow) : 0;
}

U32 Window::GetFlags() const
{
    return (mWindow != nullptr) ? SDL_GetWindowFlags(mWindow) : 0;
}

Window* Window::GetGrabbingWindow()
{
    return GetWindowFromSDLWindow(SDL_GetGrabbedWindow());
}*/

sf::RenderWindow& Window::GetSFMLWindow()
{
    return mWindow;
}

const sf::RenderWindow& Window::GetSFMLWindow() const
{
    return mWindow;
}

} // namespace en