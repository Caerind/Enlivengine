#include <Enlivengine/SDL/SDLWrapper.hpp>

#include <Enlivengine/Utils/Assert.hpp>

namespace en
{

bool SDLWrapper::Init()
{
    SDLWrapper& sdl = GetInstance();

    enAssert(!sdl.mInitialized);

	U32 initFlags = 0;
	initFlags |= SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC; // | SDL_INIT_TIMER | SDL_INIT_SENSOR | SDL_INIT_GAMECONTROLLER
    initFlags |= SDL_INIT_VIDEO;
    initFlags |= SDL_INIT_AUDIO; // Seems like we still need SDL AUDIO with SoLoud using the SDL backend (but not 100%, need to try)

    sdl.mInitialized = SDL_Init(initFlags) == 0;
    if (sdl.mInitialized)
    {
		sdl.mInitFlags = initFlags;
		enLogInfo(LogChannel::Global, "SDL initialized");
        return true;
    }
    else
    {
		sdl.mInitFlags = 0;
		enLogError(LogChannel::Global, "Can't initialize SDL : {}", GetError());
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