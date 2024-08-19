#include <Enlivengine/Graphics/RenderTarget.hpp>

#include <Enlivengine/Utils/Assert.hpp>

namespace en
{
    
RenderTarget::~RenderTarget()
{
    Destroy();
}
    
bool RenderTarget::ExtractFromWindow(Window& window)
{
    enAssert(mWindow == nullptr);
    enAssert(mRenderTarget == nullptr);
    
    enAssert(window.IsValid());
    mWindow = &window;
    
    mRenderTarget = &window.GetSFMLWindow();
    
    return true;
}

bool RenderTarget::Create(const Vector2u& size)
{
    enAssert(mWindow == nullptr);
    enAssert(mRenderTarget == nullptr);
    
    sf::RenderTexture* rt = new sf::RenderTexture();
    rt->create(size.x, size.y);
    mRenderTarget = rt;
    return true;
}

bool RenderTarget::IsValid() const
{
    return mRenderTarget != nullptr;
}

bool RenderTarget::IsRenderWindow() const
{
    return mWindow != nullptr && mRenderTarget != nullptr;
}

Vector2u RenderTarget::GetSize() const
{
    enAssert(IsValid());
    const auto size = mRenderTarget->getSize();
    return { size.x, size.y };
}

void RenderTarget::Resize(const Vector2u& newSize)
{
    enAssert(IsValid() && !IsRenderWindow());
    Destroy();
    Create(newSize);
}
    
void RenderTarget::Clear(const Color& color)
{
    enAssert(mRenderTarget != nullptr);
    mRenderTarget->clear(sf::Color(color.r, color.g, color.b, color.a));
}

void RenderTarget::Destroy()
{
    if (mWindow != nullptr)
    {
        mRenderTarget = nullptr;
    }
    else if (mRenderTarget != nullptr)
    {
        delete mRenderTarget;
        mRenderTarget = nullptr;
    }

    enAssert(mRenderTarget == nullptr);
}
    
sf::RenderTarget* RenderTarget::GetSFMLTarget()
{
    return mRenderTarget;
}
    
} // namespace en