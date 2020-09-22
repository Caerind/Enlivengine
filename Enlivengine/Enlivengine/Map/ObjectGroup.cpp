#include <Enlivengine/Map/ObjectGroup.hpp>

#include <Enlivengine/Map/RectangleObject.hpp>
#include <Enlivengine/Map/PointObject.hpp>
#include <Enlivengine/Map/EllipseObject.hpp>
#include <Enlivengine/Map/PolygonObject.hpp>

namespace en
{
namespace tmx
{

ObjectGroup::ObjectGroup(Map& map)
	: LayerBase(map)
	, mColor(Color::Transparent)
	, mObjects()
{
}

const Color& ObjectGroup::GetColor() const
{
	return mColor;
}

ObjectBase* ObjectGroup::GetObjectByIndex(U32 objectIndex)
{
	enAssert(objectIndex < GetObjectCount());
	return mObjects[objectIndex].get();
}

ObjectBase::ObjectType ObjectGroup::GetObjectTypeByIndex(U32 objectIndex) const
{
	enAssert(objectIndex < GetObjectCount());
	return mObjects[objectIndex]->GetObjectType();
}

ObjectBase* ObjectGroup::GetObjectByID(U32 objectID)
{
	for (ObjectBase::Ptr& object : mObjects)
	{
		if (object->GetID() == objectID)
		{
			return object.get();
		}
	}
	return nullptr;
}

ObjectBase::ObjectType ObjectGroup::GetObjectTypeByID(U32 objectID) const
{
	for (const ObjectBase::Ptr& object : mObjects)
	{
		if (object->GetID() == objectID)
		{
			return object->GetObjectType();
		}
	}
	enAssert(false);
	return ObjectBase::ObjectType::Rectangle;
}

U32 ObjectGroup::GetObjectCount() const
{
	return static_cast<U32>(mObjects.size());
}

bool ObjectGroup::Parse(ParserXml& parser)
{
	if (!LayerBase::Parse(parser))
	{
		return false;
	}

	std::string attribStr = "";
	parser.GetAttribute("color", attribStr);
	if (attribStr.size() > 0)
	{
		if (attribStr[0] == '#')
		{
			mColor.fromString(attribStr.substr(1));
		}
		else
		{
			mColor.fromString(attribStr);
		}
	}

	// TODO : DrawOrder
	/*
    attribStr = "";
	parser.getAttribute("draworder", attribStr);
	if (attribStr == "")
	{

	}
	else if (attribStr == "index")
	{

	}
	else
	{

	}
    */

	if (parser.ReadNode("object"))
	{
		do 
		{
			if (parser.HasNode("point"))
			{
				std::unique_ptr<PointObject> object = std::make_unique<PointObject>(*this);
				if (object != nullptr && object->Parse(parser))
				{
					mObjects.emplace_back(std::move(object));
				}
			}
			else if (parser.HasNode("ellipse"))
			{
				std::unique_ptr<EllipseObject> object = std::make_unique<EllipseObject>(*this);
				if (object != nullptr && object->Parse(parser))
				{
					mObjects.emplace_back(std::move(object));
				}
			}
			else if (parser.HasNode("polygon"))
			{
				std::unique_ptr<PolygonObject> object = std::make_unique<PolygonObject>(*this);
				if (object != nullptr && object->Parse(parser))
				{
					mObjects.emplace_back(std::move(object));
				}
			}
			else
			{
				// TODO : Do other object types

				std::unique_ptr<RectangleObject> object = std::make_unique<RectangleObject>(*this);
				if (object != nullptr && object->Parse(parser))
				{
					mObjects.emplace_back(std::move(object));
				}
			}

		} while (parser.NextSibling("object"));
		parser.CloseNode();
	}

	return true;
}

} // namespace tmx
} // namespace en