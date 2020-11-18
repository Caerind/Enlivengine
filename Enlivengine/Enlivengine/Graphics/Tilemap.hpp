#pragma once

#include <vector>

#include <bgfx/bgfx.h>

#include <Enlivengine/Math/Rect.hpp>
#include <Enlivengine/Graphics/Shader.hpp>
#include <Enlivengine/Graphics/Tileset.hpp>

namespace en
{

class Tilemap
{
public:
	Tilemap();
	~Tilemap();

	void SetTileset(const Tileset& tileset);
	const Tileset* GetTileset() const;

	void SetSize(const Vector2u& size);
	const Vector2u& GetSize() const;

	void SetTile(const Vector2u& tileCoords, U32 tileID);
	U32 GetTile(const Vector2u& tileCoords) const;

	Rectf GetLocalBounds() const;
	Rectf GetGlobalBounds() const;

	void Render(const bgfx::ViewId& viewId) const;

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