#pragma once

#include <Enlivengine/Utils/Meta.hpp>
#include <Enlivengine/Math/Rect.hpp>
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

private:
	Recti mTextureRect;
	TexturePtr mTexture;
};

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::Sprite, en::Type_ClassSerialization, en::Type_ClassEditor)
	ENLIVE_META_CLASS_MEMBER("texture", &en::Sprite::GetTexture, &en::Sprite::SetTexture),
	ENLIVE_META_CLASS_MEMBER("textureRect", &en::Sprite::GetTextureRect, &en::Sprite::SetTextureRect)
ENLIVE_META_CLASS_END()