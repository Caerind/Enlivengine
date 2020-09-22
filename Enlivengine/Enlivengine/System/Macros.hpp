#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>

#define ENLIVE_ARRAY_SIZE(arr) static_cast<en::U32>(sizeof(arr)/sizeof(arr[0]))

#define ENLIVE_SIZE_OF(type) static_cast<en::U32>(sizeof(type))

#define ENLIVE_ALIGN_OF(type) static_cast<en::U32>(alignof(type))

#define ENLIVE_OFFSET_OF(type, member) static_cast<en::U32>(offsetof(type, member))

#define ENLIVE_SIZE_OF_MEMBER(type, member) static_cast<en::U32>(en::priv::GetSizeOfMember(&type::member))
namespace en
{
	namespace priv
	{
		template <typename T, typename MemberType>
		constexpr std::size_t GetSizeOfMember(MemberType T::*)
		{
			return sizeof(MemberType);
		}
	} // namespace priv
} // namespace en