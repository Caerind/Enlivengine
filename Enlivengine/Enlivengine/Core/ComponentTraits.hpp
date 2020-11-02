#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_CORE

// TODO : Namespace en::Traits ?

template <typename T>
struct ComponentHasCustomInitialization
{
	static constexpr bool value = false;
};

#endif // ENLIVE_MODULE_CORE