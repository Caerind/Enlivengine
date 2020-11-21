#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <bx/allocator.h>

#include <bgfx/bgfx.h>

#include <imgui/imgui.h>

#include <IconFontCppHeaders/IconsFontAwesome5.h>
#include <IconFontCppHeaders/IconsKenney.h>

#include <Enlivengine/Platform/PrimitiveTypes.hpp>

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
	static bool Init();
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

	friend void ImGui::PushFont(ImGui::Font::Enum font);
	friend class ImGuiWrapperAllocatorAccess;
};

} // namespace en

#define IMGUI_FLAGS_NONE UINT8_C(0x00)
#define IMGUI_FLAGS_ALPHA_BLEND UINT8_C(0x01)

namespace ImGui
{

	// TODO : Add ImGui helpers here
	inline ImTextureID toId(bgfx::TextureHandle _handle, uint8_t _flags, uint8_t _mip)
	{
		union { struct { bgfx::TextureHandle handle; uint8_t flags; uint8_t mip; } s; ImTextureID id; } tex;
		tex.s.handle = _handle;
		tex.s.flags = _flags;
		tex.s.mip = _mip;
		return tex.id;
	}

	// Helper function for passing bgfx::TextureHandle to ImGui::Image.
	inline void Image(bgfx::TextureHandle _handle
		, uint8_t _flags
		, uint8_t _mip
		, const ImVec2& _size
		, const ImVec2& _uv0 = ImVec2(0.0f, 0.0f)
		, const ImVec2& _uv1 = ImVec2(1.0f, 1.0f)
		, const ImVec4& _tintCol = ImVec4(1.0f, 1.0f, 1.0f, 1.0f)
		, const ImVec4& _borderCol = ImVec4(0.0f, 0.0f, 0.0f, 0.0f)
	)
	{
		Image(toId(_handle, _flags, _mip), _size, _uv0, _uv1, _tintCol, _borderCol);
	}

	// Helper function for passing bgfx::TextureHandle to ImGui::Image.
	inline void Image(bgfx::TextureHandle _handle
		, const ImVec2& _size
		, const ImVec2& _uv0 = ImVec2(0.0f, 0.0f)
		, const ImVec2& _uv1 = ImVec2(1.0f, 1.0f)
		, const ImVec4& _tintCol = ImVec4(1.0f, 1.0f, 1.0f, 1.0f)
		, const ImVec4& _borderCol = ImVec4(0.0f, 0.0f, 0.0f, 0.0f)
	)
	{
		Image(_handle, IMGUI_FLAGS_ALPHA_BLEND, 0, _size, _uv0, _uv1, _tintCol, _borderCol);
	}

	// Helper function for passing bgfx::TextureHandle to ImGui::ImageButton.
	inline bool ImageButton(bgfx::TextureHandle _handle
		, uint8_t _flags
		, uint8_t _mip
		, const ImVec2& _size
		, const ImVec2& _uv0 = ImVec2(0.0f, 0.0f)
		, const ImVec2& _uv1 = ImVec2(1.0f, 1.0f)
		, int _framePadding = -1
		, const ImVec4& _bgCol = ImVec4(0.0f, 0.0f, 0.0f, 0.0f)
		, const ImVec4& _tintCol = ImVec4(1.0f, 1.0f, 1.0f, 1.0f)
	)
	{
		return ImageButton(toId(_handle, _flags, _mip), _size, _uv0, _uv1, _framePadding, _bgCol, _tintCol);
	}

	// Helper function for passing bgfx::TextureHandle to ImGui::ImageButton.
	inline bool ImageButton(bgfx::TextureHandle _handle
		, const ImVec2& _size
		, const ImVec2& _uv0 = ImVec2(0.0f, 0.0f)
		, const ImVec2& _uv1 = ImVec2(1.0f, 1.0f)
		, int _framePadding = -1
		, const ImVec4& _bgCol = ImVec4(0.0f, 0.0f, 0.0f, 0.0f)
		, const ImVec4& _tintCol = ImVec4(1.0f, 1.0f, 1.0f, 1.0f)
	)
	{
		return ImageButton(_handle, IMGUI_FLAGS_ALPHA_BLEND, 0, _size, _uv0, _uv1, _framePadding, _bgCol, _tintCol);
	}

} // namespace ImGui

#endif // ENLIVE_ENABLE_IMGUI