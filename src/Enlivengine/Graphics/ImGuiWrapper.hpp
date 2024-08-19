#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <string>

#include <imgui/imgui.h>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>

#include <Enlivengine/Window/Window.hpp>
#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Platform/Time.hpp>

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
    static bool Init(Window& window, const std::string& iniFilename = "");
    static bool IsInitialized();
    static void Release();

    static void BeginFrame(Window& window, Time dt);
    static void EndFrame(Window& window);

private:
    static ImGuiWrapper& GetInstance();

    ImGuiWrapper() = default;
    ~ImGuiWrapper();

    bool mInitialized{ false };
    std::string mIniFilename; 
    ImGuiContext* mContext{ nullptr };
    sf::Texture* mFontTexture{ nullptr };
    ImFont* mFonts[ImGui::Font::Count];

    friend void ImGui::PushFont(ImGui::Font::Enum font);
};

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
