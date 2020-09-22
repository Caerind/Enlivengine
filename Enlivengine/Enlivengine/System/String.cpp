#include <Enlivengine/System/String.hpp>

#include <algorithm> // std::transform
#include <cctype>

#include <Enlivengine/System/Assert.hpp>

namespace en
{

void LTrim(std::string& s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch)
	{
		return (ch >= 0 && ch < 255) ? !std::isspace(ch) : false; // TODO : false or true ?
	}));
}

void RTrim(std::string& s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch)
	{
		return (ch >= 0 && ch < 255) ? !std::isspace(ch) : false; // TODO : false or true ?
	}).base(), s.end());
}

void Trim(std::string& s)
{
	LTrim(s);
	RTrim(s);
}

std::string LTrimCopy(std::string s)
{
	LTrim(s);
	return s;
}

std::string RTrimCopy(std::string s)
{
	RTrim(s);
	return s;
}

std::string TrimCopy(std::string s)
{
	Trim(s);
	return s;
}

void ToLower(std::string& string)
{
	std::transform(string.begin(), string.end(), string.begin(), [](unsigned char c) -> unsigned char
	{
		return static_cast<unsigned char>(std::tolower(c));
	});
}

void ToUpper(std::string& string)
{
	std::transform(string.begin(), string.end(), string.begin(), [](unsigned char c) -> unsigned char
	{
		return static_cast<unsigned char>(std::toupper(c));
	});
}

bool IsNumber(const std::string& string)
{
	return !string.empty() && std::find_if(string.begin(), string.end(), [](unsigned char c)
	{ 
		return !std::isdigit(c) && c != '-' && c != '.'; 
	}) == string.end();
}

void ToLower(const std::string& string, std::string& result)
{
	result = string;
	ToLower(result);
}

void ToUpper(const std::string& string, std::string& result)
{
	result = string;
	ToUpper(result);
}

bool Split(std::string& base, std::string& result, char separator)
{
	std::size_t found = base.find(separator);
	if (found != std::string::npos)
	{
		result = base.substr(0, found);
		base = base.substr(found + 1);
		return true;
	}
	return false;
}

bool Split(std::string& base, std::string& result, const std::string& separator)
{
	std::size_t found = base.find(separator);
	if (found != std::string::npos)
	{
		result = base.substr(0, found);
		base = base.substr(found + separator.size());
		return true;
	}
	return false;
}

bool Contains(const std::string& string, char c)
{
	return string.find(c) != std::string::npos;
}

bool Contains(const std::string& string, const std::string& c)
{
	return string.find(c) != std::string::npos;
}

bool LimitSize(std::string& string, U32 size)
{
	if (string.size() > size)
	{
		string = string.substr(0, size);
		return true;
	}
	return false;
}

bool LimitSize(const std::string& string, std::string& result, U32 size)
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

} // namespace en
