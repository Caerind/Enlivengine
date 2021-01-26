namespace en
{

template <typename T>
bool GenericEdit(ObjectEditor& objectEditor, const char* name, T& object)
{
	static_assert(TypeInfo<T>::IsKnown());

	if constexpr (TypeInfo<T>::HasCustomEditor())
	{
		return object.Edit(objectEditor, name);
	}
	else if constexpr (Meta::IsRegistered<T>())
	{
		if (objectEditor.BeginClass(name, TypeInfo<T>::GetName(), TypeInfo<T>::GetHash()))
		{
			bool ret = true;
			Meta::ForEachMember<T>([&ret, &objectEditor, &object](const auto& member)
				{
					if (member.HasEditor())
					{
						using MemberType = typename Traits::Decay<decltype(member)>::type::Type;
						if (member.CanGetRef())
						{
							ret = GenericEdit(objectEditor, member.GetName(), member.GetRef(object)) || ret;
						}
						else if (member.CanGetCopy())
						{
							if constexpr (Traits::IsCopyAssignable<MemberType>::value)
							{
								MemberType memberCopy = member.GetCopy(object);
								if (GenericEdit(objectEditor, member.GetName(), memberCopy))
								{
									member.Set(object, memberCopy);
									ret = true;
								}
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
			objectEditor.EndClass();
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
			return objectEditor.Edit(name, object);
		}
		else if constexpr (Traits::IsEnum<T>::value)
		{
			// TODO : Not good to have this here...
			if (objectEditor.IsImGuiEditor())
			{
#ifdef ENLIVE_ENABLE_IMGUI
				static bool initialized = false;
				static constexpr U32 enumCount = Enum::GetCount<T>();
				static std::string stringsStorage[enumCount];
				static const char* stringsImGui[enumCount];
				if (!initialized)
				{
					auto enumNames = Enum::GetValueNames<T>();
					for (U32 i = 0; i < enumCount; ++i)
					{
						stringsStorage[i] = enumNames[i];
						stringsImGui[i] = stringsStorage[i].c_str();
					}
					initialized = true;
				}

				int index = static_cast<int>(Enum::GetIndex(object));
				if (ImGui::Combo(name, &index, stringsImGui, static_cast<int>(enumCount)))
				{
					object = Enum::GetFromIndex<T>(static_cast<U32>(index));
					return true;
				}
#endif // ENLIVE_ENABLE_IMGUI
			}
			else
			{
				enAssert(false);
			}
			return false;
		}
		else
		{
			enLogError(LogChannel::System, "Can't find object edition for {}", TypeInfo<T>::GetName());
			enAssert(false);
			return false;
		}
	}
}

template <typename T, std::size_t N>
bool GenericEdit(ObjectEditor& objectEditor, const char* name, std::array<T, N>& object)
{
	if (objectEditor.BeginClass(name, TypeInfo<std::array<T, N>>::GetName(), TypeInfo<std::array<T, N>>::GetHash()))
	{
		bool ret = priv::GenericEditArray<std::array<T, N>, T>(objectEditor, name, static_cast<U32>(N), object);
		objectEditor.EndClass();
		return ret;
	}
	else
	{
		return false;
	}
}

template <typename T>
bool GenericEdit(ObjectEditor& objectEditor, const char* name, std::vector<T>& object)
{
	if (objectEditor.BeginClass(name, TypeInfo<std::vector<T>>::GetName(), TypeInfo<std::vector<T>>::GetHash()))
	{
		bool ret = priv::GenericEditArray<std::vector<T>, T>(objectEditor, name, static_cast<U32>(object.size()), object);
		objectEditor.EndClass();
		return ret;
	}
	else
	{
		return false;
	}
}

template <typename T>
bool GenericEdit(ObjectEditor& objectEditor, const char* name, Array<T>& object)
{
	if (objectEditor.BeginClass(name, TypeInfo<Array<T>>::GetName(), TypeInfo<Array<T>>::GetHash()))
	{
		bool ret = priv::GenericEditArray<Array<T>, T>(objectEditor, name, object.Size(), object);
		objectEditor.EndClass();
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
bool GenericEditArray(ObjectEditor& objectEditor, const char* name, U32 size, ArrayType& arrayObject)
{
	bool ret = false;
	for (U32 i = 0; i < size; ++i)
	{
		std::string childName(name);
		childName.append("_");
		childName.append(std::to_string(i));

		if constexpr (Traits::IsPointer<ObjectType>::value)
		{
			if (arrayObject[i] != nullptr)
			{
				ret = GenericEdit(objectEditor, childName.c_str(), *(arrayObject[i])) || ret;
			}
			else
			{
				std::string nullString = "(Null)";
				GenericEdit(objectEditor, childName.c_str(), nullString);
			}
		}
		else
		{
			ret = GenericEdit(objectEditor, childName.c_str(), arrayObject[i]) || ret;
		}
	}
	return ret;
}

} // namespace priv

} // namespace en