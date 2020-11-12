#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_GRAPHICS

#include <bgfx/bgfx.h>

#include <Enlivengine/Utils/Signal.hpp>
#include <Enlivengine/Window/Window.hpp>
#include <Enlivengine/Math/Vector2.hpp>

namespace en
{
class BgfxWrapper
{
public:
    static bool Init(Window& window);
    static bool IsInitialized();
    static bool Release();

#ifdef ENLIVE_ENABLE_GRAPHICS_DEBUG
    static void ToggleDisplayStats();
    static void SetDisplayStats(bool display);
    static bool IsDisplayingStats();
#endif // ENLIVE_ENABLE_GRAPHICS_DEBUG

    static void SetCurrentView(bgfx::ViewId view);
    static bgfx::ViewId GetCurrentView();

    static Vector2u GetFramebufferSize(bgfx::FrameBufferHandle framebuffer);

private: 
    static BgfxWrapper& GetInstance();

    BgfxWrapper();
    ~BgfxWrapper();

    enSlotType(Window, OnResized) mResizeRenderer;
    void Reset(U32 width, U32 height);

	bool mInitialized;
#ifdef ENLIVE_ENABLE_GRAPHICS_DEBUG
	bool mDisplayStats;
#endif // ENLIVE_ENABLE_GRAPHICS_DEBUG

    Vector2u mBackBufferSize;
    bgfx::ViewId mCurrentView;
};

} // namespace en

#endif // ENLIVE_MODULE_GRAPHICS