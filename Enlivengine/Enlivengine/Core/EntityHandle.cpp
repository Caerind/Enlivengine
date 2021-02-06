#include <Enlivengine/Core/EntityHandle.hpp>

#include <Enlivengine/Utils/Assert.hpp>

#include <Enlivengine/Core/EntityManager.hpp>
#include <Enlivengine/Core/World.hpp>

namespace en
{

EntityHandle::EntityHandle()
	: mManager(nullptr)
	, mUID(U32_Max)
	, mID(U32_Max)
{
}

EntityHandle::EntityHandle(EntityManager& entityManager, U32 uid /*= U32_Max*/)
	: mManager(nullptr)
	, mUID(U32_Max)
	, mID(U32_Max)
{
	Set(entityManager, uid);
}

EntityHandle::EntityHandle(World& world, U32 uid /*= U32_Max*/)
	: mManager(nullptr)
	, mUID(U32_Max)
	, mID(U32_Max)
{
	Set(world, uid);
}

EntityHandle::EntityHandle(const Entity& entity)
	: mManager(nullptr)
	, mUID(U32_Max)
	, mID(U32_Max)
{
	Set(entity);
}

bool EntityHandle::operator==(const EntityHandle& other) const
{
	return mManager == other.mManager && mUID == other.mUID;
}

bool EntityHandle::operator!=(const EntityHandle& other) const
{
	return !operator==(other);
}

bool EntityHandle::IsValid() const
{
	return mManager != nullptr && mUID != kInvalidUID;
}

U32 EntityHandle::GetID() const
{
	return mID;
}

U32 EntityHandle::GetUID() const
{
	return mUID;
}

Entity EntityHandle::Get() const
{
	if (IsValid())
	{
		Entity entity(*mManager, static_cast<entt::entity>(mID));
		if (entity.IsValid())
		{
			return entity;
		}
		else
		{
			entity = mManager->GetEntityByUID(mUID);
			mID = entity.GetID();
			return entity;
		}
	}
	else
	{
		return Entity();
	}
}

bool EntityHandle::HasManager() const
{
	return mManager != nullptr;
}

EntityManager& EntityHandle::GetManager()
{
	enAssert(mManager != nullptr);
	return *mManager;
}

const EntityManager& EntityHandle::GetManager() const
{
	enAssert(mManager != nullptr);
	return *mManager;
}

World& EntityHandle::GetWorld()
{
	enAssert(mManager != nullptr);
	return mManager->GetWorld();
}

const World& EntityHandle::GetWorld() const
{
	enAssert(mManager != nullptr);
	return mManager->GetWorld();
}

void EntityHandle::Set(EntityManager& entityManager, U32 uid)
{
	mManager = &entityManager;
	mUID = uid;
	mID = U32_Max;
}

void EntityHandle::Set(World& world, U32 uid)
{
	mManager = &world.GetEntityManager();
	mUID = uid;
	mID = U32_Max;
}

void EntityHandle::Set(const Entity& entity)
{
	if (entity.HasManager())
	{
		mManager = const_cast<EntityManager*>(&entity.GetManager());
		if (entity.IsValid())
		{
			mUID = entity.GetUID();
			mID = entity.GetID();
		}
		else
		{
			mUID = U32_Max;
			mID = U32_Max;
		}
	}
	else
	{
		mManager = nullptr;
		mUID = U32_Max;
		mID = U32_Max;
	}
}

bool EntityHandle::Serialize(Serializer& serializer, const char* name)
{
	if (serializer.BeginClass(name, TypeInfo<EntityHandle>::GetName(), TypeInfo<EntityHandle>::GetHash()))
	{
		bool ret = true;

		GenericSerialization(serializer, "uid", mUID);

		ret = serializer.EndClass() && ret;
		return ret;
	}
	else
	{
		return false;
	}
}

bool EntityHandle::Edit(ObjectEditor& objectEditor, const char* name)
{
	ENLIVE_UNUSED(name);
	if (objectEditor.IsImGuiEditor())
	{
#ifdef ENLIVE_ENABLE_IMGUI
		Entity entity = Get();
		const char* entityName = "";
		if (entity.IsValid())
		{
			entityName = entity.GetName();
			if (entityName == nullptr || strlen(entityName) == 0)
			{
				entityName = "<Unknown>";
			}
		}
		else
		{
			entityName = "<Invalid>";
		}
		ImGui::Text("%s : %s (UID: %d, ID: %d)", name, entityName, mUID, mID);
#endif // ENLIVE_ENABLE_IMGUI
	}
	return false;
}

} // namespace en