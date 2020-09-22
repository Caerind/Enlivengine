#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/System/Meta.hpp>
#include <Enlivengine/System/MetaEnum.hpp>
#include <Enlivengine/System/String.hpp>
#include <Enlivengine/System/TypeTraits.hpp>
#include <Enlivengine/System/Array.hpp>

#include <array>
#include <vector>

#include <Enlivengine/System/ParserXml.hpp>
#include <Enlivengine/System/ClassManager.hpp>

#include <Enlivengine/Core/CustomTraits.hpp>

// TODO : Factorise common code in Serialize/Deserialize (Array/vector/array)

namespace en
{

class DataFile
{
public:
	DataFile(bool readable = true);

	bool IsValid() const;
	bool CreateEmptyFile();
	bool LoadFromFile(const std::string& filename);
	bool SaveToFile(const std::string& filename);

	template <typename T>
	bool Serialize(const T& object, const char* name);
	template <typename T>
	bool Serialize(const T* object, const char* name);

	template <typename T>
	bool Serialize_Registered(const T& object, const char* name);

	template <typename T>
	bool Deserialize(T& object, const char* name);
	template <typename T>
	bool Deserialize(T* object, const char* name);

	template <typename T>
	bool Deserialize_Registered(T& object, const char* name);

private:
	template <typename T>
	bool Serialize_Common(const T& object, const char* name);

	template <typename T>
	bool Serialize_Basic(const T& object, const char* name);
	template <typename T>
	bool Serialize_Basic(const Array<T>& object, const char* name);
	template <typename T>
	bool Serialize_Basic(const std::vector<T>& object, const char* name);
	template <typename T, std::size_t N>
	bool Serialize_Basic(const std::array<T, N>& object, const char* name);
	template <typename T>
	bool Serialize_Basic(const Array<T*>& object, const char* name);
	template <typename T>
	bool Serialize_Basic(const std::vector<T*>& object, const char* name);
	template <typename T, std::size_t N>
	bool Serialize_Basic(const std::array<T*, N>& object, const char* name);

	template <typename T>
	bool Deserialize_Common(T& object, const char* name);

	template <typename T>
	bool Deserialize_Basic(T& object, const char* name);
	template <typename T>
	bool Deserialize_Basic(Array<T>& object, const char* name);
	template <typename T>
	bool Deserialize_Basic(std::vector<T>& object, const char* name);
	template <typename T, std::size_t N>
	bool Deserialize_Basic(std::array<T, N>& object, const char* name);
	template <typename T>
	bool Deserialize_Basic(Array<T*>& object, const char* name);
	template <typename T>
	bool Deserialize_Basic(std::vector<T*>& object, const char* name);
	template <typename T, std::size_t N>
	bool Deserialize_Basic(std::array<T*, N>& object, const char* name);

private:
	template <typename T>
	void WriteCurrentType();
	U32 ReadCurrentType() const;

