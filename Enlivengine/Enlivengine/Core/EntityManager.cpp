#include <Enlivengine/Core/EntityManager.hpp>

#include <Enlivengine/Core/World.hpp>
#include <Enlivengine/Core/Entity.hpp>

namespace en
{

EntityManager::EntityManager(World& world)
	: mWorld(world)
	, mRegistry()
	, mUIDGenerator(0)
{
}

Entity EntityManager::CreateEntity()
{
	Entity entity(*this, mRegistry.create());
	if (entity.IsValid())
	{
		UIDComponent& uidComponent = entity.Add<UIDComponent>();
		uidComponent.mUID = mUIDGenerator++;
		return entity;
	}
	else
	{
		return Entity();
	}
}

void EntityManager::DestroyEntity(Entity& entity)
{
	if (entity.IsValid())
	{
		mRegistry.destroy(entity.GetEntity());
		entity.mEntity = entt::null;
	}
}

U32 EntityManager::GetEntityCount() const
{
	return static_cast<U32>(mRegistry.alive());
}

Entity EntityManager::GetEntityByUID(UID uid) const
{
	Entity resultEntity = Entity();
	bool found = false;
	mRegistry.each([&found, &resultEntity, &uid, this](auto entt)
	{
		if (!found)
		{
			Entity entity(const_cast<EntityManager&>(*this), entt);
			enAssert(entity.IsValid());
			enAssert(entity.Has<UIDComponent>());
			UIDComponent& uidComponent = entity.Get<UIDComponent>();
			if (uidComponent.GetUID() == uid)
			{
				resultEntity = entity;
				found = true;
			}
		}
	});
	return resultEntity;
}

World& EntityManager::GetWorld()
{
	return mWorld;
}

const World& EntityManager::GetWorld() const
{
	return mWorld;
}


// Used to display Entities in the same order
// For serialization & edition
namespace priv
{
	struct EntityUID
	{
		EntityUID(const Entity& e, U32 u) : entity(e), uid(u) {}

		Entity entity;
		U32 uid;
	};

	bool GetEntities(EntityManager& entityManager, std::vector<EntityUID>& entities)
	{
		entities.reserve(entityManager.GetEntityCount());
		entityManager.Each([&entities, &entityManager](auto entt)
		{
			Entity entity(entityManager, entt);
			if (entity.IsValid())
			{
				entities.push_back(EntityUID(entity, entity.GetUID()));
			}
		});
		enAssert(entityManager.GetEntityCount() == static_cast<U32>(entities.size()));
		std::sort(entities.begin(), entities.end(), [](const EntityUID& a, const EntityUID& b)
		{
				return a.uid < b.uid;
		});
		return true;
	}
}

bool EntityManager::Serialize(Serializer& serializer, const char* name)
{
	if (serializer.BeginClass(name, TypeInfo<EntityManager>::GetName(), TypeInfo<EntityManager>::GetHash()))
	{
		bool ret = true;

		GenericSerialization(serializer, "uidGenerator", mUIDGenerator) && ret;

		if (serializer.IsReading())
		{
			U32 size = 0;
			ret = GenericSerialization(serializer, "size", size) && ret;
			for (U32 i = 0; i < size; ++i)
			{
				const std::string entityName = "Entity_" + ToString(i);
				if (serializer.HasNode(entityName.c_str()))
				{
					Entity entity = Entity(*this, mRegistry.create());
					if (entity.IsValid())
					{
						ret = GenericSerialization(serializer, entityName.c_str(), entity) && ret;
					}
					else
					{
						enAssert(false);
						ret = false;
					}
				}
				else
				{
					enAssert(false);
					ret = false;
				}
			}
		}
		else if (serializer.IsWriting())
		{
			std::vector<priv::EntityUID> entities;
			priv::GetEntities(*this, entities);

			const U32 size = static_cast<U32>(entities.size());
			enAssert(size == GetEntityCount());
			ret = GenericSerialization(serializer, "size", size) && ret;
			for (U32 i = 0; i < size; ++i)
			{
				Entity entity = entities[i].entity;
				enAssert(entity.IsValid());

				const std::string entityName = "Entity_" + ToString(i);
				ret = GenericSerialization(serializer, entityName.c_str(), entity) && ret;
			}
		}
		else
		{
			enAssert(false);
			ret = false;
		}
		ret = serializer.EndClass() && ret;
		return ret;
	}
	else
	{
		return false;
	}
}

bool EntityManager::Edit(ObjectEditor& objectEditor, const char* name)
{
	if (objectEditor.BeginClass(name, TypeInfo<EntityManager>::GetName(), TypeInfo<EntityManager>::GetHash()))
	{
		bool ret = false;
#ifdef ENLIVE_ENABLE_IMGUI
		if (objectEditor.IsImGuiEditor())
		{
			if (ImGui::Button("New Entity"))
			{
				CreateEntity();
				ret = true;
			}

			std::vector<priv::EntityUID> entities;
			priv::GetEntities(*this, entities);

			const U32 size = static_cast<U32>(entities.size());
			enAssert(size == GetEntityCount());
			for (U32 i = 0; i < size; ++i)
			{
				Entity entity = entities[i].entity;
				enAssert(entity.IsValid());

				ImGui::PushID(entity.GetID());
				if (ImGui::Button("X"))
				{
					DestroyEntity(entity);
					ret = true;
				}
				else
				{
					ImGui::SameLine();
					if (entity.Edit(objectEditor, entity.GetName()))
					{
						ret = true;
					}
				}
				ImGui::PopID();
			}
		}
#endif // ENLIVE_ENABLE_IMGUI
		objectEditor.EndClass();
		return ret;
	}
	else
	{
		return false;
	}
}

entt::registry& EntityManager::GetRegistry()
{
	return mRegistry;
}

const entt::registry& EntityManager::GetRegistry() const
{
	return mRegistry;
}

} // namespace en