#pragma once

#include <type_traits>
#include <Enlivengine/System/PrimitiveTypes.hpp>

namespace en
{

// All std::type_traits are in this namespace (traits specific to the engine are not)
// "Recreate" this might be a bad idea, but also might reveal useful for specific traits or override in the future
namespace Traits
{
#define ENLIVE_DEFINE_TYPE_TRAITS_VALUE(name, expr) \
	template <typename T> \
	struct name \
	{ \
		static constexpr auto value = expr; \
	};
#define ENLIVE_DEFINE_TYPE_TRAITS_ARGS(name, expr) \
	template <typename T, typename... Args> \
	struct name \
	{ \
		static constexpr auto value = expr; \
	};
#define ENLIVE_DEFINE_TYPE_TRAITS_TYPE(name, expr) \
	template <typename T> \
	struct name \
	{ \
		using type = typename expr; \
	};

	// Primary type categories
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsVoid, std::is_void<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsNullPointer, std::is_null_pointer<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsIntegral, std::is_integral<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsFloatingPoint, std::is_floating_point<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsArray, std::is_array<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsEnum, std::is_enum<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsUnion, std::is_union<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsClass, std::is_class<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsFunction, std::is_function<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsPointer, std::is_pointer<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsLValueRef, std::is_lvalue_reference<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsRValueRef, std::is_rvalue_reference<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsMemberObjectPointer, std::is_member_object_pointer<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsMemberFunctionPointer, std::is_member_function_pointer<T>::value)

