#include <Enlivengine/Utils/ClassFactory.hpp>

#include <iterator>

namespace en
{

std::unordered_map<U32, ClassFactory::ClassInfo> ClassFactory::mClasses;

U32 ClassFactory::GetRegisteredClassCount()
{
	return static_cast<U32>(mClasses.size());
}

U32 ClassFactory::GetClassHashAtIndex(U32 index)
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

const char* ClassFactory::GetClassNameAtIndex(U32 index)
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

const char* ClassFactory::GetClassNameFromHash(U32 classHash)
{
	const auto itr = mClasses.find(classHash);
	if (itr != mClasses.end())
	{
		return itr->second.name;
	}
	else
	{
		return "<Unknown>";
	}
}

void* ClassFactory::CreateClassFromHash(U32 classHash)
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

bool ClassFactory::IsRegistered(U32 classHash)
{
	return mClasses.find(classHash) != mClasses.end();
}

} // namespace en