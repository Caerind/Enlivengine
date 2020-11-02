#include <Enlivengine/Tiled/PolygonObject.hpp>

#ifdef ENLIVE_MODULE_TILED

#include <Enlivengine/Utils/Assert.hpp>
#include <Enlivengine/Utils/String.hpp>

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
			std::string token2;
			Split(token, token2, ',');
			Vector2f point;
			point.x = FromString<F32>(token2);
			point.y = FromString<F32>(token);
			mPoints.push_back(point);
		}
	}
	parser.CloseNode();

	return true;
}

} // namespace tmx
} // namespace en

#endif // ENLIVE_MODULE_TILED