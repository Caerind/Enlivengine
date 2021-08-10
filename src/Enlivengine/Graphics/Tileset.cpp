#include <Enlivengine/Graphics/Tileset.hpp>

namespace en
{

Tileset::Tileset()
	: mTexture()
	, mTileSize(0, 0)
	, mGridSize(0, 0)
	, mSpacing(0)
	, mMargin(0)
{
}

Tileset::~Tileset()
{
}

void Tileset::SetTexture(TexturePtr texture)
{
	mTexture = texture;
}

TexturePtr Tileset::GetTexture() const
{
	return mTexture;
}

void Tileset::SetTileSize(const Vector2u& tileSize)
{
	mTileSize = tileSize;
}

const Vector2u& Tileset::GetTileSize() const
{
	return mTileSize;
}

void Tileset::SetGridSize(const Vector2u& gridSize)
{
	mGridSize = gridSize;
}

const Vector2u& Tileset::GetGridSize() const
{
	return mGridSize;
}

U32 Tileset::GetTileCount() const
{
	return mGridSize.x * mGridSize.y;
}

void Tileset::SetSpacing(U32 spacing)
{
	mSpacing = spacing;
}

U32 Tileset::GetSpacing() const
{
	return mSpacing;
}

void Tileset::SetMargin(U32 margin)
{
	mMargin = margin;
}

U32 Tileset::GetMargin() const
{
	return mMargin;
}

Vector2f Tileset::GetTexCoords(U32 tileID) const
{
	if (mGridSize.x > 0)
	{
		return Vector2f(static_cast<F32>((tileID % mGridSize.x) * (mTileSize.x + mSpacing) + mMargin), static_cast<F32>((tileID / mGridSize.x) * (mTileSize.y + mSpacing) + mMargin));
	}
	else
	{
		return Vector2f::Zero();
	}
}

} // namespace en