#include <Enlivengine/Map/PropertyHolder.hpp>

#include <Enlivengine/System/String.hpp>
#include <Enlivengine/System/Log.hpp>

namespace en
{
namespace tmx
{

PropertyHolder::PropertyHolder()
    : mTypeMap()
    , mBooleans(nullptr)
    , mColors(nullptr)
    , mFloats(nullptr)
    , mFiles(nullptr)
    , mInts(nullptr)
    , mStrings(nullptr)
{
}

bool PropertyHolder::Parse(ParserXml& parser)
{
	if (parser.readNode("property"))
	{
		do 
		{
			std::string nameStr = "";
			parser.getAttribute("name", nameStr);
			if (nameStr.size() == 0)
			{
				continue;
			}
			const U32 hash = Hash::CRC32(nameStr.c_str());

			std::string typeStr = "";
			parser.getAttribute("type", typeStr);
			if (typeStr.size() == 0)
			{
				continue;
			}
			std::string valueStr = "";
			parser.getAttribute("value", valueStr);
			if (typeStr == "bool")
			{
				mTypeMap[hash] = PropertyType::Boolean;
                if (mBooleans == nullptr)
                {
                    mBooleans = std::make_unique<std::unordered_map<U32, bool>>();
                }
				(*mBooleans)[hash] = fromString<bool>(valueStr);
			}
			else if (typeStr == "color")
			{
				mTypeMap[hash] = PropertyType::Color;
				if (valueStr.size() > 0)
				{
					valueStr = valueStr.substr(1);
				}
                if (mColors == nullptr)
                {
                    mColors = std::make_unique<std::unordered_map<U32, Color>>();
                }
				(*mColors)[hash].fromString(valueStr);
			}
			else if (typeStr == "file")
			{
				mTypeMap[hash] = PropertyType::File;
                if (mFiles == nullptr)
                {
                    mFiles = std::make_unique<std::unordered_map<U32, std::string>>();
                }
				(*mFiles)[hash] = valueStr;
			}
			else if (typeStr == "float")
			{
                mTypeMap[hash] = PropertyType::Float;
                if (mFloats == nullptr)
                {
                    mFloats = std::make_unique<std::unordered_map<U32, F32>>();
                }
				(*mFloats)[hash] = fromString<F32>(valueStr);
			}
			else if (typeStr == "int")
			{
                mTypeMap[hash] = PropertyType::Int;
                if (mInts == nullptr)
                {
                    mInts = std::make_unique<std::unordered_map<U32, I32>>();
                }
				(*mInts)[hash] = fromString<I32>(valueStr);
			}
			else if (typeStr == "string")
			{
                mTypeMap[hash] = PropertyType::String;
                if (mStrings == nullptr)
                {
                    mStrings = std::make_unique<std::unordered_map<U32, std::string>>();
                }
				(*mStrings)[hash] = valueStr;
			}
			else
			{
				LogError(en::LogChannel::Map, 6, "Unknown property type %s, property ignored", typeStr.c_str());
			}

		} while (parser.nextSibling("property"));
		parser.closeNode();
	}
	return true;
}

bool PropertyHolder::HasProperty(const std::string& name) const
{
	const U32 hash = Hash::CRC32(name.c_str());
	return mTypeMap.find(hash) != mTypeMap.end();
}

PropertyHolder::PropertyType PropertyHolder::GetPropertyType(const std::string& name) const
{
	assert(HasProperty(name));
	const U32 hash = Hash::CRC32(name.c_str());
	return mTypeMap.at(hash);
}

bool PropertyHolder::GetBooleanProperty(const std::string& name) const
{
	assert(GetPropertyType(name) == PropertyType::Boolean);
    assert(mBooleans != nullptr);
	const U32 hash = Hash::CRC32(name.c_str());
	return mBooleans->at(hash);
}

const Color& PropertyHolder::GetColorProperty(const std::string& name) const
{
    assert(GetPropertyType(name) == PropertyType::Color);
    assert(mColors != nullptr);
	const U32 hash = Hash::CRC32(name.c_str());
	return mColors->at(hash);
}

F32 PropertyHolder::GetFloatProperty(const std::string& name) const
{
    assert(GetPropertyType(name) == PropertyType::Float);
    assert(mFloats != nullptr);
	const U32 hash = Hash::CRC32(name.c_str());
	return mFloats->at(hash);
}

const std::string& PropertyHolder::GetFileProperty(const std::string& name) const
{
    assert(GetPropertyType(name) == PropertyType::File);
    assert(mFiles != nullptr);
	const U32 hash = Hash::CRC32(name.c_str());
	return mFiles->at(hash);
}

I32 PropertyHolder::GetIntProperty(const std::string& name) const
{
    assert(GetPropertyType(name) == PropertyType::Int);
    assert(mInts != nullptr);
	const U32 hash = Hash::CRC32(name.c_str());
	return mInts->at(hash);
}

const std::string& PropertyHolder::GetStringProperty(const std::string& name) const
{
    assert(GetPropertyType(name) == PropertyType::String);
    assert(mStrings != nullptr);
	const U32 hash = Hash::CRC32(name.c_str());
	return mStrings->at(hash);
}

} // namespace tmx
} // namespace en