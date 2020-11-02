#include <Enlivengine/Tiled/PointObject.hpp>

#ifdef ENLIVE_MODULE_TILED

#include <Enlivengine/Utils/Assert.hpp>

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

#endif // ENLIVE_MODULE_TILED