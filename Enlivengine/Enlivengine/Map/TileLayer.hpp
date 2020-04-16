#pragma once

#include <Enlivengine/Map/Map.hpp>
#include <Enlivengine/Map/LayerBase.hpp>

#include <SFML/Graphics/VertexArray.hpp>

namespace en
{
namespace tmx
{

class TileLayer : public LayerBase
{
public:
	TileLayer(Map& map);

	virtual LayerType GetLayerType() const 
	{ 
		return LayerBase::LayerType::TileLayer;
	}

	enum class EncodingType
	{
		Base64,
		CSV,
		XML
	};

	enum class CompressionType
	{
		None,
		Zlib,
		Gzip
	};

	const Vector2u& GetSize() const;
	EncodingType GetEncoding() const;
	CompressionType GetCompression() const;

	void SetTile(const Vector2u& tileCoords, U32 tileID);
	U32 GetTile(const Vector2u& tileCoords) const;

    virtual void Render(sf::RenderTarget& target) const;

private:
	friend class Map;
	bool Parse(ParserXml& parser);

	bool ParseBase64(ParserXml& parser);
	bool ParseCsv(ParserXml& parser);
	bool ParseXml(ParserXml& parser);

	void Update();

	Vector2u mSize;
	EncodingType mEncoding;
	CompressionType mCompression;
	std::vector<U32> mTiles;
	std::vector<sf::VertexArray> mVertexArrays;
};

} // namespace tmx
} // namespace en