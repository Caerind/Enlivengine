#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <string>

#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Utils/Singleton.hpp>
#include <Enlivengine/Utils/Array.hpp>

#include <Enlivengine/Resources/Resource.hpp>

namespace en
{

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

namespace priv
{

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

} // namespace priv

} // namespace en

namespace std
{
	template <>
	struct hash<en::priv::ResourceIDType>
	{
		static_assert(sizeof(std::size_t) == sizeof(en::U64));
		static_assert(sizeof(en::priv::ResourceIDType) == sizeof(en::U64));

		std::size_t operator()(const en::priv::ResourceIDType& r) const
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
	template <typename T = void> void GetResourceInfosOfType(Array<ResourceInfo>& resourceInfos);
	void GetResourceInfosOfType(Array<ResourceInfo>& resourceInfos, U32 resourceType);
	void GetResourceInfos(Array<ResourceInfo>& resourceInfos);
	template <typename T> std::string_view GetResourceTypeName() const;
	std::string_view GetResourceTypeName(U32 resourceType) const;
	U32 GetResourceTypeCount() const;
#endif // ENLIVE_DEBUG

	static ResourceID StringToResourceID(const std::string& str);

private:
	template <typename T> friend class ResourcePtr;
	template <typename T> T* GetRawPtr(ResourceID id);
	template <typename T> const T* GetRawPtr(ResourceID id) const;

private:
	template <typename T>
	static priv::ResourceIDType CreateResourceIDTypeFromResourceID(ResourceID id);
	static priv::ResourceIDType CreateResourceIDTypeFromResourceIDAndType(ResourceID id, U32 type);

private:
	std::unordered_map<priv::ResourceIDType, std::unique_ptr<priv::BaseResource>> mResources;

#ifdef ENLIVE_DEBUG
	std::vector<std::string_view> mClientResourceTypeNames;
#endif // ENLIVE_DEBUG
};

} // namespace en

#include "ResourceManager.inl"