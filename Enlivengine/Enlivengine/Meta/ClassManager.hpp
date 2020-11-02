#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_META

#include <functional>
#include <iterator>
#include <unordered_map>

#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Utils/Hash.hpp>
#include <Enlivengine/Utils/TypeTraits.hpp>
#include <Enlivengine/Utils/MemoryAllocator.hpp>
#include <Enlivengine/Meta/TypeInfo.hpp>
#include <Enlivengine/Meta/Meta.hpp>

namespace en
{

class ClassManager
{
public:
	ClassManager() = delete;

	static U32 GetRegisteredClassCount()
	{
		return static_cast<U32>(mClasses.size());
	}

	static const char* GetClassNameAtIndex(U32 index)
	{
		auto itr = mClasses.begin();
		std::advance(itr, index);
		if (itr != mClasses.end())
		{
			return itr->second.name;
		}
		enAssert(false);
		return nullptr;
	}

	static U32 GetClassHashAtIndex(U32 index)
	{
		auto itr = mClasses.begin();
		std::advance(itr, index);
		if (itr != mClasses.end())
		{
			return itr->first;
		}
		enAssert(false);
		return U32_Max;
	}

	static const char* GetClassNameFromHash(U32 classHash)
	{
		const auto itr = mClasses.find(classHash);
		if (itr != mClasses.end())
		{
			return itr->second.name;
		}
		else
		{
			return nullptr;
		}
	}

	static void* CreateClassFromHash(U32 classHash)
	{
		const auto itr = mClasses.find(classHash);
		if (itr != mClasses.end())
		{
			return itr->second.factory();
		}
		else
		{
			return nullptr;
		}
	}

	static bool IsRegistered(U32 classHash)
	{
		return mClasses.find(classHash) != mClasses.end();
	}

	template <typename T>
	static bool IsRegistered()
	{
		return IsRegistered(TypeInfo<T>::GetHash());
	}

	template <typename T>
	static bool Register()
	{
		static_assert(Meta::IsRegistered<T>());
		static_assert(TypeInfo<T>::IsKnown());
		constexpr U32 hash = TypeInfo<T>::GetHash();
		mClasses[hash].name = TypeInfo<T>::GetName();
		mClasses[hash].factory = []()
		{
			return (void*)enNew(T, TypeInfo<T>::GetName());
		};
		return true;
	}

private:
	struct ClassInfo
	{
		const char* name;
		std::function<void* ()> factory;
	};
	static std::unordered_map<U32, ClassInfo> mClasses;
};

} // namespace en

#endif // ENLIVE_MODULE_META