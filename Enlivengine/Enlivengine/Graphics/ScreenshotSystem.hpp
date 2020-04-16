#pragma once

#include <string>

namespace sf
{
	class RenderWindow;
	class Texture;
}

namespace en
{

class ScreenshotSystem
{
public:
	ScreenshotSystem();

	bool Screenshot(const sf::RenderWindow& window);

private:
	bool SaveTexture(const sf::Texture& texture) const;

	static std::string GetScreenshotName();
	static std::string GetScreenshotFormat();
};

} // namespace en