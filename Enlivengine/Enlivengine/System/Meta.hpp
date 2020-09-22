#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/System/Hash.hpp>
#include <Enlivengine/System/TypeTraits.hpp>
#include <Enlivengine/System/TypeInfo.hpp>

namespace en::Meta
{

// A large part of this is based on the awesome work of EliasDaler in https://github.com/eliasdaler/MetaStuff
// I modified it to fit my needs and use C++17
// Plus the different other usage I needed for serialization and such

static constexpr en::U32 Attribute_None = 0;
static constexpr en::U32 Attribute_NoEditor = 1 << 0;
static constexpr en::U32 Attribute_NoSerialization = 1 << 1;

template <typename Class, typename T>
using MemberTypeT = T Class::*;

template <typename Class, typename T>
using ConstRefGetterFuncPtrT = const T& (Class::*)() const;

template <typename Class, typename T>
using ConstRefSetterFuncPtrT = void (Class::*)(const T&);

template <typename Class, typename T>
using NonConstRefGetterFuncPtrT = T& (Class::*)();

template <typename Class, typename T>
using CopyGetterFuncPtrT = T(Class::*)() const;

template <typename Class, typename T>
using CopySetterFuncPtrT = void (Class::*)(T);

template <typename Class, typename T>
class Member
{
public:
	using ClassType = Class;
	using Type = T;

	constexpr Member(const char* name, MemberTypeT<Class, T> ptr, U32 attributes = 0)
		: mName(name)
		, mAttributes(attributes)
		, mHasMemberPtr(true)
		, mMemberPtr(ptr)
		, mConstRefGetter(nullptr)
		, mNonConstRefGetter(nullptr)
		, mCopyGetter(nullptr)
		, mConstRefSetter(nullptr)
		, mCopySetter(nullptr)
	{
	}
	constexpr Member(const char* name, NonConstRefGetterFuncPtrT<Class, T> getter, U32 attributes = 0)
		: mName(name)
		, mAttributes(attributes)
		, mHasMemberPtr(false)
		, mMemberPtr(nullptr)
		, mConstRefGetter(nullptr)
		, mNonConstRefGetter(getter)
		, mCopyGetter(nullptr)
		, mConstRefSetter(nullptr)
		, mCopySetter(nullptr)
	{
	}
	constexpr Member(const char* name, ConstRefGetterFuncPtrT<Class, T> getter, ConstRefSetterFuncPtrT<Class, T> setter, U32 attributes = 0)
		: mName(name)
		, mAttributes(attributes)
		, mHasMemberPtr(false)
		, mMemberPtr(nullptr)
		, mConstRefGetter(getter)
		, mNonConstRefGetter(nullptr)
		, mCopyGetter(nullptr)
		, mConstRefSetter(setter)
		, mCopySetter(nullptr)
	{
	}
	constexpr Member(const char* name, CopyGetterFuncPtrT<Class, T> getter, ConstRefSetterFuncPtrT<Class, T> setter, U32 attributes = 0)
		: mName(name)
		, mAttributes(attributes)
		, mHasMemberPtr(false)
		, mMemberPtr(nullptr)
		, mConstRefGetter(nullptr)
		, mNonConstRefGetter(nullptr)
		, mCopyGetter(getter)
		, mConstRefSetter(setter)
		, mCopySetter(nullptr)
	{
	}
	constexpr Member(const char* name, CopyGetterFuncPtrT<Class, T> getter, CopySetterFuncPtrT<Class, T> setter, U32 attributes = 0)
		: mName(name)
		, mAttributes(attributes)
		, mHasMemberPtr(false)
		, mMemberPtr(nullptr)
		, mConstRefGetter(nullptr)
		, mNonConstRefGetter(nullptr)
		, mCopyGetter(getter)
		, mConstRefSetter(nullptr)
		, mCopySetter(setter)
	{
	}

	constexpr const char* GetName() const { return mName; }
	constexpr U32 GetHash() const { return Hash::SlowHash(mName); }

	constexpr U32 GetAttributes() const { return mAttributes; }
	constexpr bool HasEditor() const { return (mAttributes & en::Meta::Attribute_NoEditor) == 0; }
	constexpr bool HasSerialization() const { return (mAttributes & en::Meta::Attribute_NoSerialization) == 0; }

	constexpr bool HasMemberPtr() const { return mHasMemberPtr; }
	constexpr bool HasConstRefGetter() const { return mConstRefGetter != nullptr; }
	constexpr bool HasNonConstRefGetter() const { return mNonConstRefGetter != nullptr; }
	constexpr bool HasCopyGetter() const { return mCopyGetter != nullptr; }
	constexpr bool HasConstRefSetter() const { return mConstRefSetter != nullptr; }
	constexpr bool HasCopySetter() const { return mCopySetter != nullptr; }

