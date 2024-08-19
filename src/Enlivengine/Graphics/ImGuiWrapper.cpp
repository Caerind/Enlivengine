#include <Enlivengine/Graphics/ImGuiWrapper.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <imgui/imgui_internal.h>
#include <IconFontCppHeaders/IconsFontAwesome5.h>
#include <IconFontCppHeaders/IconsFontAwesome5.ttf.h>
#include <IconFontCppHeaders/IconsKenney.h>
#include <IconFontCppHeaders/IconsKenney.ttf.h>
#include <SFML/OpenGL.hpp>

#include <Enlivengine/Utils/Assert.hpp>
#include <Enlivengine/Window/Mouse.hpp>
#include <Enlivengine/Window/Keyboard.hpp>
#include <Enlivengine/Window/Clipboard.hpp>

#include <Enlivengine/Graphics/CompiledFonts/roboto_regular.ttf.h>
#include <Enlivengine/Graphics/CompiledFonts/robotomono_regular.ttf.h>

namespace en
{

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

ImTextureID ConvertGLTextureHandleToImTextureID(GLuint glTextureHandle) 
{
    ImTextureID textureID = (ImTextureID)NULL;
    std::memcpy(&textureID, &glTextureHandle, sizeof(GLuint));
    return textureID;
}

GLuint ConvertImTextureIDToGLTextureHandle(ImTextureID textureID) 
{
    GLuint glTextureHandle;
    std::memcpy(&glTextureHandle, &textureID, sizeof(GLuint));
    return glTextureHandle;
}

void SetClipboardText(void* /*userData*/, const char* text) 
{
    Clipboard::SetString(text);
}

const char* GetClipboardText(void* /*userData*/) 
{
    static std::string s_clipboardBuffer = Clipboard::GetString();
    return s_clipboardBuffer.c_str();
}

// copied from imgui/backends/imgui_impl_opengl2.cpp
void SetupRenderState(ImDrawData* draw_data, int fb_width, int fb_height) {
    // Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor
    // enabled, vertex/texcoord/color pointers, polygon fill.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA); //
    // In order to composite our output buffer we need to preserve alpha
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_SCISSOR_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glEnable(GL_TEXTURE_2D);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_SMOOTH);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // Setup viewport, orthographic projection matrix
    // Our visible imgui space lies from draw_data->DisplayPos (top left) to
    // draw_data->DisplayPos+data_data->DisplaySize (bottom right). DisplayPos is (0,0) for single
    // viewport apps.
    glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
#ifdef GL_VERSION_ES_CL_1_1
    glOrthof(draw_data->DisplayPos.x, draw_data->DisplayPos.x + draw_data->DisplaySize.x,
        draw_data->DisplayPos.y + draw_data->DisplaySize.y, draw_data->DisplayPos.y, -1.0f,
        +1.0f);
#else
    glOrtho(draw_data->DisplayPos.x, draw_data->DisplayPos.x + draw_data->DisplaySize.x,
        draw_data->DisplayPos.y + draw_data->DisplaySize.y, draw_data->DisplayPos.y, -1.0f,
        +1.0f);
#endif
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
}

