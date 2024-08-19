#pragma once

#include <SFML/Graphics.hpp>

#include <Enlivengine/Window/Window.hpp>
#include <Enlivengine/Math/Color.hpp>

namespace en
{
    
class RenderTarget
{
public:
    RenderTarget() = default;
    RenderTarget(RenderTarget&& other) noexcept;
    RenderTarget(const RenderTarget& other) = delete;
    ~RenderTarget();
    
    bool ExtractFromWindow(Window& window);
    bool Create(const Vector2u& size);

    bool IsValid() const;
    bool IsRenderWindow() const;

    Vector2u GetSize() const; 
    void Resize(const Vector2u& size);
    
    void Clear(const Color& color = Colors::Black);
    void Destroy();
    
    sf::RenderTarget* GetSFMLTarget();

private:
    sf::RenderTarget* mRenderTarget{nullptr};
    Window* mWindow{nullptr};
};
    
} // namespace en