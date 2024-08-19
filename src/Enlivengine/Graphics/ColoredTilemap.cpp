#include <Enlivengine/Graphics/ColoredTilemap.hpp>

#include <Enlivengine/Utils/Assert.hpp>

namespace en
{

ColoredTilemap::ColoredTilemap()
{
    enAssert(sf::VertexBuffer::isAvailable());
    mVertexBuffer.setPrimitiveType(sf::PrimitiveType::Triangles); // sf::Quads not supported on Android
    mVertexBuffer.setUsage(sf::VertexBuffer::Usage::Dynamic);
}

void ColoredTilemap::SetSize(const Vector2u& size)
{
	if (mSize != size)
	{
		mSize = size;

		mTiles.resize(mSize.x * mSize.y, Color());
		mVertices.resize(mSize.x * mSize.y * 6, sf::Vertex());

        UpdatePositions();
        UpdateTexCoords();
        mDirtyVertexBuffer = true;
	}
}

const Vector2u& ColoredTilemap::GetSize() const
{
	return mSize;
}

void ColoredTilemap::SetTileSize(const Vector2f& tileSize)
{
    if (mTileSize != tileSize)
    {
        mTileSize = tileSize;
        
        UpdatePositions();
        mDirtyVertexBuffer = true;
    }
}

const Vector2f& ColoredTilemap::GetTileSize() const
{
    return mTileSize;
}

void ColoredTilemap::SetTileColor(const Vector2u& tileCoords, const Color& color)
{
	enAssert(tileCoords.x < mSize.x && tileCoords.y < mSize.y);
	const U32 tileIndex = tileCoords.x + mSize.x * tileCoords.y;
	if (mTiles[tileIndex] != color)
	{
		mTiles[tileIndex] = color;

        UpdateTileTexCoords(tileIndex);
        mDirtyVertexBuffer = true;
	}
}

const Color& ColoredTilemap::GetTileColor(const Vector2u& tileCoords) const
{
	enAssert(tileCoords.x < mSize.x&& tileCoords.y < mSize.y);
	const U32 tileIndex = tileCoords.x + mSize.x * tileCoords.y;
	return mTiles[tileIndex];
}

void ColoredTilemap::Render(RenderTarget& target, const Matrix4f& transform)
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
        target.GetSFMLTarget()->draw(mVertexBuffer, states);
	}
}

void ColoredTilemap::UpdatePositions()
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

void ColoredTilemap::UpdateTexCoords()
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

void ColoredTilemap::UpdateTileTexCoords(U32 tileIndex)
{
	if (mVertices.size() > 0)
	{
		const Color& color = mTiles[tileIndex];
        const sf::Color sfColor = sf::Color(color.r, color.g, color.b, color.a);

        const U32 vertexIndex = tileIndex * 6;

        mVertices[vertexIndex + 0].color = sfColor;
        mVertices[vertexIndex + 1].color = sfColor;
        mVertices[vertexIndex + 2].color = sfColor;
        mVertices[vertexIndex + 3].color = sfColor;
        mVertices[vertexIndex + 4].color = sfColor;
        mVertices[vertexIndex + 5].color = sfColor;
	}
}

void ColoredTilemap::UpdateVertexBuffer()
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