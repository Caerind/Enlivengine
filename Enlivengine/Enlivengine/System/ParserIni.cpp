#include <Enlivengine/System/ParserIni.hpp>

#include <fstream>

#include <Enlivengine/System/Assert.hpp>
#include <Enlivengine/System/Hash.hpp>

namespace en
{

ParserIni::ParserIni()
	: mValues()
	, mFilename("")
{
}

ParserIni::ParserIni(const std::string& filename)
	: mValues()
	, mFilename("")
{
	loadFromFile(filename);
}

bool ParserIni::loadFromFile(const std::string& filename)
{
	std::ifstream file(filename);
	if (file)
	{
		mFilename = filename;
		std::string currentSection = "";
		std::string line;
		while (std::getline(file, line))
		{
			if (line.size() > 0 && line[0] == '[' && line.back() == ']')
			{
				currentSection = line.substr(1, line.size() - 2);
			}
			else if (line.size() > 0)
			{
				std::string index;
				std::string value;
				size_t found = line.find('=');
				if (found != std::string::npos)
				{
					index = line.substr(0, found);
					value = line.substr(found + 1);
					mValues.emplace_back(value, index, currentSection);
				}
			}
		}
		file.close();
		return true;
	}
	return false;
}

bool ParserIni::saveToFile(const std::string& filename)
{
	std::ofstream file((!filename.empty()) ? filename : mFilename);
	if (file && mValues.size() > 0)
	{
		for (U32 i = 0; i < mValues.size(); i++)
		{
			mValues[i].saved = false;
		}

		bool writedLine = false;

		std::string currentSection("");
		for (U32 i = 0; i < mValues.size(); i++)
		{
			if (!mValues[i].saved)
			{
				currentSection = mValues[i].section;
				if (writedLine && currentSection.size() > 0)
				{
					file << '\n';
				}
				if (currentSection.size() > 0)
				{
					file << '[' << currentSection << "]\n";
					writedLine = true;
				}
				for (U32 j = 0; j < mValues.size(); j++)
				{
					if (mValues[j].section == currentSection && !mValues[j].saved)
					{
						file << mValues[j].index << "=" << mValues[j].value << '\n';
						mValues[j].saved = true;
						writedLine = true;
					}
				}
			}
		}
		file.close();
		return true;
	}
	return false;
}

void ParserIni::set(const std::string& value, const std::string& index, const std::string& section)
{
	U32 key = Hash::CRC32(std::string(section + ":" + index).c_str());
	bool found = false;
	for (U32 i = 0; i < mValues.size(); i++)
	{
		if (mValues[i].hash == key)
		{
			found = true;
			mValues[i].value = value;
			return;
		}
	}
	if (!found)
	{
		mValues.emplace_back(value, index, section);
		return;
	}
}

std::string ParserIni::get(const std::string& index, const std::string& section)
{
	U32 key = Hash::CRC32(std::string(section + ":" + index).c_str());
	for (U32 i = 0; i < mValues.size(); i++)
	{
		if (mValues[i].hash == key)
		{
			return mValues[i].value;
		}
	}
	return "";
}

ParserIni::IniProperty& ParserIni::getProperty(U32 index)
{
	assert(index < mValues.size());
	return mValues[index];
}

const ParserIni::IniProperty& ParserIni::getProperty(U32 index) const
{
	assert(index < mValues.size());
	return mValues[index];
}

U32 ParserIni::getSize() const
{
	return static_cast<U32>(mValues.size());
}

const std::string& ParserIni::getFilename() const
{
	return mFilename;
}

ParserIni::IniProperty::IniProperty(const std::string& pValue, const std::string& pIndex, const std::string& pSection)
	: hash(Hash::CRC32(std::string(pSection + ":" + pIndex).c_str()))
	, value(pValue)
	, index(pIndex)
	, section(pSection)
	, saved(false)
{
}

} // namespace en