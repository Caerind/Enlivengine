#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_TILED

#include <Enlivengine/Tiled/Map.hpp>
#include <Enlivengine/Tiled/LayerBase.hpp>
#include <Enlivengine/Tiled/ObjectBase.hpp>

namespace en
{
namespace tmx
{

class ObjectGroup : public LayerBase
{
public:
	ObjectGroup(Map& map);

	virtual LayerType GetLayerType() const 
	{ 
		return LayerBase::LayerType::ObjectGroup;
	}

	const Color& GetColor() const;

	ObjectBase* GetObjectByIndex(U32 objectIndex);
	ObjectBase::ObjectType GetObjectTypeByIndex(U32 objectIndex) const;
	template <typename T>
	T& GetObjectByIndexAs(U32 objectIndex)
	{
		return static_cast<T&>(*GetObjectByIndex(objectIndex));
	}
	ObjectBase* GetObjectByID(U32 objectID);
	ObjectBase::ObjectType GetObjectTypeByID(U32 objectID) const;
	template <typename T>
	T& GetObjectByIDAs(U32 objectID)
	{
		return static_cast<T&>(*GetObjectByIndex(objectID));
	}
	U32 GetObjectCount() const;

private:
	friend class Map;
	bool Parse(ParserXml& parser);

	Color mColor;

	std::vector<ObjectBase::Ptr> mObjects;
};

} // namespace tmx
} // namespace en

#endif // ENLIVE_MODULE_TILED