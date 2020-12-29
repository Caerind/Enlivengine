#include <Enlivengine/Resources/ResourceManager.hpp>

#include <Enlivengine/Utils/Enums.hpp>

namespace en
{

ResourceManager::ResourceManager()
{
}

bool ResourceManager::Has(ResourceID id, U32 resourceType) const
{
	const auto resourceIdType = CreateResourceIDTypeFromResourceIDAndType(id, resourceType);
	return mResources.find(resourceIdType) != mResources.end();
}

void ResourceManager::Release(ResourceID id, U32 resourceType)
{
	const auto resourceIdType = CreateResourceIDTypeFromResourceIDAndType(id, resourceType);
	const auto itr = mResources.find(resourceIdType);
	if (itr != mResources.end())
	{
		mResources.erase(itr);
	}
}

void ResourceManager::ReleaseAll()
{
	mResources.clear();
}

U32 ResourceManager::Count(U32 resourceType) const
{
	U32 count = 0;
	for (auto itr = mResources.begin(); itr != mResources.end(); ++itr)
	{
		if (itr->second->GetResourceType() == resourceType)
		{
			count++;
		}
	}
	return count;
}

#ifdef ENLIVE_DEBUG
void ResourceManager::GetResourceInfosOfType(Array<ResourceInfo>& resourceInfos, U32 resourceType)
{
	resourceInfos.Clear();
	for (auto itr = mResources.begin(); itr != mResources.end(); ++itr)
	{
		if (priv::BaseResource* r = itr->second.get())
		{
			if (r->GetResourceType() == resourceType)
			{
				resourceInfos.Add(r->GetResourceInfo());
			}
		}
		else
		{
			enLogError(LogChannel::Application, "Nullptr resource in ResourceManager");
		}
	}
}

void ResourceManager::GetResourceInfos(Array<ResourceInfo>& resourceInfos)
{
	resourceInfos.Clear();
	for (auto itr = mResources.begin(); itr != mResources.end(); ++itr)
	{
		if (priv::BaseResource* r = itr->second.get())
		{
			resourceInfos.Add(r->GetResourceInfo());
		}
		else
		{
			enLogError(LogChannel::Application, "Nullptr resource in ResourceManager");
		}
	}
	resourceInfos.Sort([](const ResourceInfo& a, const ResourceInfo& b)
	{
		if (a.type != b.type)
		{
			return a.type < b.type;
		}
		else
		{
			return a.id < b.id;
		}
	});
}

std::string_view ResourceManager::GetResourceTypeName(U32 resourceType) const
{
	if (resourceType >= static_cast<U32>(ResourceType::Max))
	{
		const U32 index = resourceType - static_cast<U32>(ResourceType::Max);
		enAssert(index < static_cast<U32>(mClientResourceTypeNames.size()));
		return mClientResourceTypeNames[index];
	}
	else
	{
		return Enum::GetValueName(static_cast<ResourceType>(resourceType));
	}
}

U32 ResourceManager::GetResourceTypeCount() const
{
	return static_cast<U32>(ResourceType::Max) - 1 + static_cast<U32>(mClientResourceTypeNames.size());
}
#endif // ENLIVE_DEBUG

ResourceID ResourceManager::StringToResourceID(const std::string& str)
{
	return Hash::SlowHash(str);
}

priv::ResourceIDType ResourceManager::CreateResourceIDTypeFromResourceIDAndType(ResourceID id, U32 type)
{
	priv::ResourceIDType resourceIdType;
	resourceIdType.id = id;
	resourceIdType.type = type;
	return resourceIdType;
}

} // namespace en