#include <Enlivengine/Graphics/SFMLResources.hpp>

namespace en
{

std::function<void(sf::Image& r)> ImageLoader::loadFromFile(const std::string& filename)
{
	return [&filename](sf::Image& r)
	{
		r.loadFromFile(filename);
	};
}

std::function<void(sf::Texture& r)> TextureLoader::loadFromFile(const std::string& filename)
{
	return [&filename](sf::Texture& r)
	{
		r.loadFromFile(filename);
	};
}

std::function<void(sf::Font& r)> FontLoader::loadFromFile(const std::string& filename)
{
	return [&filename](sf::Font& r)
	{
		r.loadFromFile(filename);
	};
}

std::function<void(sf::SoundBuffer& r)> SoundBufferLoader::loadFromFile(const std::string& filename)
{
	return [&filename](sf::SoundBuffer& r)
	{
		r.loadFromFile(filename);
	};
}

} // namespace en