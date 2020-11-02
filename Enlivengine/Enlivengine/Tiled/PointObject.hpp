#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_TILED

#include <Enlivengine/Tiled/ObjectBase.hpp>

namespace en
{
namespace tmx
{

class PointObject : public ObjectBase
{
public:
	PointObject(ObjectGroup& objectGroup);

	virtual ObjectType GetObjectType() const
	{
		return ObjectBase::ObjectType::Point;
	}

private:
	friend class ObjectGroup;
	bool Parse(ParserXml& parser);
};

} // namespace tmx
} // namespace en

#endif // ENLIVE_MODULE_TILED