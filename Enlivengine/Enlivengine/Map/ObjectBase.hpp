#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/Math/Vector2.hpp>

#include <Enlivengine/System/ParserXml.hpp>

#include <Enlivengine/Map/PropertyHolder.hpp>

#include <memory>
#include <string>

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