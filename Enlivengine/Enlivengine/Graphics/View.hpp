#pragma once

#include <SFML/Graphics/View.hpp>

#include <Enlivengine/Graphics/SFMLWrapper.hpp>
#include <Enlivengine/Math/Rect.hpp>

namespace en
{

// TODO : Add rotation
// TODO : Add viewport

class View
{
	public:
		View();
		View(F32 x, F32 y, F32 w, F32 h);
		View(const Vector2f& center, const Vector2f& size);
		View(const sf::View& view);

		void setCenter(F32 x, F32 y);
		void setCenter(const Vector2f& center);
		Vector2f getCenter() const;

		void setSize(F32 w, F32 h);
		void setSize(const Vector2f& size);
		Vector2f getSize() const;

		void setZoom(F32 factor);
		F32 getZoom() const;

		void move(F32 x, F32 y);
		void move(const Vector2f& offset);

		void zoom(F32 factor);

		void reset(F32 x, F32 y, F32 w, F32 h);

		Rectf getBounds() const;

		const sf::View& getHandle() const;
		void setHandle(const sf::View& view);

	private:
		sf::View mView;
		F32 mZoomFactor;
};

} // namespace en