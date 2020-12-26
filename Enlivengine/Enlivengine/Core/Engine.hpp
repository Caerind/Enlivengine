#pragma once

#include <Enlivengine/Window/Window.hpp>
#include <Enlivengine/Platform/Time.hpp>

namespace en
{

class Engine
{
public:
	static bool Init(int argc, char** argv);
	static bool IsInitialized();
	static bool Release();

	static Window& GetWindow();
	static bool Update(Time& dt);

private:
	static Engine& GetInstance();

	Engine();
	~Engine();

	bool mInitialized;
	Window mWindow;
	Clock mDTClock;
};

} // namespace en