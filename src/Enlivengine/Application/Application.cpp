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
	, mWindow(sf::VideoMode(1024, 768), "")
	, mAudioSystem()
	, mFps(0)
	, mRunning(true)
	, mTotalFrames(0)
	, mTotalDuration()
{
	LogManager::initialize();

	#ifdef ENLIVE_ENABLE_IMGUI
	ImGui::SFML::Init(mWindow.getHandle());
	ImGui::StyleColorsClassic();
	mShowImGui = true;
	#endif
}

Application::~Application()
{
	mAudioSystem.stop();

	if (mWindow.isOpen())
	{
		mWindow.close();
	}

#ifdef ENLIVE_ENABLE_IMGUI
	ImGui::SFML::Shutdown();
#endif

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


U32 Application::getTotalFrames() const
{
	return mTotalFrames;
}

Time Application::getTotalDuration() const
{
	return mTotalDuration.getElapsedTime();
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
#ifdef ENLIVE_ENABLE_PROFILE
		Profiler::beginFrame();
		bool importantFrame = false;
#endif

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

#ifdef ENLIVE_ENABLE_PROFILE
			importantFrame = true;
#endif

#ifdef ENLIVE_ENABLE_IMGUI
			ImGui::SFML::Update(mWindow.getHandle(), toSF(dt));
			if (mShowImGui)
			{
				ImGuiMain();
			}
#endif

			preUpdate();
			update(dt);
			postUpdate();

#ifdef ENLIVE_ENABLE_IMGUI
			ImGui::Render();
#endif
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

		mTotalFrames++;

		// Stop ?
		if (!mWindow.isOpen() || mStates.getStateCount() == 0)
		{
			stop();
		}

#ifdef ENLIVE_ENABLE_PROFILE
		Profiler::endFrame(importantFrame);
#endif
	}
}

void Application::events()
{
	ENLIVE_PROFILE_FUNCTION();

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
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Menu)
		{
			mShowImGui = !mShowImGui;
		}

		if (mShowImGui)
		{
			ImGui::SFML::ProcessEvent(event);
		}
#endif	

		mStates.handleEvent(event);
	}
}

void Application::preUpdate()
{
	ENLIVE_PROFILE_FUNCTION();
	mAudioSystem.update();
}

void Application::update(Time dt)
{
	ENLIVE_PROFILE_FUNCTION();
	mStates.update(dt);
}

void Application::postUpdate()
{
	ENLIVE_PROFILE_FUNCTION();
}

void Application::render()
{
	ENLIVE_PROFILE_FUNCTION();

	mWindow.clear();

	mStates.render(mWindow.getHandle());

#ifdef ENLIVE_ENABLE_IMGUI
	if (mShowImGui)
	{
		mWindow.getHandle().resetGLStates();
		ImGui::SFML::Render(mWindow.getHandle());
	}
#endif	

	mWindow.display();
}

void Application::ImGuiMain()
{
#ifdef ENLIVE_ENABLE_IMGUI
	static bool bDemo = false;
	static bool bStyleEditor = false;
	static bool bLogger = false;
	static bool bProfiler = false;

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			bool todo = true;
			ImGui::MenuItem("TODO", NULL, &todo);
			/*
			if (ImGui::MenuItem("Save")) FileIO::SaveToFile(mData.GetFilename(), mData);
			ImGui::MenuItem("Save...", NULL, &bFileSaveAs);
			if (ImGui::MenuItem("Close")) mData.Clear();
			ImGui::Spacing();
			bool continuousSave = mData.IsContinuousSave();
			ImGui::MenuItem("Continuous Save", NULL, &continuousSave);
			*/
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Tools"))
		{
			bool todo = true;
			ImGui::MenuItem("TODO", NULL, &todo);

			/*
			bool wasFalse = (bEditor == false);
			ImGui::MenuItem("Editor", NULL, &bEditor);
			if (wasFalse && bEditor) bStars = false;

			wasFalse = (bStars == false);
			ImGui::MenuItem("Stars", NULL, &bStars);
			if (wasFalse && bStars) bEditor = false;
			*/

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			ImGui::MenuItem("Demo", NULL, &bDemo);
			ImGui::MenuItem("Style", NULL, &bStyleEditor);
			ImGui::MenuItem("Logger", NULL, &bLogger);
#ifdef ENLIVE_ENABLE_PROFILE
			ImGui::MenuItem("Profiler", NULL, &bProfiler);
#else
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			ImGui::MenuItem("Profiler");
			ImGui::PopStyleVar();
#endif
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (bDemo)
	{
		ImGui::ShowDemoWindow(&bDemo);
	}
	if (bStyleEditor)
	{
		if (!ImGui::Begin("StyleEditor", &bStyleEditor))
		{
			ImGui::End();
			return;
		}
		ImGui::ShowStyleEditor();
		ImGui::End();
	}
	if (bLogger)
	{
		mLogger.draw();
	}
#ifdef ENLIVE_ENABLE_PROFILE
	if (bProfiler)
	{
		mProfiler.draw();
	}
#endif
#endif // ENLIVE_ENABLE_IMGUI
}

} // namespace en
