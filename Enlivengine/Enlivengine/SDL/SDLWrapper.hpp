#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_WINDOW

#include <SDL.h>

#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Platform/Time.hpp>

namespace en
{

class SDLWrapper
{
public:
	static bool Init();
	static bool IsInitialized();
	static bool Release();

	static bool PollEvent(SDL_Event& event);
	static bool WaitEvent(SDL_Event& event, Time timeout = Time::Milliseconds(-1));

	static const char* GetError();

	static U32 GetInitFlags();

private:
	static SDLWrapper& GetInstance();

	SDLWrapper();
	~SDLWrapper();

	bool mInitialized;
	U32 mInitFlags;
};

} // namespace en

#endif // ENLIVE_MODULE_WINDOW