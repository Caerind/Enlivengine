#pragma once

#include <string>
#include <Enlivengine/Math/Transform.hpp>
#include <Enlivengine/Graphics/Sprite.hpp>
#include <Enlivengine/Graphics/Tilemap.hpp>

namespace en
{

struct NameComponent
{
	NameComponent() : name("") {}
	NameComponent(const char* pName) : name(pName) {}
	NameComponent(const std::string& pName) : name(pName) {}
	NameComponent(const std::string_view& pName) : name(pName) {}

	std::string name;
};

struct UIDComponent
{
	UIDComponent() : id(0) {}
	UIDComponent(U32 pId) : id(pId) {}

	U32 id;
};

struct RenderableComponent
{
};

struct SpriteComponent
{
	SpriteComponent() : sprite() {}

	Sprite sprite;
};

struct TilemapComponent
{
	TilemapComponent() : tilemap() {}

	Tilemap tilemap;
};

} // namespace en