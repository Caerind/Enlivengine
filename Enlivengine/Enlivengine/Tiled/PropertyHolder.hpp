#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_TILED

#include <memory>
#include <unordered_map>

#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Utils/Hash.hpp>
#include <Enlivengine/Utils/ParserXml.hpp>
#include <Enlivengine/Math/Color.hpp>

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

	// TODO : Might have been cool, but I don't know if it really is...
	std::unique_ptr<std::unordered_map<U32, bool>> mBooleans;
    std::unique_ptr<std::unordered_map<U32, Color>> mColors;
    std::unique_ptr<std::unordered_map<U32, F32>> mFloats;
    std::unique_ptr<std::unordered_map<U32, std::string>> mFiles;
    std::unique_ptr<std::unordered_map<U32, I32>> mInts;
    std::unique_ptr<std::unordered_map<U32, std::string>> mStrings;
};

} // namespace tmx
} // namespace en

#endif // ENLIVE_MODULE_TILED