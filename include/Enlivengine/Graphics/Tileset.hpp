#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

#include <Enlivengine/System/PrimitiveTypes.hpp>

#include <Enlivengine/Graphics/SFMLWrapper.hpp>

namespace en
{

// TODO : Read from .tsx

using TileId = U32;

class Tileset
{
	public:
		Tileset();

		void setFirstGid(TileId id);
		void setTileSize(const Vector2i& tileSize);
		void setSpacing(U32 spacing);
		void setMargin(U32 margin);
		void setTileCount(U32 tileCount);
		void setColumns(U32 columns);
		void setRelativePath(const std::string& path);
		void setImageSource(const std::string& source);
		void setImageTransparent(const Color& transparent);

		TileId getFirstGid() const;
		const Vector2i& getTileSize() const;
		U32 getSpacing() const;
		U32 getMargin() const;
		U32 getTileCount() const;
		U32 getColumns() const;
		const std::string& getRelativePath() const;
		const std::string& getImageSource() const;
		const Color& getImageTransparent() const;

		sf::Texture& getTexture();

		bool hasId(TileId gid);
		sf::Vector2f toPos(TileId gid);

	private:
		U32 mFirstGid;
		Vector2i mTileSize;
		U32 mSpacing;
		U32 mMargin;
		U32 mTileCount;
		U32 mColumns;
		std::string mRelativePath;
		std::string mImageSource;
		Color mImageTransparent;

		sf::Texture mTexture;

		bool mImageChanged;
};

} // namespace en