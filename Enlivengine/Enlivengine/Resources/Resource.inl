#include <Enlivengine/Utils/Assert.hpp>
#include <Enlivengine/Utils/Hash.hpp>

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

} // namespace en
