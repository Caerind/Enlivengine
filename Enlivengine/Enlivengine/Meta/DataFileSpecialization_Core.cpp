#include <Enlivengine/Meta/DataFileSpecialization_Core.hpp>

#include <Enlivengine/Meta/ComponentFactory.hpp>

namespace en::DataFileSpecialization
{

bool Serialize(DataFile& dataFile, const Entity& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.CreateNode(name))
	{
		dataFile.WriteCurrentType<Entity>();
		const auto& componentInfos = ComponentFactory::GetComponentInfos();
		const auto endItr = componentInfos.cend();
		for (auto itr = componentInfos.cbegin(); itr != endItr; ++itr)
		{
			const auto& ci = itr->second;
			if (ci.has(object))
			{
				ci.serialize(dataFile, object);
			}
		}
		parser.CloseNode();
		return true;
	}
	else
	{
		return false;
	}
}

bool Deserialize(en::DataFile& dataFile, Entity& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (strcmp(name, "") == 0)
	{
		bool anyError = false;

		// We should already be on the node of the entity : See explanation on EntityManager::Deserialize
		enAssert(dataFile.ReadCurrentType() == TypeInfo<Entity>::GetHash());
		if (parser.ReadFirstNode())
		{
			struct DeserializationComponentNode
			{
				std::string name;
				U32 hash;
			};
			static std::vector<DeserializationComponentNode> componentNodes;
			componentNodes.clear();
			do
			{
				const std::string nodeName = parser.GetNodeName();
				const U32 nodeNameHash = Hash::SlowHash(nodeName);
				const U32 nodeType = dataFile.ReadCurrentType();
				const bool registeredComponent = ComponentFactory::IsRegistered(nodeNameHash);
				if (nodeNameHash == nodeType && registeredComponent)
				{
					componentNodes.push_back({ nodeName, nodeNameHash });
				}
				else
				{
					if (nodeNameHash != nodeType)
					{
						enLogWarning(LogChannel::Core, "Incompatible component : {}({}) <-> {} ?", nodeNameHash, nodeName, nodeType);
					}
					if (!registeredComponent)
					{
						enLogWarning(LogChannel::Core, "Unregistered component : {}", nodeName);
					}
					anyError = true;
				}
			} while (parser.NextSibling());
			parser.CloseNode();

			// Now, we are back at the entity level, parse components
			for (const auto& componentNode : componentNodes)
			{
				const auto& componentInfos = ComponentFactory::GetComponentInfos();
				enAssert(componentInfos.find(componentNode.hash) != componentInfos.end());
				if (!componentInfos.at(componentNode.hash).deserialize(dataFile, object))
				{
					anyError = true;
				}
			}
		}
		return !anyError;
	}
	else
	{
		// Single entity deserialization is not supported yet
		enAssert(false);
		return false;
	}
}

bool Serialize(DataFile& dataFile, const EntityManager& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.CreateNode(name))
	{
		bool anyError = false;
		dataFile.WriteCurrentType<EntityManager>();
		object.Each([&dataFile, &object, &anyError](auto entityEntt)
			{
				Entity entity(const_cast<EntityManager&>(object), entityEntt);
				if (entity.IsValid())
				{
					if (!HasCustomSerialization<Entity>::Serialize(dataFile, entity, entity.GetName()))
					{
						anyError = true;
					}
				}
			});
		parser.CloseNode();
		return !anyError;
	}
	else
	{
		return false;
	}
}

bool Deserialize(DataFile& dataFile, EntityManager& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.ReadNode(name))
	{
		bool anyError = false;
		enAssert(dataFile.ReadCurrentType() == TypeInfo<EntityManager>::GetHash());
		if (parser.ReadFirstNode())
		{
			do
			{
				Entity entity = object.CreateEntity();
				if (entity.IsValid())
				{
					static const char* invalidEntityName = ""; // We already have the node open + we can have many entities with the same name
					if (!HasCustomSerialization<Entity>::Deserialize(dataFile, entity, invalidEntityName))
					{
						anyError = true;
					}
				}
			} while (parser.NextSibling());
			parser.CloseNode();
		}
		parser.CloseNode();
		return !anyError;
	}
	else
	{
		return false;
	}
}

bool Serialize(DataFile& dataFile, const World& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.CreateNode(name))
	{
		bool valid = true;
		dataFile.WriteCurrentType<World>();
		valid = dataFile.Serialize(object.GetEntityManager(), "EntityManager") && valid;
		//valid = dataFile.Serialize(object.mSystems, "Systems") && valid;
		parser.CloseNode();
		return valid;
	}
	else
	{
		return false;
	}
}

bool Deserialize(DataFile& dataFile, World& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.ReadNode(name))
	{
		bool valid = true;
		enAssert(dataFile.ReadCurrentType() == TypeInfo<EntityManager>::GetHash());
		valid = dataFile.Deserialize(object.GetEntityManager(), "EntityManager") && valid;
		//valid = dataFile.Deserialize(object.mSystems, "Systems") && valid;
		parser.CloseNode();
		return valid;
	}
	else
	{
		return false;
	}
}

} // namespace en::DataFileSpecialization
