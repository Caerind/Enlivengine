#pragma once

#include <Enlivengine/Math/Vector2.hpp>
#include <Enlivengine/Graphics/Texture.hpp>
#include <Enlivengine/Resources/ResourceManager.hpp>

namespace en
{

class Tileset : public Resource<Tileset>
{
public:
	static U32 GetStaticResourceType() { return static_cast<U32>(ResourceType::Tileset); }
	U32 GetResourceType() const override { return GetStaticResourceType(); }

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

class TilesetLoader
{
public:
	TilesetLoader() = delete;

	static ResourceLoader<Tileset> FromCode(TexturePtr texture, const Vector2u& tileSize, const Vector2u& gridSize, U32 spacing = 0, U32 margin = 0)
	{
		return ResourceLoader<Tileset>([&](Tileset& r)
			{
				r.SetTexture(texture);
				r.SetTileSize(tileSize);
				r.SetGridSize(gridSize);
				r.SetSpacing(spacing);
				r.SetMargin(margin);
				
				const bool result = true;
				r.SetLoaded(result);
				r.SetLoadInfo(ResourceLoadInfo(ResourceLoadInfo::Procedural));
				return result;
			});
	}
};

using TilesetPtr = ResourcePtr<Tileset>;

} // namespace en