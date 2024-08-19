#pragma once

#include <SFML/Graphics/Texture.hpp>

#include <Enlivengine/Math/Vector3.hpp>
#include <Enlivengine/Resources/ResourceManager.hpp>

namespace en
{

class Texture : public Resource<Texture>
{
public:
	static U32 GetStaticResourceType() { return static_cast<U32>(ResourceType::Texture); }
	U32 GetResourceType() const override { return GetStaticResourceType(); }

	Texture() = default;

    bool LoadFromFile(const char* filename);

	bool IsValid() const;

	Vector2u GetSize() const;
	U32 GetWidth() const;
	U32 GetHeight() const;
	
	sf::Texture& GetSFMLTexture() { return mTexture; }

private:
	sf::Texture mTexture;
	bool mValid{false};
};

class TextureLoader
{
public:
	TextureLoader() = delete;

	static ResourceLoader<Texture> FromFile(const std::string& filename)
	{
		return ResourceLoader<Texture>([&filename](Texture& r)
			{
				const bool result = r.LoadFromFile(filename.c_str());
				r.SetLoaded(result);
				r.SetLoadInfo(ResourceLoadInfo(ResourceLoadInfo::File, filename));
				return result;
			});
	}
};

using TexturePtr = ResourcePtr<Texture>;

} // namespace en