#include <Enlivengine/Math/Color.hpp>

#include <Enlivengine/Math/Utilities.hpp>
#include <Enlivengine/System/Assert.hpp>

#include <sstream>

namespace en
{

// Static member data
const Color Color::Black(0, 0, 0);
const Color Color::White(255, 255, 255);
const Color Color::Red(255, 0, 0);
const Color Color::Green(0, 255, 0);
const Color Color::Blue(0, 0, 255);
const Color Color::Yellow(255, 255, 0);
const Color Color::Magenta(255, 0, 255);
const Color Color::Cyan(0, 255, 255);
const Color Color::Transparent(0, 0, 0, 0);
const Color Color::Brown(128, 80, 32);
const Color Color::Orange(255, 128, 0);
const Color Color::Pink(255, 128, 192);
const Color Color::BabyPink(255, 192, 224);
const Color Color::HotPink(255, 0, 192);
const Color Color::Salmon(255, 128, 128);
const Color Color::Violet(128, 0, 255);
const Color Color::Purple(64, 0, 128);
const Color Color::Peach(255, 128, 96);
const Color Color::Lime(128, 255, 0);
const Color Color::Mint(64, 255, 192);
const Color Color::Gray(128, 128, 128);
const Color Color::LightBlack(64, 64, 64);
const Color Color::LightBlue(128, 128, 255);
const Color Color::LightRed(255, 128, 128);
const Color Color::LightMagenta(255, 128, 255);
const Color Color::LightGreen(128, 255, 128);
const Color Color::LightCyan(128, 255, 255);
const Color Color::LightYellow(255, 255, 128);
const Color Color::LightBrown(192, 128, 64);
const Color Color::LightOrange(255, 160, 64);
const Color Color::LightPink(255, 160, 224);
const Color Color::LightBabyPink(255, 208, 232);
const Color Color::LightHotPink(255, 96, 224);
const Color Color::LightSalmon(255, 160, 160);
const Color Color::LightViolet(160, 96, 255);
const Color Color::LightPurple(128, 64, 192);
const Color Color::LightPeach(255, 160, 128);
const Color Color::LightLime(192, 255, 128);
const Color Color::LightMint(128, 255, 224);
const Color Color::LightGray(192, 192, 192);
const Color Color::DarkBlue(0, 0, 128);
const Color Color::DarkRed(128, 0, 0);
const Color Color::DarkMagenta(128, 0, 128);
const Color Color::DarkGreen(0, 128, 0);
const Color Color::DarkCyan(0, 128, 128);
const Color Color::DarkYellow(128, 128, 0);
const Color Color::DarkWhite(128, 128, 128);
const Color Color::DarkBrown(64, 32, 0);
const Color Color::DarkOrange(128, 64, 0);
const Color Color::DarkPink(128, 64, 96);
const Color Color::DarkBabyPink(160, 96, 128);
const Color Color::DarkHotPink(128, 0, 96);
const Color Color::DarkSalmon(128, 64, 64);
const Color Color::DarkViolet(64, 0, 128);
const Color Color::DarkPurple(32, 0, 64);
const Color Color::DarkPeach(128, 64, 48);
const Color Color::DarkLime(64, 128, 0);
const Color Color::DarkMint(32, 128, 96);
const Color Color::DarkGray(64, 64, 64);

Color::Color()
	: r(0)
	, g(0)
	, b(0)
	, a(255)
{
}

Color::Color(U8 red, U8 green, U8 blue, U8 alpha)
	: r(red)
	, g(green)
	, b(blue)
	, a(alpha)
{
}

Color::Color(U8 lightness)
	: r(lightness)
	, g(lightness)
	, b(lightness)
	, a(255)
{
}

Color::Color(U32 color)
{
	fromInteger(color);
}

Color::Color(const std::string& color)
{
	fromString(color);
}

U8& Color::operator[](U8 index)
{
	enAssert(index < 4);
	switch (index)
	{
		case 0: return r;
		case 1: return g;
		case 2: return b;
	}
	return a;
}

const U8& Color::operator[](U8 index) const
{
	enAssert(index < 4);
	switch (index)
	{
		case 0: return r;
		case 1: return g;
		case 2: return b;
	}
	return a;
}

bool Color::isOpaque() const
{
	return a == 255;
}

Color Color::withAlpha(U8 alpha) const
{
	return Color(r, g, b, alpha);
}

std::string Color::toString() const
{
	std::ostringstream oss;
	oss << std::hex << toInteger();
	return oss.str();
}

Color& Color::fromString(const std::string& color)
{
	std::istringstream iss(color);
	U32 integer;
	iss >> std::hex >> integer;
	fromInteger(integer);
	return *this;
}

U32 Color::toInteger() const
{
	return (r << 24) | (g << 16) | (b << 8) | a;
}

Color& Color::fromInteger(U32 color)
{
	r = static_cast<U8>((color & 0xff000000) >> 24);
	g = static_cast<U8>((color & 0x00ff0000) >> 16);
	b = static_cast<U8>((color & 0x0000ff00) >> 8);
	a = static_cast<U8>((color & 0x000000ff) >> 0);
	return *this;
}

#ifdef ENLIVE_ENABLE_IMGUI
ImVec4 Color::toImGuiColor() const
{
	return ImVec4(r * 0.00392156862f, g * 0.00392156862f, b * 0.00392156862f, a * 0.00392156862f);
}

Color& Color::fromImGuiColor(const ImVec4& color)
{
	r = U8(Math::Clamp(color.x, 0.0f, 1.0f) * 255.0f);
	g = U8(Math::Clamp(color.y, 0.0f, 1.0f) * 255.0f);
	b = U8(Math::Clamp(color.z, 0.0f, 1.0f) * 255.0f);
	a = U8(Math::Clamp(color.w, 0.0f, 1.0f) * 255.0f);
	return *this;
}
#endif // ENLIVE_ENABLE_IMGUI

bool operator==(const Color& left, const Color& right)
{
	return (left.r == right.r) && (left.g == right.g) && (left.b == right.b) && (left.a == right.a);
}

bool operator!=(const Color& left, const Color& right)
{
	return !(left == right);
}

Color operator+(const Color& left, const Color& right)
{
	return Color(U8(Math::Min(I32(left.r) + right.r, 255)),
		U8(Math::Min(I32(left.g) + right.g, 255)),
		U8(Math::Min(I32(left.b) + right.b, 255)),
		U8(Math::Min(I32(left.a) + right.a, 255)));
}

Color operator-(const Color& left, const Color& right)
{
	return Color(U8(Math::Max(I32(left.r) - right.r, 0)),
		U8(Math::Max(I32(left.g) + right.g, 0)),
		U8(Math::Max(I32(left.b) + right.b, 0)),
		U8(Math::Max(I32(left.a) + right.a, 0)));
}

Color operator*(const Color& left, const Color& right)
{
	return Color(U8(I32(left.r) * right.r / 255),
		U8(I32(left.g) * right.g / 255),
		U8(I32(left.b) * right.b / 255),
		U8(I32(left.a) * right.a / 255));
}

Color& operator+=(Color& left, const Color& right)
{
	return left = left + right;
}

Color& operator-=(Color& left, const Color& right)
{
	return left = left - right;
}

Color& operator*=(Color& left, const Color& right)
{
	return left = left * right;
}

} // namespace en
