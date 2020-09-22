#include <Enlivengine/Map/RectangleObject.hpp>

namespace en
{
namespace tmx
{

RectangleObject::RectangleObject(ObjectGroup& objectGroup)
	: ObjectBase(objectGroup)
	, mSize(0.0f, 0.0f)
{
}

const Vector2f& RectangleObject::GetSize() const
{
	return mSize;
}

Rectf RectangleObject::GetRect() const
{
    return Rectf(mPosition, mSize);
}

bool RectangleObject::Parse(ParserXml& parser)
{
	if (!ObjectBase::Parse(parser))
	{
		return false;
	}

	parser.GetAttribute("width", mSize.x);
	parser.GetAttribute("height", mSize.y);

	return true;
}

} // namespace tmx
} // namespace en