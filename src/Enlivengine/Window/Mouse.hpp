#pragma once

#include <SFML/Window/Event.hpp>

#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Utils/TypeInfo.hpp>
#include <Enlivengine/Math/Vector2.hpp>

namespace en
{

class Window;

class Mouse
{
public:
    static void SetPositionAbsolute(const Vector2i& mousePos);
    static Vector2i GetPositionAbsolute();
    static void SetPositionRelative(const Vector2i& mousePos, const Window& window);
    static Vector2i GetPositionRelative(const Window& window);

    static Vector2i GetPosition();
    static Vector2i GetPreviousPosition(); 
    static bool HasMouseMoved();
    static Vector2i GetMouseMovement();

    static bool HasWheelMoved();
    static I32 GetWheel();
    static bool HasHorizontalWheelMoved();
    static I32 GetHorizontalWheel();

    enum class Button
    {
        Left = 1,
        Middle = 2,
        Right = 4,
        X1 = 8,
        X2 = 16
    };
    static bool IsHold(Button button);
    static bool IsPressed(Button button);
    static bool IsReleased(Button button);

private:
    friend class EventSystem;
    static void PreUpdate();
    static void HandleEvent(const sf::Event& event);
    static void PostUpdate();

private:
    static Mouse& GetInstance();

    Mouse();

    // NonCopyable & NonMovable
    Mouse(const Mouse&) = delete;
    Mouse& operator=(const Mouse&) = delete;
    Mouse(Mouse&&) = delete;
    Mouse& operator=(Mouse&&) = delete;

    Vector2i mPreviousPosition;
    Vector2i mPosition;
    Vector2i mMouseMovement;
    U32 mPreviousButtonMask;
    U32 mButtonMask;
    I32 mWheel;
    I32 mHorizontalWheel;
};

} // namespace en

ENLIVE_DEFINE_ENUM_INFO(en::Mouse::Button)