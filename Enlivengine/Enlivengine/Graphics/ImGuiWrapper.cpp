#include <Enlivengine/Graphics/ImGuiWrapper.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <bx/math.h>
#include <bx/timer.h>

#include <bgfx/embedded_shader.h>

#include <ImGuizmo/ImGuizmo.h>

#include <IconFontCppHeaders/IconsFontAwesome5.ttf.h>
#include <IconFontCppHeaders/IconsKenney.ttf.h>

#include <Enlivengine/Utils/Assert.hpp>
#include <Enlivengine/Window/Mouse.hpp>
#include <Enlivengine/Window/Keyboard.hpp>

#include <Enlivengine/Graphics/CompiledFonts/roboto_regular.ttf.h>
#include <Enlivengine/Graphics/CompiledFonts/robotomono_regular.ttf.h>

#include <Enlivengine/Graphics/CompiledShaders/vs_ocornut_imgui.bin.h>
#include <Enlivengine/Graphics/CompiledShaders/fs_ocornut_imgui.bin.h>
#include <Enlivengine/Graphics/CompiledShaders/vs_imgui_image.bin.h>
#include <Enlivengine/Graphics/CompiledShaders/fs_imgui_image.bin.h>

namespace en
{

static const bgfx::EmbeddedShader s_embeddedShaders[] =
{
	BGFX_EMBEDDED_SHADER(vs_ocornut_imgui),
	BGFX_EMBEDDED_SHADER(fs_ocornut_imgui),
	BGFX_EMBEDDED_SHADER(vs_imgui_image),
	BGFX_EMBEDDED_SHADER(fs_imgui_image),

	BGFX_EMBEDDED_SHADER_END()
};

struct FontRangeMerge
{
	const void* data;
	size_t size;
	ImWchar ranges[3];
};

static FontRangeMerge s_fontRangeMerge[] =
{
	{ s_kenney_icon_font,   sizeof(s_kenney_icon_font), { ICON_MIN_KI, ICON_MAX_KI, 0 } },
	{ s_fa_solid_900,		sizeof(s_fa_solid_900),	{ ICON_MIN_FA, ICON_MAX_FA, 0 } }
};

bool ImGuiWrapper::Init(const std::string& iniFilename /*= ""*/)
{
    ImGuiWrapper& imgui = GetInstance();

    enAssert(!imgui.mInitialized);

	// Create ctx
	static bx::DefaultAllocator defaultAllocator;
	imgui.mAllocator = &defaultAllocator;

	imgui.mLast = bx::getHPCounter();
	imgui.mViewId = 255;
	imgui.mIniFilename = (iniFilename.size() != 0) ? iniFilename : "imgui.ini";

	ImGui::SetAllocatorFunctions(MemAlloc, MemFree, nullptr);
	imgui.mContext = ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();

	io.DisplaySize = ImVec2(1280.0f, 720.0f);
	io.DeltaTime = 1.0f / 60.0f; 
	io.IniFilename = imgui.mIniFilename.c_str();

	// Style
	const float fontSize = 18.0f;
	ImGuiStyle& style = ImGui::GetStyle();
	ImGui::StyleColorsDark(&style);

	io.KeyMap[ImGuiKey_Tab]        = (int)Keyboard::Key::Tab;
	io.KeyMap[ImGuiKey_LeftArrow]  = (int)Keyboard::Key::Left;
	io.KeyMap[ImGuiKey_RightArrow] = (int)Keyboard::Key::Right;
	io.KeyMap[ImGuiKey_UpArrow]    = (int)Keyboard::Key::Up;
	io.KeyMap[ImGuiKey_DownArrow]  = (int)Keyboard::Key::Down;
	io.KeyMap[ImGuiKey_PageUp]     = (int)Keyboard::Key::PageUp;
	io.KeyMap[ImGuiKey_PageDown]   = (int)Keyboard::Key::PageDown;
	io.KeyMap[ImGuiKey_Home]       = (int)Keyboard::Key::Home;
	io.KeyMap[ImGuiKey_End]        = (int)Keyboard::Key::End;
	io.KeyMap[ImGuiKey_Insert]     = (int)Keyboard::Key::Insert;
	io.KeyMap[ImGuiKey_Delete]     = (int)Keyboard::Key::Delete;
	io.KeyMap[ImGuiKey_Backspace]  = (int)Keyboard::Key::Backspace;
	io.KeyMap[ImGuiKey_Space]      = (int)Keyboard::Key::Space;
	io.KeyMap[ImGuiKey_Enter]      = (int)Keyboard::Key::Return;
	io.KeyMap[ImGuiKey_Escape]     = (int)Keyboard::Key::Escape;
	io.KeyMap[ImGuiKey_A]          = (int)Keyboard::Key::A;
	io.KeyMap[ImGuiKey_C]          = (int)Keyboard::Key::C;
	io.KeyMap[ImGuiKey_V]          = (int)Keyboard::Key::V;
	io.KeyMap[ImGuiKey_X]          = (int)Keyboard::Key::X;
	io.KeyMap[ImGuiKey_Y]          = (int)Keyboard::Key::Y;
	io.KeyMap[ImGuiKey_Z]          = (int)Keyboard::Key::Z;

	/*
	io.ConfigFlags |= 0
		| ImGuiConfigFlags_NavEnableGamepad
		| ImGuiConfigFlags_NavEnableKeyboard
		;

	io.NavInputs[ImGuiNavInput_Activate]    = (int)entry::Key::GamepadA;
	io.NavInputs[ImGuiNavInput_Cancel]      = (int)entry::Key::GamepadB;
	//io.NavInputs[ImGuiNavInput_Input]       = (int)entry::Key::;
	//io.NavInputs[ImGuiNavInput_Menu]        = (int)entry::Key::;
	io.NavInputs[ImGuiNavInput_DpadLeft]    = (int)entry::Key::GamepadLeft;
	io.NavInputs[ImGuiNavInput_DpadRight]   = (int)entry::Key::GamepadRight;
	io.NavInputs[ImGuiNavInput_DpadUp]      = (int)entry::Key::GamepadUp;
	io.NavInputs[ImGuiNavInput_DpadDown]    = (int)entry::Key::GamepadDown;
	//io.NavInputs[ImGuiNavInput_LStickLeft]  = (int)entry::Key::;
	//io.NavInputs[ImGuiNavInput_LStickRight] = (int)entry::Key::;
	//io.NavInputs[ImGuiNavInput_LStickUp]    = (int)entry::Key::;
	//io.NavInputs[ImGuiNavInput_LStickDown]  = (int)entry::Key::;
	//io.NavInputs[ImGuiNavInput_FocusPrev]   = (int)entry::Key::;
	//io.NavInputs[ImGuiNavInput_FocusNext]   = (int)entry::Key::;
	//io.NavInputs[ImGuiNavInput_TweakSlow]   = (int)entry::Key::;
	//io.NavInputs[ImGuiNavInput_TweakFast]   = (int)entry::Key::;
	*/

	bgfx::RendererType::Enum type = bgfx::getRendererType();
	imgui.mProgram = bgfx::createProgram(
		bgfx::createEmbeddedShader(s_embeddedShaders, type, "vs_ocornut_imgui")
		, bgfx::createEmbeddedShader(s_embeddedShaders, type, "fs_ocornut_imgui")
		, true
	);
	imgui.mImageProgram = bgfx::createProgram(
		bgfx::createEmbeddedShader(s_embeddedShaders, type, "vs_imgui_image")
		, bgfx::createEmbeddedShader(s_embeddedShaders, type, "fs_imgui_image")
		, true
	);

	imgui.mVertexLayout
		.begin()
		.add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
		.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
		.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
		.end();

	imgui.mImageLodEnabled = bgfx::createUniform("u_imageLodEnabled", bgfx::UniformType::Vec4);
	imgui.mSamplerTexture = bgfx::createUniform("s_tex", bgfx::UniformType::Sampler);

	uint8_t* data;
	int32_t width;
	int32_t height;
	{
		ImFontConfig config;
		config.FontDataOwnedByAtlas = false;
		config.MergeMode = false;

		const ImWchar* ranges = io.Fonts->GetGlyphRangesCyrillic();
		imgui.mFonts[ImGui::Font::Regular] = io.Fonts->AddFontFromMemoryTTF((void*)s_robotoRegularTtf, sizeof(s_robotoRegularTtf), fontSize, &config, ranges);
		imgui.mFonts[ImGui::Font::Mono] = io.Fonts->AddFontFromMemoryTTF((void*)s_robotoMonoRegularTtf, sizeof(s_robotoMonoRegularTtf), fontSize - 3.0f, &config, ranges);

		config.MergeMode = true;
		config.DstFont = imgui.mFonts[ImGui::Font::Regular];

		for (uint32_t i = 0; i < BX_COUNTOF(s_fontRangeMerge); ++i)
		{
			const FontRangeMerge& frm = s_fontRangeMerge[i];
			io.Fonts->AddFontFromMemoryTTF((void*)frm.data, (int)frm.size, fontSize - 3.0f, &config, frm.ranges);
		}
	}
	io.Fonts->GetTexDataAsRGBA32(&data, &width, &height);
	imgui.mTexture = bgfx::createTexture2D((uint16_t)width, (uint16_t)height, false, 1, bgfx::TextureFormat::BGRA8, 0, bgfx::copy(data, width * height * 4));

	// Docking
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigDockingWithShift = true;

	imgui.mInitialized = true;
    return true;
}

bool ImGuiWrapper::IsInitialized()
{
    return GetInstance().mInitialized;
}

bool ImGuiWrapper::Release()
{
    ImGuiWrapper& imgui = GetInstance();

	enAssert(imgui.mInitialized);

	ImGui::DestroyContext(imgui.mContext);

	bgfx::destroy(imgui.mTexture);
	bgfx::destroy(imgui.mSamplerTexture);
	bgfx::destroy(imgui.mImageLodEnabled);
	bgfx::destroy(imgui.mProgram);
	bgfx::destroy(imgui.mImageProgram);

	imgui.mAllocator = nullptr;

	imgui.mInitialized = false;
    return true;
}

void ImGuiWrapper::BeginFrame(bgfx::ViewId viewId, U32 windowWidth, U32 windowHeight, F32 dtSeconds)
{
	ImGuiWrapper& imgui = GetInstance();
	enAssert(imgui.mInitialized);
    
	imgui.mViewId = viewId;

	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(static_cast<float>(windowWidth), static_cast<float>(windowHeight));

	io.DeltaTime = dtSeconds;
	
	const Vector2i mousePos = Mouse::GetPositionCurrentWindow();
	io.MousePos = ImVec2(static_cast<F32>(mousePos.x), static_cast<F32>(mousePos.y));
	io.MouseDown[0] = Mouse::IsHold(Mouse::Button::Left);
	io.MouseDown[1] = Mouse::IsHold(Mouse::Button::Right);
	io.MouseDown[2] = Mouse::IsHold(Mouse::Button::Middle);
	io.MouseDown[3] = Mouse::IsHold(Mouse::Button::X1);
	io.MouseDown[4] = Mouse::IsHold(Mouse::Button::X2);
	io.MouseWheel = static_cast<float>(Mouse::GetWheel());

	static constexpr U32 keyCount = static_cast<U32>(Keyboard::Key::Count);
	for (U32 i = 0; i < keyCount; ++i)
	{
		io.KeysDown[i] = Keyboard::IsHold(static_cast<Keyboard::Key>(i));
	}
	io.KeyShift = Keyboard::IsShiftHold();
	io.KeyCtrl = Keyboard::IsControlHold();
	io.KeyAlt = Keyboard::IsAltHold();
	const U32 inputCharacterCount = Keyboard::GetInputCharacterCount();
	const char* inputCharacters = Keyboard::GetInputCharacters();
	for (U32 i = 0; i < inputCharacterCount; ++i)
	{
		io.AddInputCharacter(inputCharacters[i]);
	}

	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
}

void ImGuiWrapper::EndFrame()
{
	ImGuiWrapper& imgui = GetInstance();
	enAssert(imgui.mInitialized);

	ImGui::Render();

	const ImGuiIO& io = ImGui::GetIO();
	const float width = io.DisplaySize.x;
	const float height = io.DisplaySize.y;

	bgfx::setViewName(imgui.mViewId, "ImGui");
	bgfx::setViewMode(imgui.mViewId, bgfx::ViewMode::Sequential);

	const bgfx::Caps* caps = bgfx::getCaps();
	{
		float ortho[16];
		bx::mtxOrtho(ortho, 0.0f, width, height, 0.0f, 0.0f, 1000.0f, 0.0f, caps->homogeneousDepth);
		bgfx::setViewTransform(imgui.mViewId, nullptr, ortho);
		bgfx::setViewRect(imgui.mViewId, 0, 0, uint16_t(width), uint16_t(height));
	}

	// Render command lists
	ImDrawData* drawData = ImGui::GetDrawData();
	for (int32_t ii = 0, num = drawData->CmdListsCount; ii < num; ++ii)
	{
		bgfx::TransientVertexBuffer tvb;
		bgfx::TransientIndexBuffer tib;

		const ImDrawList* drawList = drawData->CmdLists[ii];
		uint32_t numVertices = (uint32_t)drawList->VtxBuffer.size();
		uint32_t numIndices  = (uint32_t)drawList->IdxBuffer.size();

		// TODO : CheckAvailableTransientBuffers
		/*
		if (!checkAvailTransientBuffers(numVertices, m_layout, numIndices))
		{
			// Not enough space in transient buffer just quit drawing the rest...
			break;
		}
		*/

		bgfx::allocTransientVertexBuffer(&tvb, numVertices, imgui.mVertexLayout);
		bgfx::allocTransientIndexBuffer(&tib, numIndices);

		ImDrawVert* verts = (ImDrawVert*)tvb.data;
		bx::memCopy(verts, drawList->VtxBuffer.begin(), numVertices * sizeof(ImDrawVert));

		ImDrawIdx* indices = (ImDrawIdx*)tib.data;
		bx::memCopy(indices, drawList->IdxBuffer.begin(), numIndices * sizeof(ImDrawIdx));

		uint32_t offset = 0;
		for (const ImDrawCmd* cmd = drawList->CmdBuffer.begin(), *cmdEnd = drawList->CmdBuffer.end(); cmd != cmdEnd; ++cmd)
		{
			if (cmd->UserCallback)
			{
				cmd->UserCallback(drawList, cmd);
			}
			else if (0 != cmd->ElemCount)
			{
				uint64_t state = 0
					| BGFX_STATE_WRITE_RGB
					| BGFX_STATE_WRITE_A
					| BGFX_STATE_MSAA
					;

				bgfx::TextureHandle th = imgui.mTexture;
				bgfx::ProgramHandle program = imgui.mProgram;

				if (NULL != cmd->TextureId)
				{
					union { ImTextureID ptr; struct { bgfx::TextureHandle handle; uint8_t flags; uint8_t mip; } s; } texture = { cmd->TextureId };
					state |= 0 != (IMGUI_FLAGS_ALPHA_BLEND & texture.s.flags)
						? BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA)
						: BGFX_STATE_NONE
						;
					th = texture.s.handle;
					if (0 != texture.s.mip)
					{
						const float lodEnabled[4] = { float(texture.s.mip), 1.0f, 0.0f, 0.0f };
						bgfx::setUniform(imgui.mImageLodEnabled, lodEnabled);
						program = imgui.mImageProgram;
					}
				}
				else
				{
					state |= BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);
				}

				const uint16_t xx = uint16_t(bx::max(cmd->ClipRect.x, 0.0f) );
				const uint16_t yy = uint16_t(bx::max(cmd->ClipRect.y, 0.0f) );
				bgfx::setScissor(xx, yy
					, uint16_t(bx::min(cmd->ClipRect.z, 65535.0f)-xx)
					, uint16_t(bx::min(cmd->ClipRect.w, 65535.0f)-yy)
					);

				bgfx::setState(state);
				bgfx::setTexture(0, imgui.mSamplerTexture , th);
				bgfx::setVertexBuffer(0, &tvb, 0, numVertices);
				bgfx::setIndexBuffer(&tib, offset, cmd->ElemCount);
				bgfx::submit(imgui.mViewId, program);
			}

			offset += cmd->ElemCount;
		}
	}
}

