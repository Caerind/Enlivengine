#include <Enlivengine/Map/Map.hpp>

#include <Enlivengine/System/ParserXml.hpp>
#include <filesystem>

#include <Enlivengine/Map/TileLayer.hpp>
#include <Enlivengine/Map/ObjectGroup.hpp>

namespace en
{
namespace tmx
{
	
Map::Map()
	: mName("")
	, mSize(0,0)
	, mTileSize(0,0)
	, mOrientation(Orientation::Orthogonal)
	, mRenderOrder(RenderOrder::RightDown)
	, mBackgroundColor(Color::Transparent)
	, mStaggerAxis(StaggerAxis::Y)
	, mStaggerIndex(StaggerIndex::Odd)
	, mHexSideLength(0)
	, mNextLayerID(0)
	, mNextObjectID(0)
{
}

bool Map::LoadFromFile(const std::string& filename)
{
	ParserXml xml;
	if (!xml.LoadFromFile(filename))
	{
		enLogError(en::LogChannel::Map, "Can't open file at {}", filename.c_str());
		return false;
	}

	if (xml.ReadNode("map"))
	{
		xml.GetAttribute("name", mName);

		std::string attribStr;
		
		attribStr = "orthogonal";
		xml.GetAttribute("orientation", attribStr);
		if (attribStr == "orthogonal")
		{
			mOrientation = Orientation::Orthogonal;
		}
		else if (attribStr == "isometric")
		{
			mOrientation = Orientation::Isometric;
		}
		else if (attribStr == "staggered")
		{
			mOrientation = Orientation::Staggered;
		}
		else if (attribStr == "hexagonal")
		{
			mOrientation = Orientation::Hexagonal;
		}
		else
		{
			enLogError(en::LogChannel::Map, "{} maps aren't supported yet", attribStr.c_str());
			return false;
		}

		attribStr = "right-down";
		xml.GetAttribute("renderorder", attribStr);
		if (attribStr == "right-down")
		{
			mRenderOrder = RenderOrder::RightDown;
		}
		else if (attribStr == "right-up")
		{
			mRenderOrder = RenderOrder::RightUp;
		}
		else if (attribStr == "left-down")
		{
			mRenderOrder = RenderOrder::LeftDown;
		}
		else if (attribStr == "left-up")
		{
			mRenderOrder = RenderOrder::LeftUp;
		}
		else
		{
			enLogWarning(en::LogChannel::Map, "Invalid renderorder {}", attribStr.c_str());
		}

		xml.GetAttribute("width", mSize.x);
		xml.GetAttribute("height", mSize.y);

		xml.GetAttribute("tilewidth", mTileSize.x);
		xml.GetAttribute("tileheight", mTileSize.y);

		xml.GetAttribute("hexsidelength", mHexSideLength);
		if (mOrientation == Orientation::Hexagonal && mHexSideLength <= 0)
		{
			enLogWarning(en::LogChannel::Map, "Invalid hexsidelength {}", attribStr.c_str());
		}

		attribStr = "y";
		xml.GetAttribute("staggeraxis", attribStr);
		if (attribStr == "y")
		{
			mStaggerAxis = StaggerAxis::Y;
		}
		else if (attribStr == "x")
		{
			mStaggerAxis = StaggerAxis::X;
		}
		else
		{
			if (mOrientation == Orientation::Staggered || mOrientation == Orientation::Hexagonal)
			{
				enLogWarning(en::LogChannel::Map, "Invalid staggeraxis {}", attribStr.c_str());
			}
		}

		attribStr = "odd";
		xml.GetAttribute("staggerindex", attribStr);
		if (attribStr == "odd")
		{
			mStaggerIndex = StaggerIndex::Odd;
		}
		else if (attribStr == "even")
		{
			mStaggerIndex = StaggerIndex::Even;
		}
		else
		{
			if (mOrientation == Orientation::Staggered || mOrientation == Orientation::Hexagonal)
			{
				enLogWarning(en::LogChannel::Map, "Invalid staggerindex {}", attribStr.c_str());
			}
		}

		attribStr = "";
		xml.GetAttribute("backgroundcolor", attribStr);
		if (attribStr.size() > 0)
		{
			if (attribStr[0] == '#')
			{
				mBackgroundColor.fromString(attribStr.substr(1));
			}
			else
			{
				mBackgroundColor.fromString(attribStr);
			}
		}

		I32 infinite = 0;
		xml.GetAttribute("infinite", infinite);
		if (infinite > 0)
		{
			// TODO : Infinite maps
			enLogError(en::LogChannel::Map, "Infinite maps aren't supported yet for {}", mName.c_str());
			return false;
		}

		xml.GetAttribute("nextlayerid", mNextLayerID);
		xml.GetAttribute("nextobjectid", mNextObjectID);

		if (xml.ReadFirstNode())
		{
			do 
			{
				std::string nodeName = xml.GetNodeName();
				if (nodeName == "tileset")
				{
					TilesetMapData tilesetData;
					tilesetData.tileset = TilesetPtr();
					tilesetData.firstGid = 0;

					bool validTileset = true;

					xml.GetAttribute("firstgid", tilesetData.firstGid);
					if (tilesetData.firstGid <= 0)
					{
						validTileset = false;
						enLogWarning(en::LogChannel::Map, "Invalid firstGid for {}", mName.c_str());
					}

					std::string source = "";
					xml.GetAttribute("source", source);
					if (source.size() > 0)
					{
						const std::string currentPath = std::filesystem::path(filename).remove_filename().string();
						source = currentPath + source;

						tilesetData.tileset = ResourceManager::GetInstance().GetFromFilename<Tileset>(source);
						if (!tilesetData.tileset.IsValid())
						{
							const std::string identifier = mName + "-tileset" + std::to_string(mTilesets.size());
							tilesetData.tileset = ResourceManager::GetInstance().Create<Tileset>(identifier, TilesetLoader::FromFile(source));
							if (!tilesetData.tileset.IsValid())
							{
								validTileset = false;
								enLogError(en::LogChannel::Map, "Can't load tileset {}", source.c_str());
							}
						}
					}
					else
					{
						validTileset = false;
						enLogError(en::LogChannel::Map, "Tileset inside maps aren't supported yet for {}", mName.c_str());
						// TODO : Tilesets inside maps
					}

					if (validTileset)
					{
						mTilesets.push_back(tilesetData);
					}
				}
				else if (nodeName == "layer")
				{
					std::unique_ptr<TileLayer> layer = std::make_unique<TileLayer>(*this);
					if (layer != nullptr && layer->Parse(xml))
					{
						mLayers.emplace_back(std::move(layer));
					}
				}
				else if (nodeName == "objectgroup")
				{
					std::unique_ptr<ObjectGroup> objectgroup = std::make_unique<ObjectGroup>(*this);
					if (objectgroup != nullptr && objectgroup->Parse(xml))
					{
						mLayers.emplace_back(std::move(objectgroup));
					}
				}
				else if (nodeName == "imagelayer")
				{
					// TODO : ImageLayer
				}
				else if (nodeName == "group")
				{
					// TODO : LayerGroup
				}
				else if (nodeName == "properties")
				{
					PropertyHolder::Parse(xml);
				}
				else
				{
					enLogError(en::LogChannel::Map, "Unknown layer type {}", nodeName.c_str());
				}
			} while (xml.NextSibling());
			xml.CloseNode();
		}
	}
	else
	{
		enLogError(en::LogChannel::Map, "Invalid map file at {}", filename.c_str());
		return false;
	}

	return true;
}

const std::string& Map::GetName() const
{
	return mName;
}

const Vector2u& Map::GetSize() const
{
	return mSize;
}

const Vector2u& Map::GetTileSize() const
{
	return mTileSize;
}

Map::Orientation Map::GetOrientation() const
{
	return mOrientation;
}

Map::RenderOrder Map::GetRenderOrder() const
{
	return mRenderOrder;
}

const Color& Map::GetBackgroundColor() const
{
	return mBackgroundColor;
}

Map::StaggerAxis Map::GetStaggerAxis() const
{
	return mStaggerAxis;
}

Map::StaggerIndex Map::GetStaggerIndex() const
{
	return mStaggerIndex;
}

U32 Map::GetHexSideLength() const
{
	return mHexSideLength;
}

U32 Map::GetTilesetIndexFromGID(U32 gid) const
{
	const U32 tilesetCount = GetTilesetCount();
	for (U32 tilesetIndex = 0; tilesetIndex < tilesetCount; ++tilesetIndex)
	{
		const U32 firstGid = GetTilesetFirstGid(tilesetIndex);
		if (firstGid <= gid)
		{
			gid -= firstGid;
			const TilesetPtr& tilesetPtr = mTilesets[tilesetIndex].tileset;
			if (tilesetPtr.IsValid())
			{
				// TODO : < or <= ?
				if (gid < tilesetPtr.Get().GetTileCount())
				{
					return tilesetIndex;
				}
			}
		}
	}
	return U32_Max;
}

TilesetPtr Map::GetTileset(U32 tilesetIndex) const
{
	enAssert(tilesetIndex < GetTilesetCount());
	return mTilesets[tilesetIndex].tileset;
}

U32 Map::GetTilesetFirstGid(U32 tilesetIndex) const
{
	enAssert(tilesetIndex < GetTilesetCount());
	return mTilesets[tilesetIndex].firstGid;
}

U32 Map::GetTilesetCount() const
{
	return static_cast<U32>(mTilesets.size());
}

LayerBase* Map::GetLayerByIndex(U32 layerIndex)
{
	enAssert(layerIndex < GetLayerCount());
	return mLayers[layerIndex].get();
}

LayerBase::LayerType Map::GetLayerTypeByIndex(U32 layerIndex) const
{
	enAssert(layerIndex < GetLayerCount());
	return mLayers[layerIndex]->GetLayerType();
}

LayerBase* Map::GetLayerByID(U32 layerID)
{
	for (LayerBase::Ptr& layer : mLayers)
	{
		if (layer->GetID() == layerID)
		{
			return layer.get();
		}
	}
	return nullptr;
}

LayerBase::LayerType Map::GetLayerTypeByID(U32 layerID) const
{
	for (const LayerBase::Ptr& layer : mLayers)
	{
		if (layer->GetID() == layerID)
		{
			return layer->GetLayerType();
		}
	}
	enAssert(false);
	return LayerBase::LayerType::TileLayer;
}

U32 Map::GetLayerCount() const
{
	return static_cast<U32>(mLayers.size());
}

std::vector<Vector2u> Map::GetNeighbors(const Vector2u& tileCoords, bool diag /*= false*/) const
{
	// TODO : Filter out invalid pos ?
	std::vector<Vector2u> n;
	if (mOrientation == Orientation::Orthogonal)
	{
		n.resize(diag ? 8 : 4);
		n[0].Set(tileCoords.x, tileCoords.y - 1);
		n[1].Set(tileCoords.x, tileCoords.y + 1);
		n[2].Set(tileCoords.x - 1, tileCoords.y);
		n[3].Set(tileCoords.x + 1, tileCoords.y);
		if (diag)
		{
			n[4].Set(tileCoords.x + 1, tileCoords.y - 1);
			n[5].Set(tileCoords.x + 1, tileCoords.y + 1);
			n[6].Set(tileCoords.x - 1, tileCoords.y + 1);
			n[7].Set(tileCoords.x - 1, tileCoords.y - 1);
		}
	}
	else if (mOrientation == Orientation::Isometric)
	{
		n.resize(diag ? 8 : 4);
		n[0].Set(tileCoords.x - 1, tileCoords.y);
		n[1].Set(tileCoords.x, tileCoords.y - 1);
		n[2].Set(tileCoords.x + 1, tileCoords.y);
		n[3].Set(tileCoords.x, tileCoords.y + 1);
		if (diag)
		{
			n[4].Set(tileCoords.x - 1, tileCoords.y - 1);
			n[5].Set(tileCoords.x + 1, tileCoords.y - 1);
			n[6].Set(tileCoords.x + 1, tileCoords.y + 1);
			n[7].Set(tileCoords.x - 1, tileCoords.y + 1);
		}
	}
	else if (mOrientation == Orientation::Staggered)
	{
		n.resize(diag ? 8 : 4);
		if (tileCoords.y % 2 == 0)
		{
			n[0].Set(tileCoords.x - 1, tileCoords.y - 1);
			n[1].Set(tileCoords.x, tileCoords.y - 1);
			n[2].Set(tileCoords.x, tileCoords.y + 1);
			n[3].Set(tileCoords.x - 1, tileCoords.y + 1);
		}
		else
		{
			n[0].Set(tileCoords.x, tileCoords.y - 1);
			n[1].Set(tileCoords.x + 1, tileCoords.y - 1);
			n[2].Set(tileCoords.x + 1, tileCoords.y + 1);
			n[3].Set(tileCoords.x, tileCoords.y + 1);
		}

		if (diag)
		{
			n[4].Set(tileCoords.x, tileCoords.y - 2);
			n[5].Set(tileCoords.x + 1, tileCoords.y);
			n[6].Set(tileCoords.x, tileCoords.y + 2);
			n[7].Set(tileCoords.x - 1, tileCoords.y);
		}
	}
	else if (mOrientation == Orientation::Hexagonal)
	{
		n.resize(6);
		if (mStaggerAxis == StaggerAxis::Y) // Pointy
		{
			if ((tileCoords.y % 2) == static_cast<U32>(mStaggerIndex))
			{
				n[0].Set(tileCoords.x - 1, tileCoords.y - 1);
				n[1].Set(tileCoords.x, tileCoords.y - 1);
				n[2].Set(tileCoords.x + 1, tileCoords.y);
				n[3].Set(tileCoords.x, tileCoords.y + 1);
				n[4].Set(tileCoords.x - 1, tileCoords.y + 1);
				n[5].Set(tileCoords.x - 1, tileCoords.y);
			}
			else
			{
				n[0].Set(tileCoords.x, tileCoords.y - 1);
				n[1].Set(tileCoords.x + 1, tileCoords.y - 1);
				n[2].Set(tileCoords.x + 1, tileCoords.y);
				n[3].Set(tileCoords.x + 1, tileCoords.y + 1);
				n[4].Set(tileCoords.x, tileCoords.y + 1);
				n[5].Set(tileCoords.x - 1, tileCoords.y);
			}
		}
		else // Flat
		{
			if ((tileCoords.x % 2) == static_cast<U32>(mStaggerIndex))
			{
				n[0].Set(tileCoords.x - 1, tileCoords.y - 1);
				n[1].Set(tileCoords.x, tileCoords.y - 1);
				n[2].Set(tileCoords.x + 1, tileCoords.y - 1);
				n[3].Set(tileCoords.x + 1, tileCoords.y);
				n[4].Set(tileCoords.x, tileCoords.y + 1);
				n[5].Set(tileCoords.x - 1, tileCoords.y);
			}
			else
			{
				n[0].Set(tileCoords.x - 1, tileCoords.y);
				n[1].Set(tileCoords.x, tileCoords.y - 1);
				n[2].Set(tileCoords.x + 1, tileCoords.y);
				n[3].Set(tileCoords.x + 1, tileCoords.y + 1);
				n[4].Set(tileCoords.x, tileCoords.y + 1);
				n[5].Set(tileCoords.x - 1, tileCoords.y + 1);
			}
		}
	}
	return n;
}

Vector2f Map::CoordsToWorld(const Vector2u& tileCoords) const
{
	if (mOrientation == Orientation::Orthogonal)
	{
		return Vector2f(
			static_cast<F32>(tileCoords.x * mTileSize.x), 
			static_cast<F32>(tileCoords.y * mTileSize.y)
		);
	}
	else if (mOrientation == Orientation::Isometric)
	{
		return Vector2f(
			static_cast<F32>((tileCoords.x - tileCoords.y) * mTileSize.x * 0.5f), 
			static_cast<F32>((tileCoords.x + tileCoords.y) * mTileSize.y * 0.5f)
		);
	}
	else if (mOrientation == Orientation::Staggered)
	{
		if (mStaggerAxis == StaggerAxis::Y)
		{
			return Vector2f(
				static_cast<F32>((tileCoords.y % 2 == static_cast<U32>(mStaggerIndex)) ? tileCoords.x * mTileSize.x : (tileCoords.x + 0.5f) * mTileSize.x),
				tileCoords.y * mTileSize.y * 0.5f
			);
		}
		else
		{
			return Vector2f(
				tileCoords.x * mTileSize.x * 0.5f,
				static_cast<F32>((tileCoords.x % 2 == static_cast<U32>(mStaggerIndex)) ? tileCoords.y * mTileSize.y : (tileCoords.y + 0.5f) * mTileSize.y)
			);
		}
	}
	else if (mOrientation == Orientation::Hexagonal)
	{
		if (mStaggerAxis == StaggerAxis::Y) // Pointy
		{
			return Vector2f(
				static_cast<F32>((tileCoords.y % 2 == static_cast<U32>(mStaggerIndex)) ? tileCoords.x * mTileSize.x : (tileCoords.x + 0.5f) * mTileSize.x),
				tileCoords.y* (mTileSize.y + mHexSideLength) * 0.5f
			);
		}
		else // Flat
		{
			return Vector2f(
				tileCoords.x * (mTileSize.x + mHexSideLength) * 0.5f,
				static_cast<F32>((tileCoords.x % 2 == static_cast<U32>(mStaggerIndex)) ? tileCoords.y * mTileSize.y : (tileCoords.y + 0.5f) * mTileSize.y)
			);
		}
	}

	enAssert(false);
    return Vector2f(0.0f, 0.0f);
}

Vector2u Map::WorldToCoords(const Vector2f& worldPos) const
{
    if (mOrientation == Orientation::Orthogonal)
    {
        return Vector2u(
            static_cast<U32>(worldPos.x) / mTileSize.x,
            static_cast<U32>(worldPos.y) / mTileSize.y
        );
    }
    else if (mOrientation == Orientation::Isometric)
    {
        enLogError(en::LogChannel::Map, "Isometric WorldToCoords unimplemented for {}", mName.c_str());
        return Vector2u(0, 0);
    }
    else if (mOrientation == Orientation::Staggered)
    {
        enLogError(en::LogChannel::Map, "Staggered WorldToCoords unimplemented for {}", mName.c_str());
        return Vector2u(0, 0);
    }
    else if (mOrientation == Orientation::Hexagonal)
    {
        enLogError(en::LogChannel::Map, "Hexagonal WorldToCoords unimplemented for {}", mName.c_str());
        return Vector2u(0, 0);
    }

	enAssert(false);
	return Vector2u(0, 0);
}

void Map::Render(sf::RenderTarget& target, bool renderObjects /*= false*/) const
{
    for (const LayerBase::Ptr& layer : mLayers)
    {
        if (layer != nullptr)
        {
            LayerBase::LayerType layerType = layer->GetLayerType();
            if (layer->IsVisible() && (layerType != LayerBase::LayerType::ObjectGroup || (renderObjects && layerType == LayerBase::LayerType::ObjectGroup)))
            {
                layer->Render(target);
            }
        }
    }
}

} // namespace tmx
} // namespace en
