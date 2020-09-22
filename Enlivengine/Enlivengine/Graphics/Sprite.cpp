#include <Enlivengine/Graphics/Sprite.hpp>

namespace en
{

Sprite::Sprite()
	: mTexture()
	, mTextureRect()
{
}

void Sprite::SetTexture(TexturePtr texture)
{
	if (texture.IsValid())
	{
		if (!mTexture.IsValid() && (mTextureRect == Recti()))
		{
			Texture& textureRef = texture.Get();
			SetTextureRect(Recti(0, 0, textureRef.getSize().x, textureRef.getSize().y));
		}
		mTexture = texture;
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
		UpdatePositions();
		UpdateTexCoords();
	}
}

const Recti& Sprite::GetTextureRect() const
{
	return mTextureRect;
}

Rectf Sprite::GetLocalBounds() const
{
	const F32 x = static_cast<F32>(Math::Abs(mTextureRect.width()));
	const F32 y = static_cast<F32>(Math::Abs(mTextureRect.height()));
	return Rectf(0.0f, 0.0f, x, y);
}

Rectf Sprite::GetGlobalBounds() const
{
	// TODO : Sprite::GetGlobalBounds()
	enAssert(false);
	return GetLocalBounds();
}

void Sprite::Render(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (mTexture.IsValid())
	{
		states.texture = mTexture.GetPtr();
		states.transform *= toSF(GetMatrix());
		target.draw(mVertices, 4, sf::PrimitiveType::TriangleStrip, states);
	}
}

void Sprite::UpdatePositions()
{
	const Rectf bounds = GetLocalBounds();
	mVertices[0].position = sf::Vector2f(0.0f, 0.0f);
	mVertices[1].position = sf::Vector2f(0.0f, bounds.height());
	mVertices[2].position = sf::Vector2f(bounds.width(), 0.0f);
	mVertices[3].position = sf::Vector2f(bounds.width(), bounds.height());
}

void Sprite::UpdateTexCoords()
{
	const F32 left = static_cast<F32>(mTextureRect.left());
	const F32 right = left + mTextureRect.width();
	const F32 top = static_cast<F32>(mTextureRect.top());
	const F32 bottom = top + mTextureRect.height();
	mVertices[0].texCoords = sf::Vector2f(left, top);
	mVertices[1].texCoords = sf::Vector2f(left, bottom);
	mVertices[2].texCoords = sf::Vector2f(right, top);
	mVertices[3].texCoords = sf::Vector2f(right, bottom);
}

} // namespace en
