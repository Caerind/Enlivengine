#include <Enlivengine/SDL/SDLWrapper.hpp>

#ifdef ENLIVE_MODULE_WINDOW

#include <Enlivengine/Utils/Assert.hpp>

namespace en
{

bool SDLWrapper::Init()
{
    SDLWrapper& sdl = GetInstance();

    enAssert(!sdl.mInitialized);

    U32 initFlags = 0;

#ifdef ENLIVE_MODULE_WINDOW
    initFlags |= SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC; // | SDL_INIT_TIMER | SDL_INIT_SENSOR | SDL_INIT_GAMECONTROLLER
#endif // ENLIVE_MODULE_WINDOW

#ifdef ENLIVE_MODULE_AUDIO
    initFlags |= SDL_INIT_AUDIO;
#endif // ENLIVE_MODULE_AUDIO

    sdl.mInitialized = SDL_Init(initFlags) >= 0;
    if (sdl.mInitialized)
    {
        sdl.mInitFlags = initFlags;
        return true;
    }
    else
    {
        sdl.mInitFlags = 0;
        return false;
    }
}

bool SDLWrapper::IsInitialized()
{
    return GetInstance().mInitialized;
}

bool SDLWrapper::Release()
{
    SDLWrapper& sdl = GetInstance();

    enAssert(sdl.mInitialized);

    SDL_Quit();
    sdl.mInitialized = false;

    return true;
}

bool SDLWrapper::PollEvent(SDL_Event& event)
{
    return GetInstance().mInitialized && SDL_PollEvent(&event) > 0;
}

bool SDLWrapper::WaitEvent(SDL_Event& event, Time timeout /*= Time::Milliseconds(-1)*/)
{
    return GetInstance().mInitialized && SDL_WaitEventTimeout(&event, timeout.AsMilliseconds()) > 0;
}

const char* SDLWrapper::GetError()
{
    return SDL_GetError();
}

U32 SDLWrapper::GetInitFlags()
{
    return GetInstance().mInitFlags;
}

SDLWrapper& SDLWrapper::GetInstance()
{
    static SDLWrapper instance;
    return instance;
}

SDLWrapper::SDLWrapper()
    : mInitialized(false)
    , mInitFlags(0)
{
}

SDLWrapper::~SDLWrapper()
{
    enAssert(!mInitialized);
}

} // namespace en

#endif // ENLIVE_MODULE_WINDOW