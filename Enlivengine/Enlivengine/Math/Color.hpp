#pragma once

#include <string>

#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/System/Meta.hpp>

#ifdef ENLIVE_ENABLE_IMGUI
#include <imgui/imgui.h>
#endif // ENLIVE_ENABLE_IMGUI

namespace en
{

class Color
{
	public:
		Color();
		Color(U8 red, U8 green, U8 blue, U8 alpha = 255);
		Color(U8 lightness);
		Color(U32 color);
		Color(const std::string& color);

		U8& operator[](U8 index);
		const U8& operator[](U8 index) const;

		bool isOpaque() const;

		Color withAlpha(U8 alpha) const;

		std::string toString() const;
		Color& fromString(const std::string& color);

		U32 toInteger() const;
		Color& fromInteger(U32 color);

#ifdef ENLIVE_ENABLE_IMGUI
		ImVec4 toImGuiColor() const;
		Color& fromImGuiColor(const ImVec4& color);
#endif // ENLIVE_ENABLE_IMGUI

		U8 r;
		U8 g;
		U8 b;
		U8 a;

		// Basic color
		static const Color Black;
		static const Color White;
		static const Color Red;
		static const Color Green;
		static const Color Blue;
		static const Color Yellow;
		static const Color Magenta;
		static const Color Cyan;
		static const Color Transparent;

		// Standard
		static const Color Brown;
		static const Color Orange;
		static const Color Pink;
		static const Color BabyPink;
		static const Color HotPink;
		static const Color Salmon;
		static const Color Violet;
		static const Color Purple;
		static const Color Peach;
		static const Color Lime;
		static const Color Mint;
		static const Color Gray;

		// Lights
		static const Color LightBlack;
		static const Color LightBlue;
		static const Color LightRed;
		static const Color LightMagenta;
		static const Color LightGreen;
		static const Color LightCyan;
		static const Color LightYellow;
		static const Color LightBrown;
		static const Color LightOrange;
		static const Color LightPink;
		static const Color LightBabyPink;
		static const Color LightHotPink;
		static const Color LightSalmon;
		static const Color LightViolet;
		static const Color LightPurple;
		static const Color LightPeach;
		static const Color LightLime;
		static const Color LightMint;
		static const Color LightGray;

		// Darks
		static const Color DarkBlue;
		static const Color DarkRed;
		static const Color DarkMagenta;
		static const Color DarkGreen;
		static const Color DarkCyan;
		static const Color DarkYellow;
		static const Color DarkWhite;
		static const Color DarkBrown;
		static const Color DarkOrange;
		static const Color DarkPink;
		static const Color DarkBabyPink;
		static const Color DarkHotPink;
		static const Color DarkSalmon;
		static const Color DarkViolet;
		static const Color DarkPurple;
		static const Color DarkPeach;
		static const Color DarkLime;
		static const Color DarkMint;
		static const Color DarkGray;
};

bool operator==(const Color& left, const Color& right);
bool operator!=(const Color& left, const Color& right);
Color operator+(const Color& left, const Color& right);
Color operator-(const Color& left, const Color& right);
Color operator*(const Color& left, const Color& right);
Color& operator+=(Color& left, const Color& right);
Color& operator-=(Color& left, const Color& right);
Color& operator*=(Color& left, const Color& right);

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::Color)
	ENLIVE_META_CLASS_MEMBER("r", &en::Color::r),
	ENLIVE_META_CLASS_MEMBER("g", &en::Color::g),
	ENLIVE_META_CLASS_MEMBER("b", &en::Color::b),
	ENLIVE_META_CLASS_MEMBER("a", &en::Color::a)
ENLIVE_META_CLASS_END()