	// Composite type categories
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsFundamental, std::is_fundamental<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsArithmetic, std::is_arithmetic<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsScalar, std::is_scalar<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsObject, std::is_object<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsCompound, std::is_compound<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsReference, std::is_reference<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsMemberPointer, std::is_member_pointer<T>::value)

	// Type properties
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsConst, std::is_const<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsVolatile, std::is_volatile<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsTrivial, std::is_trivial<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsTriviallyCopyable, std::is_trivially_copyable<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsStandardLayout, std::is_standard_layout<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsPOD, std::is_pod<T>::value) // Deprecated C++20
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsLiteralType, std::is_literal_type<T>::value) // Deprecated C++17, Removed C++20
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(HasUniqueObjectRepresentations, std::has_unique_object_representations<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsEmpty, std::is_empty<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsPolymorphic, std::is_polymorphic<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsAbstract, std::is_abstract<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsFinal, std::is_final<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsAggregate, std::is_aggregate<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsSigned, std::is_signed<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsUnsigned, std::is_unsigned<T>::value)
	//ENLIVE_DEFINE_TYPE_TRAITS(IsBoundedArray, std::is_union<T>::value) // New C++20
	//ENLIVE_DEFINE_TYPE_TRAITS(IsUnboundedArray, std::is_union<T>::value) // New C++20

	// Supported operations
	// TODO : Args...
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsConstructible, std::is_constructible<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsTriviallyConstructible, std::is_trivially_constructible<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsNothrowConstructible, std::is_nothrow_constructible<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsDefaultConstructible, std::is_default_constructible<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsTriviallyDefaultConstructible, std::is_trivially_default_constructible<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsNothrowDefaultConstructible, std::is_nothrow_default_constructible<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsCopyConstructible, std::is_copy_constructible<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsTriviallyCopyConstructible, std::is_trivially_copy_constructible<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsNothrowCopyConstructible, std::is_nothrow_copy_constructible<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsMoveConstructible, std::is_move_constructible<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsTriviallyMoveConstructible, std::is_trivially_move_constructible<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsNothrowMoveConstructible, std::is_nothrow_move_constructible<T>::value)
	template <typename T1, typename T2>
	struct IsAssignable
	{
		static constexpr auto value = std::is_assignable<T1, T2>::value;
	};
	template <typename T1, typename T2>
	struct IsTriviallyAssignable
	{
		static constexpr auto value = std::is_trivially_assignable<T1, T2>::value;
	};
	template <typename T1, typename T2>
	struct IsNothrowAssignable
	{
		static constexpr auto value = std::is_nothrow_assignable<T1, T2>::value;
	};
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsCopyAssignable, std::is_copy_assignable<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsTriviallyCopyAssignable, std::is_trivially_copy_assignable<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsNothrowCopyAssignable, std::is_nothrow_copy_assignable<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsMoveAssignable, std::is_move_assignable<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsTriviallyMoveAssignable, std::is_trivially_move_assignable<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsNothrowMoveAssignable, std::is_nothrow_move_assignable<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsDestructible, std::is_destructible<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsTriviallyDestructible, std::is_trivially_destructible<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsNothrowDestructible, std::is_nothrow_destructible<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(HasVirtualDestructor, std::has_virtual_destructor<T>::value)
	template <typename T1, typename T2>
	struct IsSwappableWith
	{
		static constexpr auto value = std::is_swappable_with<T1, T2>::value;
	};
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsSwappable, std::is_swappable<T>::value)
	template <typename T1, typename T2>
	struct IsNothrowSwappableWith
	{
		static constexpr auto value = std::is_nothrow_swappable_with<T1, T2>::value;
	};
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(IsNothrowSwappable, std::is_nothrow_swappable<T>::value)

	// Properties queries
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(AlignementOf, std::alignment_of<T>::value)
	ENLIVE_DEFINE_TYPE_TRAITS_VALUE(Rank, std::rank<T>::value)
	template <typename T, unsigned N = 0>
	struct Extent
	{
		static constexpr auto value = std::extent<T, N>::value;
	};

	// Type relationships
	template <typename T1, typename T2>
	struct IsSame
	{
		static constexpr auto value = std::is_same<T1, T2>::value;
	};
	template <typename Base, typename Derived>
	struct IsBaseOf
	{
		static constexpr auto value = std::is_base_of<Base, Derived>::value;
	};
	template <typename From, typename To>
	struct IsConvertible
	{
		static constexpr auto value = std::is_convertible<From, To>::value;
	};
	//ENLIVE_DEFINE_TYPE_TRAITS_VALUE_2(IsNothrowConvertible, (std::is_nothrow_convertible<T1, T2>::value)) // T1:From, T2:To // New C++20
	//ENLIVE_DEFINE_TYPE_TRAITS_2(IsLayoutCompatible, std::is_layout_compatible<T1, T2>::value) // New C++20
	//ENLIVE_DEFINE_TYPE_TRAITS_2(IsPointerInterconvertibleBaseOf, std::is_pointer_interconvertible_base_of<T1, T2>::value) // New C++20 // T1:Base, T2:Derived
	template <typename Fn, typename... Args>
	struct IsInvocable
	{
		static constexpr auto value = std::is_invocable<Fn, Args...>::value;
	};
	template <typename R, typename Fn, typename... Args>
	struct IsInvocableR
	{
		static constexpr auto value = std::is_invocable_r<R, Fn, Args...>::value;
	};
	template <typename Fn, typename... Args>
	struct IsNothrowInvocable
	{
		static constexpr auto value = std::is_nothrow_invocable<Fn, Args...>::value;
	};
	template <typename R, typename Fn, typename... Args>
	struct IsNothrowInvocableR
	{
		static constexpr auto value = std::is_nothrow_invocable_r<R, Fn, Args...>::value;
	};

	// Const-volatility specifiers
	ENLIVE_DEFINE_TYPE_TRAITS_TYPE(RemoveCv, std::remove_cv<T>::type)
	ENLIVE_DEFINE_TYPE_TRAITS_TYPE(RemoveConst, std::remove_const<T>::type)
	ENLIVE_DEFINE_TYPE_TRAITS_TYPE(RemoveVolatile, std::remove_volatile<T>::type)
	ENLIVE_DEFINE_TYPE_TRAITS_TYPE(AddCv, std::add_cv<T>::type)
	ENLIVE_DEFINE_TYPE_TRAITS_TYPE(AddConst, std::add_const<T>::type)
	ENLIVE_DEFINE_TYPE_TRAITS_TYPE(AddVolatile, std::add_volatile<T>::type)

	// References
	ENLIVE_DEFINE_TYPE_TRAITS_TYPE(RemoveReference, std::remove_reference<T>::type)
	ENLIVE_DEFINE_TYPE_TRAITS_TYPE(AddLValueReference, std::add_lvalue_reference<T>::type)
	ENLIVE_DEFINE_TYPE_TRAITS_TYPE(AddRValueReference, std::add_rvalue_reference<T>::type)

	// Pointers
	ENLIVE_DEFINE_TYPE_TRAITS_TYPE(RemovePointer, std::remove_pointer<T>::type)
	ENLIVE_DEFINE_TYPE_TRAITS_TYPE(AddPointer, std::add_pointer<T>::type)

	// Sign modifiers
	ENLIVE_DEFINE_TYPE_TRAITS_TYPE(MakeSigned, std::make_signed<T>::type)
	ENLIVE_DEFINE_TYPE_TRAITS_TYPE(MakeUnsigned, std::make_unsigned<T>::type)

	// Arrays
	ENLIVE_DEFINE_TYPE_TRAITS_TYPE(RemoveExtent, std::remove_extent<T>::type)
	ENLIVE_DEFINE_TYPE_TRAITS_TYPE(RemoveAllExtents, std::remove_all_extents<T>::type)

	// Miscellaneous transformations
	// TODO : aligned_storage
	// TODO : aligned_union
	ENLIVE_DEFINE_TYPE_TRAITS_TYPE(Decay, std::decay<T>::type)
	//ENLIVE_DEFINE_TYPE_TRAITS_TYPE(RemoveCvRef, std::remove_cvref<T>::type) // New C++20
	template <bool B, typename T = void> 
	struct EnableIf 
	{
	};
	template <typename T> 
	struct EnableIf<true, T> 
	{
		using type = T;
	};
	template <bool B, typename T, typename F>
	struct Conditional 
	{
		using type = T;
	};
	template <typename T, typename F>
	struct Conditional<false, T, F>
	{
		using type = F;
	}; 
	template <typename... Args>
	struct CommonType
	{
		using type = typename std::common_type<Args...>::type;
	};
	// ResultOf ?
	ENLIVE_DEFINE_TYPE_TRAITS_TYPE(UnderlyingType, std::underlying_type<T>::type)
	template <typename F, typename... Args>
	struct InvokeResult
	{
		using type = typename std::invoke_result<F, Args...>::type;
	};
	// VoidT ?
	//ENLIVE_DEFINE_TYPE_TRAITS_TYPE(TypeIdentity, std::type_identity<T>::type); // New C++20

	// Operations on traits
	template <typename... B>
	struct Conjunction
	{
		static constexpr auto value = std::conjunction<B...>::value;
	};
	template <typename... B>
	struct Disjunction
	{
		static constexpr auto value = std::disjunction<B...>::value;
	};
	template <typename B>
	struct Negation
	{
		static constexpr auto value = std::negation<B>::value;
	};

	// Member relationships
	// is_pointer_interconvertible_with_class : New C++20
	// is_corresponding_member : New C++20

	// Constant evaluation context
	// is_constant_evaluated : New C++20

#undef ENLIVE_DEFINE_TYPE_TRAITS_VALUE
#undef ENLIVE_DEFINE_TYPE_TRAITS_VALUE_2
#undef ENLIVE_DEFINE_TYPE_TRAITS_ARGS
#undef ENLIVE_DEFINE_TYPE_TRAITS_TYPE

} // namespace Traits

} // namespace en
