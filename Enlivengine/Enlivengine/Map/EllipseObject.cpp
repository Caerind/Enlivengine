#include <Enlivengine/Map/EllipseObject.hpp>

namespace en
{
namespace tmx
{

EllipseObject::EllipseObject(ObjectGroup& objectGroup)
	: ObjectBase(objectGroup)
	, mSize(0.0f, 0.0f)
{
}

const Vector2f& EllipseObject::GetSize() const
{
	return mSize;
}

Vector2f EllipseObject::GetPointFromAngle(F32 angle) const
{
    return std::move(GetPointFromCosSine(Math::Cos(angle), Math::Sin(angle)));
}

Vector2f EllipseObject::GetPointFromCosSine(F32 cosValue, F32 sinValue) const
{
    return Vector2f(cosValue, sinValue) * mSize * 0.5f;
}

bool EllipseObject::Parse(ParserXml& parser)
{
	if (!ObjectBase::Parse(parser))
	{
		return false;
	}

	parser.GetAttribute("width", mSize.x);
	parser.GetAttribute("height", mSize.y);

	enAssert(parser.HasNode("ellipse"));

	return true;
}

} // namespace tmx
} // namespace en