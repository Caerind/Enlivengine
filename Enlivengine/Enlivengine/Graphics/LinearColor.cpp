#include <Enlivengine/Graphics/LinearColor.hpp>

#include <Enlivengine/Math/Utilities.hpp>
#include <sstream>

#include <Enlivengine/Graphics/Color.hpp>

namespace en
{

// Static member data
const LinearColor LinearColor::Black(Color(0, 0, 0));
const LinearColor LinearColor::White(Color(255, 255, 255));
const LinearColor LinearColor::Red(Color(255, 0, 0));
const LinearColor LinearColor::Green(Color(0, 255, 0));
const LinearColor LinearColor::Blue(Color(0, 0, 255));
const LinearColor LinearColor::Yellow(Color(255, 255, 0));
const LinearColor LinearColor::Magenta(Color(255, 0, 255));
const LinearColor LinearColor::Cyan(Color(0, 255, 255));
const LinearColor LinearColor::Transparent(Color(0, 0, 0, 0));
const LinearColor LinearColor::Brown(Color(128, 80, 32));
const LinearColor LinearColor::Orange(Color(255, 128, 0));
const LinearColor LinearColor::Pink(Color(255, 128, 192));
const LinearColor LinearColor::BabyPink(Color(255, 192, 224));
const LinearColor LinearColor::HotPink(Color(255, 0, 192));
const LinearColor LinearColor::Salmon(Color(255, 128, 128));
const LinearColor LinearColor::Violet(Color(128, 0, 255));
const LinearColor LinearColor::Purple(Color(64, 0, 128));
const LinearColor LinearColor::Peach(Color(255, 128, 96));
const LinearColor LinearColor::Lime(Color(128, 255, 0));
const LinearColor LinearColor::Mint(Color(64, 255, 192));
const LinearColor LinearColor::Gray(Color(128, 128, 128));
const LinearColor LinearColor::LightBlack(Color(64, 64, 64));
const LinearColor LinearColor::LightBlue(Color(128, 128, 255));
const LinearColor LinearColor::LightRed(Color(255, 128, 128));
const LinearColor LinearColor::LightMagenta(Color(255, 128, 255));
const LinearColor LinearColor::LightGreen(Color(128, 255, 128));
const LinearColor LinearColor::LightCyan(Color(128, 255, 255));
const LinearColor LinearColor::LightYellow(Color(255, 255, 128));
const LinearColor LinearColor::LightBrown(Color(192, 128, 64));
const LinearColor LinearColor::LightOrange(Color(255, 160, 64));
const LinearColor LinearColor::LightPink(Color(255, 160, 224));
const LinearColor LinearColor::LightBabyPink(Color(255, 208, 232));
const LinearColor LinearColor::LightHotPink(Color(255, 96, 224));
const LinearColor LinearColor::LightSalmon(Color(255, 160, 160));
const LinearColor LinearColor::LightViolet(Color(160, 96, 255));
const LinearColor LinearColor::LightPurple(Color(128, 64, 192));
const LinearColor LinearColor::LightPeach(Color(255, 160, 128));
const LinearColor LinearColor::LightLime(Color(192, 255, 128));
const LinearColor LinearColor::LightMint(Color(128, 255, 224));
const LinearColor LinearColor::LightGray(Color(192, 192, 192));
const LinearColor LinearColor::DarkBlue(Color(0, 0, 128));
const LinearColor LinearColor::DarkRed(Color(128, 0, 0));
const LinearColor LinearColor::DarkMagenta(Color(128, 0, 128));
const LinearColor LinearColor::DarkGreen(Color(0, 128, 0));
const LinearColor LinearColor::DarkCyan(Color(0, 128, 128));
const LinearColor LinearColor::DarkYellow(Color(128, 128, 0));
const LinearColor LinearColor::DarkWhite(Color(128, 128, 128));
const LinearColor LinearColor::DarkBrown(Color(64, 32, 0));
const LinearColor LinearColor::DarkOrange(Color(128, 64, 0));
const LinearColor LinearColor::DarkPink(Color(128, 64, 96));
const LinearColor LinearColor::DarkBabyPink(Color(160, 96, 128));
const LinearColor LinearColor::DarkHotPink(Color(128, 0, 96));
const LinearColor LinearColor::DarkSalmon(Color(128, 64, 64));
const LinearColor LinearColor::DarkViolet(Color(64, 0, 128));
const LinearColor LinearColor::DarkPurple(Color(32, 0, 64));
const LinearColor LinearColor::DarkPeach(Color(128, 64, 48));
const LinearColor LinearColor::DarkLime(Color(64, 128, 0));
const LinearColor LinearColor::DarkMint(Color(32, 128, 96));
const LinearColor LinearColor::DarkGray(Color(64, 64, 64));

LinearColor::LinearColor()
	: r(0.0f)
	, g(0.0f)
	, b(0.0f)
	, a(1.0f)
{
}

LinearColor::LinearColor(F32 red, F32 green, F32 blue, F32 alpha)
	: r(red)
	, g(green)
	, b(blue)
	, a(alpha)
{
}

LinearColor::LinearColor(F32 lightness)
	: r(lightness)
	, g(lightness)
	, b(lightness)
	, a(255)
{
}

LinearColor::LinearColor(U32 color)
{
	fromInteger(color);
}

LinearColor::LinearColor(const Color& color)
{
	fromColor(color);
}

LinearColor::LinearColor(const std::string& color)
{
	fromString(color);
}

F32& LinearColor::operator[](U8 index)
{
	assert(index < 4);
	switch (index)
	{
		case 0: return r;
		case 1: return g;
		case 2: return b;
	}
	return a;
}

const F32& LinearColor::operator[](U8 index) const
{
	assert(index < 4);
	switch (index)
	{
		case 0: return r;
		case 1: return g;
		case 2: return b;
	}
	return a;
}

bool LinearColor::isOpaque() const
{
	return Math::Equals(a, 1.0f);
}

bool LinearColor::equals(const LinearColor & color, F32 tolerance) const
{
	return Math::Equals(r, color.r, tolerance)
		&& Math::Equals(g, color.g, tolerance)
		&& Math::Equals(b, color.b, tolerance)
		&& Math::Equals(a, color.a, tolerance);
}

LinearColor LinearColor::withAlpha(F32 alpha) const
{
	return LinearColor(r, g, b, alpha);
}

LinearColor& LinearColor::clamp()
{
	r = Math::Clamp(r, 0.0f, 1.0f);
	g = Math::Clamp(g, 0.0f, 1.0f);
	b = Math::Clamp(b, 0.0f, 1.0f);
	a = Math::Clamp(a, 0.0f, 1.0f);
	return *this;
}

LinearColor LinearColor::clamped() const
{
	return LinearColor(*this).clamp();
}

std::string LinearColor::toString() const
{
	std::ostringstream oss;
	oss << std::hex << toInteger();
	return oss.str();
}

LinearColor& LinearColor::fromString(const std::string& color)
{
	std::istringstream iss(color);
	U32 integer;
	iss >> std::hex >> integer;
	return fromInteger(integer);
}

U32 LinearColor::toInteger() const
{
	return toColor().toInteger();
}

LinearColor& LinearColor::fromInteger(U32 color)
{
	return fromColor(Color(color));
}

Color LinearColor::toColor() const
{
	LinearColor c = clamped();
	return Color(U8(c.r * 255.0f), U8(c.g *255.0f), U8(c.b * 255.0f), U8(c.a * 255.0f));
}

LinearColor& LinearColor::fromColor(const Color& color)
{
	r = color.r * 0.00392156862f;
	g = color.g * 0.00392156862f;
	b = color.b * 0.00392156862f;
	a = color.a * 0.00392156862f;
	return *this;
}

#ifdef ENLIVE_ENABLE_IMGUI
ImVec4 LinearColor::toImGuiColor() const
{
	return ImVec4(r, g, b, a);
}

LinearColor& LinearColor::fromImGuiColor(const ImVec4& color)
{
	r = color.x;
	g = color.y;
	b = color.z;
	a = color.w;
	return *this;
}
#endif // ENLIVE_ENABLE_IMGUI

LinearColor& LinearColor::fromRedGreenScalar(F32 scalar)
{
	r = Math::Clamp(1.0f - scalar, 0.f, 1.f);
	g = Math::Clamp(scalar, 0.f, 1.f);
	b = 0.0f;
	return *this;
}

bool operator==(const LinearColor& left, const LinearColor& right)
{
	return left.equals(right, 0.00001f);
}

bool operator!=(const LinearColor& left, const LinearColor& right)
{
	return !operator==(left, right);
}

LinearColor operator+(const LinearColor& left, const LinearColor& right)
{
	return LinearColor(left.r + right.r, left.g + right.g, left.b + right.b, left.a + right.a).clamp();
}

LinearColor operator-(const LinearColor& left, const LinearColor& right)
{
	return LinearColor(left.r - right.r, left.g - right.g, left.b - right.b, left.a - right.a).clamp();
}

LinearColor operator*(const LinearColor& left, const LinearColor& right)
{
	return LinearColor(left.r * right.r, left.g * right.g, left.b * right.b, left.a * right.a).clamp();
}

LinearColor& operator+=(LinearColor& left, const LinearColor& right)
{
	left.r += right.r;
	left.g += right.g;
	left.b += right.b;
	left.a += right.a;
	return left.clamp();
}

LinearColor& operator-=(LinearColor& left, const LinearColor& right)
{
	left.r -= right.r;
	left.g -= right.g;
	left.b -= right.b;
	left.a -= right.a;
	return left.clamp();
}

LinearColor& operator*=(LinearColor& left, const LinearColor& right)
{
	left.r *= right.r;
	left.g *= right.g;
	left.b *= right.b;
	left.a *= right.a;
	return left.clamp();
}

LinearColor operator+(const LinearColor& left, F32 right)
{
	return LinearColor(left.r + right, left.g + right, left.b + right, left.a + right).clamp();
}

LinearColor operator-(const LinearColor& left, F32 right)
{
	return LinearColor(left.r - right, left.g - right, left.b - right, left.a - right).clamp();
}

LinearColor operator*(const LinearColor& left, F32 right)
{
	return LinearColor(left.r * right, left.g * right, left.b * right, left.a * right).clamp();
}

LinearColor operator/(const LinearColor& left, F32 right)
{
	assert(right != 0.0f);
	return left * (1.0f / right);
}

LinearColor operator+(F32 left, const LinearColor& right)
{
	return right + left;
}

LinearColor operator-(F32 left, const LinearColor& right)
{
	return LinearColor(left - right.r, left - right.g, left - right.b, left - right.a).clamp();
}

LinearColor operator*(F32 left, const LinearColor& right)
{
	return right * left;
}

LinearColor& operator+=(LinearColor& left, F32 right)
{
	left.r += right;
	left.g += right;
	left.b += right;
	left.a += right;
	return left.clamp();
}

LinearColor& operator-=(LinearColor& left, F32 right)
{
	left.r -= right;
	left.g -= right;
	left.b -= right;
	left.a -= right;
	return left.clamp();
}

LinearColor& operator*=(LinearColor& left, F32 right)
{
	left.r *= right;
	left.g *= right;
	left.b *= right;
	left.a *= right;
	return left.clamp();
}

LinearColor& operator/=(LinearColor& left, F32 right)
{
	assert(right != 0.0f);
	return left *= (1.0f / right);
}

} // namespace en