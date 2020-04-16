#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>

#include <Enlivengine/Math/Vector2.hpp>
#include <Enlivengine/Graphics/Color.hpp>
#include <Enlivengine/Graphics/SFMLResources.hpp>

namespace en
{
namespace tmx
{

class Tileset : public Resource<Tileset>
{
	public:
		Tileset();

		bool LoadFromFile(const std::string& filename);

		const Vector2u& GetTileSize() const;
		U32 GetSpacing() const;
		U32 GetMargin() const;
		U32 GetTileCount() const;
		U32 GetColumns() const;
		const std::string& GetName() const;
		const std::string& GetPath() const;
		const std::string& GetImageSource() const;
		const Color& GetImageTransparent() const;

		TexturePtr& GetTexture();
        const TexturePtr& GetTexture() const;

		Vector2f ToPos(U32 tileId) const;

	private:
		Vector2u mTileSize;
		U32 mSpacing;
		U32 mMargin;
		U32 mTileCount;
		U32 mColumns;
		std::string mName;
		std::string mPath;

		std::string mImageSource;
		Color mImageTransparent;

		TexturePtr mTexture;
};

using TilesetPtr = ResourcePtr<Tileset>;

class TilesetLoader
{
public:
	TilesetLoader() = delete;
	~TilesetLoader() = delete;

	static ResourceLoader<Tileset> FromFile(const std::string& filename)
	{
		return ResourceLoader<Tileset>([&filename](Tileset& r)
		{
			const bool result = r.LoadFromFile(filename);
			r.mFilename = (result) ? filename : "";
			return result;
		});
	}
};

} // namespace tmx
} // namespace en