#pragma once

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include <Enlivengine/Application/ResourceManager.hpp>

namespace en
{

class ImageLoader
{
	public:
		ImageLoader() = delete;
		~ImageLoader() = delete;

		static std::function<void(sf::Image& r)> loadFromFile(const std::string& filename);
};

class TextureLoader
{
	public:
		TextureLoader() = delete;
		~TextureLoader() = delete;

		static std::function<void(sf::Texture& r)> loadFromFile(const std::string& filename);
};

class FontLoader
{
	public:
		FontLoader() = delete;
		~FontLoader() = delete;

		static std::function<void(sf::Font& r)> loadFromFile(const std::string& filename);
};

class SoundBufferLoader
{
	public:
		SoundBufferLoader() = delete;
		~SoundBufferLoader() = delete;

		static std::function<void(sf::SoundBuffer& r)> loadFromFile(const std::string& filename);
};

using ImageManager = ResourceManager<sf::Image>;
using TextureManager = ResourceManager<sf::Texture>;
using FontManager = ResourceManager<sf::Font>;
using SoundManager = ResourceManager<sf::SoundBuffer>;
using ShaderManager = ResourceManager<sf::Shader>;

} // namespace en
