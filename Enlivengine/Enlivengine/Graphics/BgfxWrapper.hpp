#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_GRAPHICS

#include <bgfx/bgfx.h>

#include <Enlivengine/Window/Window.hpp>

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

private: 
    static BgfxWrapper& GetInstance();

    BgfxWrapper();
    ~BgfxWrapper();

	bool mInitialized;
#ifdef ENLIVE_ENABLE_GRAPHICS_DEBUG
	bool mDisplayStats;
#endif // ENLIVE_ENABLE_GRAPHICS_DEBUG
};

} // namespace en

#endif // ENLIVE_MODULE_GRAPHICS