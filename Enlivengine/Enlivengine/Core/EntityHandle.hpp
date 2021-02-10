#pragma once

#include <Enlivengine/Core/Entity.hpp>

namespace en
{

class EntityManager;
class World;

class EntityHandle
{
public:
	EntityHandle();
	EntityHandle(EntityManager& entityManager, U32 uid = U32_Max);
	EntityHandle(World& world, U32 uid = U32_Max);
	explicit EntityHandle(const Entity& entity);

	bool operator==(const EntityHandle& other) const;
	bool operator!=(const EntityHandle& other) const;

	bool IsValid() const; // Manager & UID are valid
	U32 GetID() const;
	U32 GetUID() const;

	Entity Get() const;

	bool HasManager() const;
	EntityManager& GetManager();
	const EntityManager& GetManager() const;
	World& GetWorld();
	const World& GetWorld() const;

	void Set(EntityManager& entityManager, U32 uid = U32_Max);
	void Set(World& world, U32 uid = U32_Max);
	void Set(const Entity& entity);

	bool Serialize(Serializer& serializer, const char* name);
	bool Edit(ObjectEditor& objectEditor, const char* name);

private:
	EntityManager* mManager;
	U32 mUID;
	mutable U32 mID;
};

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::EntityHandle, en::Type_CustomSerialization, en::Type_CustomEditor)
ENLIVE_META_CLASS_END()