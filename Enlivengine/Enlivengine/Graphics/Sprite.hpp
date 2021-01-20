#pragma once

#include <bgfx/bgfx.h>

#include <Enlivengine/Utils/Meta.hpp>
#include <Enlivengine/Math/Rect.hpp>
#include <Enlivengine/Graphics/Shader.hpp>
#include <Enlivengine/Graphics/Texture.hpp>

namespace en
{

class Sprite
{
public:
	Sprite();
	Sprite(Sprite&& other);
	Sprite(const Sprite& other) = delete;
	~Sprite();

	Sprite& operator=(Sprite&& other);
	Sprite& operator=(const Sprite& other) = delete;

	void SetTexture(TexturePtr texture);
	TexturePtr GetTexture() const;

	void SetTextureRect(const Recti& textureRect);
	const Recti& GetTextureRect() const;

	Rectf GetLocalBounds() const;
	Rectf GetGlobalBounds() const;

	void Render() const;

private:
	void UpdateVertices();
	void UpdateBuffer();

private:
	struct Vertex
	{
		Vector2f pos;
		F32 unusedZ{ 0.0f };
		Vector2f texCoords;

		static bgfx::VertexLayout kLayout;
	}; 

	Vertex mVertices[4];
	Recti mTextureRect;
	bgfx::VertexBufferHandle mBuffer;
	TexturePtr mTexture;

private:
	static const U16 kIndices[6];
	static Shader kShader;
	static bgfx::UniformHandle kUniformTexture;
	static bgfx::IndexBufferHandle kIndexBuffer;

public:
	// TODO : Make these private
	//friend class BgfxWrapper;
	static bool InitializeSprites();
	static bool ReleaseSprites();
};

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::Sprite, true, true)
	ENLIVE_META_CLASS_MEMBER("texture", &en::Sprite::GetTexture, &en::Sprite::SetTexture),
	ENLIVE_META_CLASS_MEMBER("textureRect", &en::Sprite::GetTextureRect, &en::Sprite::SetTextureRect)
ENLIVE_META_CLASS_END()