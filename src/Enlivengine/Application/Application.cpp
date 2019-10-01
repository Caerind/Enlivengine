#include <Enlivengine/Application/Application.hpp>

#include <Enlivengine/System/Config.hpp>
#include <Enlivengine/System/String.hpp>

#ifdef ENLIVE_ENABLE_IMGUI
#include <imgui/imgui.h>
#include <imgui-sfml/imgui-SFML.h>
#endif // ENLIVE_ENABLE_IMGUI

namespace en
{

Application::Application()
	: mStates(*this)
	, mWindow(sf::VideoMode(800, 600), "Enlivengine") // TODO : Name from config
	, mAudioSystem()
	, mFps(0)
	, mRunning(true)
{
	LogManager::initialize();

	#ifdef ENLIVE_ENABLE_IMGUI
	//ImGuiWrapper::init(mWindow);
	#endif
}

Application::~Application()
{
	#ifdef ENLIVE_ENABLE_IMGUI
	//ImGuiWrapper::shutdown();
	#endif

	if (mWindow.isOpen())
	{
		mWindow.close();
	}

	mAudioSystem.stop();

	#ifdef ENLIVE_PLATFORM_MOBILE
	std::exit(0);
	#endif
}

Window& Application::getWindow()
{
	return mWindow;
}

AudioSystem& Application::getAudio()
{
	return mAudioSystem;
}

ImageManager& Application::getImages()
{
	return mImages;
}

TextureManager& Application::getTextures()
{
	return mTextures;
}

FontManager& Application::getFonts()
{
	return mFonts;
}

void Application::stop()
{
	mRunning = false;
	mAudioSystem.stop();
}

void Application::popState()
{
	mStates.popState();
}

void Application::clearStates()
{
	mStates.clearStates();
}

U32 Application::getFPS() const
{
	return mFps;
}

void Application::run()
{
	const Time TimeUpdateFPS = seconds(0.5f);
	const Time TimePerFrame = seconds(1.0f / 60.0f);
	Time accumulator = Time::Zero;
	Time fpsAccumulator = Time::Zero;
	U32 framesFps = 0;
	Clock clock;

	// Running loop
	while (mRunning)
	{
		// Time
		Time dt = clock.restart();

		// Usefull when using Breakpoints on Debugging
#ifdef ENLIVE_DEBUG
		if (dt > Time::Second)
		{
			dt = TimePerFrame;
		}
#endif

		accumulator += dt;
		fpsAccumulator += dt;

		events();

		// Fixed time 60 FPS
		while (accumulator >= TimePerFrame)
		{
			accumulator -= TimePerFrame;

			preUpdate();
			update(dt);
			postUpdate();
		}

		render();

		// FPS
		framesFps++;
		if (fpsAccumulator > TimeUpdateFPS)
		{
			mFps = framesFps / TimeUpdateFPS.asSeconds();
			fpsAccumulator = Time::Zero;
			framesFps = 0;

#ifdef ENLIVE_DEBUG
			mWindow.setTitle("FPS : " + toString(mFps));
#endif
		}

		// Stop ?
		if (!mWindow.isOpen() || mStates.getStateCount() == 0)
		{
			stop();
		}
	}
}

void Application::events()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		// Might have used Signal of the Window, but clearer like this
		if (event.type == sf::Event::GainedFocus)
		{
			mAudioSystem.play();
		}
		if (event.type == sf::Event::LostFocus)
		{
			mAudioSystem.pause();
		}

		#ifdef ENLIVE_ENABLE_IMGUI
		//ImGuiWrapper::handleEvent(mWindow.getHandle(), event);
		#endif	

		mStates.handleEvent(event);
	}
}

void Application::preUpdate()
{
	mAudioSystem.update();
}

void Application::update(Time dt)
{
	#ifdef ENLIVE_ENABLE_IMGUI
	//ImGuiWrapper::newFrame();
	#endif

	mStates.update(dt);
}

void Application::postUpdate()
{
	#ifdef ENLIVE_ENABLE_IMGUI
	//ImGuiWrapper::endFrame();
	#endif
}

void Application::render()
{
	mWindow.clear();
	mStates.render(mWindow.getHandle());

	#ifdef ENLIVE_ENABLE_IMGUI
	//mWindow.draw(sf::RectangleShape()); // This fix a bug when rendering with the World // Empty window ? // ResetOpenGLState ?
	//ImGuiWrapper::render();
	#endif

	mWindow.display();
}

} // namespace en
