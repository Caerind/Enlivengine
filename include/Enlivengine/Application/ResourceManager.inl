
#include <Enlivengine/System/Hash.hpp>

namespace en
{

template<typename T>
inline ResourceManager<T>::ResourceManager()
	: mResources()
{
}

template<typename T>
inline ResourceManager<T>::~ResourceManager()
{
	releaseAll();
}

template<typename T>
inline ResourceId ResourceManager<T>::create(const char* name, std::function<void(T& r)> loader)
{
	ResourceId index(Hash::CRC32(name));
	if (!has(index))
	{
		loader(mResources[index]);
	}
	return index;
}

template<typename T>
inline bool ResourceManager<T>::has(const char* name) const
{
	return has(Hash::CRC32(name));
}

template<typename T>
inline bool ResourceManager<T>::has(ResourceId resource) const
{
	return (mResources.find(resource) != mResources.end());
}

template<typename T>
inline T& ResourceManager<T>::get(const char* name)
{
	return get(Hash::CRC32(name));
}

template<typename T>
inline T& ResourceManager<T>::get(ResourceId resource)
{
	return mResources[resource];
}

template<typename T>
inline void ResourceManager<T>::release(const char* name)
{
	release(Hash::CRC32(name));
}

template<typename T>
inline void ResourceManager<T>::release(ResourceId resource)
{
	auto itr = mResources.find(resource);
	if (itr != mResources.end())
	{
		mResources.erase(itr);
	}
}

template<typename T>
inline void ResourceManager<T>::releaseAll()
{
	mResources.clear();
}

template <typename T>
inline U32 ResourceManager<T>::count() const
{
	return mResources.size();
}

} // namespace en
