#pragma once

#include <functional>
#include <unordered_map>

#include <Enlivengine/System/PrimitiveTypes.hpp>

namespace en
{

using ResourceId = U32;

template <typename T>
class ResourceManager
{
	public:
		ResourceManager();
		~ResourceManager();

		ResourceId create(const char* name, std::function<void(T& r)> loader);
		bool has(const char* name) const;
		bool has(ResourceId resource) const;
		T& get(const char* name);
		T& get(ResourceId resource);
		void release(const char* name);
		void release(ResourceId resource);
		void releaseAll();
		U32 count() const;

	private:
		std::unordered_map<ResourceId, T> mResources;
};

} // namespace en

#include "ResourceManager.inl"