bool ImGuiWrapper::Init(Window& window, const std::string& iniFilename)
{
    ImGuiWrapper& imgui = GetInstance();
    enAssert(!imgui.mInitialized);

    imgui.mIniFilename = (iniFilename.size() != 0) ? iniFilename : "imgui.ini";
    imgui.mContext = ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(static_cast<float>(window.GetSize().x), static_cast<float>(window.GetSize().y));
    io.DeltaTime = 1.0f / 60.0f;
    io.IniFilename = imgui.mIniFilename.c_str();

    // tell ImGui which features we support
    io.BackendFlags |= ImGuiBackendFlags_HasGamepad;
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
    io.BackendPlatformName = "imgui_impl_enlivengine";

    // clipboard
    io.SetClipboardTextFn = SetClipboardText;
    io.GetClipboardTextFn = GetClipboardText;

    // Style
    const float fontSize = 18.0f;
    ImGuiStyle& style = ImGui::GetStyle();
    ImGui::StyleColorsDark(&style);

    io.KeyMap[ImGuiKey_Tab]         = (int)Keyboard::Key::Tab;
    io.KeyMap[ImGuiKey_LeftArrow]   = (int)Keyboard::Key::Left;
    io.KeyMap[ImGuiKey_RightArrow]  = (int)Keyboard::Key::Right;
    io.KeyMap[ImGuiKey_UpArrow]     = (int)Keyboard::Key::Up;
    io.KeyMap[ImGuiKey_DownArrow]   = (int)Keyboard::Key::Down;
    io.KeyMap[ImGuiKey_PageUp]      = (int)Keyboard::Key::PageUp;
    io.KeyMap[ImGuiKey_PageDown]    = (int)Keyboard::Key::PageDown;
    io.KeyMap[ImGuiKey_Home]        = (int)Keyboard::Key::Home;
    io.KeyMap[ImGuiKey_End]         = (int)Keyboard::Key::End;
    io.KeyMap[ImGuiKey_Insert]      = (int)Keyboard::Key::Insert;
    io.KeyMap[ImGuiKey_Delete]      = (int)Keyboard::Key::Delete;
    io.KeyMap[ImGuiKey_Backspace]   = (int)Keyboard::Key::Backspace;
    io.KeyMap[ImGuiKey_Space]       = (int)Keyboard::Key::Space;
    io.KeyMap[ImGuiKey_Enter]       = (int)Keyboard::Key::Return;
    io.KeyMap[ImGuiKey_Escape]      = (int)Keyboard::Key::Escape;
    io.KeyMap[ImGuiKey_A]           = (int)Keyboard::Key::A;
    io.KeyMap[ImGuiKey_C]           = (int)Keyboard::Key::C;
    io.KeyMap[ImGuiKey_V]           = (int)Keyboard::Key::V;
    io.KeyMap[ImGuiKey_X]           = (int)Keyboard::Key::X;
    io.KeyMap[ImGuiKey_Y]           = (int)Keyboard::Key::Y;
    io.KeyMap[ImGuiKey_Z]           = (int)Keyboard::Key::Z;

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

        for (const FontRangeMerge& frm : s_fontRangeMerge)
        {
            io.Fonts->AddFontFromMemoryTTF((void*)frm.data, (int)frm.size, fontSize - 3.0f, &config, frm.ranges);
        }
    }
    io.Fonts->GetTexDataAsRGBA32(&data, &width, &height);
    imgui.mFontTexture = new sf::Texture();
    imgui.mFontTexture->create(width, height);
    imgui.mFontTexture->update(data);
    io.Fonts->ClearInputData();
    io.Fonts->ClearTexData();
    io.Fonts->SetTexID(ConvertGLTextureHandleToImTextureID(imgui.mFontTexture->getNativeHandle()));

    imgui.mInitialized = true;
    return true;
}

void RenderDrawLists(ImDrawData* draw_data)
{
    if (draw_data->CmdListsCount == 0) 
        return;

    ImGuiIO& io = ImGui::GetIO();
    assert(io.Fonts->TexID != (ImTextureID) nullptr); // You forgot to create and set font texture

    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates !=
    // framebuffer coordinates)
    int fb_width = (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
    int fb_height = (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
    if (fb_width == 0 || fb_height == 0) return;
    draw_data->ScaleClipRects(io.DisplayFramebufferScale);

    // Backup GL state
    // Backup GL state
    GLint last_texture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    // GLint last_polygon_mode[2];
     //glGetIntegerv(GL_POLYGON_MODE, last_polygon_mode);
    GLint last_viewport[4];
    glGetIntegerv(GL_VIEWPORT, last_viewport);
    GLint last_scissor_box[4];
    glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
    GLint last_shade_model;
    glGetIntegerv(GL_SHADE_MODEL, &last_shade_model);
    GLint last_tex_env_mode;
    glGetTexEnviv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, &last_tex_env_mode);

#ifdef GL_VERSION_ES_CL_1_1
    GLint last_array_buffer;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
    GLint last_element_array_buffer;
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer);
#else
    glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TRANSFORM_BIT);
#endif

    // Setup desired GL state
    SetupRenderState(draw_data, fb_width, fb_height);

    // Will project scissor/clipping rectangles into framebuffer space
    ImVec2 clip_off = draw_data->DisplayPos; // (0,0) unless using multi-viewports
    ImVec2 clip_scale = draw_data->FramebufferScale; // (1,1) unless using retina display which are
    // often (2,2)

