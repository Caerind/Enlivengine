#include <Enlivengine/Window/Mouse.hpp>

#include <SFML/Window/Mouse.hpp>

#include <Enlivengine/Utils/Assert.hpp>
#include <Enlivengine/Window/EventSystem.hpp>
#include <Enlivengine/Window/Window.hpp>

namespace en
{

void Mouse::SetPositionAbsolute(const Vector2i& mousePos)
{
    sf::Mouse::setPosition(sf::Vector2i(mousePos.x, mousePos.y));
}

Vector2i Mouse::GetPositionAbsolute()
{
    const auto mousePos = sf::Mouse::getPosition();
    return Vector2i(static_cast<I32>(mousePos.x), static_cast<I32>(mousePos.y));
}

void Mouse::SetPositionRelative(const Vector2i& mousePos, const Window& window)
{
    sf::Mouse::setPosition(sf::Vector2i(mousePos.x, mousePos.y), window.GetSFMLWindow());
}

Vector2i Mouse::GetPositionRelative(const Window& window)
{
    const auto mousePos = sf::Mouse::getPosition(window.GetSFMLWindow());
    return Vector2i(static_cast<I32>(mousePos.x), static_cast<I32>(mousePos.y));
}

Vector2i Mouse::GetPosition()
{
    return GetInstance().mPosition;
}

Vector2i Mouse::GetPreviousPosition()
{
    return GetInstance().mPreviousPosition;
}

bool Mouse::HasMouseMoved()
{
    Mouse& mouse = GetInstance();
    return mouse.mMouseMovement.x != 0 || mouse.mMouseMovement.y != 0;
}

Vector2i Mouse::GetMouseMovement()
{
    return GetInstance().mMouseMovement;
}

bool Mouse::HasWheelMoved()
{
    return GetInstance().mWheel != 0;
}

I32 Mouse::GetWheel()
{
    return GetInstance().mWheel;
}

bool Mouse::HasHorizontalWheelMoved()
{
    return GetInstance().mHorizontalWheel != 0;
}

I32 Mouse::GetHorizontalWheel()
{
    return GetInstance().mHorizontalWheel;
}

bool Mouse::IsHold(Button button)
{
    Mouse& mouse = GetInstance();
    return (static_cast<U32>(button) & mouse.mButtonMask) > 0;
}

bool Mouse::IsPressed(Button button)
{
    Mouse& mouse = GetInstance();
    return (static_cast<U32>(button) & mouse.mButtonMask) > 0 && (static_cast<U32>(button) & mouse.mPreviousButtonMask) == 0;
}

bool Mouse::IsReleased(Button button)
{
    Mouse& mouse = GetInstance();
    return (static_cast<U32>(button) & mouse.mButtonMask) == 0 && (static_cast<U32>(button) & mouse.mPreviousButtonMask) > 0;
}

void Mouse::PreUpdate()
{
    Mouse& mouse = GetInstance();

    mouse.mPreviousButtonMask = mouse.mButtonMask;
    mouse.mPreviousPosition = mouse.mPosition;
    mouse.mWheel = 0;
    mouse.mHorizontalWheel = 0;
}

Mouse::Button sfButtonToEnButton(sf::Mouse::Button button)
{
    switch (button)
    {
    case sf::Mouse::Button::Left: return Mouse::Button::Left;
    case sf::Mouse::Button::Right: return Mouse::Button::Right;
    case sf::Mouse::Button::Middle: return Mouse::Button::Middle;
    case sf::Mouse::Button::XButton1: return Mouse::Button::X1;
    case sf::Mouse::Button::XButton2: return Mouse::Button::X2;
    default: enAssert(false);
    }
    return Mouse::Button::Left;
}

void Mouse::HandleEvent(const sf::Event& event)
{
    Mouse& mouse = GetInstance();
    switch (event.type)
    {
    case sf::Event::MouseWheelMoved:
        // Deprecated
        break;
    case sf::Event::MouseWheelScrolled:
        if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
            mouse.mWheel += static_cast<I32>(event.mouseWheelScroll.delta);
        else if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel)
            mouse.mHorizontalWheel += static_cast<I32>(event.mouseWheelScroll.delta);
        break;
    case sf::Event::MouseButtonPressed:
        mouse.mButtonMask |= static_cast<U32>(sfButtonToEnButton(event.mouseButton.button));
        mouse.mPosition.x = event.mouseButton.x;
        mouse.mPosition.y = event.mouseButton.y;
        break;
    case sf::Event::MouseButtonReleased:
        mouse.mButtonMask &= ~static_cast<U32>(sfButtonToEnButton(event.mouseButton.button));
        mouse.mPosition.x = event.mouseButton.x;
        mouse.mPosition.y = event.mouseButton.y;
        break;
    case sf::Event::MouseMoved:
        mouse.mPosition.x = event.mouseMove.x;
        mouse.mPosition.y = event.mouseMove.y;
        break;
    case sf::Event::MouseEntered:
    case sf::Event::MouseLeft:
        // Nothing
        break;
    }
}

void Mouse::PostUpdate()
{
    Mouse& mouse = GetInstance();
    mouse.mMouseMovement.x = mouse.mPosition.x - mouse.mPreviousPosition.x;
    mouse.mMouseMovement.y = mouse.mPosition.y - mouse.mPreviousPosition.y;
}

Mouse& Mouse::GetInstance()
{
    static Mouse instance;
    return instance;
}

Mouse::Mouse()
    : mPreviousPosition(0, 0)
    , mPosition(0, 0)
    , mMouseMovement(0, 0)
    , mPreviousButtonMask(0)
    , mButtonMask(0)
    , mWheel(0)
    , mHorizontalWheel(0)
{
}

} // namespace en