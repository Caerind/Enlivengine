#pragma once

#include <Enlivengine/Utils/String.hpp>
#include <Enlivengine/Utils/TypeInfo.hpp>
#include <Enlivengine/Utils/Meta.hpp>

#include <Enlivengine/Meta/MetaTraits.hpp>
#include <Enlivengine/Meta/DataFile.hpp>

#include <Enlivengine/Resources/ResourceManager.hpp>

//////////////////////////////////////////////////////////////////
// en::ResourcePtr<T>
//////////////////////////////////////////////////////////////////
template <typename T>
struct HasCustomSerialization<en::ResourcePtr<T>>
{
	static constexpr bool value = true;
	static bool Serialize(en::DataFile& dataFile, const en::ResourcePtr<T>& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.CreateNode(name))
		{
			dataFile.WriteCurrentType<en::ResourcePtr<T>>();
			dataFile.Serialize(static_cast<en::U32>(object.GetID()), "resourceID");
			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	static bool Deserialize(en::DataFile& dataFile, en::ResourcePtr<T>& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.ReadNode(name))
		{
			enAssert(dataFile.ReadCurrentType() == en::TypeInfo<en::ResourcePtr<T>>::GetHash());

			en::U32 resourceID;
			dataFile.Deserialize(resourceID, "resourceID");

			object = en::ResourcePtr<T>(static_cast<en::ResourceID>(resourceID));

			parser.CloseNode();
			return true;
		}
		else
		{
			return false;
		}
	}
};
