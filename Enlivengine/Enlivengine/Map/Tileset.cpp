#include <Enlivengine/Map/Tileset.hpp>

#include <Enlivengine/System/ParserXml.hpp>
#include <filesystem>

namespace en
{
namespace tmx
{

Tileset::Tileset() 
	: mTileSize()
	, mSpacing(0)
	, mMargin(0)
	, mTileCount(0)
	, mColumns(0)
	, mName()
	, mPath()
	, mImageSource()
	, mImageTransparent(Color::Transparent)
	, mTexture()
{
}

bool Tileset::LoadFromFile(const std::string& filename)
{
	ParserXml xml;
	if (!xml.loadFromFile(filename))
	{
		LogError(en::LogChannel::Map, 9, "Can't open tileset file at %s", filename.c_str());
		return false;
	}

	if (xml.readNode("tileset"))
	{
		xml.getAttribute("name", mName);
		xml.getAttribute("tilewidth", mTileSize.x);
		xml.getAttribute("tileheight", mTileSize.y);
		xml.getAttribute("tilecount", mTileCount);
		xml.getAttribute("columns", mColumns);
		xml.getAttribute("spacing", mSpacing);
		xml.getAttribute("margin", mMargin);

		mPath = std::filesystem::path(filename).remove_filename().string();

		if (xml.readNode("image"))
		{
			xml.getAttribute("source", mImageSource);
			
			if (xml.hasAttribute("trans"))
			{
				std::string transparentStr;
				xml.getAttribute("trans", transparentStr);
				mImageTransparent.fromString(transparentStr);
			}

			xml.closeNode();
		}

		xml.closeNode();
	}
	else
	{
		LogError(en::LogChannel::Map, 9, "Invalid tileset file at %s", filename.c_str());
		return false;
	}

    // Update texture
    const std::string filepath = mPath + mImageSource;
    if (mImageTransparent != Color::Transparent)
    {
        LogWarning(en::LogChannel::Map, 8, "%s : Transparent color for Tileset isn't supported yet -> Use alpha values", filepath.c_str());
    }
    mTexture = ResourceManager::GetInstance().GetFromFilename<Texture>(filepath);
    if (!mTexture.IsValid())
	{
		LogWarning(en::LogChannel::Map, 5, "ResourceDependencyNeeded: %s from %s", filepath.c_str(), filename.c_str());
        mTexture = ResourceManager::GetInstance().Create<Texture>(GetIdentifier() + "-texture", TextureLoader::FromFile(filepath));
        if (!mTexture.IsValid())
        {
            LogError(en::LogChannel::Map, 10, "Can't load tileset texture : %s", filepath.c_str());
        }
    }

	return mTexture.IsValid();
}

const Vector2u& Tileset::GetTileSize() const
{
	return mTileSize;
}

U32 Tileset::GetSpacing() const
{
	return mSpacing;
}

U32 Tileset::GetMargin() const
{
	return mMargin;
}

U32 Tileset::GetTileCount() const
{
	return mTileCount;
}

U32 Tileset::GetColumns() const
{
	return mColumns;
}

const std::string& Tileset::GetName() const
{
	return mName;
}

const std::string& Tileset::GetPath() const
{
	return mPath;
}

const std::string& Tileset::GetImageSource() const
{
	return mImageSource;
}

const Color& Tileset::GetImageTransparent() const
{
	return mImageTransparent;
}

TexturePtr& Tileset::GetTexture()
{
	return mTexture;
}

const TexturePtr& Tileset::GetTexture() const
{
    return mTexture;
}

Vector2f Tileset::ToPos(U32 tileId) const
{
	if (mColumns > 0)
	{
		return {
			(F32)((tileId % mColumns) * (mTileSize.x + mSpacing) + mMargin),
			(F32)((tileId / mColumns) * (mTileSize.y + mSpacing) + mMargin)
		};
	}
	return Vector2f::zero;
}

} // namespace tmx
} // namespace en