#include <Enlivengine/Core/EntityManager.hpp>

#include <Enlivengine/Core/World.hpp>
#include <Enlivengine/Core/Entity.hpp>

#include <Enlivengine/Core/ComponentFactory.hpp>

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
		const auto& componentInfos = ComponentFactory::GetComponentInfos();
		const auto endItr = componentInfos.cend();

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
						if (serializer.BeginClass(entityName.c_str(), kManagedEntityName, kManagedEntityHash))
						{
							U32 id = 0;
							ret = GenericSerialization(serializer, "id", id) && ret;
							for (auto itr = componentInfos.cbegin(); itr != endItr; ++itr)
							{
								const auto& ci = itr->second;
								if (serializer.HasNode(ci.name))
								{
									ci.add(entity);
									ret = ci.serialize(serializer, entity) && ret;
								}
							}
							ret = serializer.EndClass() && ret;
						}
						else
						{
							ret = false;
						}
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
			mRegistry.each([&ret, &serializer, &i, &componentInfos, &endItr, this](auto entt)
			{
				Entity entity(*this, entt);
				if (entity.IsValid())
				{
					const std::string entityName = "Entity_" + ToString(i);
					if (serializer.BeginClass(entityName.c_str(), kManagedEntityName, kManagedEntityHash))
					{
						ret = GenericSerialization(serializer, "id", entity.GetID()) && ret;
						for (auto itr = componentInfos.cbegin(); itr != endItr; ++itr)
						{
							const auto& ci = itr->second;
							if (ci.has(entity))
							{
								ret = ci.serialize(serializer, entity) && ret;
							}
						}
						ret = serializer.EndClass() && ret;
					}
					else
					{
						ret = false;
					}
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