#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>

#if defined(ENLIVE_ARRAY_SIZE)
#undef ENLIVE_ARRAY_SIZE
#endif
#define ENLIVE_ARRAY_SIZE(arr) static_cast<en::U32>(sizeof(arr)/sizeof(arr[0]))

#if defined(ENLIVE_SIZE_OF)
#undef ENLIVE_SIZE_OF
#endif
#define ENLIVE_SIZE_OF(type) static_cast<en::U32>(sizeof(type))

#if defined(ENLIVE_ALIGN_OF)
#undef ENLIVE_ALIGN_OF
#endif
#define ENLIVE_ALIGN_OF(type) static_cast<en::U32>(alignof(type))

#if defined(ENLIVE_OFFSET_OF)
#undef ENLIVE_OFFSET_OF
#endif
#define ENLIVE_OFFSET_OF(type, member) static_cast<en::U32>(offsetof(type, member))

#if defined(ENLIVE_SIZE_OF_MEMBER)
#undef ENLIVE_SIZE_OF_MEMBER
#endif
#define ENLIVE_SIZE_OF_MEMBER(type, member) static_cast<en::U32>(en::GetSizeOfMember(&type::member))
namespace en
{
	template <typename T, typename MemberType>
	constexpr std::size_t GetSizeOfMember(MemberType T::*)
	{
		return sizeof(MemberType);
	}
} // namespace en