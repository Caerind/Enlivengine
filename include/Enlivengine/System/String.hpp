#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/System/Hash.hpp>

#include <map>
#include <sstream>
#include <string>
#include <locale>
#include <codecvt>

namespace en
{

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

void trimWhitespace(std::string& string);

inline bool empty(const std::string& string) { return string.size() == 0; } // Faster than the std::string::empty

std::string toNarrowString(const std::wstring& wideString);
std::wstring toWideString(const std::string& narrowString);

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
		static constexpr StringId hash(const char* string) { return Hash::CRC32(string); }
		static StringId hashAndStore(const std::string& string);
		static StringId hashAndStore(const char* string);

	private:
		static std::map<U32, const char*> gStrings;

	private:
		U32 mStringId;
};

inline bool operator==(U32 id, const StringId& stringId)
{
	return stringId == id;
}

} // namespace en