	template <typename T> friend struct ::CustomXmlSerialization;
	ParserXml& GetParser() { return mParserXml; }
	const ParserXml& GetParser() const { return mParserXml; }

private:
	ParserXml mParserXml;
	bool mValid;
	bool mReadable;
};

template <typename T>
bool DataFile::Serialize(const T& object, const char* name)
{
	enAssert(mValid);
	enAssert(name != nullptr);
	if (mParserXml.HasNode(name))
	{
		mParserXml.RemoveNode(name);
	}
	return Serialize_Common(object, name);
}

template <typename T>
bool DataFile::Serialize(const T* object, const char* name)
{
	enAssert(object != nullptr);
	return Serialize(*object, name);
}

template <typename T>
bool DataFile::Serialize_Registered(const T& object, const char* name)
{
	static_assert(Meta::IsRegistered<T>());
	if (mParserXml.CreateNode(name))
	{
		// TODO : Use return of Serialize_Common for return
		WriteCurrentType<T>();
		Meta::ForEachMember<T>([this, &object](const auto& member)
		{
			if (member.HasSerialization())
			{
				using MemberType = typename Traits::Decay<decltype(member)>::type::Type;
				if (member.CanGetConstRef())
				{
					Serialize_Common(member.GetConstRef(object), member.GetName());
				}
				else if (member.CanGetCopy())
				{
					if constexpr (Traits::IsCopyAssignable<MemberType>::value)
					{
						Serialize_Common(member.GetCopy(object), member.GetName());
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
		mParserXml.CloseNode();
		return true;
	}
	else
	{
		return false;
	}
}

template <typename T>
bool DataFile::Deserialize(T& object, const char* name)
{
	enAssert(mValid);
	enAssert(name != nullptr);
	return Deserialize_Common(object, name);
}

template <typename T>
bool DataFile::Deserialize(T* object, const char* name)
{
	enAssert(object != nullptr);
	return Deserialize(*object, name);
}

template <typename T>
bool DataFile::Deserialize_Registered(T& object, const char* name)
{
	static_assert(Meta::IsRegistered<T>());
	if (mParserXml.ReadNode(name))
	{
		bool result = true;
		const U32 typeHash = ReadCurrentType();
		if (typeHash == TypeInfo<T>::GetHash())
		{
			// TODO : Use return of Serialize_Common for return
			Meta::ForEachMember<T>([this, &object](const auto& member)
			{
				if (member.HasSerialization())
				{
					if (member.CanGetRef())
					{
						Deserialize_Common(member.GetRef(object), member.GetName());
					}
					else if (member.CanGetCopy())
					{
						using MemberType = typename Traits::Decay<decltype(member)>::type::Type;
						if constexpr (Traits::IsCopyAssignable<MemberType>::value)
						{
							MemberType memberCopy = member.GetCopy(object);
							if (Deserialize_Common(memberCopy, member.GetName()))
							{
								if (member.CanSet())
								{
									member.Set(object, memberCopy);
								}
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
		}
		else
		{
			enLogWarning(LogChannel::Core, "Type mismatch for {}", name);
			result = false;
		}
		mParserXml.CloseNode();
		return result;
	}
	else
	{
		return false;
	}
}

template <typename T>
bool DataFile::Serialize_Common(const T& object, const char* name)
{
	if constexpr (CustomXmlSerialization<T>::value)
	{
		return CustomXmlSerialization<T>::Serialize(*this, object, name);
	}
	else if constexpr (Meta::IsRegistered<T>())
	{
		return Serialize_Registered(object, name);
	}
	else
	{
		return Serialize_Basic(object, name);
	}
}

template <typename T>
bool DataFile::Serialize_Basic(const T& object, const char* name)
{
	enAssert(name);
	if (mParserXml.CreateNode(name))
	{
		WriteCurrentType<T>();
		
		using Type = typename Traits::Decay<T>::type;
		
		if constexpr (Traits::IsSame<Type, bool>::value)
		{
			mParserXml.SetValue(ToBoolString(object));
		}
		else if constexpr (Traits::IsEnum<T>::value)
		{
			const std::string enumValueStr(Meta::GetEnumName(object));
			mParserXml.SetValue(enumValueStr);
		}
		else if constexpr (Traits::IsSame<Type, std::string>::value)
		{
			mParserXml.SetValue(object);
		}
		else
		{
			mParserXml.SetValue(ToString(object));
		}
		mParserXml.CloseNode();
		return true;
	}
	else
	{
		return false;
	}
}

template <typename T>
bool DataFile::Serialize_Basic(const Array<T>& object, const char* name)
{
	enAssert(name);
	if (mParserXml.CreateNode(name))
	{
		bool result = true;
		for (U32 i = 0; i < object.Size(); ++i)
		{
			std::string childName(name);
			childName.append("_");
			childName.append(std::to_string(i));
			result = Serialize_Common(object[i], childName.c_str()) && result;
		}
		WriteCurrentType<T>();
		mParserXml.SetAttribute("size", object.Size());
		mParserXml.CloseNode();
		return result;
	}
	else
	{
		return false;
	}
}

template <typename T>
bool DataFile::Serialize_Basic(const std::vector<T>& object, const char* name)
{
	enAssert(name);
	if (mParserXml.CreateNode(name))
	{
		bool result = true;
		for (std::size_t i = 0; i < object.size(); ++i)
		{
			std::string childName(name);
			childName.append("_");
			childName.append(std::to_string(i));
			result = Serialize_Common(object[i], childName.c_str()) && result;
		}
		WriteCurrentType<T>();
		mParserXml.SetAttribute("size", static_cast<U32>(object.size()));
		mParserXml.CloseNode();
		return result;
	}
	else
	{
		return false;
	}
}

template <typename T, std::size_t N>
bool DataFile::Serialize_Basic(const std::array<T, N>& object, const char* name)
{
	enAssert(name);
	if (mParserXml.CreateNode(name))
	{
		bool result = true;
		for (std::size_t i = 0; i < object.size(); ++i)
		{
			std::string childName(name);
			childName.append("_");
			childName.append(std::to_string(i));
			result = Serialize_Common(object[i], childName.c_str()) && result;
		}
		WriteCurrentType<T>();
		mParserXml.SetAttribute("size", static_cast<U32>(object.size()));
		mParserXml.CloseNode();
		return result;
	}
	else
	{
		return false;
	}
}

template <typename T>
bool DataFile::Serialize_Basic(const Array<T*>& object, const char* name)
{
	enAssert(name);
	if (mParserXml.CreateNode(name))
	{
		U32 size = object.Size();
		bool result = true;
		for (U32 i = 0; i < object.Size(); ++i)
		{
			std::string childName(name);
			childName.append("_");
			childName.append(std::to_string(i));
			if (object[i] != nullptr)
			{
				result = Serialize_Common(*object[i], childName.c_str()) && result;
			}
			else
			{
				size--;
				enLogWarning(LogChannel::Core, "Skipped nullptr object in {}", name);
			}
		}
		WriteCurrentType<T>();
		mParserXml.SetAttribute("size", size);
		mParserXml.CloseNode();
		return result;
	}
	else
	{
		return false;
	}
}

template <typename T>
bool DataFile::Serialize_Basic(const std::vector<T*>& object, const char* name)
{
	enAssert(name);
	if (mParserXml.CreateNode(name))
	{
		U32 size = static_cast<U32>(object.size());
		bool result = true;
		for (std::size_t i = 0; i < object.size(); ++i)
		{
			std::string childName(name);
			childName.append("_");
			childName.append(std::to_string(i));
			if (object[i] != nullptr)
			{
				result = Serialize_Common(*object[i], childName.c_str()) && result;
			}
			else
			{
				size--;
				enLogWarning(LogChannel::Core, "Skipped nullptr object in {}", name);
			}
		}
		WriteCurrentType<T>();
		mParserXml.SetAttribute("size", size);
		mParserXml.CloseNode();
		return result;
	}
	else
	{
		return false;
	}
}

template <typename T, std::size_t N>
bool DataFile::Serialize_Basic(const std::array<T*, N>& object, const char* name)
{
	enAssert(name);
	if (mParserXml.CreateNode(name))
	{
		U32 size = static_cast<U32>(object.size());
		bool result = true;
		for (std::size_t i = 0; i < object.size(); ++i)
		{
			std::string childName(name);
			childName.append("_");
			childName.append(std::to_string(i));
			if (object[i] != nullptr)
			{
				result = Serialize_Common(*object[i], childName.c_str()) && result;
			}
			else
			{
				size--;
				enLogWarning(LogChannel::Core, "Skipped nullptr object in {}", name);
			}
		}
		WriteCurrentType<T>();
		mParserXml.SetAttribute("size", size);
		mParserXml.CloseNode();
		return result;
	}
	else
	{
		return false;
	}
}

template <typename T>
bool DataFile::Deserialize_Common(T& object, const char* name)
{
	if constexpr (CustomXmlSerialization<T>::value)
	{
		return CustomXmlSerialization<T>::Deserialize(*this, object, name);
	}
	else if constexpr (Meta::IsRegistered<T>())
	{
		return Deserialize_Registered(object, name);
	}
	else
	{
		return Deserialize_Basic(object, name);
	}
}

template <typename T>
bool DataFile::Deserialize_Basic(T& object, const char* name)
{
	enAssert(name);
	if (mParserXml.ReadNode(name))
	{
		bool result = true;
		const U32 typeHash = ReadCurrentType();
		if (typeHash == TypeInfo<T>::GetHash())
		{
			using Type = typename Traits::Decay<T>::type;
		
			if constexpr (Traits::IsSame<Type, bool>::value)
			{
				std::string value;
				mParserXml.GetValue(value);
				object = FromBoolString(value);
			}
			else if constexpr (Traits::IsEnum<T>::value)
			{
				std::string value;
				mParserXml.GetValue(value);
				object = Meta::EnumCast<T>(value);
			}
			else if constexpr (Traits::IsSame<Type, std::string>::value)
			{
				mParserXml.GetValue(object);
			}
			else
			{
				std::string value;
				mParserXml.GetValue(value);
				object = FromString<T>(value);
			}
		}
		else
		{
			enLogWarning(LogChannel::Core, "Type mismatch for {}", name);
			result = false;
		}
		mParserXml.CloseNode();
		return result;
	}
	else
	{
		return false;
	}
}

template <typename T>
bool DataFile::Deserialize_Basic(Array<T>& object, const char* name)
{
	enAssert(name);
	if (mParserXml.ReadNode(name))
	{
		bool result = true;
		const U32 typeHash = ReadCurrentType();
		if (typeHash == TypeInfo<T>::GetHash())
		{
			U32 size = 0;
			mParserXml.GetAttribute("size", size);
			object.Clear();
			object.Resize(size);
			if constexpr (!Traits::IsDefaultConstructible<T>::value)
			{
				// For now T must be default constructible
				static_assert(Traits::IsDefaultConstructible<T>::value);
			}

			for (U32 i = 0; i < size; ++i)
			{
				std::string childName(name);
				childName.append("_");
				childName.append(std::to_string(i));
				result = Deserialize_Common(object[i], childName.c_str()) && result;
			}
		}
		else
		{
			enLogWarning(LogChannel::Core, "Type mismatch for {}", name);
			object.Clear();
			result = false;
		}
		mParserXml.CloseNode();
		return result;
	}
	else
	{
		return false;
	}
}

template <typename T>
bool DataFile::Deserialize_Basic(std::vector<T>& object, const char* name)
{
	enAssert(name);
	if (mParserXml.ReadNode(name))
	{
		bool result = true;
		const U32 typeHash = ReadCurrentType();
		if (typeHash == TypeInfo<T>::GetHash())
		{
			U32 size = 0;
			mParserXml.GetAttribute("size", size);
			object.clear();
			object.resize(static_cast<std::size_t>(size));
			if constexpr (!Traits::IsDefaultConstructible<T>::value)
			{
				// For now T must be default constructible
				static_assert(Traits::IsDefaultConstructible<T>::value);
			}

			for (U32 i = 0; i < size; ++i)
			{
				std::string childName(name);
				childName.append("_");
				childName.append(std::to_string(i));
				result = Deserialize_Common(object[i], childName.c_str()) && result;
			}
		}
		else
		{
			enLogWarning(LogChannel::Core, "Type mismatch for {}", name);
			object.clear();
			result = false;
		}
		mParserXml.CloseNode();
		return result;
	}
	else
	{
		return false;
	}
}

template <typename T, std::size_t N>
bool DataFile::Deserialize_Basic(std::array<T, N>& object, const char* name)
{
	enAssert(name);
	if (mParserXml.ReadNode(name))
	{
		bool result = true;
		const U32 typeHash = ReadCurrentType();
		if (typeHash == TypeInfo<T>::GetHash())
		{
			U32 size = 0;
			mParserXml.GetAttribute("size", size);
			if (size != static_cast<U32>(N))
			{
				// TODO : Find a solution to allow updates
				enLogError(LogChannel::Core, "Invalid size of {} for std::array named {} of size {}", size, name, N);
				return false;
			}
			if constexpr (!Traits::IsDefaultConstructible<T>::value)
			{
				// For now T must be default constructible
				static_assert(Traits::IsDefaultConstructible<T>::value);
			}

			for (U32 i = 0; i < size; ++i)
			{
				std::string childName(name);
				childName.append("_");
				childName.append(std::to_string(i));
				result = Deserialize_Common(object[i], childName.c_str()) && result;
			}
		}
		else
		{
			enLogWarning(LogChannel::Core, "Type mismatch for {}", name);
			result = false;
		}
		mParserXml.CloseNode();
		return result;
	}
	else
	{
		return false;
	}
}

template <typename T>
bool DataFile::Deserialize_Basic(Array<T*>& object, const char* name)
{
	enAssert(name);
	if (mParserXml.ReadNode(name))
	{
		bool result = true;
		const U32 typeHash = ReadCurrentType();
		if (typeHash == TypeInfo<T>::GetHash())
		{
			U32 size = 0;
			mParserXml.GetAttribute("size", size);
			object.Clear();
			object.Resize(size);

			for (U32 i = 0; i < size; ++i)
			{
				std::string childName(name);
				childName.append("_");
				childName.append(std::to_string(i));

				if (!mParserXml.ReadNode(childName))
				{
					enLogError(LogChannel::Core, "Can't read node {} for {}", childName, name);
					result = false;
					continue;
				}
				const U32 childTypeHash = ReadCurrentType();
				mParserXml.CloseNode();

				// TODO : Check inheritance childTypeHash is child of typeHash
				T* childObject = (T*)ClassManager::CreateClassFromHash(childTypeHash);
				if (childObject != nullptr)
				{
					const bool thisResult = Deserialize_Common(*childObject, childName.c_str());
					if (thisResult)
					{
						object[i] = childObject;
					}
					result = thisResult && result;
				}
				else
				{
					enLogError(LogChannel::Core, "Can't create object of type {} for {}", ClassManager::GetClassNameFromHash(childTypeHash), name);
					result = false;
				}
			}
		}
		else
		{
			enLogWarning(LogChannel::Core, "Type mismatch for {}", name);
			object.Clear();
			result = false;
		}

		mParserXml.CloseNode();
		return result;
	}
	else
	{
		return false;
	}
}

template <typename T>
bool DataFile::Deserialize_Basic(std::vector<T*>& object, const char* name)
{
	enAssert(name);
	if (mParserXml.ReadNode(name))
	{
		bool result = true;
		const U32 typeHash = ReadCurrentType();
		if (typeHash == TypeInfo<T>::GetHash())
		{
			U32 size = 0;
			mParserXml.GetAttribute("size", size);
			object.clear();
			object.resize(static_cast<std::size_t>(size));

			for (U32 i = 0; i < size; ++i)
			{
				std::string childName(name);
				childName.append("_");
				childName.append(std::to_string(i));

				if (!mParserXml.ReadNode(childName))
				{
					enLogError(LogChannel::Core, "Can't read node {} for {}", childName, name);
					result = false;
					continue;
				}
				const U32 childTypeHash = ReadCurrentType();
				mParserXml.CloseNode();

				// TODO : Check inheritance childTypeHash is child of typeHash
				T* childObject = (T*)ClassManager::CreateClassFromHash(childTypeHash);
				if (childObject != nullptr)
				{
					const bool thisResult = Deserialize_Common(*childObject, childName.c_str());
					if (thisResult)
					{
						object[i] = childObject;
					}
					result = thisResult && result;
				}
				else
				{
					enLogError(LogChannel::Core, "Can't create object of type {} for {}", ClassManager::GetClassNameFromHash(childTypeHash), name);
					result = false;
				}
			}
		}
		else
		{
			enLogWarning(LogChannel::Core, "Type mismatch for {}", name);
			object.clear();
			result = false;
		}

		mParserXml.CloseNode();
		return result;
	}
	else
	{
		return false;
	}
}

template <typename T, std::size_t N>
bool DataFile::Deserialize_Basic(std::array<T*, N>& object, const char* name)
{
	enAssert(name);
	if (mParserXml.ReadNode(name))
	{
		bool result = true;
		const U32 typeHash = ReadCurrentType();
		if (typeHash == TypeInfo<T>::GetHash())
		{
			U32 size = 0;
			mParserXml.GetAttribute("size", size);
			if (size != static_cast<U32>(N))
			{
				// TODO : Find a solution to allow updates
				enLogError(LogChannel::Core, "Invalid size of {} for std::array named {} of size {}", size, name, N);
				return false;
			}

			for (U32 i = 0; i < size; ++i)
			{
				std::string childName(name);
				childName.append("_");
				childName.append(std::to_string(i));

				if (!mParserXml.ReadNode(childName))
				{
					enLogError(LogChannel::Core, "Can't read node {} for {}", childName, name);
					result = false;
					continue;
				}
				const U32 childTypeHash = ReadCurrentType();
				mParserXml.CloseNode();

				// TODO : Check inheritance childTypeHash is child of typeHash
				T* childObject = (T*)ClassManager::CreateClassFromHash(childTypeHash);
				if (childObject != nullptr)
				{
					const bool thisResult = Deserialize_Common(*childObject, childName.c_str());
					if (thisResult)
					{
						object[i] = childObject;
					}
					result = thisResult && result;
				}
				else
				{
					enLogError(LogChannel::Core, "Can't create object of type {} for {}", ClassManager::GetClassNameFromHash(childTypeHash), name);
					result = false;
				}
			}
		}
		else
		{
			enLogWarning(LogChannel::Core, "Type mismatch for {}", name);
			result = false;
		}

		mParserXml.CloseNode();
		return result;
	}
	else
	{
		return false;
	}
}

template <typename T>
void DataFile::WriteCurrentType()
{
	if (mReadable)
	{
		mParserXml.SetAttribute("type", TypeInfo<T>::GetName());
	}
	else
	{
		mParserXml.SetAttribute("type", TypeInfo<T>::GetHash());
	}
}

} // namespace en

#include "DataFileSpecialization.inl"
