#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/System/Singleton.hpp>
#include <Enlivengine/System/NonCopyable.hpp>

namespace en
{

using ResourceID = U32;
constexpr ResourceID InvalidResourceID = U32_Max;

namespace priv
{

ResourceID StringToResourceID(const std::string& str);

class BaseResource : private NonCopyable
{
public:
	BaseResource();

	bool IsLoaded() const;
	bool IsFromFile() const;
	bool IsManaged() const;
	ResourceID GetID() const;
	const std::string& GetIdentifier() const;
	const std::string& GetFilename() const;

public:
	ResourceID mID;
	bool mLoaded;
	std::string mIdentifier;
	std::string mFilename;
};

} // namespace priv

class ResourceManager;

template <typename T>
class ResourcePtr
{
public:
	ResourcePtr(ResourceID id = InvalidResourceID, ResourceManager* mgr = nullptr);

	ResourceID GetID() const;
	bool IsValid() const;
	operator bool() const;
	void Release();

	T* GetPtr() const;
	T& Get() const;

	void ReleaseFromManager();

private:
	ResourceID mID;
	ResourceManager* mManager;
};

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

class ResourceHolder;

class ResourceManager
{
	ENLIVE_SINGLETON(ResourceManager);

public:
	template <typename T> ResourcePtr<T> Create(const std::string& str, const ResourceLoader<T>& loader, ResourceKnownStrategy knownStrategy = ResourceKnownStrategy::Reuse);
	template <typename T> ResourcePtr<T> Get(const std::string& str);
	template <typename T> ResourcePtr<T> Get(ResourceID id);

    template <typename T> ResourcePtr<T> GetFromFilename(const std::string& filename);

	bool Has(const std::string& str) const;
	bool Has(ResourceID id) const;

	void Release(const std::string& str);
	void Release(ResourceID id);

	void ReleaseAll();

	U32 Count() const;

private:
	template <typename T> friend class ResourcePtr;
	template <typename T> T* GetRawPtr(ResourceID id);
	template <typename T> const T* GetRawPtr(ResourceID id) const;

private:
	std::unordered_map<ResourceID, std::unique_ptr<priv::BaseResource>> mResources;
};

} // namespace en

#include "ResourceManager.inl"
