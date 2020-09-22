#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>

#include <Enlivengine/Math/Vector2.hpp>
#include <Enlivengine/Math/Color.hpp>
#include <Enlivengine/Graphics/SFMLResources.hpp>

#include <Enlivengine/Map/PropertyHolder.hpp>
#include <Enlivengine/Map/Tileset.hpp>
#include <Enlivengine/Map/LayerBase.hpp>

#include <SFML/Graphics/RenderTarget.hpp>

namespace en
{
namespace tmx
{

class Map : public Resource<Map>, public PropertyHolder
{
public:
	enum class Orientation
	{
		Orthogonal,
		Isometric,
		Staggered,
		Hexagonal
	};

	enum class RenderOrder
	{
		RightDown,
		RightUp,
		LeftDown,
		LeftUp
	};

	enum class StaggerAxis
	{
		X = 0,
		Y = 1
	};

	enum class StaggerIndex
	{
		Odd = 0,
		Even = 1
	};

	static U32 GetStaticResourceType() { return static_cast<U32>(ResourceType::Map); }
	U32 GetResourceType() const override { return GetStaticResourceType(); }

	Map();

	bool LoadFromFile(const std::string& filename);

	const std::string& GetName() const;
	const Vector2u& GetSize() const;
	const Vector2u& GetTileSize() const;
	Orientation GetOrientation() const;
	RenderOrder GetRenderOrder() const;
	const Color& GetBackgroundColor() const;
	StaggerAxis GetStaggerAxis() const;
	StaggerIndex GetStaggerIndex() const;
	U32 GetHexSideLength() const;

	U32 GetTilesetIndexFromGID(U32 gid) const;
	TilesetPtr GetTileset(U32 tilesetIndex) const;
	U32 GetTilesetFirstGid(U32 tilesetIndex) const;
	U32 GetTilesetCount() const;

	LayerBase* GetLayerByIndex(U32 layerIndex);
	LayerBase::LayerType GetLayerTypeByIndex(U32 layerIndex) const;
	template <typename T>
	T& GetLayerByIndexAs(U32 layerIndex)
	{
		return static_cast<T&>(*GetLayerByIndex(layerIndex));
	}
	LayerBase* GetLayerByID(U32 layerID);
	LayerBase::LayerType GetLayerTypeByID(U32 layerID) const;
	template <typename T>
	T& GetLayerByIDAs(U32 layerID)
	{
		return static_cast<T&>(*GetLayerByID(layerID));
	}
	U32 GetLayerCount() const;

	std::vector<Vector2u> GetNeighbors(const Vector2u& tileCoords, bool diag = false) const;
	Vector2f CoordsToWorld(const Vector2u& tileCoords) const;
	Vector2u WorldToCoords(const Vector2f& worldPos) const;

    void Render(sf::RenderTarget& target, bool renderObjects = false) const;

private:
	std::string mName;
	Vector2u mSize;
	Vector2u mTileSize;
	Orientation mOrientation;
	RenderOrder mRenderOrder;
	Color mBackgroundColor;
	StaggerAxis mStaggerAxis;
	StaggerIndex mStaggerIndex;
	U32 mHexSideLength;
	U32 mNextLayerID;
	U32 mNextObjectID;

	struct TilesetMapData
	{
		TilesetPtr tileset;
		U32 firstGid;
	};
	std::vector<TilesetMapData> mTilesets;

	std::vector<LayerBase::Ptr> mLayers;
};

using MapPtr = ResourcePtr<Map>;

class MapLoader
{
public:
	MapLoader() = delete;
	~MapLoader() = delete;

	static ResourceLoader<Map> FromFile(const std::string& filename)
	{
		return ResourceLoader<Map>([&filename](Map& r)
		{
			const bool result = r.LoadFromFile(filename);
			r.SetLoaded(result);
			r.SetLoadInfo(ResourceLoadInfo(true, filename));
			return result;
		});
	}
};

} // namespace tmx
} // namespace en