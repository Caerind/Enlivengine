#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <Enlivengine/Math/Vector2.hpp>
#include <Enlivengine/Math/Vector3.hpp>
#include <Enlivengine/Math/Vector4.hpp>
#include <Enlivengine/Math/Matrix4.hpp>
#include <Enlivengine/Math/Rect.hpp>
#include <Enlivengine/Math/Color.hpp>
#include <Enlivengine/System/Time.hpp>

namespace en
{

inline Color toEN(const sf::Color& color)
{
	return Color(color.r, color.g, color.b, color.a);
}
inline sf::Color toSF(const Color& color)
{
	return sf::Color(color.r, color.g, color.b, color.a);
}

inline Rectf toEN(const sf::FloatRect& rect)
{
	return Rectf(rect.left, rect.top, rect.width, rect.height);
}

inline sf::FloatRect toSF(const Rectf& rect)
{
	return sf::FloatRect(rect.left(), rect.top(), rect.width(), rect.height());
}

inline Recti toEN(const sf::IntRect& rect)
{
	return Recti(rect.left, rect.top, rect.width, rect.height);
}

inline sf::IntRect toSF(const Recti& rect)
{
	return sf::IntRect(rect.left(), rect.top(), rect.width(), rect.height());
}

inline sf::IntRect toSF(const Rectu& rect)
{
    return sf::IntRect(static_cast<int>(rect.left()), static_cast<int>(rect.top()), static_cast<int>(rect.width()), static_cast<int>(rect.height()));
}

inline Matrix4f toEN(const sf::Transform& transform)
{
	return Matrix4f(transform.getMatrix()[0], transform.getMatrix()[1], transform.getMatrix()[2],  transform.getMatrix()[3],
	                transform.getMatrix()[4], transform.getMatrix()[5], transform.getMatrix()[6],  transform.getMatrix()[7],
			        transform.getMatrix()[8], transform.getMatrix()[9], transform.getMatrix()[10], transform.getMatrix()[11],
			        transform.getMatrix()[12], transform.getMatrix()[13], transform.getMatrix()[14], transform.getMatrix()[15]);
}
inline sf::Transform toSF(const Matrix4f& transform)
{
	return sf::Transform(transform(0, 0), transform(0, 1), transform(0, 3),
	                     transform(1, 0), transform(1, 1), transform(1, 3),
						 transform(3, 0), transform(3, 1), transform(3, 3));
}

inline Vector2i toEN(const sf::Vector2u& vector)
{
	return Vector2i(static_cast<I32>(vector.x), static_cast<I32>(vector.y));
}
inline Vector2i toEN(const sf::Vector2i& vector)
{
	return Vector2i(static_cast<I32>(vector.x), static_cast<I32>(vector.y));
}
inline sf::Vector2i toSF(const Vector2i& vector)
{
	return sf::Vector2i(vector.x, vector.y);
}
	
inline Vector2f toEN(const sf::Vector2f& vector)
{
	return Vector2f(vector.x, vector.y);
}
inline sf::Vector2f toSF(const Vector2f& vector)
{
	return sf::Vector2f(vector.x, vector.y);
}

inline Vector3f toEN(const sf::Vector3f& vector)
{
	return Vector3f(vector.x, vector.y, vector.z);
}	
inline sf::Vector3f toSF(const Vector3f& vector)
{
	return sf::Vector3f(vector.x, vector.y, vector.z);
}

inline Time toEN(const sf::Time& time)
{
	return Time::Microseconds(time.asMicroseconds());
}
inline sf::Time toSF(const Time& time)
{
	return sf::microseconds(time.AsMicroseconds());
}

const char* sfKeyToString(sf::Keyboard::Key key);
sf::Keyboard::Key stringToSfKey(const char* keyString);

} // namespace en