// Render command lists
    for (int n = 0; n < draw_data->CmdListsCount; n++) {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        const ImDrawVert* vtx_buffer = cmd_list->VtxBuffer.Data;
        const ImDrawIdx* idx_buffer = cmd_list->IdxBuffer.Data;
        glVertexPointer(2, GL_FLOAT, sizeof(ImDrawVert),
            (const GLvoid*)((const char*)vtx_buffer + IM_OFFSETOF(ImDrawVert, pos)));
        glTexCoordPointer(2, GL_FLOAT, sizeof(ImDrawVert),
            (const GLvoid*)((const char*)vtx_buffer + IM_OFFSETOF(ImDrawVert, uv)));
        glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(ImDrawVert),
            (const GLvoid*)((const char*)vtx_buffer + IM_OFFSETOF(ImDrawVert, col)));

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++) {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback) {
                // User callback, registered via ImDrawList::AddCallback()
                // (ImDrawCallback_ResetRenderState is a special callback value used by the user to
                // request the renderer to reset render state.)
                if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
                    SetupRenderState(draw_data, fb_width, fb_height);
                else
                    pcmd->UserCallback(cmd_list, pcmd);
            }
            else {
                // Project scissor/clipping rectangles into framebuffer space
                ImVec4 clip_rect;
                clip_rect.x = (pcmd->ClipRect.x - clip_off.x) * clip_scale.x;
                clip_rect.y = (pcmd->ClipRect.y - clip_off.y) * clip_scale.y;
                clip_rect.z = (pcmd->ClipRect.z - clip_off.x) * clip_scale.x;
                clip_rect.w = (pcmd->ClipRect.w - clip_off.y) * clip_scale.y;

                if (clip_rect.x < fb_width && clip_rect.y < fb_height && clip_rect.z >= 0.0f &&
                    clip_rect.w >= 0.0f) {
                    // Apply scissor/clipping rectangle
                    glScissor((int)clip_rect.x, (int)(fb_height - clip_rect.w),
                        (int)(clip_rect.z - clip_rect.x), (int)(clip_rect.w - clip_rect.y));

                    // Bind texture, Draw
                    GLuint textureHandle = ConvertImTextureIDToGLTextureHandle(pcmd->TextureId);
                    glBindTexture(GL_TEXTURE_2D, textureHandle);
                    glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount,
                        sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT,
                        idx_buffer + pcmd->IdxOffset);
                }
            }
        }
    }

    // Restore modified GL state
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindTexture(GL_TEXTURE_2D, (GLuint)last_texture);
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    //glPopAttrib();
 //   glPolygonMode(GL_FRONT, (GLenum)last_polygon_mode[0]);
 //   glPolygonMode(GL_BACK, (GLenum)last_polygon_mode[1]);
    glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2],
        (GLsizei)last_viewport[3]);
    glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2],
        (GLsizei)last_scissor_box[3]);
    glShadeModel(last_shade_model);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, last_tex_env_mode);

#ifdef GL_VERSION_ES_CL_1_1
    glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer);
    glDisable(GL_SCISSOR_TEST);
#endif
}

bool ImGuiWrapper::IsInitialized()
{
    return GetInstance().mInitialized;
}

void ImGuiWrapper::Release()
{
    ImGuiWrapper& imgui = GetInstance();
    enAssert(imgui.mInitialized);

    ImGui::DestroyContext(imgui.mContext);

    delete imgui.mFontTexture;

    imgui.mInitialized = false;
}

void ImGuiWrapper::BeginFrame(Window& window, Time dt)
{
    ImGuiWrapper& imgui = GetInstance();
    enAssert(imgui.mInitialized);

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(static_cast<float>(window.GetSize().x), static_cast<float>(window.GetSize().y));
    io.DeltaTime = dt.AsSeconds();

    const Vector2i mousePos = Mouse::GetPosition();
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
}

void ImGuiWrapper::EndFrame(Window& window)
{
    ImGuiWrapper& imgui = GetInstance();
    enAssert(imgui.mInitialized);

    window.GetSFMLWindow().resetGLStates();
    window.GetSFMLWindow().pushGLStates();
    ImGui::Render();
    RenderDrawLists(ImGui::GetDrawData());
    window.GetSFMLWindow().popGLStates();
}

ImGuiWrapper& ImGuiWrapper::GetInstance()
{
    static ImGuiWrapper instance;
    return instance;
}

ImGuiWrapper::~ImGuiWrapper()
{
    enAssert(!mInitialized);
}

} // namespace en

namespace ImGui
{

void PushFont(Font::Enum font)
{
    PushFont(en::ImGuiWrapper::GetInstance().mFonts[font]);
}

} // namespace ImGui

#endif // ENLIVE_ENABLE_IMGUI