	constexpr bool CanGetRef() const { return HasMemberPtr() || HasNonConstRefGetter(); }
	constexpr bool CanGetConstRef() const { return HasMemberPtr() || HasNonConstRefGetter() || HasConstRefGetter(); }
	constexpr bool CanGetCopy() const { return HasMemberPtr() || HasNonConstRefGetter() || HasConstRefGetter() || HasCopyGetter(); }
	constexpr bool CanSet() const { return HasMemberPtr() || HasNonConstRefGetter() || HasConstRefSetter() || HasCopySetter(); }

	constexpr U32 GetTotalHash() const
	{
		static_assert(TypeInfo<Class>::IsKnown());
		static_assert(TypeInfo<T>::IsKnown());
		return Hash::Combine32(TypeInfo<Class>::GetHash(), Hash::Combine32(TypeInfo<T>::GetHash(), GetHash()));
	}

	constexpr T& GetRef(Class& obj) const
	{
		if (HasMemberPtr())
		{
			return obj.*mMemberPtr;
		}
		else if (HasNonConstRefGetter())
		{
			return (obj.*mNonConstRefGetter)();
		}
		else
		{
			throw std::logic_error("Invalid member setup/use");
		}
	}
	
	constexpr const T& GetConstRef(const Class& obj) const
	{
		if (HasMemberPtr())
		{
			return obj.*mMemberPtr;
		}
		else if (HasNonConstRefGetter())
		{
			return const_cast<const T&>((const_cast<Class&>(obj).*mNonConstRefGetter)());
		}
		else if (HasConstRefGetter())
		{
			return (obj.*mConstRefGetter)();
		}
		else
		{
			throw std::logic_error("Invalid member setup/use");
		}
	}

	constexpr T GetCopy(const Class& obj) const
	{
		if (HasMemberPtr())
		{
			return obj.*mMemberPtr;
		}
		else if (HasConstRefGetter())
		{
			return (obj.*mConstRefGetter)();
		}
		else if (HasCopyGetter())
		{
			return (obj.*mCopyGetter)();
		}
		else if (HasNonConstRefGetter())
		{
			return (const_cast<Class&>(obj).*mNonConstRefGetter)();
		}
		else
		{
			throw std::logic_error("Invalid member setup/use");
		}
	}

	template <typename V, typename = Traits::EnableIf<std::is_constructible<T, V>::value>> // TODO : Remove this std::type_traits
	constexpr void Set(Class& obj, V&& value) const
	{
		if (HasMemberPtr())
		{
			obj.*mMemberPtr = value;
		}
		else if (HasNonConstRefGetter())
		{
			(obj.*mNonConstRefGetter)() = value;
		}
		else if (HasConstRefSetter())
		{
			(obj.*mConstRefSetter)(value);
		}
		else if (HasCopySetter())
		{
			(obj.*mCopySetter)(value);
		}
		else
		{
			throw std::logic_error("Invalid member setup/use");
		}
	}

private:
	const char* mName;
	U32 mAttributes;
	bool mHasMemberPtr;

	MemberTypeT<Class, T> mMemberPtr;

	NonConstRefGetterFuncPtrT<Class, T> mNonConstRefGetter;

	ConstRefGetterFuncPtrT<Class, T> mConstRefGetter;
	ConstRefSetterFuncPtrT<Class, T> mConstRefSetter;

