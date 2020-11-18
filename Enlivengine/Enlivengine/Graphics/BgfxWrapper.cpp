#include <Enlivengine/Graphics/BgfxWrapper.hpp>

#include <SDL.h>
#include <SDL_syswm.h>

#include <bgfx/platform.h>

#include <Enlivengine/Platform/PlatformDetection.hpp>
#include <Enlivengine/Utils/Assert.hpp>

namespace en
{

bool BgfxWrapper::Init(Window& window)
{
    BgfxWrapper& bgfx = GetInstance();

    enAssert(!bgfx.mInitialized);

    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    if (!SDL_GetWindowWMInfo(window.mWindow, &wmi))
    {
        return false;
    }

    bgfx::PlatformData pd;

    switch (wmi.subsystem)
    {
#ifdef ENLIVE_PLATFORM_WINDOWS
    case SDL_SYSWM_WINDOWS:
	{
		pd.ndt = nullptr;
		pd.nwh = wmi.info.win.window;
        break;
    }
#endif // ENLIVE_PLATFORM_WINDOWS

#ifdef ENLIVE_PLATFORM_WINRT
    case SDL_SYSWM_WINRT:
    {
		pd.ndt = nullptr;
		pd.nwh = wmi.info.winrt.window;
        break;
    }
#endif // ENLIVE_PLATFORM_WINRT

#ifdef ENLIVE_PLATFORM_X11
    case SDL_SYSWM_X11:
    {
        pd.ndt = wmi.info.x11.display;
        pd.nwh = (void*)(uintptr_t)wmi.info.x11.window;
        break;
    }
#endif // ENLIVE_PLATFORM_X11

#ifdef ENLIVE_PLATFORM_DIRECTFB
	case SDL_SYSWM_DIRECTFB:
	{
		pd.ndt = nullptr;
		pd.nwh = wmi.info.dfb.window;
		break;
	}
#endif // ENLIVE_PLATFORM_DIRECTFB

#ifdef ENLIVE_PLATFORM_COCOA // OSX
	case SDL_SYSWM_COCOA:
	{
		pd.ndt = nullptr;
		pd.nwh = wmi.info.cocoa.window;
		break;
	}
#endif // ENLIVE_PLATFORM_COCOA

#ifdef ENLIVE_PLATFORM_UIKIT // iOS
	case SDL_SYSWM_UIKIT:
	{
		pd.ndt = nullptr;
		pd.nwh = wmi.info.uikit.window;
		break;
	}
#endif // ENLIVE_PLATFORM_UIKIT

#ifdef ENLIVE_PLATFORM_WAYLAND
	case SDL_SYSWM_WAYLAND:
	{
		pd.ndt = nullptr;
		pd.nwh = (void*)(uintptr_t)wmi.info.wl.display;
		break;
	}
#endif // ENLIVE_PLATFORM_WAYLAND

#ifdef ENLIVE_PLATFORM_MIR
	case SDL_SYSWM_MIR:
	{
		pd.ndt = nullptr;
		pd.nwh = nullptr;
		break;
	}
#endif // ENLIVE_PLATFORM_MIR

#ifdef ENLIVE_PLATFORM_ANDROID
	case SDL_SYSWM_ANDROID:
	{
		pd.ndt = nullptr;
		pd.nwh = wmi.info.android.window;
		break;
	}
#endif // ENLIVE_PLATFORM_ANDROID

#ifdef ENLIVE_PLATFORM_VIVANTE // Steamlink
    case SDL_SYSWM_VIVANTE:
	{
        pd.ndt = wmi.info.vivante.display;
		pd.nwh = (void*)(uintptr_t)wmi.info.vivante.window;
		break;
	}
#endif // ENLIVE_PLATFORM_VIVANTE

    case SDL_SYSWM_UNKNOWN:
    default:
    {
        enAssert(false);
        return false;
    }
    }
	
    pd.context = nullptr;
    pd.backBuffer = nullptr;
    pd.backBufferDS = nullptr;
    bgfx::setPlatformData(pd);

	// Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
	// Most graphics APIs must be used on the same thread that created the window.
    bgfx::renderFrame();

    bgfx::Init init;
    init.type = bgfx::RendererType::Count; // Automatically choose a renderer
    init.resolution.width = window.GetWidth();
    init.resolution.height = window.GetHeight();
    init.resolution.reset = BGFX_RESET_VSYNC;
    if (!bgfx::init(init))
    {
        return false;
    }

    bgfx.mBackBufferSize.x = window.GetWidth();
    bgfx.mBackBufferSize.y = window.GetHeight();
    bgfx.mCurrentView = BGFX_INVALID_HANDLE;

#ifdef ENLIVE_DEBUG
    bgfx::setDebug(BGFX_DEBUG_TEXT);
#endif // ENLIVE_DEBUG

    bgfx.mResizeRenderer.Connect(window.OnResized, [](const Window*, U32 width, U32 height) { GetInstance().Reset(static_cast<U32>(width), static_cast<U32>(height)); });

    bgfx.mInitialized = true;
    return true;
}

bool BgfxWrapper::IsInitialized()
{
    return GetInstance().mInitialized;
}

bool BgfxWrapper::Release()
{
    BgfxWrapper& bgfx = GetInstance();

	enAssert(bgfx.mInitialized);

	bgfx::shutdown();
    bgfx.mInitialized = false;

    return true;
}

#ifdef ENLIVE_ENABLE_GRAPHICS_DEBUG
void BgfxWrapper::ToggleDisplayStats()
{
    SetDisplayStats(!GetInstance().mDisplayStats);
}

void BgfxWrapper::SetDisplayStats(bool display)
{
	GetInstance().mDisplayStats = display;
    bgfx::setDebug(BGFX_DEBUG_TEXT | (display ? BGFX_DEBUG_STATS : 0));
}

bool BgfxWrapper::IsDisplayingStats()
{
    return GetInstance().mDisplayStats;
}

void BgfxWrapper::SetCurrentView(bgfx::ViewId view)
{
    GetInstance().mCurrentView = view;
}

bgfx::ViewId BgfxWrapper::GetCurrentView()
{
    return GetInstance().mCurrentView;
}

Vector2u BgfxWrapper::GetFramebufferSize(bgfx::FrameBufferHandle framebuffer)
{
    // TODO : Framebuffers
    ENLIVE_UNUSED(framebuffer);
    return GetInstance().mBackBufferSize;
}

#endif // ENLIVE_ENABLE_GRAPHICS_DEBUG

BgfxWrapper& BgfxWrapper::GetInstance()
{
    static BgfxWrapper instance;
    return instance;
}

BgfxWrapper::BgfxWrapper()
    : mResizeRenderer()
    , mInitialized(false)
    , mDisplayStats(false)
{
}

BgfxWrapper::~BgfxWrapper()
{
    enAssert(!mInitialized);
}

void BgfxWrapper::Reset(U32 width, U32 height)
{
    BgfxWrapper& bgfx = GetInstance();
    bgfx.mBackBufferSize.x = width;
	bgfx.mBackBufferSize.y = height;
	bgfx::reset(width, height);
}

} // namespace en