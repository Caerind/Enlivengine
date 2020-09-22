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
	enAssert(pointIndex < GetPointCount());
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

	enAssert(parser.HasNode("polygon"));

	parser.ReadNode("polygon");
	{
		std::string pointsStr;
		parser.GetAttribute("points", pointsStr);

		std::string token;
		while (Split(pointsStr, token, ' '))
		{
			mPoints.push_back(FromString<Vector2f>(token));
		}
	}
	parser.CloseNode();

	return true;
}

} // namespace tmx
} // namespace en