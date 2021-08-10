#include <Enlivengine/Core/SystemFactory.hpp>

namespace en
{

std::unordered_map<U32, SystemFactory::SystemInfo> SystemFactory::mSystems;

bool SystemFactory::IsRegistered(U32 systemHash)
{
	return mSystems.find(systemHash) != mSystems.end();
}

const std::unordered_map<U32, SystemFactory::SystemInfo>& SystemFactory::GetSystemInfos()
{
	return mSystems;
}

} // namespace en