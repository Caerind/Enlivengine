#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <bx/allocator.h>

#include <bgfx/bgfx.h>

#include <imgui/imgui.h>

#include <IconFontCppHeaders/IconsFontAwesome5.h>
#include <IconFontCppHeaders/IconsKenney.h>

#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <string>

#define IMGUI_FLAGS_NONE UINT8_C(0x00)
#define IMGUI_FLAGS_ALPHA_BLEND UINT8_C(0x01)

namespace ImGui
{
	struct Font
	{
		enum Enum
		{
			Regular,
			Mono,

			Count
		};
	};

	void PushFont(Font::Enum font);

} // namespace ImGui

namespace en
{

class ImGuiWrapper
{
public:
	static bool Init(const std::string& iniFilename = "");
	static bool IsInitialized();
	static bool Release();

	static void BeginFrame(bgfx::ViewId viewId, U32 windowWidth, U32 windowHeight, F32 dtSeconds);
	static void EndFrame();

private:
	static void* MemAlloc(std::size_t size, void* userData);
	static void MemFree(void* ptr, void* userData);

private:
	static ImGuiWrapper& GetInstance();

	ImGuiWrapper();
	~ImGuiWrapper();

	bool mInitialized;
	ImGuiContext* mContext;
	bx::AllocatorI* mAllocator;
	bgfx::VertexLayout mVertexLayout;
	bgfx::ProgramHandle mProgram;
	bgfx::ProgramHandle mImageProgram;
	bgfx::TextureHandle mTexture;
	bgfx::UniformHandle mSamplerTexture;
	bgfx::UniformHandle mImageLodEnabled;
	ImFont* mFonts[ImGui::Font::Count];
	I64 mLast;
	bgfx::ViewId mViewId;
	std::string mIniFilename;

	friend void ImGui::PushFont(ImGui::Font::Enum font);
	friend class ImGuiWrapperAllocatorAccess;

	static constexpr F32 k_ImDisplayWidth{ 1280.0f };
	static constexpr F32 k_ImDisplayHeight{ 720.0f };
	static constexpr F32 k_ImDisplayDT{ 1.0f / 60.0f };
};

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI