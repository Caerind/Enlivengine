#include <Enlivengine/Graphics/Tilemap.hpp>

#include <Enlivengine/Utils/Assert.hpp>

namespace en
{

Tilemap::Tilemap()
{
    enAssert(sf::VertexBuffer::isAvailable());
    mVertexBuffer.setPrimitiveType(sf::PrimitiveType::Triangles); // sf::Quads not supported on Android
    mVertexBuffer.setUsage(sf::VertexBuffer::Usage::Dynamic);
}

void Tilemap::SetTileset(const Tileset& tileset)
{
	mTileset = &tileset;

	UpdateTexCoords();
	mDirtyVertexBuffer = true;
}

const Tileset* Tilemap::GetTileset() const
{
	return mTileset;
}

void Tilemap::SetSize(const Vector2u& size)
{
	if (mSize != size)
	{
		mSize = size;

		mTiles.resize(mSize.x * mSize.y, 0);
		mVertices.resize(mSize.x * mSize.y * 6, sf::Vertex());

        UpdatePositions();
        UpdateTexCoords();
        mDirtyVertexBuffer = true;
	}
}

const Vector2u& Tilemap::GetSize() const
{
	return mSize;
}

void Tilemap::SetTileSize(const Vector2f& tileSize)
{
    if (mTileSize != tileSize)
    {
        mTileSize = tileSize;
        
        UpdatePositions();
        mDirtyVertexBuffer = true;
    }
}

const Vector2f& Tilemap::GetTileSize() const
{
    return mTileSize;
}

void Tilemap::SetTile(const Vector2u& tileCoords, U32 tileID)
{
	enAssert(tileCoords.x < mSize.x && tileCoords.y < mSize.y);
	const U32 tileIndex = tileCoords.x + mSize.x * tileCoords.y;
	if (mTiles[tileIndex] != tileID)
	{
		mTiles[tileIndex] = tileID;

        UpdateTileTexCoords(tileIndex);
        mDirtyVertexBuffer = true;
	}
}

U32 Tilemap::GetTile(const Vector2u& tileCoords) const
{
	enAssert(tileCoords.x < mSize.x&& tileCoords.y < mSize.y);
	const U32 tileIndex = tileCoords.x + mSize.x * tileCoords.y;
	return mTiles[tileIndex];
}

void Tilemap::Render(RenderTarget& target, const Matrix4f& transform)
{
	if (mVertices.size() > 0)
    {
		if (mDirtyVertexBuffer)
		{
			UpdateVertexBuffer();
            mDirtyVertexBuffer = false;
		}

        sf::RenderStates states;
        states.transform.translate(transform.GetTranslation().x, transform.GetTranslation().y);
        states.texture = &(mTileset->GetTexture().GetPtr()->GetSFMLTexture());
        target.GetSFMLTarget()->draw(mVertexBuffer, states);
	}
}

void Tilemap::UpdatePositions()
{
    if (mTileSize.x == 0.0f || mTileSize.y == 0.0f || mSize.x == 0 || mSize.y == 0)
        return;
        
    Vector2u v;
    for (v.y = 0; v.y < mSize.y; ++v.y)
	{
	    for (v.x = 0; v.x < mSize.x; ++v.x)
		{
			const U32 vertexIndex = 6 * (v.x + mSize.x * v.y);

			const auto topLeft = sf::Vector2f(v.x * mTileSize.x, v.y * mTileSize.y);
			const auto topRight = sf::Vector2f((v.x + 1) * mTileSize.x, v.y * mTileSize.y);
			const auto botRight = sf::Vector2f((v.x + 1) * mTileSize.x, (v.y + 1) * mTileSize.y);
			const auto botLeft = sf::Vector2f(v.x * mTileSize.x, (v.y + 1) * mTileSize.y);

			mVertices[vertexIndex + 0].position = topLeft;
			mVertices[vertexIndex + 1].position = topRight;
			mVertices[vertexIndex + 2].position = botLeft;
            mVertices[vertexIndex + 3].position = topRight;
			mVertices[vertexIndex + 4].position = botRight;
			mVertices[vertexIndex + 5].position = botLeft;
		}
	}
}

void Tilemap::UpdateTexCoords()
{
	if (mTileset != nullptr)
	{
		Vector2u v;
		for (v.y = 0; v.y < mSize.y; ++v.y)
		{
			for (v.x = 0; v.x < mSize.x; ++v.x)
			{
				const U32 tileIndex = v.x + mSize.x * v.y;
				UpdateTileTexCoords(tileIndex);
			}
		}
	}
}

void Tilemap::UpdateTileTexCoords(U32 tileIndex)
{
	if (mTileset != nullptr && mVertices.size() > 0)
	{
		const Vector2f texCoords = mTileset->GetTexCoords(mTiles[tileIndex]);
		const Vector2f tileSize = mTileset->GetTileSize();
		
		if (Texture* texturePtr = mTileset->GetTexture().GetPtr())
		{
			if (texturePtr->IsValid())
			{
				enAssert(texturePtr->GetWidth() > 0);
				enAssert(texturePtr->GetHeight() > 0);

				const U32 vertexIndex = tileIndex * 6;

                const auto topLeft = sf::Vector2f(texCoords.x, texCoords.y);
                const auto topRight = sf::Vector2f(texCoords.x + tileSize.x, texCoords.y);
                const auto botRight = sf::Vector2f(texCoords.x + tileSize.x, texCoords.y + tileSize.y);
                const auto botLeft = sf::Vector2f(texCoords.x, texCoords.y + tileSize.y);
				
				mVertices[vertexIndex + 0].texCoords = topLeft;
                mVertices[vertexIndex + 1].texCoords = topRight;
                mVertices[vertexIndex + 2].texCoords = botLeft;
                mVertices[vertexIndex + 3].texCoords = topRight;
                mVertices[vertexIndex + 4].texCoords = botRight;
                mVertices[vertexIndex + 5].texCoords = botLeft;
			}
		}
	}
}

void Tilemap::UpdateVertexBuffer()
{
	if (mVertices.size() > 0)
    {
        if (mVertexBuffer.getVertexCount() != mVertices.size())
        {
            mVertexBuffer.create(mVertices.size());
        }
        mVertexBuffer.update(&mVertices[0], mVertices.size(), 0);
	}
}

} // namespace en