	CopyGetterFuncPtrT<Class, T> mCopyGetter;
	CopySetterFuncPtrT<Class, T> mCopySetter;
};

template <typename Class, typename T>
constexpr Member<Class, T> RegisterMember(const char* name, T Class::* ptr, U32 attributes = 0)
{
	return Member<Class, T>(name, ptr, attributes);
}

template <typename Class, typename T>
constexpr Member<Class, T> RegisterMember(const char* name, NonConstRefGetterFuncPtrT<Class, T> getter, U32 attributes = 0)
{
	return Member<Class, T>(name, getter, attributes);
}

template <typename Class, typename T>
constexpr Member<Class, T> RegisterMember(const char* name, ConstRefGetterFuncPtrT<Class, T> getter, ConstRefSetterFuncPtrT<Class, T> setter, U32 attributes = 0)
{
	return Member<Class, T>(name, getter, setter, attributes);
}

template <typename Class, typename T>
constexpr Member<Class, T> RegisterMember(const char* name, CopyGetterFuncPtrT<Class, T> getter, ConstRefSetterFuncPtrT<Class, T> setter, U32 attributes = 0)
{
	return Member<Class, T>(name, getter, setter, attributes);
}

template <typename Class, typename T>
constexpr Member<Class, T> RegisterMember(const char* name, CopyGetterFuncPtrT<Class, T> getter, CopySetterFuncPtrT<Class, T> setter, U32 attributes = 0)
{
	return Member<Class, T>(name, getter, setter, attributes);
}

template <typename T>
constexpr bool IsRegistered()
{
	return false;
}

template <typename T>
constexpr auto RegisterMembers()
{
	return std::make_tuple();
}

namespace priv
{
	template <typename T, typename TupleType>
	struct Holder
	{
		static constexpr TupleType members = RegisterMembers<T>();
	};

} // namespace priv

// Used to access private members. Try to avoid using it if possible
#define ENLIVE_META_CLASS() template <typename T> friend constexpr auto ::en::Meta::RegisterMembers();

#define ENLIVE_META_CLASS_BEGIN(className) ENLIVE_DEFINE_TYPE_INFO(className) \
	namespace en::Meta { \
		template <> \
		constexpr bool IsRegistered<className>() { return true; } \
		template <> \
		constexpr auto RegisterMembers<className>() { return std::make_tuple(
#define ENLIVE_META_CLASS_MEMBER(name, ...) en::Meta::RegisterMember(name, __VA_ARGS__)
#define ENLIVE_META_CLASS_END() ); } }

template <typename T>
constexpr const auto& GetMembers()
{
	return priv::Holder<T, decltype(RegisterMembers<T>())>::members;
}

template <typename T>
constexpr U32 GetMemberCount()
{
	return static_cast<U32>(std::tuple_size<decltype(RegisterMembers<T>())>::value);
}

template <typename T, typename F>
constexpr void ForEachMember(F&& f)
{
	static_assert(IsRegistered<T>());
	std::apply(
		[&f](auto&&... elems)
		{
			((f(elems)), ...);
		},
		GetMembers<T>()
	);
}

template <typename T>
using MemberTypeOf = typename Traits::Decay<T>::type::Type;

template <typename T>
constexpr bool HasMember(const char* name)
{
	U32 hash = Hash::SlowHash(name);
	bool found = false;
	ForEachMember([&found, &hash](const auto& member)
	{
		if (member.GetHash() == hash)
		{
			found = true;
		}
	});
	return found;
}

template <typename T, typename MemberT>
constexpr bool HasMemberOfType(const char* name)
{
	U32 hash = Hash::SlowHash(name);
	bool found = false;
	ForEachMember([&found, &hash](const auto& member)
	{
		if (Traits::IsSame<T, MemberTypeOf<decltype(member)>>::value && member.GetHash() == hash)
		{
			found = true;
		}
	});
	return found;
}

template <typename T, typename MemberT, typename F>
constexpr void ForMember(const char* name, F&& f)
{
	static_assert(HasMemberOfType<T, MemberT>(name));
	U32 hash = Hash::SlowHash(name);
	ForEachMember<T>([&](const auto& member)
	{
		if (member.GetHash() == hash)
		{
			f(member);
		}
	});
}

template <typename T>
constexpr U32 GetClassVersion()
{
	static_assert(IsRegistered<T>() && TypeInfo<T>::IsKnown());
	U32 hash = TypeInfo<T>::GetHash();
	ForEachMember<T>([&hash](const auto& member)
	{
		hash = Hash::Combine32(hash, member.GetTotalHash());
	});
	return hash;
}

// TODO : Move to ObjectEditor ?
// Custom ImGui Editor
#define ENLIVE_META_CLASS_DEFAULT_TRAITS_VIRTUAL_IMGUI_EDITOR(className) \
	template <> \
	struct CustomObjectEditor<className> \
	{ \
		static constexpr bool value = true; \
		static bool ImGuiEditor(className& object, const char* name) \
		{ \
			return object.ImGuiEditor(name); \
		} \
	};
#define ENLIVE_META_CLASS_DEFAULT_VIRTUAL_IMGUI_EDITOR() \
	virtual bool ImGuiEditor(const char* name) \
	{ \
		 return en::ObjectEditor::ImGuiEditor_Registered(*this, name); \
	}

// TODO : Move to DataFile ?
// Custom Serialization
#define ENLIVE_META_CLASS_DEFAULT_TRAITS_VIRTUAL_SERIALIZATION(className) \
	template <> \
	struct CustomXmlSerialization<className> \
	{ \
		static constexpr bool value = true; \
		static bool Serialize(en::DataFile& dataFile, const className& object, const char* name) \
		{ \
			return object.Serialize(dataFile, name); \
		} \
		static bool Deserialize(en::DataFile& dataFile, className& object, const char* name) \
		{ \
			return object.Deserialize(dataFile, name); \
		} \
	};
#define ENLIVE_META_CLASS_DEFAULT_VIRTUAL_SERIALIZATION() \
	virtual bool Serialize(en::DataFile& dataFile, const char* name) const \
	{ \
		 return dataFile.Serialize_Registered(*this, name); \
	} \
	virtual bool Deserialize(en::DataFile& dataFile, const char* name) \
	{ \
		 return dataFile.Deserialize_Registered(*this, name); \
	}

} // namespace en::Meta
