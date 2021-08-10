#pragma once

#include <Enlivengine/Window/Window.hpp>
#include <Enlivengine/Platform/Time.hpp>
#include <Enlivengine/Utils/ClassFactory.hpp>
#include <Enlivengine/Core/ComponentFactory.hpp>
#include <Enlivengine/Core/SystemFactory.hpp>

namespace en
{

class Engine
{
public:
	static int Main(int argc, char** argv);

	static bool Init(int argc, char** argv);
	static bool IsInitialized();
	static bool Release();

	static Window& GetWindow();
	static bool Update();

	template <typename T>
	static void RegisterComponent();

	template <typename T>
	static void RegisterSystem();

private:
	static Engine& GetInstance();

	Engine();
	~Engine();

	bool mInitialized;
	Window mWindow;
	Clock mDTClock;
};

template <typename T>
void Engine::RegisterComponent()
{
	ComponentFactory::Register<T>();
}

template <typename T>
void Engine::RegisterSystem()
{
	ClassFactory::Register<T>();
	SystemFactory::Register<T>();
}

} // namespace en