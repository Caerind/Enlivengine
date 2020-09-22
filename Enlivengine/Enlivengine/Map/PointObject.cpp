#include <Enlivengine/Map/PointObject.hpp>

namespace en
{
namespace tmx
{

PointObject::PointObject(ObjectGroup& objectGroup)
	: ObjectBase(objectGroup)
{
}

bool PointObject::Parse(ParserXml& parser)
{
	enAssert(parser.HasNode("point"));
	return ObjectBase::Parse(parser);
}

} // namespace tmx
} // namespace en