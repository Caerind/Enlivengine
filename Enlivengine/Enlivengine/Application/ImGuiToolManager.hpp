#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <Enlivengine/Application/Window.hpp>
#include <Enlivengine/System/Singleton.hpp>

#include <Enlivengine/Tools/ImGuiHelper.hpp>

#include <vector>

namespace en
{

class Application;
class ImGuiToolManager;

enum class ImGuiToolTab : U32
{
	Main = 0,
	Engine,
	Game,
	Count
};
class ImGuiToolTabNames
{
	public:
		static constexpr const char* names[] =
		{
			"Main",
			"Engine",
			"Game"
		};

		//static_assert(static_cast<size_t>(ImGuiToolTab::Count) == (sizeof(char*)/sizeof(names)));
};

class ImGuiTool
{
	public:
		ImGuiTool();

		virtual ImGuiToolTab GetTab() const = 0;
		virtual const char* GetName() const = 0;

		virtual void Display();

		void Register();
		void Unregister();
		bool IsRegistered() const;

		virtual int GetWindowFlags() const;
		virtual bool IsImGuiDemoTool() const;

		void AskForResize();
		void AskForFocus();

	private:
		bool mRegistered;

		friend class ImGuiToolManager;
		bool ShouldResize() const;
		void Resize();
		bool ShouldFocus() const;
		void Focus();

	protected:
		bool mVisible;
		bool mShouldResize;
		bool mShouldFocus;
};

class ImGuiToolManager
{
	ENLIVE_SINGLETON(ImGuiToolManager);

    public:
		static constexpr bool kImGuiColorsClassic = false;

	private:
		friend class ImGuiTool;
		void RegisterTool(ImGuiTool* tool);
		void UnregisterTool(ImGuiTool* tool);

		friend class Application;
		void Initialize(Window& window);
		void Shutdown();
		void Update(Window& window, Time dt);
		void Render();
		void RenderFrame(Window& window);
		void HandleEvent(const sf::Event& event);
		
		void ImGuiMain();

	private:
		bool mShowImGui;
		bool mRunning;
		std::vector<ImGuiTool*> mTools[static_cast<U32>(ImGuiToolTab::Count)];
};

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
