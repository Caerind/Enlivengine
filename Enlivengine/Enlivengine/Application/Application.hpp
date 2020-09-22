#pragma once

#include <Enlivengine/System/Time.hpp>
#include <Enlivengine/Config.hpp>
#include <Enlivengine/System/Profiler.hpp>

#include <Enlivengine/Graphics/ScreenshotSystem.hpp>
#include <Enlivengine/Application/PathManager.hpp>
#include <Enlivengine/Application/AudioSystem.hpp>
#include <Enlivengine/Application/Window.hpp>
#include <Enlivengine/Application/StateManager.hpp>
#include <Enlivengine/Application/ResourceManager.hpp>
#include <Enlivengine/Application/ActionSystem.hpp>

namespace en
{

class Application
{
	ENLIVE_SINGLETON(Application);

public:
	bool Initialize();

	Window& GetWindow();
	ScreenshotSystem& GetScreenshotSystem();
	ActionSystem& GetActionSystem();

	template <typename State, typename ... Args>
	void Start(Args&& ... args);
	void Stop();
	bool IsRunning() const;

	template <typename T, typename ... Args>
	void PushState(Args&& ... args);
	void PopState();
	void ClearStates();

	U32 GetFPS() const;
	U32 GetTotalFrames() const;
	Time GetTotalDuration() const;

	bool LoadResources();
#ifdef ENLIVE_DEBUG
	bool LoadResource(ResourceID resourceID, U32 type, const std::string& filename, const std::string& identifier);
#else
	bool LoadResource(ResourceID resourceID, U32 type, const std::string& filename);
#endif // ENLIVE_DEBUG

	enSignal(onApplicationStopped, const Application*);

private:
	~Application();

	void Run();
	void Events();
	void PreUpdate();
	void Update(Time dt);
	void PostUpdate();
	void Render();

	void RegisterTools();

private:
	StateManager mStates;
	Window mWindow;
	ScreenshotSystem mScreenshotSystem;
	ActionSystem mActionSystem;

	enSlot(en::Window, onWindowClosed, mWindowClosedSlot);

	U32 mFps;
	bool mRunning;
	bool mInitialized;

	U32 mTotalFrames;
	Clock mTotalDuration;
};

template <typename State, typename ... Args>
void Application::Start(Args&& ... args)
{
	PushState<State>(std::forward<Args>(args)...);
	mStates.applyPendingChanges();

	mRunning = true;

	Run();

	ClearStates();
	mStates.applyPendingChanges();
}

template <typename T, typename ... Args>
void Application::PushState(Args&& ... args)
{
	mStates.pushState<T>(std::forward<Args>(args)...);
}

} // namespace en
