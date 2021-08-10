#pragma once

// TODO : Namespace en::Traits ?

template <typename T>
struct ComponentHasCustomInitialization
{
	static constexpr bool value = false;
};

#define ENLIVE_COMPONENT_CUSTOM_INITIALIZATION(componentName) template <> \
	struct ComponentHasCustomInitialization<componentName> \
	{ \
		static constexpr bool value = true; \
	};
