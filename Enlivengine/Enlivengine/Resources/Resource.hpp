#pragma once

#include <string>

#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Utils/NonCopyable.hpp>
#include <Enlivengine/Utils/TypeInfo.hpp>
#include <Enlivengine/Utils/Meta.hpp>

namespace en
{

using ResourceID = U32;
constexpr ResourceID InvalidResourceID = U32_Max;

// ResourceType is always used as U32 in order to allow user defined ResourceTypes
// Have your own enum start as Max
enum class ResourceType : U32
{
	Invalid = 0,
	Image,
	Texture,
	Font,
	Music,
	Sound,
	Tileset,
	Map,
	Animation,
	AnimationStateMachine,
	Shader,

	// TODO : Add others

	Max
};

struct ResourceLoadInfo
{
	enum Method
	{
		Unknown,
		File,
		Download,
		Memory,
		Procedural
	};

	ResourceLoadInfo(Method pMethod = Method::Unknown, const std::string& pInfoString = "");

	bool IsFromFile() const ;

	Method method;
	std::string infoString;
};

#ifdef ENLIVE_DEBUG
struct ResourceInfo
{
	ResourceInfo();

	ResourceLoadInfo loadInfo;
	ResourceID id;
	U32 type;
	std::string identifier;
	bool loaded;
};
#endif // ENLIVE_DEBUG

class ResourceManager;

namespace priv
{

class BaseResource : private NonCopyable
{
public:
	BaseResource();
	virtual ~BaseResource();

	static U32 GetStaticResourceType();
	virtual U32 GetResourceType() const = 0;

	ResourceID GetID() const;
	bool IsLoaded() const;
	bool IsManaged() const;

	void SetLoaded(bool loaded);

	const ResourceLoadInfo& GetLoadInfo() const;
	void SetLoadInfo(const ResourceLoadInfo& info);

#ifdef ENLIVE_DEBUG
	virtual ResourceInfo GetResourceInfo() const;

	const std::string& GetIdentifier() const;
#endif // ENLIVE_DEBUG

private:
	friend class ::en::ResourceManager;
#ifdef ENLIVE_DEBUG
	void InitFromResourceManager(ResourceID id, const std::string& identifier);
#else
	void InitFromResourceManager(ResourceID id);
#endif // ENLIVE_DEBUG

	ResourceID mID;
	ResourceLoadInfo mLoadInfo;
	bool mLoaded;

#ifdef ENLIVE_DEBUG
	std::string mIdentifier;
#endif // ENLIVE_DEBUG
};

} // namespace priv

template <typename T>
class ResourcePtr
{
public:
	ResourcePtr(ResourceID id = InvalidResourceID);

	ResourceID GetID() const;
	U32 GetResourceType() const;

	bool HasValidID() const;
	bool IsValid() const;
	operator bool() const;
	void Release();

	T* GetPtr() const;
	T& Get() const;

	void ReleaseFromManager();
	
	bool operator==(const ResourcePtr<T>& other) const;
	bool operator!=(const ResourcePtr<T>& other) const;

private:
	ResourceID mID;
};

template <typename T>
class Resource : public priv::BaseResource
{
public:
	using Ptr = ResourcePtr<T>;

public:
	Resource();
};

} // namespace en

ENLIVE_DEFINE_TYPE_INFO(en::ResourceLoadInfo::Method)

ENLIVE_META_CLASS_BEGIN(en::ResourceLoadInfo)
	ENLIVE_META_CLASS_MEMBER("method", &en::ResourceLoadInfo::method),
	ENLIVE_META_CLASS_MEMBER("infoString", &en::ResourceLoadInfo::infoString)
ENLIVE_META_CLASS_END()

#ifdef ENLIVE_DEBUG
ENLIVE_META_CLASS_BEGIN(en::ResourceInfo)
	ENLIVE_META_CLASS_MEMBER("id", &en::ResourceInfo::id),
	ENLIVE_META_CLASS_MEMBER("type", &en::ResourceInfo::type),
	ENLIVE_META_CLASS_MEMBER("identifier", &en::ResourceInfo::identifier),
	ENLIVE_META_CLASS_MEMBER("loadInfo", &en::ResourceInfo::loadInfo),
	ENLIVE_META_CLASS_MEMBER("loaded", &en::ResourceInfo::loaded)
ENLIVE_META_CLASS_END()
#endif // ENLIVE_DEBUG

ENLIVE_DEFINE_TYPE_INFO_TEMPLATE(en::ResourcePtr)

#include "Resource.inl"