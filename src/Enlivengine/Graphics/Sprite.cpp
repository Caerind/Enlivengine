#include <Enlivengine/Graphics/Sprite.hpp>

#include <Enlivengine/Utils/Assert.hpp>

namespace en
{

Sprite::Sprite()
	: mTextureRect()
	, mTexture()
{
}

Sprite::Sprite(Sprite&& other)
	: mTextureRect(other.mTextureRect)
	, mTexture(other.mTexture)
{
	other.mTextureRect = Recti();
	other.mTexture.Release();
}

Sprite::~Sprite()
{
}

Sprite& Sprite::operator=(Sprite&& other)
{
	if (&other != this)
	{
		mTextureRect = other.mTextureRect;
		mTexture = other.mTexture;

		other.mTextureRect = Recti();
		other.mTexture.Release();
	}
	return *this;
}

void Sprite::SetTexture(TexturePtr texture)
{
	if (mTexture != texture)
	{
		mTexture = texture;
		if (Texture* texturePtr = texture.GetPtr())
		{
			SetTextureRect(Recti(0, 0, texturePtr->GetWidth(), texturePtr->GetHeight()));
		}
	}
}

TexturePtr Sprite::GetTexture() const
{
	return mTexture;
}

void Sprite::SetTextureRect(const Recti& textureRect)
{
	if (mTextureRect != textureRect)
	{
		mTextureRect = textureRect;
		//UpdateVertices
	}
}

const Recti& Sprite::GetTextureRect() const
{
	return mTextureRect;
}

} // namespace en