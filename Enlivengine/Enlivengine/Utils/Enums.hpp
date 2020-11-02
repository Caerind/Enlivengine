#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_UTILS

#include <magic_enum/magic_enum.hpp>

#include <Enlivengine/Platform/PrimitiveTypes.hpp>

namespace en
{

namespace Enum
{

static_assert(magic_enum::is_magic_enum_supported);

template <typename E> 
constexpr std::string_view GetTypeName() 
{ 
	return magic_enum::enum_type_name<E>(); 
}

template <typename E> 
constexpr std::string_view GetValueName(E value) 
{ 
	return magic_enum::enum_name(value); 
}

// Return type is std::array<std::string_view, N> where N is auto detected at compile-time depending on E
template <typename E> 
constexpr auto GetValueNames() 
{ 
	return magic_enum::enum_names<E>();
}

template <typename E>
constexpr auto GetValues()
{
	return magic_enum::enum_values<E>();
}

template <typename E, typename T> 
constexpr E Cast(T value) 
{ 
	return magic_enum::enum_cast<E>(value).value();
}

template <typename E> 
constexpr en::U32 GetCount()
{ 
	return static_cast<en::U32>(magic_enum::enum_count<E>());
}

template <typename E> 
constexpr en::U32 GetIndex(E value) 
{ 
	return static_cast<en::U32>(magic_enum::enum_index(value).value());
}

template <typename E> 
constexpr E GetFromIndex(en::U32 index) 
{ 
	return magic_enum::enum_value<E>(static_cast<std::size_t>(index)); 
}

} // namespace Enum

} // namespace en

#endif // ENLIVE_MODULE_UTILS