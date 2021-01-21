#pragma once

#include <vector>

#include <bgfx/bgfx.h>

#include <Enlivengine/Utils/Meta.hpp>
#include <Enlivengine/Math/Rect.hpp>
#include <Enlivengine/Graphics/Shader.hpp>
#include <Enlivengine/Graphics/Tileset.hpp>

namespace en
{

class Tilemap
{
public:
	Tilemap();
	Tilemap(Tilemap&& other);
	Tilemap(const Tilemap& other) = delete;
	~Tilemap();

	Tilemap& operator=(Tilemap&& other);
	Tilemap& operator=(const Tilemap& other) = delete;

	void SetTileset(const Tileset& tileset);
	const Tileset* GetTileset() const;

	void SetSize(const Vector2u& size);
	const Vector2u& GetSize() const;

	void SetTile(const Vector2u& tileCoords, U32 tileID);
	U32 GetTile(const Vector2u& tileCoords) const;

	Rectf GetLocalBounds() const;
	Rectf GetGlobalBounds() const;

	void Render() const;

private:
	void UpdateTexCoords();
	void UpdateTileTexCoords(U32 tileIndex);
	void UpdateVertexBuffer();
	void UpdateIndexBuffer();

private:
	struct Vertex
	{
		Vector2f pos;
		F32 unusedZ{ 0.0f };
		Vector2f texCoords;

		static bgfx::VertexLayout kLayout;
	};

	Vector2u mSize;
	std::vector<U32> mTiles;
	std::vector<Vertex> mVertices;
	bgfx::VertexBufferHandle mVertexBuffer;
	std::vector<U16> mIndices;
	bgfx::IndexBufferHandle mIndexBuffer;
	const Tileset* mTileset;

private:
	static const U16 kIndices[6];
	static Shader kShader;
	static bgfx::UniformHandle kUniformTexture;

public:
	// TODO : Make these private
	//friend class BgfxWrapper;
	static bool InitializeTilemaps();
	static bool ReleaseTilemaps();
};

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::Tilemap, en::Type_ClassSerialization, en::Type_ClassEditor)
	ENLIVE_META_CLASS_MEMBER("size", &en::Tilemap::GetSize, &en::Tilemap::SetSize)
ENLIVE_META_CLASS_END()