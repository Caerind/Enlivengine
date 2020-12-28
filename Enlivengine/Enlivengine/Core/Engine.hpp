#pragma once

#include <Enlivengine/Window/Window.hpp>
#include <Enlivengine/Platform/Time.hpp>
#include <Enlivengine/Core/World.hpp>
#include <Enlivengine/Core/ComponentManager.hpp>
#include <Enlivengine/Core/SystemManager.hpp>

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
	static bool Update(Time& dt);

	static bool CreateWorld(const std::string& worldName);
	static bool RemoveWorld(const std::string& worldName);

	static bool LoadWorld(const std::string& worldName);
	static void UnloadCurrentWorld();

	static World* GetCurrentWorld();

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
	World* mCurrentWorld;
};

template <typename T>
void Engine::RegisterComponent()
{
	ComponentManager::Register<T>();
}

template <typename T>
void Engine::RegisterSystem()
{
	ClassFactory::Register<T>();
	SystemManager::Register<T>();
}

} // namespace en