ImGuiWrapper& ImGuiWrapper::GetInstance()
{
    static ImGuiWrapper instance;
    return instance;
}

ImGuiWrapper::ImGuiWrapper()
	: mInitialized(false)
	, mContext(nullptr)
	, mAllocator(nullptr)
	, mVertexLayout()
	, mProgram(BGFX_INVALID_HANDLE)
	, mImageProgram(BGFX_INVALID_HANDLE)
	, mTexture(BGFX_INVALID_HANDLE)
	, mSamplerTexture(BGFX_INVALID_HANDLE)
	, mImageLodEnabled(BGFX_INVALID_HANDLE)
	, mFonts()
	, mLast(0)
	, mViewId(255)
{
}

ImGuiWrapper::~ImGuiWrapper()
{
	enAssert(!mInitialized);
}

void* ImGuiWrapper::MemAlloc(std::size_t size, void* userData)
{
	BX_UNUSED(userData);
	return BX_ALLOC(GetInstance().mAllocator, size);
}

void ImGuiWrapper::MemFree(void* ptr, void* userData)
{
	BX_UNUSED(userData);
	BX_FREE(GetInstance().mAllocator, ptr);
}

class ImGuiWrapperAllocatorAccess
{
public:
	static void* MemAlloc(std::size_t size, void* userData)
	{
		return ImGuiWrapper::GetInstance().MemAlloc(size, userData);
	}

