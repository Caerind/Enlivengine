#include <Enlivengine/Meta/DataFileSpecialization_Platform.hpp>

namespace en::DataFileSpecialization
{

bool Serialize(en::DataFile& dataFile, const en::Time& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.CreateNode(name))
	{
		dataFile.WriteCurrentType<en::Time>();
		parser.SetValue(en::ToString(object.AsSeconds()));
		parser.CloseNode();
		return true;
	}
	else
	{
		return false;
	}
}

bool Deserialize(en::DataFile& dataFile, en::Time& object, const char* name)
{
	auto& parser = dataFile.GetParser();
	if (parser.ReadNode(name))
	{
		enAssert(dataFile.ReadCurrentType() == en::TypeInfo<en::Time>::GetHash());
		std::string valueString;
		parser.GetValue(valueString);
		object = en::Time::Seconds(en::FromString<en::F32>(valueString));
		parser.CloseNode();
		return true;
	}
	else
	{
		return false;
	}
}

} // en::DataFileSpecialization
