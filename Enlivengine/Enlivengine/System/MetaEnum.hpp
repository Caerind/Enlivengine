#pragma once

#include <magic_enum/magic_enum.hpp>

#include <Enlivengine/System/PrimitiveTypes.hpp>

namespace en
{

namespace Meta
{

static_assert(magic_enum::is_magic_enum_supported);

template <typename E> 
constexpr std::string_view GetEnumTypeName() 
{ 
	return magic_enum::enum_type_name<E>(); 
}

template <typename E> 
constexpr std::string_view GetEnumName(E value) 
{ 
	return magic_enum::enum_name(value); 
}

// Return type is std::array<std::string_view, N> where N is "undefined"/auto detected at compile-time depending on E
template <typename E> 
constexpr auto GetEnumNames() 
{ 
	return magic_enum::enum_names<E>();
}

template <typename E>
constexpr auto GetEnumValues()
{
	return magic_enum::enum_values<E>();
}

template <typename E, typename T> 
constexpr E EnumCast(T value) 
{ 
	return magic_enum::enum_cast<E>(value).value();
}

template <typename E> 
constexpr en::U32 GetEnumCount()
{ 
	return static_cast<en::U32>(magic_enum::enum_count<E>());
}

template <typename E> 
constexpr en::U32 GetEnumIndex(E value) 
{ 
	return static_cast<en::U32>(magic_enum::enum_index(value).value());
}

template <typename E> 
constexpr E GetEnumFromIndex(en::U32 index) 
{ 
	return magic_enum::enum_value<E>(static_cast<std::size_t>(index)); 
}

} // namespace Meta

} // namespace en