	static void MemFree(void* ptr, void* userData)
	{
		ImGuiWrapper::GetInstance().MemFree(ptr, userData);
	}
};

} // namespace en

namespace ImGui
{

void PushFont(Font::Enum font)
{
	PushFont(en::ImGuiWrapper::GetInstance().mFonts[font]);
}

} // namespace ImGui

BX_PRAGMA_DIAGNOSTIC_IGNORED_MSVC(4505) // error C4505: '' : unreferenced local function has been removed
BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC("-Wunused-function") // warning: 'int rect_width_compare(const void*, const void*)' defined but not used
BX_PRAGMA_DIAGNOSTIC_PUSH()
BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG("-Wunknown-pragmas")
BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC("-Wtype-limits") // warning: comparison is always true due to limited range of data type
#define STBTT_malloc(size, userData) en::ImGuiWrapperAllocatorAccess::MemAlloc(size, userData)
#define STBTT_free(ptr, userData) en::ImGuiWrapperAllocatorAccess::MemFree(ptr, userData)
#define STB_RECT_PACK_IMPLEMENTATION
#include <imgui/imstb_rectpack.h>
#define STB_TRUETYPE_IMPLEMENTATION
#include <imgui/imstb_truetype.h>
BX_PRAGMA_DIAGNOSTIC_POP()

#endif // ENLIVE_ENABLE_IMGUI