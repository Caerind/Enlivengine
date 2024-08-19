#include <Enlivengine/Graphics/Texture.hpp>

namespace en
{

bool Texture::LoadFromFile(const char* filename)
{
    mValid = mTexture.loadFromFile(filename);
    return mValid;
}

bool Texture::IsValid() const
{
	return mValid;
}

Vector2u Texture::GetSize() const
{
    const auto size = mTexture.getSize();
    return Vector2u(size.x, size.y);
}

U32 Texture::GetWidth() const
{
	return mTexture.getSize().x;
}

U32 Texture::GetHeight() const
{
	return mTexture.getSize().y;
}

} // namespace en