#include <Enlivengine/Core/DataFile.hpp>

namespace en
{

DataFile::DataFile(bool readable)
	: mParserXml()
	, mValid(false)
	, mReadable(readable)
{
}

bool DataFile::IsValid() const
{
	return mValid;
}

bool DataFile::CreateEmptyFile()
{
	mParserXml.NewFile();
	if (mParserXml.CreateNode("DataFile"))
	{
		mValid = true;
	}
	else
	{
		enLogError(LogChannel::Core, "Can't create \"DataFile\" root node for new DataFile");
		mValid = false;
	}
	return mValid;
}

bool DataFile::LoadFromFile(const std::string& filename)
{
	if (mParserXml.LoadFromFile(filename))
	{
		if (mParserXml.ReadNode("DataFile"))
		{
			mValid = true;
		}
		else
		{
			enLogError(LogChannel::Core, "Can't find \"DataFile\" root node for DataFile {}", filename);
			mValid = false;
		}
	}
	else
	{
		enLogError(LogChannel::Core, "Invalid DataFile file {}", filename);
		mValid = false;
	}
	return mValid;
}

bool DataFile::SaveToFile(const std::string& filename)
{
	return mParserXml.SaveToFile(filename);
}

U32 DataFile::ReadCurrentType() const
{
	// TODO : Improve perfs of this method based on mReadable for non debug versions
	std::string typeStr;
	mParserXml.GetAttribute("type", typeStr);
	if (IsNumber(typeStr))
	{
		return FromString<en::U32>(typeStr);
	}
	else
	{
		return Hash::SlowHash(typeStr.c_str());
	}
}

} // namespac en
