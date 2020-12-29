#include <Enlivengine/Utils/Assert.hpp>
#include <Enlivengine/Utils/Hash.hpp>

namespace en
{

template <typename T>
ResourceLoader<T>::ResourceLoader(LoaderFunc loader)
	: mFunc(std::move(loader))
{
}

template <typename T>
bool ResourceLoader<T>::Load(T& resource) const
{
	return mFunc(resource);
}

template <typename T> 
ResourcePtr<T> ResourceManager::Create(const std::string& str, const ResourceLoader<T>& loader, ResourceKnownStrategy knownStrategy /*= ResourceKnownStrategy::Reuse*/)
{
	const auto id = StringToResourceID(str);
	const auto resourceIdType = CreateResourceIDTypeFromResourceID<T>(id);
	const auto itr = mResources.find(resourceIdType);
	if (itr == mResources.end() || knownStrategy == ResourceKnownStrategy::Reload)
	{
		std::unique_ptr<T> resource = std::make_unique<T>();
		if (T* resourcePtr = resource.get())
		{
			const bool loaded = loader.Load(*resourcePtr);
			if (!loaded)
			{
				enLogWarning(LogChannel::Application, "Resource {} cannot be loaded", str);
			}

#ifdef ENLIVE_DEBUG
			static_cast<priv::BaseResource*>(resourcePtr)->InitFromResourceManager(id, str);
#else
			static_cast<priv::BaseResource*>(resourcePtr)->InitFromResourceManager(id);
#endif // ENLIVE_DEBUG

			mResources[resourceIdType] = std::move(resource);

			return ResourcePtr<T>(id);
		}
		else
		{
			return ResourcePtr<T>(InvalidResourceID);
		}
	}
	else if (knownStrategy == ResourceKnownStrategy::Reuse)
	{
		return ResourcePtr<T>(id);
	}
	else // if (knownStrategy == ResourceKnownStrategy::Null)
	{
		return ResourcePtr<T>(InvalidResourceID);
	}
}

template <typename T> 
ResourcePtr<T> ResourceManager::Create(ResourceID id, const ResourceLoader<T>& loader, ResourceKnownStrategy knownStrategy /*= ResourceKnownStrategy::Reuse*/)
{
	const auto resourceIdType = CreateResourceIDTypeFromResourceID<T>(id);
	const auto itr = mResources.find(resourceIdType);
	if (itr == mResources.end() || knownStrategy == ResourceKnownStrategy::Reload)
	{
		std::unique_ptr<T> resource = std::make_unique<T>();
		if (T* resourcePtr = resource.get())
		{
			const bool loaded = loader.Load(*resourcePtr);
			if (!loaded)
			{
				enLogWarning(LogChannel::Application, "Resource with ID {} cannot be loaded", id);
			}

			resourcePtr->InitFromResourceManager(id);

			mResources[resourceIdType] = std::move(resource);

			return ResourcePtr<T>(id);
		}
		else
		{
			return ResourcePtr<T>(InvalidResourceID);
		}
	}
	else if (knownStrategy == ResourceKnownStrategy::Reuse)
	{
		return ResourcePtr<T>(id);
	}
	else // if (knownStrategy == ResourceKnownStrategy::Null)
	{
		return ResourcePtr<T>(InvalidResourceID);
	}
}

template <typename T> 
ResourcePtr<T> ResourceManager::Get(const std::string& str)
{
	return Get<T>(StringToResourceID(str));
}

template <typename T> 
ResourcePtr<T> ResourceManager::Get(ResourceID id)
{
	return ResourcePtr<T>(id);
}

template <typename T> 
ResourcePtr<T> ResourceManager::GetFromFilename(const std::string& filename)
{
    for (auto itr = mResources.begin(); itr != mResources.end(); ++itr)
    {
		if (T* resource = static_cast<T*>(itr->second.get()))
		{
			if (resource->GetLoadInfo().IsFromFile() && resource->GetLoadInfo().infoString == filename)
			{
				return ResourcePtr<T>(itr->first.id);
			}
		}
    }
    return ResourcePtr<T>(InvalidResourceID);
}

template <typename T>
bool ResourceManager::Has(const std::string& str) const
{
	return Has<T>(StringToResourceID(str));
}

template <typename T>
bool ResourceManager::Has(ResourceID id) const
{
	const auto resourceIdType = CreateResourceIDTypeFromResourceID<T>(id);
	return mResources.find(resourceIdType) != mResources.end();
}

template <typename T>
void ResourceManager::Release(const std::string& str)
{
	Release<T>(StringToResourceID(str));
}

template <typename T>
void ResourceManager::Release(ResourceID id)
{
	const auto resourceIdType = CreateResourceIDTypeFromResourceID<T>(id);
	const auto itr = mResources.find(resourceIdType);
	if (itr != mResources.end())
	{
		mResources.erase(itr);
	}
}

template <typename T>
U32 ResourceManager::Count() const
{
	if constexpr (Traits::IsVoid<T>::value)
	{
		return static_cast<U32>(mResources.size());
	}
	else
	{
		const U32 resourceType = T::GetStaticResourceType();
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
}

template <typename EnumClient>
bool ResourceManager::InitializeClientResourceTypes()
{
	const auto enumValues = Enum::GetValues<EnumClient>();
	mClientResourceTypeNames.clear();
	if (static_cast<U32>(enumValues[0]) == static_cast<U32>(ResourceType::Max))
	{
		mClientResourceTypeNames.reserve(enumValues.size());
		for (const auto& enumValue : enumValues)
		{
			mClientResourceTypeNames.push_back(Enum::GetValueName(enumValue));
		}
		return true;
	}
	else
	{
		enAssert(false);
		return false;
	}
}

#ifdef ENLIVE_DEBUG
template <typename T>
void ResourceManager::GetResourceInfosOfType(Array<ResourceInfo>& resourceInfos)
{
	if constexpr (Traits::IsVoid<T>::value)
	{
		GetResourceInfos(resourceInfos);
	}
	else
	{
		GetResourceInfosOfType(resourceInfos, T::GetStaticResourceType());
	}
}

template <typename T>
std::string_view en::ResourceManager::GetResourceTypeName() const
{
	return GetResourceTypeName(T::GetStaticResourceType());
}
#endif // ENLIVE_DEBUG

template <typename T> 
T* ResourceManager::GetRawPtr(ResourceID id)
{
	const auto resourceIdType = CreateResourceIDTypeFromResourceID<T>(id);
	const auto itr = mResources.find(resourceIdType);
	enAssert(itr != mResources.end());
	return static_cast<T*>(itr->second.get());
}

template <typename T> 
const T* ResourceManager::GetRawPtr(ResourceID id) const
{
	const auto resourceIdType = CreateResourceIDTypeFromResourceID<T>(id);
	const auto itr = mResources.find(resourceIdType);
	enAssert(itr != mResources.end());
	return static_cast<T*>(itr->second.get());
}

template <typename T>
priv::ResourceIDType ResourceManager::CreateResourceIDTypeFromResourceID(ResourceID id)
{
	priv::ResourceIDType resourceIdType;
	resourceIdType.id = id;
	resourceIdType.type = T::GetStaticResourceType();
	return resourceIdType;
}

} // namespace en
