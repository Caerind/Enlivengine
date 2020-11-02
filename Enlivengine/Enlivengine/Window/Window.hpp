#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_WINDOW

#include <SDL.h>

#include <Enlivengine/Platform/PrimitiveTypes.hpp>

namespace en
{

class Window
{
public:
    Window();
    ~Window();

    bool Create(const char* name, I32 width, I32 height, U32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	void Destroy();
	bool IsValid() const;

    void Close();
	void ResetShouldClose();
	bool ShouldClose() const;

    void SetVisible(bool visible);
    bool IsVisible() const;

    void Minimize();
    void Maximize();
    bool IsMinimized() const;
    bool IsMaximized() const;

    void SetSize(I32 width, I32 height);
    I32 GetWidth() const;
    I32 GetHeight() const;
    void GetSize(I32& width, I32& height);

    void SetTitle(const char* title);
    const char* GetTitle() const;

    U32 GetFlags() const;

private:
    friend class BgfxWrapper;
    friend class Mouse;

    SDL_Window* mWindow;
    bool mShouldClose;

    static constexpr U32 kMaxWindows = 10;
    static U32 sWindowCount;
    static Window* sWindows[kMaxWindows];
    static void RegisterWindow(Window* window);
    static void UnregisterWindow(Window* window);
    static Window* GetWindowFromSDL(SDL_Window* sdlWindow);
};

} // namespace en

#endif // ENLIVE_MODULE_WINDOW