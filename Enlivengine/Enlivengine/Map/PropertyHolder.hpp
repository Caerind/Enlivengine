#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/System/Hash.hpp>
#include <Enlivengine/System/ParserXml.hpp>
#include <Enlivengine/Math/Color.hpp>

#include <memory>
#include <unordered_map>

namespace en
{
namespace tmx
{

class PropertyHolder
{
protected:
	PropertyHolder();

	bool Parse(ParserXml& parser);

public:
	enum class PropertyType
	{
		Boolean,
		Color,
		File,
		Float,
		Int,
		String
	};

	bool HasProperty(const std::string& name) const;
	PropertyType GetPropertyType(const std::string& name) const;

	bool GetBooleanProperty(const std::string& name) const;
	const Color& GetColorProperty(const std::string& name) const;
	F32 GetFloatProperty(const std::string& name) const;
	const std::string& GetFileProperty(const std::string& name) const;
	I32 GetIntProperty(const std::string& name) const;
	const std::string& GetStringProperty(const std::string& name) const;

private:
	std::unordered_map<U32, PropertyType> mTypeMap;

	std::unique_ptr<std::unordered_map<U32, bool>> mBooleans;
    std::unique_ptr<std::unordered_map<U32, Color>> mColors;
    std::unique_ptr<std::unordered_map<U32, F32>> mFloats;
    std::unique_ptr<std::unordered_map<U32, std::string>> mFiles;
    std::unique_ptr<std::unordered_map<U32, I32>> mInts;
    std::unique_ptr<std::unordered_map<U32, std::string>> mStrings;
};

} // namespace tmx
} // namespace en