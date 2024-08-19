#include <Enlivengine/Window/Clipboard.hpp>

#include <SFML/Window/Clipboard.hpp>

namespace en
{

std::string Clipboard::GetString()
{
    const std::basic_string<std::uint8_t> tmp = sf::Clipboard::getString().toUtf8();
    return std::string(tmp.begin(), tmp.end());
}

void Clipboard::SetString(const std::string& str)
{
    sf::Clipboard::setString(sf::String::fromUtf8(str.begin(), str.end()));
}
    
} // namespace en
