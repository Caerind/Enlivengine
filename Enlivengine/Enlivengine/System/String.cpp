#include <Enlivengine/System/String.hpp>

#include <Enlivengine/System/Config.hpp>
#include <Enlivengine/System/Assert.hpp>

#include <algorithm> // std::transform
#include <cctype>

namespace en
{

void ltrim(std::string& s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch)
	{
		return !std::isspace(ch);
	}));
}

void rtrim(std::string& s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch)
	{
		return !std::isspace(ch);
	}).base(), s.end());
}

void trim(std::string& s)
{
	ltrim(s);
	rtrim(s);
}

std::string ltrim_copy(std::string s)
{
	ltrim(s);
	return s;
}

std::string rtrim_copy(std::string s)
{
	rtrim(s);
	return s;
}

std::string trim_copy(std::string s)
{
	trim(s);
	return s;
}

void toLower(std::string& string)
{
	std::transform(string.begin(), string.end(), string.begin(), [](unsigned char c) -> unsigned char
	{
		return static_cast<unsigned char>(std::tolower(c));
	});
}

void toUpper(std::string& string)
{
	std::transform(string.begin(), string.end(), string.begin(), [](unsigned char c) -> unsigned char
	{
		return static_cast<unsigned char>(std::toupper(c));
	});
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
