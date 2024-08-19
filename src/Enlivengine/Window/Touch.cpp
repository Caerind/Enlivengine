#include <Enlivengine/Window/Touch.hpp>

#include <SFML/Window/Touch.hpp>
#include <Enlivengine/Window/Window.hpp>
#include <Enlivengine/Window/EventSystem.hpp>

namespace en
{
    
U32 Touch::GetHoldCount()
{
    return static_cast<U32>(GetInstance().mHoldTouches.size());
}

U32 Touch::GetPressedCount()
{
    return static_cast<U32>(GetInstance().mPressedTouches.size());
}

U32 Touch::GetReleasedCount()
{
    return static_cast<U32>(GetInstance().mReleasedTouches.size());
}

const std::vector<U32>& Touch::GetHoldTouches()
{
    return GetInstance().mHoldTouches;
}

const std::vector<U32>& Touch::GetPressedTouches()
{
    return GetInstance().mPressedTouches;
}

const std::vector<U32>& Touch::GetReleasedTouches()
{
    return GetInstance().mReleasedTouches;
}
    
bool Touch::IsHold(U32 finger)
{
    enAssert(finger < kTouchCount);
    return (Touch::GetInstance().mTouches[finger].state & static_cast<U32>(TouchState::Hold)) > 0;
}

bool Touch::IsPressed(U32 finger)
{
    enAssert(finger < kTouchCount);
    return (Touch::GetInstance().mTouches[finger].state & static_cast<U32>(TouchState::Pressed)) > 0;
}

bool Touch::IsReleased(U32 finger)
{
    enAssert(finger < kTouchCount);
    return (Touch::GetInstance().mTouches[finger].state & static_cast<U32>(TouchState::Released)) > 0;
}

const Vector2i& Touch::GetPosition(U32 finger)
{
    enAssert(finger < kTouchCount);
    return GetInstance().mTouches[finger].position;
}

const Vector2i& Touch::GetPreviousPosition(U32 finger)
{
    enAssert(finger < kTouchCount);
    return GetInstance().mTouches[finger].previousPosition;
}

Vector2i Touch::GetPositionAbsolute(U32 finger)
{
    const auto p = sf::Touch::getPosition(finger);
    return Vector2i(p.x, p.y);
}

Vector2i Touch::GetPositionRelative(U32 finger, const Window& window)
{
    const auto p = sf::Touch::getPosition(finger, window.GetSFMLWindow());
    return Vector2i(p.x, p.y);
}

void Touch::PreUpdate()
{
    Touch& touch = GetInstance();
    
    constexpr U32 clearEvents = static_cast<U32>(TouchState::Pressed) | static_cast<U32>(TouchState::Released);
    for (U32 i = 0; i < kTouchCount; ++i)
    {
        touch.mTouches[i].state &= ~clearEvents;
        if (touch.mTouches[i].state > 0)
        {
            touch.mTouches[i].previousPosition = touch.mTouches[i].position;
        }
    }
}

void Touch::HandleEvent(const sf::Event& event)
{
    Touch& touch = GetInstance();
    if (event.type == sf::Event::TouchBegan)
    {
        touch.mTouches[event.touch.finger].state = static_cast<U32>(TouchState::Hold) | static_cast<U32>(TouchState::Pressed);
        touch.mTouches[event.touch.finger].position = {event.touch.x, event.touch.y};
        touch.mTouches[event.touch.finger].previousPosition = {event.touch.x, event.touch.y};
        EventSystem::AddEventDebug("TouchBegan:" + std::to_string(event.touch.finger) + ":" + std::to_string(event.touch.x) + ":" + std::to_string(event.touch.y));
    }
    else if (event.type == sf::Event::TouchEnded)
    {
        touch.mTouches[event.touch.finger].state = static_cast<U32>(TouchState::Released);
        touch.mTouches[event.touch.finger].position = {event.touch.x, event.touch.y};
        EventSystem::AddEventDebug("TouchEnded:" + std::to_string(event.touch.finger) + ":" + std::to_string(event.touch.x) + ":" + std::to_string(event.touch.y));
    }
    else if (event.type == sf::Event::TouchMoved)
    {
        touch.mTouches[event.touch.finger].position = {event.touch.x, event.touch.y};
        EventSystem::AddEventDebug("TouchMoved:" + std::to_string(event.touch.finger) + ":" + std::to_string(event.touch.x) + ":" + std::to_string(event.touch.y));
    }
}

void Touch::PostUpdate()
{
    auto& touchInst = GetInstance();
    
    touchInst.mHoldTouches.clear();
    touchInst.mPressedTouches.clear();
    touchInst.mReleasedTouches.clear();
    
    for (U32 i = 0; i < kTouchCount; ++i)
    {
        const U32 state = touchInst.mTouches[i].state;
        if ((state & static_cast<U32>(TouchState::Released)) > 0)
        {
            touchInst.mReleasedTouches.push_back(i);
            continue;
        }
        
        if ((state & static_cast<U32>(TouchState::Pressed)) > 0)
        {
            touchInst.mPressedTouches.push_back(i);
        }
        if ((state & static_cast<U32>(TouchState::Hold)) > 0)
        {
            touchInst.mHoldTouches.push_back(i);
        }
    }
}

Touch& Touch::GetInstance()
{
    static Touch touch;
    return touch;
}
    
} // namespace en
