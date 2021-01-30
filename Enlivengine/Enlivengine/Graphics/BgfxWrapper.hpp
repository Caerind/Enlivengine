#pragma once

#include <bgfx/bgfx.h>

#include <Enlivengine/Math/Color.hpp>

namespace en
{

class Window;
class Framebuffer;

class BgfxWrapper
{
public:
    static bool Init(Window& window);
    static bool IsInitialized();
    static bool Release();

    static void Frame();

    static void ClearFramebuffer(Framebuffer& framebuffer, const Color& color = Colors::Black, const bgfx::ViewId& viewId = 0);

#ifdef ENLIVE_ENABLE_GRAPHICS_DEBUG
    static void ToggleDisplayStats();
    static void SetDisplayStats(bool display);
    static bool IsDisplayingStats();
#endif // ENLIVE_ENABLE_GRAPHICS_DEBUG

    static void SetCurrentView(bgfx::ViewId view);
    static bgfx::ViewId GetCurrentView();

private: 
    static BgfxWrapper& GetInstance();

    BgfxWrapper();
    ~BgfxWrapper();

	bool mInitialized;
#ifdef ENLIVE_ENABLE_GRAPHICS_DEBUG
	bool mDisplayStats;
#endif // ENLIVE_ENABLE_GRAPHICS_DEBUG

    bgfx::ViewId mCurrentView;
};

} // namespace en