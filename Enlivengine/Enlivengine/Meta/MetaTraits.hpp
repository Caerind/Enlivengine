#pragma once

#include <Enlivengine/Config.hpp>

// TODO : Namespace en::Traits ?

template <typename T>
struct HasCustomSerialization
{
	static constexpr bool value = false;
};

#ifdef ENLIVE_ENABLE_IMGUI
template <typename T>
struct HasCustomEditor
{
	static constexpr bool value = false;
};
#endif // ENLIVE_ENABLE_IMGUI