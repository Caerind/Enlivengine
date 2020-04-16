#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/System/Hash.hpp>

#include <unordered_map>
#include <sstream>
#include <string>
#include <locale>
#include <codecvt>

namespace en
{

constexpr U32 StringLength(const char* const str) { return *str ? 1 + StringLength(str + 1) : 0; }

void ltrim(std::string& s);
void rtrim(std::string& s);
void trim(std::string& s);
std::string ltrim_copy(std::string s);
std::string rtrim_copy(std::string s);
std::string trim_copy(std::string s);

void toLower(std::string& string);
void toUpper(std::string& string);
void toLower(const std::string& string, std::string& result);
void toUpper(const std::string& string, std::string& result);

bool split(std::string& base, std::string& result, char separator);
bool split(std::string& base, std::string& result, const std::string& separator);

bool contains(const std::string& string, char c);
bool contains(const std::string& string, const std::string& c);

bool limitSize(std::string& string, U32 size);
bool limitSize(const std::string& string, std::string& result, U32 size);

inline std::string toBoolString(bool value) { return (value) ? "true" : "false"; }

template <typename T>
std::string toString(const T& value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

template <> inline std::string toString<std::string>(const std::string& value)
{
	return value;
}

template <> inline std::string toString<bool>(const bool& value)
{
	return (value) ? "1" : "0";
}

template <> inline std::string toString<char>(const char& value)
{
	return std::string(1, value);
}

template <typename T>
T fromString(const std::string& string)
{
	T value;
	std::istringstream iss(string);
	iss >> value;
	return value;
}

template <> inline std::string fromString<std::string>(const std::string& string)
{
	return string;
}

template <> inline bool fromString<bool>(const std::string& string)
{
	if (string == "true" || string == "1")
	{
		return true;
	}
	return false;
}

template <> inline char fromString<char>(const std::string& string)
{
	if (string.size() >= 1)
	{
		return string[0];
	}
	return '\0';
}

// TODO : Fix string id hash

/*
class StringId
{
	public:
		inline constexpr StringId() : mStringId(U32_Max) { }
		inline constexpr StringId(U32 stringId) : mStringId(stringId) { }
		inline constexpr StringId(const StringId& stringId) : mStringId(stringId.mStringId) { }
		inline constexpr StringId(StringId&& stringId) : mStringId(stringId.mStringId) { stringId.mStringId = U32_Max; }

		inline constexpr StringId& operator=(U32 stringId) { mStringId = stringId; return *this; }
		inline constexpr StringId& operator=(const StringId& stringId) { mStringId = stringId.mStringId; return *this; }
		inline constexpr StringId& operator=(StringId&& stringId) { if (this != &stringId) { mStringId = stringId.mStringId; stringId.mStringId = U32_Max; } return *this; }

		inline constexpr bool operator==(U32 stringId) const { return mStringId == stringId; }
		inline constexpr bool operator==(const StringId& stringId) const { return mStringId == stringId.mStringId; }

		inline constexpr bool operator!=(U32 stringId) const { return !operator==(stringId); }
		inline constexpr bool operator!=(const StringId& stringId) const { return !operator==(stringId); }

		inline constexpr operator bool() const { return mStringId != U32_Max; }
		inline constexpr bool isValid() const { return mStringId != U32_Max; }
		inline bool isStored() const { return gStrings.find(mStringId) != gStrings.end(); }

		const char* getStringFromStorage() const;

		static StringId hash(const std::string& string);
		static StringId hash(const char* string) { return StringId(Hash::CRC32(string)); }
		static StringId hashAndStore(const std::string& string);
		static StringId hashAndStore(const char* string);

	private:
		static std::unordered_map<U32, const char*> gStrings;

	private:
		U32 mStringId;
};

inline bool operator==(U32 id, const StringId& stringId)
{
	return stringId == id;
}
*/

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

} // namespace en