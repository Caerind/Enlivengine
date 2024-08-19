#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Utils/Signal.hpp>
#include <Enlivengine/Math/Vector2.hpp>

namespace en
{

class Window
{
public:
    Window();
    ~Window();
   
    // NonCopyable / NonMovable
    Window(Window&& other) = delete;
    Window(const Window& other) = delete;
    Window& operator=(Window&& other) = delete;
    Window& operator=(const Window& other) = delete;

    bool Create(const char* name);

    void Destroy();
    bool IsValid() const;

    void Close();
    void ResetShouldClose();
    bool ShouldClose() const;
    enSignal(OnShouldClose, const Window*);

    void SetVisible(bool visible);
    bool IsVisible() const;

    /*
    void Minimize();
    void Maximize();
    bool IsMinimized() const;
    bool IsMaximized() const;
    */

    void SetSize(const Vector2u& size);
    Vector2u GetSize() const;
    enSignal(OnResized, const Window*, U32, U32);

    void SetTitle(const char* title);
    const char* GetTitle() const;

    /*
    bool IsGrabbing() const;
    void SetGrab(bool grab);
    void Grab();
    void Ungrab();

    U32 GetID() const;
    U32 GetFlags() const;

    static Window* GetGrabbingWindow();
    */
    
    sf::RenderWindow& GetSFMLWindow();
    const sf::RenderWindow& GetSFMLWindow() const;

private:
    friend class Mouse;
    friend class EventSystem;

    sf::RenderWindow mWindow; // Or sf::Window ?
    std::string mTitle;
    bool mShouldClose{false};
    bool mVisible{false};
};

} // namespace en