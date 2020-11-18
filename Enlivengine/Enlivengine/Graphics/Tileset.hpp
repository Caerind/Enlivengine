#pragma once

#include <Enlivengine/Math/Vector2.hpp>
#include <Enlivengine/Graphics/Texture.hpp>

namespace en
{

class Tileset
{
public:
	Tileset();
	~Tileset();

	void SetTexture(TexturePtr texture);
	TexturePtr GetTexture() const;

	void SetTileSize(const Vector2u& tileSize);
	const Vector2u& GetTileSize() const;

	void SetGridSize(const Vector2u& gridSize);
	const Vector2u& GetGridSize() const;
	U32 GetTileCount() const;

	void SetSpacing(U32 spacing);
	U32 GetSpacing() const;

	void SetMargin(U32 margin);
	U32 GetMargin() const;

	Vector2f GetTexCoords(U32 tileID) const;

private:
	TexturePtr mTexture;
	Vector2u mTileSize;
	Vector2u mGridSize;
	U32 mSpacing;
	U32 mMargin;
};

} // namespace en