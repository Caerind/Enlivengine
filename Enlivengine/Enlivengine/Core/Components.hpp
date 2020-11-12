#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_CORE

#include <string>
#include <Enlivengine/Math/Transform.hpp>
#include <Enlivengine/Graphics/Sprite.hpp>
#include <Enlivengine/Graphics/Camera.hpp>

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

struct TransformComponent
{
	TransformComponent() : transform() {}

	Transform transform;
};

struct SpriteComponent
{
	SpriteComponent() : sprite() {}

	Sprite sprite;
};

struct CameraComponent
{
	CameraComponent() : camera() {}

	Camera camera;
};

} // namespace en

#endif // ENLIVE_MODULE_CORE