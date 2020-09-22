#include <Enlivengine/Graphics/View.hpp>

namespace en
{

View::View()
	: mView()
	, mZoomFactor(1.0f)
{
}

View::View(F32 x, F32 y, F32 w, F32 h)
	: mView(sf::FloatRect(x, y, w, h))
	, mZoomFactor(1.0f)
{
}

View::View(const Vector2f& center, const Vector2f& size)
	: mView(toSF(center), toSF(size))
	, mZoomFactor(1.0f)
{
}

View::View(const sf::View& view)
{
	setHandle(view);
}

void View::setCenter(F32 x, F32 y)
{
	mView.setCenter(x, y);
}

void View::setCenter(const Vector2f& center)
{
	mView.setCenter(center.x, center.y);
}

Vector2f View::getCenter() const
{
	return toEN(mView.getCenter());
}

void View::setSize(F32 w, F32 h)
{
	mView.setSize(w, h);
}

void View::setSize(const Vector2f& size)
{
	mView.setSize(size.x, size.y);
}

Vector2f View::getSize() const
{
	return toEN(mView.getSize());
}

void View::setZoom(F32 factor)
{
	F32 x = factor / mZoomFactor;
	mZoomFactor *= x;
	mView.zoom(x);
}

F32 View::getZoom() const
{
	return mZoomFactor;
}

void View::move(F32 x, F32 y)
{
	mView.move(x, y);
}

void View::move(const Vector2f& offset)
{
	mView.move(offset.x, offset.y);
}

void View::zoom(F32 factor)
{
	mZoomFactor *= factor;
	mView.zoom(factor);
}

void View::reset(F32 x, F32 y, F32 w, F32 h)
{
	mView.reset(sf::FloatRect(x, y, w, h));
}

Rectf View::getBounds() const
{
	return toEN(sf::FloatRect(mView.getCenter() - mView.getSize() * 0.5f, mView.getSize()));
}

const sf::View& View::getHandle() const
{
	return mView;
}

void View::setHandle(const sf::View& view)
{
	mView = view;
	mZoomFactor = 1.0f;
}

} // namespace en
