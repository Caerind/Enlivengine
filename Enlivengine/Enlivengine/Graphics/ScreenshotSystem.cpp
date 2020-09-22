#include <Enlivengine/Graphics/ScreenshotSystem.hpp>

#include <Enlivengine/System/Log.hpp>
#include <Enlivengine/System/DateTime.hpp>

#include <Enlivengine/Application/PathManager.hpp>

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <sstream>

namespace en
{

ScreenshotSystem::ScreenshotSystem()
{
}

bool ScreenshotSystem::Screenshot(const sf::RenderWindow& window)
{
	sf::Texture texture;
	texture.create(window.getSize().x, window.getSize().y);
	texture.update(window);

	return SaveTexture(texture);
}

bool ScreenshotSystem::SaveTexture(const sf::Texture& texture) const
{
	const std::string filename = PathManager::GetInstance().GetScreenshotPath() + GetScreenshotName() + GetScreenshotFormat();
	if (texture.copyToImage().saveToFile(filename))
	{
		enLogInfo(en::LogChannel::Graphics, "Screenshot saved to : {}", filename.c_str());
		return true;
	}
	else
	{
		enLogError(en::LogChannel::Graphics, "Can't save screenshot to : {}", filename.c_str());
		return false;
	}
}

std::string ScreenshotSystem::GetScreenshotName()
{
	DateTime date;
	I32 y, m, d;
	I32 H, M, S;
	date.GetDate(y, m, d);
	date.GetTime(H, M, S);
	std::ostringstream oss;
	oss << y << "-" << m << "-" << d << "_" << H << "-" << M << "-" << S;
	return oss.str();
}

std::string ScreenshotSystem::GetScreenshotFormat()
{
	return std::string(".png");
}

} // namespace en