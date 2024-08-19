#pragma once

#include <SFML/Window/Event.hpp>

#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Math/Vector2.hpp>

namespace en
{

class Window;

class Touch
{
public:
    static U32 GetHoldCount();
    static U32 GetPressedCount();
    static U32 GetReleasedCount();
    static const std::vector<U32>& GetHoldTouches();
    static const std::vector<U32>& GetPressedTouches();
    static const std::vector<U32>& GetReleasedTouches();
    static bool IsHold(U32 finger);
    static bool IsPressed(U32 finger);
    static bool IsReleased(U32 finger);
    static const Vector2i& GetPosition(U32 finger);
    static const Vector2i& GetPreviousPosition(U32 finger);
    static Vector2i GetPositionAbsolute(U32 finger);
    static Vector2i GetPositionRelative(U32 finger, const Window& window);
    
private:
    friend class EventSystem;
    static void PreUpdate();
    static void HandleEvent(const sf::Event& event);
    static void PostUpdate();

private:
    static Touch& GetInstance();

    Touch() = default;

    // NonCopyable & NonMovable
    Touch(const Touch&) = delete;
    Touch& operator=(const Touch&) = delete;
    Touch(Touch&&) = delete;
    Touch& operator=(Touch&&) = delete;
    
    enum class TouchState
    {
         None = 0,
         Hold = 0x01,
         Pressed = 0x02,
         Released = 0x04
    };
    
    struct TouchData
    {
        U32 state = static_cast<U32>(TouchState::None);
        Vector2i position;
        Vector2i previousPosition;
    };
    
    static constexpr U32 kTouchCount = 10;
    TouchData mTouches[kTouchCount];
    std::vector<U32> mHoldTouches;
    std::vector<U32> mPressedTouches;
    std::vector<U32> mReleasedTouches;
};

} // namespace en
