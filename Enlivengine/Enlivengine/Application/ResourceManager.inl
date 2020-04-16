#include <Enlivengine/System/Assert.hpp>
#include <Enlivengine/System/Hash.hpp>

namespace en
{

template <typename T>
ResourcePtr<T>::ResourcePtr(ResourceID id, ResourceManager* mgr)
	: mID(id)
	, mManager(mgr)
{
}

template <typename T>
bool ResourcePtr<T>::IsValid() const
{
	return mID != InvalidResourceID && mManager != nullptr && mManager->Has(mID);
}

template <typename T>
ResourcePtr<T>::operator bool() const
{
	return IsValid();
}

template <typename T>
void ResourcePtr<T>::Release()
{
	mManager = nullptr;
	mID = InvalidResourceID;
}

template <typename T>
T* ResourcePtr<T>::GetPtr() const
{
	return (IsValid()) ? mManager->GetRawPtr<T>(mID) : nullptr;
}

template <typename T>
T& ResourcePtr<T>::Get() const
{
	assert(IsValid());
	return *(mManager->GetRawPtr<T>(mID));
}

template <typename T>
ResourceID ResourcePtr<T>::GetID() const
{
	return mID;
}

template <typename T>
void ResourcePtr<T>::ReleaseFromManager()
{
	if (IsValid())
	{
		mManager->Release(mID);
		Release();
	}
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
ResourcePtr<T> ResourceManager::Create(const std::string& str, const ResourceLoader<T>& loader, ResourceKnownStrategy knownStrategy)
{
	ResourceID id = priv::StringToResourceID(str);
	const auto itr = mResources.find(id);
	if (itr == mResources.end() || knownStrategy == ResourceKnownStrategy::Reload)
	{
		std::unique_ptr<T> resource = std::make_unique<T>();
		if (T* resourcePtr = resource.get())
        {
            resourcePtr->mIdentifier = str;
			resourcePtr->mLoaded = loader.Load(*resourcePtr);
			resourcePtr->mID = id;

			mResources[resourcePtr->mID] = std::move(resource);

			return ResourcePtr<T>(id, this);
		}
		else
		{
			return ResourcePtr<T>(InvalidResourceID, this);
		}
	}
	else if (knownStrategy == ResourceKnownStrategy::Reuse)
	{
		return ResourcePtr<T>(id, this);
	}
	else
	{
		return ResourcePtr<T>(InvalidResourceID, this);
	}
}

template <typename T> 
ResourcePtr<T> ResourceManager::Get(const std::string& str)
{
	return Get<T>(priv::StringToResourceID(str));
}

template <typename T> 
ResourcePtr<T> ResourceManager::Get(ResourceID id)
{
	id = Has(id) ? id : InvalidResourceID;
	return ResourcePtr<T>(id, this);
}

template <typename T> 
ResourcePtr<T> en::ResourceManager::GetFromFilename(const std::string& filename)
{
    for (auto itr = mResources.begin(); itr != mResources.end(); ++itr)
    {
        if (itr->second->GetFilename() == filename)
        {
            if (T* validType = static_cast<T*>(itr->second.get()))
            {
                return ResourcePtr<T>(itr->first, this);
            }
            else
            {
                return ResourcePtr<T>(InvalidResourceID, this);
            }
        }
    }
    return ResourcePtr<T>(InvalidResourceID, this);
}

template <typename T> 
T* ResourceManager::GetRawPtr(ResourceID id)
{
	assert(Has(id));
	return static_cast<T*>(mResources.find(id)->second.get());
}

template <typename T> 
const T* ResourceManager::GetRawPtr(ResourceID id) const
{
	assert(Has(id));
	return static_cast<T*>(mResources.find(id)->second.get());
}

} // namespace en
