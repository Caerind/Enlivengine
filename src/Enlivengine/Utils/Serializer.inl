namespace en
{

template <typename T>
bool GenericSerialization(Serializer& serializer, const char* name, T& object)
{
	static_assert(TypeInfo<T>::IsKnown());

	if constexpr (TypeInfo<T>::HasCustomSerialization())
	{
		return object.Serialize(serializer, name);
	}
	else if constexpr (Meta::IsRegistered<T>())
	{
		if (serializer.BeginClass(name, TypeInfo<T>::GetName(), TypeInfo<T>::GetHash()))
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
			enLogError(LogChannel::System, "Can't find serialization for {}", TypeInfo<T>::GetName());
			enAssert(false);
			return false;
		}
	}
}

template <typename T>
bool GenericSerialization(Serializer& serializer, const char* name, const T& object)
{
	enAssert(serializer.IsWriting());

	static_assert(TypeInfo<T>::IsKnown());

	if constexpr (TypeInfo<T>::HasCustomSerialization())
	{
		return const_cast<T&>(object).Serialize(serializer, name);
	}
	else if constexpr (Meta::IsRegistered<T>())
	{
		if (serializer.BeginClass(name, TypeInfo<T>::GetName(), TypeInfo<T>::GetHash()))
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
			enLogError(LogChannel::System, "Can't find serialization for {}", TypeInfo<T>::GetName());
			enAssert(false);
			return false;
		}
	}	  
}

template <typename T, std::size_t N>
bool GenericSerialization(Serializer& serializer, const char* name, std::array<T, N>& object)
{
	if (serializer.BeginClass(name, TypeInfo<std::array<T, N>>::GetName(), TypeInfo<std::array<T, N>>::GetHash()))
	{
		bool ret = true;

		U32 size = 0;
		if (serializer.IsReading())
		{
			serializer.Serialize("size", size);
			if (size != static_cast<U32>(N))
			{
				enLogWarning(LogChannel::System, "Encountered std::array with size {} while reading using an std::array of size {}", size, N);
				ret = false;
			}
		}
		else if (serializer.IsWriting())
		{
			size = static_cast<U32>(N);
			serializer.Serialize("size", size);
		}

		ret = priv::GenericSerializationArray<std::array<T, N>, T>(serializer, name, size, object) && ret;

		ret = serializer.EndClass() && ret;
		return ret;
	}
	else
	{
		return false;
	}
}

template <typename T, std::size_t N>
bool GenericSerialization(Serializer& serializer, const char* name, const std::array<T, N>& object)
{
	if (!serializer.IsWriting())
	{
		enAssert(false);
		return false;
	}

	if (serializer.BeginClass(name, TypeInfo<std::array<T, N>>::GetName(), TypeInfo<std::array<T, N>>::GetHash()))
	{
		bool ret = true;

		U32 size = static_cast<U32>(N);
		serializer.Serialize("size", size);

		ret = priv::GenericSerializationArray<std::array<T, N>, T>(serializer, name, size, object) && ret;

		ret = serializer.EndClass() && ret;
		return ret;
	}
	else
	{
		return false;
	}
}

template <typename T>
bool GenericSerialization(Serializer& serializer, const char* name, std::vector<T>& object)
{
	if (serializer.BeginClass(name, TypeInfo<std::vector<T>>::GetName(), TypeInfo<std::vector<T>>::GetHash()))
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

		ret = priv::GenericSerializationArray<std::vector<T>, T>(serializer, name, size, object) && ret;

		ret = serializer.EndClass() && ret;
		return ret;
	}
	else
	{
		return false;
	}
}

template <typename T>
bool GenericSerialization(Serializer& serializer, const char* name, const std::vector<T>& object)
{
	if (!serializer.IsWriting())
	{
		enAssert(false);
		return false;
	}

	if (serializer.BeginClass(name, TypeInfo<std::vector<T>>::GetName(), TypeInfo<std::vector<T>>::GetHash()))
	{
		bool ret = true;

		U32 size = static_cast<U32>(object.size());
		serializer.Serialize("size", size);

		ret = priv::GenericSerializationArray<std::vector<T>, T>(serializer, name, size, object) && ret;

		ret = serializer.EndClass() && ret;
		return ret;
	}
	else
	{
		return false;
	}
}

template <typename T>
bool GenericSerialization(Serializer& serializer, const char* name, Array<T>& object)
{
	if (serializer.BeginClass(name, TypeInfo<Array<T>>::GetName(), TypeInfo<Array<T>>::GetHash()))
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

		ret = priv::GenericSerializationArray<Array<T>, T>(serializer, name, size, object) && ret;

		ret = serializer.EndClass() && ret;
		return ret;
	}
	else
	{
		return false;
	}
}

template <typename T>
bool GenericSerialization(Serializer& serializer, const char* name, const Array<T>& object)
{
	if (!serializer.IsWriting())
	{
		enAssert(false);
		return false;
	}

	if (serializer.BeginClass(name, TypeInfo<Array<T>>::GetName(), TypeInfo<Array<T>>::GetHash()))
	{
		bool ret = true;

		U32 size = object.Size();
		serializer.Serialize("size", size);

		ret = priv::GenericSerializationArray<Array<T>, T>(serializer, name, size, object) && ret;

		ret = serializer.EndClass() && ret;
		return ret;
	}
	else
	{
		return false;
	}
}

namespace priv
{

template <typename ArrayType, typename ObjectType>
bool GenericSerializationArray(Serializer& serializer, const char* name, U32 size, ArrayType& arrayObject)
{
	bool ret = true;
	for (U32 i = 0; i < size; ++i)
	{
		std::string childName(name);
		childName.append("_");
		childName.append(std::to_string(i));

		if constexpr (Traits::IsPointer<ObjectType>::value)
		{
			if (serializer.IsReading())
			{
				// TODO : READ CHILD
			}
			else if (serializer.IsWriting())
			{
				if (arrayObject[i] != nullptr)
				{
					ret = GenericSerialization(serializer, childName.c_str(), *(arrayObject[i])) && ret;
				}
				else
				{
					std::string nullString("");
					ret = GenericSerialization(serializer, childName.c_str(), nullString) && ret;
				}
			}
			else
			{
				enAssert(false);
			}
		}
		else
		{
			ret = GenericSerialization(serializer, childName.c_str(), arrayObject[i]) && ret;
		}
	}
	return ret;
}

template <typename ArrayType, typename ObjectType>
bool GenericSerializationArray(Serializer& serializer, const char* name, U32 size, const ArrayType& arrayObject)
{
	if (!serializer.IsWriting())
	{
		enAssert(false);
		return false;
	}

	bool ret = true;
	for (U32 i = 0; i < size; ++i)
	{
		std::string childName(name);
		childName.append("_");
		childName.append(std::to_string(i));

		if constexpr (Traits::IsPointer<ObjectType>::value)
		{
			if (arrayObject[i] != nullptr)
			{
				ret = GenericSerialization(serializer, childName.c_str(), *(arrayObject[i])) && ret;
			}
			else
			{
				ret = GenericSerialization(serializer, childName.c_str(), "") && ret;
			}
		}
		else
		{
			ret = GenericSerialization(serializer, childName.c_str(), arrayObject[i]) && ret;
		}
	}
	return ret;
}

} // namespace priv

} // namespace en