#include <Enlivengine/Map/ObjectBase.hpp>

#include <Enlivengine/Map/ObjectGroup.hpp>

namespace en
{
namespace tmx
{

ObjectBase::ObjectBase(ObjectGroup& objectGroup)
	: mObjectGroup(objectGroup)
        , mID(0)
    , mName()
    , mType()
	, mPosition(0.0f, 0.0f)
    , mRotation(0.0f)
{
}

ObjectGroup& ObjectBase::GetObjectGroup()
{
	return mObjectGroup;
}

const ObjectGroup& ObjectBase::GetObjectGroup() const
{
	return mObjectGroup;
}

const std::string& ObjectBase::GetName() const
{
	return mName;
}

const std::string& ObjectBase::GetType() const
{
	return mType;
}

U32 ObjectBase::GetID() const
{
	return mID;
}

const Vector2f& ObjectBase::GetPosition() const
{
	return mPosition;
}

F32 ObjectBase::GetRotation() const
{
    return mRotation;
}

bool ObjectBase::Parse(ParserXml& parser)
{
	parser.GetAttribute("id", mID);
    parser.GetAttribute("name", mName);
    parser.GetAttribute("type", mType);
	parser.GetAttribute("x", mPosition.x);
	parser.GetAttribute("y", mPosition.y);
    parser.GetAttribute("rotation", mRotation);

	if (parser.ReadNode("properties"))
	{
		PropertyHolder::Parse(parser);
		parser.CloseNode();
	}

	return true;
}

} // namespace tmx
} // namespace en
