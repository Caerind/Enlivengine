#include <Enlivengine/Core/EntityManager.hpp>

#include <Enlivengine/Core/World.hpp>
#include <Enlivengine/Core/Entity.hpp>

namespace en
{

EntityManager::EntityManager(World& world)
	: mWorld(world)
	, mRegistry()
{
}

Entity EntityManager::CreateEntity()
{
	return Entity(*this, mRegistry.create());
}

void EntityManager::DestroyEntity(Entity& entity)
{
	if (entity.IsValid())
	{
		mRegistry.destroy(entity.GetEntity());
		entity.mEntity = entt::null;
	}
}

void EntityManager::ClearEntities()
{
	mRegistry.clear();
}

U32 EntityManager::GetEntityCount() const
{
	return static_cast<U32>(mRegistry.alive());
}

World& EntityManager::GetWorld()
{
	return mWorld;
}

const World& EntityManager::GetWorld() const
{
	return mWorld;
}

bool EntityManager::Serialize(Serializer& serializer, const char* name)
{
	if (serializer.BeginClass(name, TypeInfo<EntityManager>::GetName(), TypeInfo<EntityManager>::GetHash()))
	{
		bool ret = true;

		if (serializer.IsReading())
		{
			U32 size = 0;
			ret = GenericSerialization(serializer, "size", size) && ret;
			for (U32 i = 0; i < size; ++i)
			{
				const std::string entityName = "Entity_" + ToString(i);
				if (serializer.HasNode(entityName.c_str()))
				{
					Entity entity = CreateEntity();
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
			ret = GenericSerialization(serializer, "size", GetEntityCount()) && ret;
			U32 i = 0;
			mRegistry.each([&ret, &serializer, &i, this](auto entt)
			{
				Entity entity(*this, entt);
				if (entity.IsValid())
				{
					const std::string entityName = "Entity_" + ToString(i);
					ret = GenericSerialization(serializer, entityName.c_str(), entity) && ret;
					i++;
				}
				else
				{
					enAssert(false);
					ret = false;
				}
			});
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
			Each([this, &objectEditor, &ret](auto enttEntity)
			{
				Entity entity(*this, enttEntity);
				if (entity.IsValid())
				{
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
			});
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