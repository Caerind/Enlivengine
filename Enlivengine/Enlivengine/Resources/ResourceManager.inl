#include <Enlivengine/Utils/Assert.hpp>
#include <Enlivengine/Utils/Hash.hpp>

#ifdef ENLIVE_ENABLE_IMGUI
#include <imgui/imgui.h>
#endif // ENLIVE_ENABLE_IMGUI

namespace en
{

template <typename T>
ResourcePtr<T>::ResourcePtr(ResourceID id)
	: mID(id)
{
}

template <typename T>
ResourceID ResourcePtr<T>::GetID() const
{
	return mID;
}

template <typename T>
U32 ResourcePtr<T>::GetResourceType() const
{
	return T::GetStaticResourceType();
}

template <typename T>
bool ResourcePtr<T>::HasValidID() const
{
	return mID != InvalidResourceID;
}

template <typename T>
bool ResourcePtr<T>::IsValid() const
{
	return HasValidID() && ResourceManager::GetInstance().Has<T>(mID);
}

template <typename T>
ResourcePtr<T>::operator bool() const
{
	return IsValid();
}

template <typename T>
void ResourcePtr<T>::Release()
{
	mID = InvalidResourceID;
}

template <typename T>
T* ResourcePtr<T>::GetPtr() const
{
	return (IsValid()) ? ResourceManager::GetInstance().GetRawPtr<T>(mID) : nullptr;
}

template <typename T>
T& ResourcePtr<T>::Get() const
{
	enAssert(IsValid());
	return *(ResourceManager::GetInstance().GetRawPtr<T>(mID));
}

template <typename T>
void ResourcePtr<T>::ReleaseFromManager()
{
	if (HasValidID())
	{
		ResourceManager::GetInstance().Release<T>(mID);
		Release();
	}
}

template <typename T>
bool ResourcePtr<T>::Serialize(Serializer& serializer, const char* name)
{
	return serializer.Serialize(name, mID);
}

template <typename T>
bool ResourcePtr<T>::Edit(ObjectEditor& objectEditor, const char* name)
{
	bool modified = false;

#ifdef ENLIVE_ENABLE_IMGUI
	if (objectEditor.IsImGuiEditor())
	{
		std::vector<ResourceInfo> resourceInfos;
		ResourceManager::GetInstance().GetResourceInfosOfType<T>(resourceInfos);

		static const char* nullResourceIdentifier = "@Null";
		static const char* notLoadedResourceIdentifier = "@NotLoaded";
		const char* currentLabel = nullptr;

		if (HasValidID())
		{
			bool found = false;
			const U32 size = static_cast<U32>(resourceInfos.size());
			for (U32 i = 0; i < size && !found; ++i)
			{
				if (resourceInfos[i].id == GetID())
				{
					currentLabel = resourceInfos[i].identifier.c_str();
					found = true;
				}
			}
			if (found)
			{
				ImGui::Text(ICON_FA_CHECK);
			}
			else
			{
				ImGui::Text(ICON_FA_EXCLAMATION);
				currentLabel = notLoadedResourceIdentifier;
			}
		}
		else
		{
			ImGui::Text(ICON_FA_EXCLAMATION);
			currentLabel = nullResourceIdentifier;
		}

		ImGui::SameLine();
		ImGui::PushID("ComboResourceIdentifier");
		if (ImGui::BeginCombo(name, currentLabel))
		{
			{
				const bool nullSelected = !HasValidID();
				if (ImGui::Selectable(nullResourceIdentifier, nullSelected))
				{
					Release();
					modified = true;
				}
				if (nullSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			const U32 size = static_cast<U32>(resourceInfos.size());
			for (U32 i = 0; i < size; ++i)
			{
				bool selected = resourceInfos[i].id == GetID();
				if (ImGui::Selectable(resourceInfos[i].identifier.c_str(), selected))
				{
					mID = resourceInfos[i].id;
					modified = true;
					selected = true;
				}
				if (selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		ImGui::PopID();
	}
	else
#endif // ENLIVE_ENABLE_IMGUI
	{
		modified = GenericEdit(objectEditor, name, mID);
	}

	return modified;
}

template <typename T>
bool ResourcePtr<T>::operator==(const ResourcePtr<T>& other) const
{
	return mID == other.mID;
}

template <typename T>
bool ResourcePtr<T>::operator!=(const ResourcePtr<T>& other) const
{
	return !operator==(other);
}

template <typename T>
Resource<T>::Resource()
	: priv::BaseResource()
{
}

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
void ResourceManager::GetResourceInfosOfType(std::vector<ResourceInfo>& resourceInfos)
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
