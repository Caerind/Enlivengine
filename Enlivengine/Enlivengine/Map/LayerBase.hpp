#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/System/ParserXml.hpp>
#include <Enlivengine/Math/Vector2.hpp>
#include <Enlivengine/Map/PropertyHolder.hpp>

#include <SFML/Graphics/RenderTarget.hpp>

#include <memory>
#include <string>

namespace en
{
namespace tmx
{

class Map;

class LayerBase : public PropertyHolder
{
public:
	enum class LayerType
	{
		TileLayer,
		ObjectGroup,
		ImageLayer,
		LayerGroup
	};

	using Ptr = std::unique_ptr<LayerBase>;

	LayerBase(Map& map);
	virtual ~LayerBase() {}

	virtual LayerType GetLayerType() const = 0;

	Map& GetMap();
	const Map& GetMap() const;

	U32 GetID() const;
	const std::string& GetName() const;
	const Vector2f& GetOffset() const;
	F32 GetOpacity() const;
	bool IsVisible() const;
	bool IsLocked() const;

    virtual void Render(sf::RenderTarget& target) const;

protected:
	bool Parse(ParserXml& parser);

	Map& mMap;

	U32 mID;
	std::string mName;
	Vector2f mOffset;
	F32 mOpacity;
	bool mVisible;
	bool mLocked;
};

} // namespace tmx
} // namespace en