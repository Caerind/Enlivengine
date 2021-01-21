#pragma once

#include <string>
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

ENLIVE_META_CLASS_BEGIN(en::NameComponent, en::Type_ClassSerialization, en::Type_ClassEditor)
	ENLIVE_META_CLASS_MEMBER("name", &en::NameComponent::name)
ENLIVE_META_CLASS_END()

ENLIVE_META_CLASS_BEGIN(en::UIDComponent, en::Type_ClassSerialization, en::Type_ClassEditor)
	ENLIVE_META_CLASS_MEMBER("id", &en::UIDComponent::id)
ENLIVE_META_CLASS_END()
	
ENLIVE_META_CLASS_BEGIN(en::RenderableComponent, en::Type_ClassSerialization, en::Type_ClassEditor)
ENLIVE_META_CLASS_END()

ENLIVE_META_CLASS_BEGIN(en::SpriteComponent, en::Type_ClassSerialization, en::Type_ClassEditor)
	ENLIVE_META_CLASS_MEMBER("sprite", &en::SpriteComponent::sprite)
ENLIVE_META_CLASS_END()

ENLIVE_META_CLASS_BEGIN(en::TilemapComponent, en::Type_ClassSerialization, en::Type_ClassEditor)
	ENLIVE_META_CLASS_MEMBER("tilemap", &en::TilemapComponent::tilemap)
ENLIVE_META_CLASS_END()