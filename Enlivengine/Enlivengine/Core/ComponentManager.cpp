#include <Enlivengine/Core/ComponentManager.hpp>

#ifdef ENLIVE_MODULE_CORE

namespace en
{

std::unordered_map<U32, ComponentManager::ComponentInfo> ComponentManager::mComponents;

} // namespace en

#endif // ENLIVE_MODULE_CORE