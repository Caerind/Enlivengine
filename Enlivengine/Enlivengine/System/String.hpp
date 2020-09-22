#pragma once

#include <codecvt>
#include <locale>
#include <sstream>
#include <string>

#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/System/Hash.hpp>

namespace en
{

void LTrim(std::string& s);
void RTrim(std::string& s);
void Trim(std::string& s);
std::string LTrimCopy(std::string s);
std::string RTrimCopy(std::string s);
std::string TrimCopy(std::string s);

void ToLower(std::string& string);
void ToUpper(std::string& string);
void ToLower(const std::string& string, std::string& result);
void ToUpper(const std::string& string, std::string& result);

bool IsNumber(const std::string& string);

bool Split(std::string& base, std::string& result, char separator);
bool Split(std::string& base, std::string& result, const std::string& separator);

bool Contains(const std::string& string, char c);
bool Contains(const std::string& string, const std::string& c);

bool LimitSize(std::string& string, U32 size);
bool LimitSize(const std::string& string, std::string& result, U32 size);

inline std::string ToBoolString(bool value) { return (value) ? "true" : "false"; }
inline bool FromBoolString(const std::string& string) { return string == "true" ? true : false; }

template <typename T>
std::string ToString(const T& value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

template <> inline std::string ToString<std::string>(const std::string& value)
{
	return value;
}

template <> inline std::string ToString<bool>(const bool& value)
{
	return (value) ? "1" : "0";
}

template <> inline std::string ToString<char>(const char& value)
{
	return std::string(1, value);
}

template <typename T>
T FromString(const std::string& string)
{
	T value;
	std::istringstream iss(string);
	iss >> value;
	return value;
}

template <> inline std::string FromString<std::string>(const std::string& string)
{
	return string;
}

template <> inline bool FromString<bool>(const std::string& string)
{
	return (string == "1" || string == "true");
}

template <> inline char FromString<char>(const std::string& string)
{
	return (string.size() >= 1) ? string[0] : '\0';
}

constexpr U32 StringLength(const char* const str) { return *str ? 1 + StringLength(str + 1) : 0; }

template <U32 N>
struct ConstexprStringStorage
{
	constexpr ConstexprStringStorage()
		: mData()
		, mSize(1)
	{
		mData[0] = '\0';
	}

	constexpr ConstexprStringStorage(const char* s1)
	{
		Add(s1);
	}

	constexpr ConstexprStringStorage(const char* s1, const char* s2)
	{
		Add(s1);
		Add(s2);
	}

	constexpr ConstexprStringStorage(const char* s1, const char* s2, const char* s3)
	{
		Add(s1);
		Add(s2);
		Add(s3);
	}

	constexpr ConstexprStringStorage(const char* s1, const char* s2, const char* s3, const char* s4)
	{
		Add(s1);
		Add(s2);
		Add(s3);
		Add(s4);
	}

	constexpr ConstexprStringStorage(const char* s1, const char* s2, const char* s3, const char* s4, const char* s5)
	{
		Add(s1);
		Add(s2);
		Add(s3);
		Add(s4);
		Add(s5);
	}

	constexpr void Add(const char* s)
	{
		const U32 length = StringLength(s);
		for (U32 i = 0; i < length; ++i)
		{
			mData[mSize++] = s[i];
		}
		mData[mSize] = '\0';
	}

	constexpr const char* GetData() const { return mData; }
	constexpr U32 GetSize() const { return mSize; }
	constexpr U32 GetCapacity() const { return N; }

	char mData[N]{};
	U32 mSize{ 0 };
};

namespace priv
{
	template <U8... digits> 
	struct positive_to_chars 
	{ 
		static constexpr const char value[] = { ('0' + digits)..., 0 }; 
	};

	template <U32 rem, U8... digits>
	struct explode : explode<rem / 10, rem % 10, digits...> {};

	template <U8... digits>
	struct explode<0, digits...> : positive_to_chars<digits...> {};

} // namespace priv

template <U32 number>
struct ConstexprIntToString : priv::explode<number>{};


} // namespace en