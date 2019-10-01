#pragma once

#include <Enlivengine/System/Time.hpp>

#include <Enlivengine/Application/Window.hpp>
#include <Enlivengine/Application/StateManager.hpp>
#include <Enlivengine/Application/AudioSystem.hpp>

namespace en
{

class Application
{
	public:
		Application();
		~Application();

		Window& getWindow();
		AudioSystem& getAudio();
		ImageManager& getImages();
		TextureManager& getTextures();
		FontManager& getFonts();

		template <typename State, typename ... Args>
		void start(Args&& ... args);

		void stop();

		template <typename T, typename ... Args>
		void pushState(Args&& ... args);
		void popState();
		void clearStates();

		U32 getFPS() const;

	private:
		void run();
		void events();
		void preUpdate();
		void update(Time dt);
		void postUpdate();
		void render();

	private:
		StateManager mStates;
		Window mWindow;
		AudioSystem mAudioSystem;
		ImageManager mImages;
		TextureManager mTextures;
		FontManager mFonts;

		U32 mFps;
		bool mRunning;
};

template <typename State, typename ... Args>
void Application::start(Args&& ... args)
{
	pushState<State>(std::forward<Args>(args)...);

	mStates.applyPendingChanges();

	mRunning = true;

	run();
}

template <typename T, typename ... Args>
void Application::pushState(Args&& ... args)
{
	mStates.pushState<T>(std::forward<Args>(args)...);
}

} // namespace en
