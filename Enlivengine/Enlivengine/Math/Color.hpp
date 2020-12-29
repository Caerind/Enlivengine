#pragma once

#include <Enlivengine/Math/Math.hpp>

#ifdef ENLIVE_ENABLE_IMGUI
#include <imgui/imgui.h>
#endif // ENLIVE_ENABLE_IMGUI

#include <Box2D/Box2D.h>

namespace en
{

class Color
{
public:
	constexpr Color() : r(255), g(255), b(255), a(255) {}
	constexpr Color(U8 red, U8 green, U8 blue, U8 alpha = 255) : r(red), g(green), b(blue), a(alpha) {}
	constexpr Color(U8 grey, U8 alpha = 255) : r(grey), g(grey), b(grey), a(alpha) {}
	constexpr Color(U32 rgba) : Color() { FromRGBA(rgba); }

	constexpr U8& operator[](U8 index)
	{
		switch (index)
		{
		case 0: return r;
		case 1: return g;
		case 2: return b;
		}
		return a;
	}
	constexpr const U8& operator[](U8 index) const
	{
		switch (index)
		{
		case 0: return r;
		case 1: return g;
		case 2: return b;
		}
		return a;
	}

	constexpr bool HasAlpha() const { return a != 255; }
	constexpr Color WithAlpha(U8 alpha) const { return Color(r, g, b, alpha); }

	constexpr U32 ToRGBA() const { return (r << 24) | (g << 16) | (b << 8) | a; }
	constexpr Color& FromRGBA(U32 color)
	{
		r = static_cast<U8>((color & 0xff000000) >> 24);
		g = static_cast<U8>((color & 0x00ff0000) >> 16);
		b = static_cast<U8>((color & 0x0000ff00) >> 8);
		a = static_cast<U8>((color & 0x000000ff));
		return *this;
	}

#ifdef ENLIVE_ENABLE_IMGUI
	inline ImVec4 ToImGuiColor() const { const F32 factor = 1.0f / 255.0f; return ImVec4(factor * r, factor * g, factor * b, factor * a); }
	inline Color& FromImGuiColor(const ImVec4& color)
	{
		const F32 factor = 255.0f;
		r = static_cast<U8>(factor * color.x);
		g = static_cast<U8>(factor * color.y);
		b = static_cast<U8>(factor * color.z);
		a = static_cast<U8>(factor * color.w);
		return *this;
	}
#endif // ENLIVE_ENABLE_IMGUI

	inline b2Color ToBox2DColor() const { const F32 factor = 1.0f / 255.0f; return b2Color(factor * r, factor * g, factor * b, factor * a); }
	inline Color& FromBox2DColor(const b2Color& color)
	{
		const F32 factor = 255.0f;
		r = static_cast<U8>(factor * color.r);
		g = static_cast<U8>(factor * color.g);
		b = static_cast<U8>(factor * color.b);
		a = static_cast<U8>(factor * color.a);
		return *this;
	}

	constexpr bool operator==(const Color& other) const { return r == other.r && g == other.g && b == other.b && a == other.a; }
	constexpr bool operator!=(const Color& other) const { return !operator==(other); }

	U8 r;
	U8 g;
	U8 b;
	U8 a;
};

namespace Colors
{
	// Basic colors
	static constexpr Color Black{ 0, 0, 0 };
	static constexpr Color White{ 255, 255, 255 };
	static constexpr Color Red{ 255, 0, 0 };
	static constexpr Color Green{ 0, 255, 0 };
	static constexpr Color Blue{ 0, 0, 255 };
	static constexpr Color Yellow{ 255, 255, 0 };
	static constexpr Color Magenta{ 255, 0, 255 };
	static constexpr Color Cyan{ 0, 255, 255 };
	static constexpr Color Transparent{ 255, 255, 255, 0 };

	// Standard colors
	static constexpr Color Brown{ 128, 80, 32 };
	static constexpr Color Orange{ 255, 128, 0 };
	static constexpr Color Pink{ 255, 128, 192 };
	static constexpr Color BabyPink{ 255, 192, 224 };
	static constexpr Color HotPink{ 255, 0, 192 };
	static constexpr Color Salmon{ 255, 128, 128 };
	static constexpr Color Violet{ 128, 0, 255 };
	static constexpr Color Purple{ 64, 0, 128 };
	static constexpr Color Peach{ 255, 128, 96 };
	static constexpr Color Lime{ 128, 255, 0 };
	static constexpr Color Mint{ 64, 255, 192 };
	static constexpr Color Gray{ 128, 128, 128 };

	// Light colors
	static constexpr Color LightBlack{ 64, 64, 64 };
	static constexpr Color LightBlue{ 128, 128, 255 };
	static constexpr Color LightRed{ 255, 128, 128 };
	static constexpr Color LightMagenta{ 255, 128, 255 };
	static constexpr Color LightGreen{ 128, 255, 128 };
	static constexpr Color LightCyan{ 128, 255, 255 };
	static constexpr Color LightYellow{ 255, 255, 128 };
	static constexpr Color LightBrown{ 192, 128, 64 };
	static constexpr Color LightOrange{ 255, 160, 64 };
	static constexpr Color LightPink{ 255, 160, 224 };
	static constexpr Color LightBabyPink{ 255, 208, 232 };
	static constexpr Color LightHotPink{ 255, 96, 224 };
	static constexpr Color LightSalmon{ 255, 160, 160 };
	static constexpr Color LightViolet{ 160, 96, 255 };
	static constexpr Color LightPurple{ 128, 64, 192 };
	static constexpr Color LightPeach{ 255, 160, 128 };
	static constexpr Color LightLime{ 192, 255, 128 };
	static constexpr Color LightMint{ 128, 255, 224 };
	static constexpr Color LightGray{ 192, 192, 192 };

	// Dark colors
	static constexpr Color DarkBlue{ 0, 0, 128 };
	static constexpr Color DarkRed{ 128, 0, 0 };
	static constexpr Color DarkMagenta{ 128, 0, 128 };
	static constexpr Color DarkGreen{ 0, 128, 0 };
	static constexpr Color DarkCyan{ 0, 128, 128 };
	static constexpr Color DarkYellow{ 128, 128, 0 };
	static constexpr Color DarkWhite{ 128, 128, 128 };
	static constexpr Color DarkBrown{ 64, 32, 0 };
	static constexpr Color DarkOrange{ 128, 64, 0 };
	static constexpr Color DarkPink{ 128, 64, 96 };
	static constexpr Color DarkBabyPink{ 160, 96, 128 };
	static constexpr Color DarkHotPink{ 128, 0, 96 };
	static constexpr Color DarkSalmon{ 128, 64, 64 };
	static constexpr Color DarkViolet{ 64, 0, 128 };
	static constexpr Color DarkPurple{ 32, 0, 64 };
	static constexpr Color DarkPeach{ 128, 64, 48 };
	static constexpr Color DarkLime{ 64, 128, 0 };
	static constexpr Color DarkMint{ 32, 128, 96 };
	static constexpr Color DarkGray{ 64, 64, 64 };
} // namespace Colors

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::Color)
	ENLIVE_META_CLASS_MEMBER("r", &en::Color::r),
	ENLIVE_META_CLASS_MEMBER("g", &en::Color::g),
	ENLIVE_META_CLASS_MEMBER("b", &en::Color::b),
	ENLIVE_META_CLASS_MEMBER("a", &en::Color::a)
ENLIVE_META_CLASS_END()