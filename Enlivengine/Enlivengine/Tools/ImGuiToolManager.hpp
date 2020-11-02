#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_TOOLS
#ifdef ENLIVE_ENABLE_IMGUI

#include <vector>

#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Platform/Time.hpp>
#include <Enlivengine/Utils/Singleton.hpp>
#include <Enlivengine/Utils/Enums.hpp>
#include <Enlivengine/Window/Window.hpp>
#include <Enlivengine/Tools/ImGuiHelper.hpp>

namespace en
{

class ImGuiToolManager;

enum class ImGuiToolTab : U32
{
	Main = 0,
	Engine,
	Game
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
	void Initialize();
	void Shutdown();
	void Update();

private:
	void ImGuiMain();

	friend class ImGuiTool;
	void RegisterTool(ImGuiTool* tool);
	void UnregisterTool(ImGuiTool* tool);

private:
	bool mShowImGui;
	bool mRunning;
	std::vector<ImGuiTool*> mTools[Enum::GetCount<ImGuiToolTab>()];
};

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
#endif // ENLIVE_MODULE_TOOLS