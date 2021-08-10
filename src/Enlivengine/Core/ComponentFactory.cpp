#include <Enlivengine/Core/ComponentFactory.hpp>

namespace en
{

std::unordered_map<U32, ComponentFactory::ComponentInfo> ComponentFactory::mComponents;

bool ComponentFactory::IsRegistered(U32 componentHash)
{
	return mComponents.find(componentHash) != mComponents.end();
}

const std::unordered_map<U32, ComponentFactory::ComponentInfo>& ComponentFactory::GetComponentInfos()
{
	return mComponents;
}

} // namespace en