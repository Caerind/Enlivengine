#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_TILED

#include <memory>
#include <string>

#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Math/Vector2.hpp>
#include <Enlivengine/Utils/ParserXml.hpp>

#include <Enlivengine/Tiled/PropertyHolder.hpp>

namespace en
{
namespace tmx
{

class ObjectGroup;

class ObjectBase : public PropertyHolder
{
public:
	enum class ObjectType
	{
		Rectangle,
		Point,
		Ellipse,
		Polygon
	};

	using Ptr = std::unique_ptr<ObjectBase>;

	ObjectBase(ObjectGroup& objectGroup);
	virtual ~ObjectBase() {}

	virtual ObjectType GetObjectType() const = 0;

	ObjectGroup& GetObjectGroup();
	const ObjectGroup& GetObjectGroup() const;

    const std::string& GetName() const;
    const std::string& GetType() const;
	U32 GetID() const;
	const Vector2f& GetPosition() const;
    F32 GetRotation() const;

protected:
	bool Parse(ParserXml& parser);

	ObjectGroup& mObjectGroup;

    U32 mID;
    std::string mName;
    std::string mType;
	Vector2f mPosition;
    F32 mRotation;
};

} // namespace tmx
} // namespace en

#endif // ENLIVE_MODULE_TILED