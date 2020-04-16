#include <Enlivengine/Application/ResourceManager.hpp>

namespace en
{

namespace priv
{

ResourceID StringToResourceID(const std::string& str)
{
	return Hash::CRC32(str.c_str());
}

BaseResource::BaseResource()
	: mID(InvalidResourceID)
	, mLoaded(false)
	, mIdentifier()
	, mFilename()
{
}

bool BaseResource::IsLoaded() const
{
	return mLoaded;
}

bool BaseResource::IsFromFile() const
{
	return mFilename.size() > 0;
}

bool BaseResource::IsManaged() const
{
	return mID != InvalidResourceID;
}

ResourceID BaseResource::GetID() const
{
	return mID;
}

const std::string& BaseResource::GetIdentifier() const
{
	return mIdentifier;
}

const std::string& BaseResource::GetFilename() const
{
	return mFilename;
}

} // namespace priv

ResourceManager::ResourceManager()
{
}

bool ResourceManager::Has(const std::string& str) const
{
	return Has(priv::StringToResourceID(str));
}

bool ResourceManager::Has(ResourceID id) const
{
	return mResources.find(id) != mResources.end();
}

void ResourceManager::Release(const std::string& str)
{
	Release(priv::StringToResourceID(str));
}

void ResourceManager::Release(ResourceID id)
{
	const auto itr = mResources.find(id);
	if (itr != mResources.end())
	{
		mResources.erase(itr);
	}
}

void ResourceManager::ReleaseAll()
{
	mResources.clear();
}

U32 ResourceManager::Count() const
{
	return static_cast<U32>(mResources.size());
}

} // namespace en
