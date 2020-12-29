#include <Enlivengine/Resources/ResourceManager.hpp>

#include <Enlivengine/Utils/Enums.hpp>

namespace en
{

ResourceLoadInfo::ResourceLoadInfo(Method pMethod, const std::string& pInfoString)
	: method(pMethod)
	, infoString(pInfoString)
{
}

bool ResourceLoadInfo::IsFromFile() const 
{ 
	return method == Method::File; 
}

#ifdef ENLIVE_DEBUG
ResourceInfo::ResourceInfo()
	: loadInfo()
	, id(InvalidResourceID)
	, type(static_cast<U32>(ResourceType::Invalid))
	, identifier("")
	, loaded(false)
{
}
#endif // ENLIVE_DEBUG

namespace priv
{

BaseResource::BaseResource()
	: mID(InvalidResourceID)
	, mLoadInfo()
	, mLoaded(false)
#ifdef ENLIVE_DEBUG
	, mIdentifier()
#endif // ENLIVE_DEBUG
{
}

BaseResource::~BaseResource() 
{
}

U32 BaseResource::GetStaticResourceType()
{
	return static_cast<U32>(ResourceType::Invalid);
}

ResourceID BaseResource::GetID() const
{
	return mID;
}

bool BaseResource::IsLoaded() const
{
	return mLoaded;
}

bool BaseResource::IsManaged() const
{
	return mID != InvalidResourceID;
}

void BaseResource::SetLoaded(bool loaded)
{
	mLoaded = loaded;
}

const ResourceLoadInfo& BaseResource::GetLoadInfo() const
{
	return mLoadInfo;
}

void BaseResource::SetLoadInfo(const ResourceLoadInfo& info)
{
	mLoadInfo = info;
}

#ifdef ENLIVE_DEBUG
ResourceInfo BaseResource::GetResourceInfo() const
{
	ResourceInfo ri;
	ri.id = GetID();
	ri.type = GetResourceType();
	ri.identifier = GetIdentifier();
	ri.loaded = IsLoaded();
	ri.loadInfo = GetLoadInfo();
	return ri;
}

const std::string& BaseResource::GetIdentifier() const
{
	return mIdentifier;
}
#endif // ENLIVE_DEBUG

#ifdef ENLIVE_DEBUG
void BaseResource::InitFromResourceManager(ResourceID id, const std::string& identifier)
{
	mID = id;
	mIdentifier = identifier;
}
#else
void BaseResource::InitFromResourceManager(ResourceID id)
{
	mID = id;
}
#endif // ENLIVE_DEBUG

} // namespace priv

} // namespace en