#pragma once

#include <Enlivengine/Map/ObjectBase.hpp>

#include <vector>

namespace en
{
namespace tmx
{

class PolygonObject : public ObjectBase
{
public:
	PolygonObject(ObjectGroup& objectGroup);

	virtual ObjectType GetObjectType() const
	{
		return ObjectBase::ObjectType::Polygon;
	}

	const Vector2f& GetPoint(U32 pointIndex) const;
	U32 GetPointCount() const;

protected:
	friend class ObjectGroup;
	bool Parse(ParserXml& parser);

	std::vector<Vector2f> mPoints;
};

} // namespace tmx
} // namespace en