#include <Enlivengine/Map/PolygonObject.hpp>

#include <Enlivengine/System/String.hpp>
#include <Enlivengine/Math/MathString.hpp>

namespace en
{
namespace tmx
{

PolygonObject::PolygonObject(ObjectGroup& objectGroup)
	: ObjectBase(objectGroup)
	, mPoints()
{
}

const Vector2f& PolygonObject::GetPoint(U32 pointIndex) const
{
	assert(pointIndex < GetPointCount());
	return mPoints[pointIndex];
}

U32 PolygonObject::GetPointCount() const
{
	return static_cast<U32>(mPoints.size());
}

bool PolygonObject::Parse(ParserXml& parser)
{
	if (!ObjectBase::Parse(parser))
	{
		return false;
	}

	assert(parser.hasChild("polygon"));

	parser.readNode("polygon");
	{
		std::string pointsStr;
		parser.getAttribute("points", pointsStr);

		std::string token;
		while (split(pointsStr, token, ' '))
		{
			mPoints.push_back(fromString<Vector2f>(token));
		}
	}
	parser.closeNode();

	return true;
}

} // namespace tmx
} // namespace en