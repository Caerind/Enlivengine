#pragma once

// TODO : Namespace en::Traits ?

template <typename T>
struct ComponentHasCustomInitialization
{
	static constexpr bool value = false;
};