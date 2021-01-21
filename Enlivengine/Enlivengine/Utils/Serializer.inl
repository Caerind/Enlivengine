namespace en
{

template <typename T>
bool GenericSerialization(ClassSerializer& serializer, const char* name, T& object)
{
	static_assert(TypeInfo<T>::IsKnown());

	if constexpr (TypeInfo<T>::HasCustomSerialization())
	{
		return object.Serialize(serializer, name);
	}
	else if constexpr (Meta::IsRegistered<T>())
	{
		if (serializer.BeginClass(name, TypeInfo<T>::GetHash()))
		{
			bool ret = true;
			Meta::ForEachMember<T>([&ret, &serializer, &object](const auto& member)
				{
					if (member.HasSerialization())
					{
						using MemberType = typename Traits::Decay<decltype(member)>::type::Type;
						if (member.CanGetRef())
						{
							ret = GenericSerialization(serializer, member.GetName(), member.GetRef(object)) && ret;
						}
						else if (member.CanGetConstRef() && serializer.IsReading())
						{
							MemberType& brokeConstRef = const_cast<MemberType&>(member.GetConstRef(object));
							ret = GenericSerialization(serializer, member.GetName(), brokeConstRef) && ret;
						}
						else if (member.CanGetCopy())
						{
							if constexpr (Traits::IsCopyAssignable<MemberType>::value)
							{
								MemberType memberCopy = member.GetCopy(object);
								const bool serialize = GenericSerialization(serializer, member.GetName(), memberCopy);
								if (serialize && serializer.IsReading())
								{
									member.Set(object, memberCopy);
								}
								ret = serialize && ret;
							}
							else
							{
								enAssert(false);
							}
						}
						else
						{
							enAssert(false);
						}
					}
				});
			ret = serializer.EndClass() && ret;
			return ret;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if constexpr (TypeInfo<T>::IsPrimitive())
		{
			return serializer.Serialize(name, object);
		}
		else if constexpr (Traits::IsEnum<T>::value)
		{
			if (serializer.IsReading())
			{
				std::string enumValueStr;
				const bool ret = serializer.Serialize(name, enumValueStr);
				object = Enum::Cast<T>(enumValueStr);
				return ret;
			}
			else if (serializer.IsWriting())
			{
				std::string enumValueStr(Enum::GetValueName(object));
				return serializer.Serialize(name, enumValueStr);
			}
			else
			{
				enAssert(false);
				return false;
			}
		}
		else
		{
			enLogError(LogChannel::Utils, "Can't find serialization for {}", TypeInfo<T>::GetName());
			enAssert(false);
			return false;
		}
	}
}

template <typename T>
bool GenericSerialization(ClassSerializer& serializer, const char* name, const T& object)
{
	enAssert(serializer.IsWriting());

	static_assert(TypeInfo<T>::IsKnown());

	if constexpr (TypeInfo<T>::HasCustomSerialization())
	{
		return const_cast<T&>(object).Serialize(serializer, name);
	}
	else if constexpr (Meta::IsRegistered<T>())
	{
		if (serializer.BeginClass(name, TypeInfo<T>::GetHash()))
		{
			bool ret = true;
			Meta::ForEachMember<T>([&ret, &serializer, &object](const auto& member)
				{
					if (member.HasSerialization())
					{
						using MemberType = typename Traits::Decay<decltype(member)>::type::Type;
						if (member.CanGetConstRef())
						{
							ret = GenericSerialization(serializer, member.GetName(), member.GetRef(const_cast<T&>(object))) && ret;
						}
						else if (member.CanGetCopy())
						{
							if constexpr (Traits::IsCopyAssignable<MemberType>::value)
							{
								MemberType memberCopy = member.GetCopy(object);
								const bool serialize = GenericSerialization(serializer, member.GetName(), memberCopy);
								ret = serialize && ret;
							}
							else
							{
								enAssert(false);
							}
						}
						else
						{
							enAssert(false);
						}
					}
				});
			ret = serializer.EndClass() && ret;
			return ret;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if constexpr (TypeInfo<T>::IsPrimitive())
		{
			return serializer.Serialize(name, const_cast<T&>(object));
		}
		else if constexpr (Traits::IsEnum<T>::value)
		{
			if (serializer.IsWriting())
			{
				std::string enumValueStr(Enum::GetValueName(object));
				return serializer.Serialize(name, enumValueStr);
			}
			else
			{
				enAssert(false);
				return false;
			}
		}
		else
		{
			enLogError(LogChannel::Utils, "Can't find serialization for {}", TypeInfo<T>::GetName());
			enAssert(false);
			return false;
		}
	}	  
}

// TODO : Support std::array<T>, std::array<T&>, std::array<T*>
template <typename T, std::size_t N>
bool GenericSerialization(ClassSerializer& serializer, const char* name, std::array<T, N>& object)
{
	if (serializer.BeginClass(name, TypeInfo<std::array<T, N>>::GetHash()))
	{
		bool ret = true;

		U32 size = 0;
		if (serializer.IsReading())
		{
			serializer.Serialize("size", size);
			if (size != static_cast<U32>(N))
			{
				// TODO : ?
			}
		}
		else if (serializer.IsWriting())
		{
			size = static_cast<U32>(N);
			serializer.Serialize("size", size);
		}

		for (U32 i = 0; i < size; ++i)
		{
			std::string childName(name);
			childName.append("_");
			childName.append(std::to_string(i));

			// TODO : Polymorphism

			ret = GenericSerialization(serializer, childName.c_str(), object[i]) && ret;
		}

		ret = serializer.EndClass() && ret;
		return ret;
	}
	else
	{
		return false;
	}
}

// TODO : Support const std::array<T>, const std::array<T&>, const std::array<T*>
template <typename T, std::size_t N>
bool GenericSerialization(ClassSerializer& serializer, const char* name, const std::array<T, N>& object)
{
	if (serializer.BeginClass(name, TypeInfo<std::array<T, N>>::GetHash()))
	{
		bool ret = true;

		U32 size = 0;
		if (serializer.IsReading())
		{
			serializer.Serialize("size", size);
			if (size != static_cast<U32>(N))
			{
				// TODO : ?
			}
		}
		else if (serializer.IsWriting())
		{
			size = static_cast<U32>(N);
			serializer.Serialize("size", size);
		}

		for (U32 i = 0; i < size; ++i)
		{
			std::string childName(name);
			childName.append("_");
			childName.append(std::to_string(i));

			// TODO : Polymorphism

			ret = GenericSerialization(serializer, childName.c_str(), object[i]) && ret;
		}

		ret = serializer.EndClass() && ret;
		return ret;
	}
	else
	{
		return false;
	}
}

// TODO : Support vector<T>, vector<T&>, vector<T*>
template <typename T>
bool GenericSerialization(ClassSerializer& serializer, const char* name, std::vector<T>& object)
{
	if (serializer.BeginClass(name, TypeInfo<std::vector<T>>::GetHash()))
	{
		bool ret = true;

		U32 size = 0;
		if (serializer.IsReading())
		{
			serializer.Serialize("size", size);
			object.resize(size);
		}
		else if (serializer.IsWriting())
		{
			size = static_cast<U32>(object.size());
			serializer.Serialize("size", size);
		}

		for (U32 i = 0; i < size; ++i)
		{
			std::string childName(name);
			childName.append("_");
			childName.append(std::to_string(i));

			// TODO : Polymorphism

			ret = GenericSerialization(serializer, childName.c_str(), object[i]) && ret;
		}

		ret = serializer.EndClass() && ret;
		return ret;
	}
	else
	{
		return false;
	}
}

// TODO : Support const vector<T>, const vector<T&>, const vector<T*>
template <typename T>
bool GenericSerialization(ClassSerializer& serializer, const char* name, const std::vector<T>& object)
{
	if (!serializer.IsWriting())
	{
		enAssert(false);
		return false;
	}

	if (serializer.BeginClass(name, TypeInfo<std::vector<T>>::GetHash()))
	{
		bool ret = true;

		U32 size = 0;
		if (serializer.IsReading())
		{
			serializer.Serialize("size", size);
			if (size != static_cast<U32>(object.size()))
			{
				// TODO : ?
			}
		}
		else if (serializer.IsWriting())
		{
			size = static_cast<U32>(object.size());
			serializer.Serialize("size", size);
		}

		for (U32 i = 0; i < size; ++i)
		{
			std::string childName(name);
			childName.append("_");
			childName.append(std::to_string(i));

			// TODO : Polymorphism

			ret = GenericSerialization(serializer, childName.c_str(), object[i]) && ret;
		}

		ret = serializer.EndClass() && ret;
		return ret;
	}
	else
	{
		return false;
	}
}

// TODO : Support Array<T>, Array<T&>, Array<T*>
template <typename T>
bool GenericSerialization(ClassSerializer& serializer, const char* name, Array<T>& object)
{
	if (serializer.BeginClass(name, TypeInfo<Array<T>>::GetHash()))
	{
		bool ret = true;

		U32 size = 0;
		if (serializer.IsReading())
		{
			serializer.Serialize("size", size);
			object.Resize(size);
		}
		else if (serializer.IsWriting())
		{
			size = object.Size();
			serializer.Serialize("size", size);
		}

		for (U32 i = 0; i < size; ++i)
		{
			std::string childName(name);
			childName.append("_");
			childName.append(std::to_string(i));

			// TODO : Polymorphism

			ret = GenericSerialization(serializer, childName.c_str(), object[i]) && ret;
		}

		ret = serializer.EndClass() && ret;
		return ret;
	}
	else
	{
		return false;
	}
}

// TODO : Support const Array<T>, const Array<T&>, const Array<T*>
template <typename T>
bool GenericSerialization(ClassSerializer& serializer, const char* name, const Array<T>& object)
{
	if (!serializer.IsWriting())
	{
		enAssert(false);
		return false;
	}

	if (serializer.BeginClass(name, TypeInfo<Array<T>>::GetHash()))
	{
		bool ret = true;

		U32 size = 0;
		if (serializer.IsReading())
		{
			serializer.Serialize("size", size);
			if (size != object.Size())
			{
				// TODO : ?
			}
		}
		else if (serializer.IsWriting())
		{
			size = object.Size();
			serializer.Serialize("size", size);
		}

		for (U32 i = 0; i < size; ++i)
		{
			std::string childName(name);
			childName.append("_");
			childName.append(std::to_string(i));

			// TODO : Polymorphism

			ret = GenericSerialization(serializer, childName.c_str(), object[i]) && ret;
		}

		ret = serializer.EndClass() && ret;
		return ret;
	}
	else
	{
		return false;
	}
}

} // namespace en