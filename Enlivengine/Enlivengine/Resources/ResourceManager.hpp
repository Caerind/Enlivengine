#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_RESOURCES

#include <functional>
#include <memory>
#include <unordered_map>
#include <string>

#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Utils/Singleton.hpp>
#include <Enlivengine/Utils/NonCopyable.hpp>
#include <Enlivengine/Utils/Array.hpp>

namespace en
{

using ResourceID = U32;
constexpr ResourceID InvalidResourceID = U32_Max;

enum class ResourceType
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

// FromFile resources need to know their filenames, since they might have dependencies
struct ResourceLoadInfo
{
	ResourceLoadInfo()
		: isFromFile(false)
		, infoString("")
	{
	}

	ResourceLoadInfo(bool pIsfromFile, const std::string& pInfoString)
		: isFromFile(pIsfromFile)
		, infoString(pInfoString)
	{
	}

	bool isFromFile;
	std::string infoString;
};

#ifdef ENLIVE_DEBUG
// Used for Serialization & ResourceBrowser
struct ResourceInfo
{
	ResourceInfo()
		: id(InvalidResourceID)
		, type(static_cast<U32>(ResourceType::Invalid))
		, identifier("")
		, loaded(false)
		, info()
	{
	}

	ResourceID id;
	U32 type;
	std::string identifier;
	bool loaded;
	ResourceLoadInfo info;

	bool IsFromFile() const { return info.isFromFile; }
	const std::string& GetFilename() const { enAssert(info.isFromFile); return info.infoString; }
};
#endif // ENLIVE_DEBUG

namespace priv
{

class BaseResource : private NonCopyable
{
public:
	BaseResource();
	virtual ~BaseResource() {};

	static U32 GetStaticResourceType();
	virtual U32 GetResourceType() const = 0;

	ResourceID GetID() const;
	bool IsLoaded() const;
	bool IsManaged() const;

	void SetLoaded(bool loaded);

	const ResourceLoadInfo& GetLoadInfo() const;
	void SetLoadInfo(const ResourceLoadInfo& info);
	bool IsFromFile() const;
	const std::string& GetFilename() const;

#ifdef ENLIVE_DEBUG
	const std::string& GetIdentifier() const;
	void InitFromResourceManager(ResourceID id, const std::string& identifier);
#else
	void InitFromResourceManager(ResourceID id);
#endif // ENLIVE_DEBUG

private:
	friend class ResourceManager;

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

private:
	ResourceID mID;
};

} // namepsace en

namespace en
{

template <typename T>
class Resource : public priv::BaseResource
{
public:
	using Ptr = ResourcePtr<T>;

public:
	Resource();
};

template <typename T>
class ResourceLoader
{
public:
	using LoaderFunc = std::function<bool(T&)>;

public:
	ResourceLoader(LoaderFunc loader);

	bool Load(T& resource) const;

private:
	LoaderFunc mFunc;
};

enum class ResourceKnownStrategy
{
	Reuse = 0,
	Reload,
	Null
};

// Only used internally by ResourceManager
struct ResourceIDType
{
	ResourceID id;
	U32 type;

	bool operator==(const ResourceIDType& other) const
	{
		return id == other.id && type == other.type;
	}

	bool operator!=(const ResourceIDType& other) const
	{
		return !operator==(other);
	}
};

} // namespace en

namespace std
{
	template <>
	struct hash<en::ResourceIDType>
	{
		static_assert(sizeof(std::size_t) == sizeof(en::U64));
		static_assert(sizeof(en::ResourceIDType) == sizeof(en::U64));

		std::size_t operator()(const en::ResourceIDType& r) const
		{
			return (static_cast<std::size_t>(r.type) << 32) + static_cast<std::size_t>(r.id);
		}
	};
} // namespace std

namespace en
{

class ResourceManager
{
	ENLIVE_SINGLETON(ResourceManager);

public:
	template <typename T> ResourcePtr<T> Create(const std::string& str, const ResourceLoader<T>& loader, ResourceKnownStrategy knownStrategy = ResourceKnownStrategy::Reuse);
	template <typename T> ResourcePtr<T> Create(ResourceID id, const ResourceLoader<T>& loader, ResourceKnownStrategy knownStrategy = ResourceKnownStrategy::Reuse);

	template <typename T> ResourcePtr<T> Get(const std::string& str);
	template <typename T> ResourcePtr<T> Get(ResourceID id);

    template <typename T> ResourcePtr<T> GetFromFilename(const std::string& filename);

	template <typename T> bool Has(const std::string& str) const;
	template <typename T> bool Has(ResourceID id) const;
	bool Has(ResourceID id, U32 resourceType) const;

	template <typename T> void Release(const std::string& str);
	template <typename T> void Release(ResourceID id);
	void Release(ResourceID id, U32 resourceType);

	void ReleaseAll();

	template <typename T = void> U32 Count() const;
	U32 Count(U32 resourceType) const;

	template <typename EnumClient>
	bool InitializeClientResourceTypes();

#ifdef ENLIVE_DEBUG
	void GetResourceInfosOfType(Array<ResourceInfo>& resourceInfos, U32 resourceType);
	template <typename T> void GetResourceInfosOfType(Array<ResourceInfo>& resourceInfos);
	void GetResourceInfos(Array<ResourceInfo>& resourceInfos);
	template <typename T> std::string_view GetResourceTypeName() const;
	std::string_view GetResourceTypeName(U32 resourceType) const;
#endif // ENLIVE_DEBUG

	template <typename T> static U32 GetResourceType();
	static ResourceID StringToResourceID(const std::string& str);

private:
	template <typename T> friend class ResourcePtr;
	template <typename T> T* GetRawPtr(ResourceID id);
	template <typename T> const T* GetRawPtr(ResourceID id) const;

private:
	template <typename T>
	static ResourceIDType CreateResourceIDTypeFromResourceID(ResourceID id);
	static ResourceIDType CreateResourceIDTypeFromResourceIDAndType(ResourceID id, U32 type);

private:
	std::unordered_map<ResourceIDType, std::unique_ptr<priv::BaseResource>> mResources;

#ifdef ENLIVE_DEBUG
	std::vector<std::string_view> mClientResourceTypeNames;
#endif // ENLIVE_DEBUG
};

} // namespace en

#include "ResourceManager.inl"

#endif // ENLIVE_MODULE_RESOURCES