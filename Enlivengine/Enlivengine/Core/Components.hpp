#pragma once

#include <entt/entt.hpp>

#include <string>

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/System/Config.hpp>

namespace en
{

#ifdef ENLIVE_DEBUG
struct NameComponent : public std::string
{
};
#endif // ENLIVE_DEBUG

struct PositionComponent : public sf::Transformable
{
};

struct RenderableComponent
{
	en::F32 x;
	en::F32 y;
	en::U32 z;
};

struct SpriteComponent : public sf::Sprite
{
};

struct TextComponent : public sf::Text
{
};

struct AnimationComponent
{
};

} // namespace en