#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include <Enlivengine/Utils/Meta.hpp>
#include <Enlivengine/Math/Rect.hpp>
#include <Enlivengine/Math/Matrix4.hpp>
#include <Enlivengine/Graphics/Tileset.hpp>
#include <Enlivengine/Graphics/RenderTarget.hpp>

namespace en
{

class Tilemap
{
public:
    Tilemap();

	void SetTileset(const Tileset& tileset);
	const Tileset* GetTileset() const;

	void SetSize(const Vector2u& size);
	const Vector2u& GetSize() const;
	
	void SetTileSize(const Vector2f& tileSize);
	const Vector2f& GetTileSize() const;

	void SetTile(const Vector2u& tileCoords, U32 tileID);
	U32 GetTile(const Vector2u& tileCoords) const;
	
	void Render(RenderTarget& target, const Matrix4f& transform = en::Matrix4f::Identity());

private:
    void UpdatePositions();
    void UpdateTexCoords();
    void UpdateTileTexCoords(U32 tileIndex);
    void UpdateVertexBuffer();

private:
    sf::VertexBuffer mVertexBuffer;
    std::vector<sf::Vertex> mVertices;
    std::vector<U32> mTiles;
    const Tileset* mTileset;
    Vector2u mSize;
    Vector2f mTileSize;
	bool mDirtyVertexBuffer;
};

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::Tilemap, en::Type_ClassSerialization, en::Type_ClassEditor)
	ENLIVE_META_CLASS_MEMBER("size", &en::Tilemap::GetSize, &en::Tilemap::SetSize),
	ENLIVE_META_CLASS_MEMBER("tileSize", &en::Tilemap::GetTileSize, &en::Tilemap::SetTileSize)
ENLIVE_META_CLASS_END()