#pragma once

#include <functional>
#include <unordered_map>

#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Utils/MemoryAllocator.hpp>
#include <Enlivengine/Utils/TypeInfo.hpp>

namespace en
{

class ClassFactory
{
public:
	ClassFactory() = delete;

	static U32 GetRegisteredClassCount();

	static U32 GetClassHashAtIndex(U32 index);
	static const char* GetClassNameAtIndex(U32 index);
	static const char* GetClassNameFromHash(U32 classHash);

	static void* CreateClassFromHash(U32 classHash);

	template <typename T>
	static bool IsRegistered();
	static bool IsRegistered(U32 classHash);

	template <typename T>
	static bool Register();

private:
	struct ClassInfo
	{
		const char* name;
		std::function<void* ()> factory;
	};
	static std::unordered_map<U32, ClassInfo> mClasses;
}; 

template <typename T>
bool ClassFactory::IsRegistered()
{
	return IsRegistered(TypeInfo<T>::GetHash());
}

template <typename T>
bool ClassFactory::Register()
{
	static_assert(TypeInfo<T>::IsKnown());
	constexpr U32 hash = TypeInfo<T>::GetHash();
	mClasses[hash].name = TypeInfo<T>::GetName();
	mClasses[hash].factory = []()
	{
		return (void*)enNew(T, TypeInfo<T>::GetName());
	};
	return true;
}

} // namespace en