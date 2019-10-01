#include <Enlivengine/System/String.hpp>

#include <Enlivengine/System/Config.hpp>
#include <Enlivengine/System/Assert.hpp>

#include <algorithm> // std::transform

namespace en
{

std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> gWideConverter;

//std::unordered_map<U32, const char*> StringId::gStrings;

void toLower(std::string& string)
{
	std::transform(string.begin(), string.end(), string.begin(), ::tolower);
}

void toUpper(std::string& string)
{
	std::transform(string.begin(), string.end(), string.begin(), ::toupper);
}

void toLower(const std::string& string, std::string& result)
{
	result = string;
	toLower(result);
}

void toUpper(const std::string& string, std::string& result)
{
	result = string;
	toUpper(result);
}

bool split(std::string& base, std::string& result, char separator)
{
	size_t found = base.find(separator);
	if (found != std::string::npos)
	{
		result = base.substr(0, found);
		base = base.substr(found + 1);
		return true;
	}
	return false;
}

bool split(std::string& base, std::string& result, const std::string& separator)
{
	size_t found = base.find(separator);
	if (found != std::string::npos)
	{
		result = base.substr(0, found);
		base = base.substr(found + separator.size());
		return true;
	}
	return false;
}

bool contains(const std::string& string, char c)
{
	return string.find(c) != std::string::npos;
}

bool contains(const std::string& string, const std::string& c)
{
	return string.find(c) != std::string::npos;
}

bool limitSize(std::string& string, U32 size)
{
	if (string.size() > size)
	{
		string = string.substr(0, size);
		return true;
	}
	return false;
}

bool limitSize(const std::string& string, std::string& result, U32 size)
{
	if (string.size() > size)
	{
		result = string.substr(0, size);
		return true;
	}
	else
	{
		result = string;
		return false;
	}
}

void trimWhitespace(std::string& string)
{
	string = string.substr(0, string.find_last_not_of(" \t") + 1).substr(string.find_first_not_of(" \t"));
}

std::string toNarrowString(const std::wstring& wideString)
{
	return gWideConverter.to_bytes(wideString);
}

std::wstring toWideString(const std::string& narrowString)
{
	return gWideConverter.from_bytes(narrowString);
}

/*
const char* StringId::getStringFromStorage() const
{
	auto itr = gStrings.find(mStringId);
	if (itr != gStrings.end())
	{
		return itr->second;
	}
	return nullptr;
}

StringId StringId::hash(const std::string& string)
{
	return hash(string.c_str());
}

StringId StringId::hashAndStore(const std::string& string)
{
	return hashAndStore(string.c_str());
}

StringId StringId::hashAndStore(const char* string)
{
	U32 stringId = hash(string);

#ifdef ENLIVE_ENABLE_HASH_COLLISION_DETECTION
	auto itr = gStrings.find(stringId);
	if (itr != gStrings.end())
	{
		assert(strcmp(string, itr->second) == 0);
	}
#endif

	gStrings[stringId] = string;

	return StringId(stringId);
}
*/

} // namespace en
