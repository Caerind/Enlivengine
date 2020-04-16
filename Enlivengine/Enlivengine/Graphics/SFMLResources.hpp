#pragma once

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include <Enlivengine/Application/ResourceManager.hpp>

namespace en
{

namespace priv
{
	
template <class T>
class SFMLResourcesLoader
{
	public:
		SFMLResourcesLoader() = delete;
		~SFMLResourcesLoader() = delete;

		static ResourceLoader<T> FromFile(const std::string& filename)
		{
			return ResourceLoader<T>([&filename](T& r) 
			{
				const bool result = r.loadFromFile(filename);
				r.mFilename = (result) ? filename : "";
				return result;
			});
		}
};

} // namespace priv

class Texture : public Resource<Texture>, public sf::Texture {};
class Font : public Resource<Font>, public sf::Font {};
class Image : public Resource<Image>, public sf::Image {};
class SoundBuffer : public Resource<SoundBuffer>, public sf::SoundBuffer {};

using TexturePtr = ResourcePtr<Texture>;
using FontPtr = ResourcePtr<Font>;
using ImagePtr = ResourcePtr<Image>;
using SoundBufferPtr = ResourcePtr<SoundBuffer>;

using TextureLoader = priv::SFMLResourcesLoader<Texture>;
using FontLoader = priv::SFMLResourcesLoader<Font>;
using ImageLoader = priv::SFMLResourcesLoader<Image>;
using SoundBufferLoader = priv::SFMLResourcesLoader<SoundBuffer>;

} // namespace en
