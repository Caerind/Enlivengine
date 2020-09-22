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
	if (!xml.LoadFromFile(filename))
	{
		enLogError(en::LogChannel::Map, "Can't open tileset file at {}", filename.c_str());
		return false;
	}

	if (xml.ReadNode("tileset"))
	{
		xml.GetAttribute("name", mName);
		xml.GetAttribute("tilewidth", mTileSize.x);
		xml.GetAttribute("tileheight", mTileSize.y);
		xml.GetAttribute("tilecount", mTileCount);
		xml.GetAttribute("columns", mColumns);
		xml.GetAttribute("spacing", mSpacing);
		xml.GetAttribute("margin", mMargin);

		mPath = std::filesystem::path(filename).remove_filename().string();

		if (xml.ReadNode("image"))
		{
			xml.GetAttribute("source", mImageSource);
			
			if (xml.HasAttribute("trans"))
			{
				std::string transparentStr;
				xml.GetAttribute("trans", transparentStr);
				mImageTransparent.fromString(transparentStr);
			}

			xml.CloseNode();
		}

		xml.CloseNode();
	}
	else
	{
		enLogError(en::LogChannel::Map, "Invalid tileset file at {}", filename.c_str());
		return false;
	}

    // Update texture
    const std::string filepath = mPath + mImageSource;
    if (mImageTransparent != Color::Transparent)
    {
        enLogWarning(en::LogChannel::Map, "{} : Transparent color for Tileset isn't supported yet -> Use alpha values", filepath.c_str());
    }
    mTexture = ResourceManager::GetInstance().GetFromFilename<Texture>(filepath);
    if (!mTexture.IsValid())
	{
		enLogWarning(en::LogChannel::Map, "ResourceDependencyNeeded: {} from {}", filepath.c_str(), filename.c_str());
        mTexture = ResourceManager::GetInstance().Create<Texture>(GetIdentifier() + "-texture", TextureLoader::FromFile(filepath));
        if (!mTexture.IsValid())
        {
            enLogError(en::LogChannel::Map, "Can't load tileset texture : {}", filepath.c_str());
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
	return Vector2f::Zero();
}

} // namespace tmx
} // namespace en