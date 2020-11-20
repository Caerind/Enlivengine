#pragma once

#include <SDL.h>

#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Utils/Signal.hpp>
#include <Enlivengine/Math/Vector2.hpp>

namespace en
{

class Window
{
public:
    Window();
	~Window();
   
    // NonCopyable / NonMovable
    Window(Window&& other) = delete;
    Window(const Window& other) = delete;
    Window& operator=(Window&& other) = delete;
    Window& operator=(const Window& other) = delete;

    bool Create(const char* name, U32 width, U32 height, U32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	void Destroy();
	bool IsValid() const;

    void Close();
	void ResetShouldClose();
	bool ShouldClose() const;
    enSignal(OnShouldClose, const Window*);

    void SetVisible(bool visible);
    bool IsVisible() const;

    void Minimize();
    void Maximize();
    bool IsMinimized() const;
    bool IsMaximized() const;

    void SetSize(const Vector2u& size);
    Vector2u GetSize() const;
    enSignal(OnResized, const Window*, U32, U32);

    void SetTitle(const char* title);
    const char* GetTitle() const;

    bool IsGrabbing() const;
    void SetGrab(bool grab);
    void Grab();
    void Ungrab();

    U32 GetID() const;
    U32 GetFlags() const;

    static Window* GetFirstWindow();
    static Window* GetGrabbingWindow();

private:
    friend class BgfxWrapper;
    friend class Mouse;
    friend class EventSystem;

    SDL_Window* mWindow;
    bool mShouldClose;

    static constexpr U32 kMaxWindows = 10;
    static U32 sWindowCount;
    static Window* sWindows[kMaxWindows];
    static void RegisterWindow(Window* window);
    static void UnregisterWindow(Window* window);
    static Window* GetWindowFromSDLWindow(SDL_Window* sdlWindow);
    static Window* GetWindowFromSDLWindowID(U32 sldWindowID);
};

} // namespace en