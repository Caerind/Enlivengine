#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_TILED

#include <Enlivengine/Math/Rect.hpp>
#include <Enlivengine/Tiled/ObjectBase.hpp>

namespace en
{
namespace tmx
{

class RectangleObject : public ObjectBase
{
public:
	RectangleObject(ObjectGroup& objectGroup);

	virtual ObjectType GetObjectType() const
	{
		return ObjectBase::ObjectType::Rectangle;
	}

	const Vector2f& GetSize() const;

    Rectf GetRect() const;

private:
	friend class ObjectGroup;
	bool Parse(ParserXml& parser);

	Vector2f mSize;
};

} // namespace tmx
} // namespace en

#endif // ENLIVE_MODULE_TILED