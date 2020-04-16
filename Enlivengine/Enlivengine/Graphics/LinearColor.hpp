#pragma once

#include <string>

#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/Math/Utilities.hpp>

#ifdef ENLIVE_ENABLE_IMGUI
#include <imgui/imgui.h>
#endif // ENLIVE_ENABLE_IMGUI

namespace en
{

class Color;

class LinearColor
{
	public:
		LinearColor();
		LinearColor(F32 red, F32 green, F32 blue, F32 alpha = 1.0f);
		LinearColor(F32 lightness);
		LinearColor(U32 color);
		LinearColor(const Color& color);
		LinearColor(const std::string& color);

		F32& operator[](U8 index);
		const F32& operator[](U8 index) const;

		bool isOpaque() const;
		bool equals(const LinearColor& color, F32 tolerance = 0.01f) const;

		LinearColor withAlpha(F32 alpha) const;

		LinearColor& clamp();
		LinearColor clamped() const;

		std::string toString() const;
		LinearColor& fromString(const std::string& color);

		U32 toInteger() const;
		LinearColor& fromInteger(U32 color);

		Color toColor() const;
		LinearColor& fromColor(const Color& color);

#ifdef ENLIVE_ENABLE_IMGUI
		ImVec4 toImGuiColor() const;
		LinearColor& fromImGuiColor(const ImVec4& color);
#endif // ENLIVE_ENABLE_IMGUI

		LinearColor& fromRedGreenScalar(F32 scalar);

		F32 r;
		F32 g;
		F32 b;
		F32 a;

		// Basic color
		static const LinearColor Black;
		static const LinearColor White;
		static const LinearColor Red;
		static const LinearColor Green;
		static const LinearColor Blue;
		static const LinearColor Yellow;
		static const LinearColor Magenta;
		static const LinearColor Cyan;
		static const LinearColor Transparent;

		// Standard
		static const LinearColor Brown;
		static const LinearColor Orange;
		static const LinearColor Pink;
		static const LinearColor BabyPink;
		static const LinearColor HotPink;
		static const LinearColor Salmon;
		static const LinearColor Violet;
		static const LinearColor Purple;
		static const LinearColor Peach;
		static const LinearColor Lime;
		static const LinearColor Mint;
		static const LinearColor Gray;

		// Lights
		static const LinearColor LightBlack;
		static const LinearColor LightBlue;
		static const LinearColor LightRed;
		static const LinearColor LightMagenta;
		static const LinearColor LightGreen;
		static const LinearColor LightCyan;
		static const LinearColor LightYellow;
		static const LinearColor LightBrown;
		static const LinearColor LightOrange;
		static const LinearColor LightPink;
		static const LinearColor LightBabyPink;
		static const LinearColor LightHotPink;
		static const LinearColor LightSalmon;
		static const LinearColor LightViolet;
		static const LinearColor LightPurple;
		static const LinearColor LightPeach;
		static const LinearColor LightLime;
		static const LinearColor LightMint;
		static const LinearColor LightGray;

		// Darks
		static const LinearColor DarkBlue;
		static const LinearColor DarkRed;
		static const LinearColor DarkMagenta;
		static const LinearColor DarkGreen;
		static const LinearColor DarkCyan;
		static const LinearColor DarkYellow;
		static const LinearColor DarkWhite;
		static const LinearColor DarkBrown;
		static const LinearColor DarkOrange;
		static const LinearColor DarkPink;
		static const LinearColor DarkBabyPink;
		static const LinearColor DarkHotPink;
		static const LinearColor DarkSalmon;
		static const LinearColor DarkViolet;
		static const LinearColor DarkPurple;
		static const LinearColor DarkPeach;
		static const LinearColor DarkLime;
		static const LinearColor DarkMint;
		static const LinearColor DarkGray;
};

bool operator==(const LinearColor& left, const LinearColor& right);
bool operator!=(const LinearColor& left, const LinearColor& right);
LinearColor operator+(const LinearColor& left, const LinearColor& right);
LinearColor operator-(const LinearColor& left, const LinearColor& right);
LinearColor operator*(const LinearColor& left, const LinearColor& right);
LinearColor& operator+=(LinearColor& left, const LinearColor& right);
LinearColor& operator-=(LinearColor& left, const LinearColor& right);
LinearColor& operator*=(LinearColor& left, const LinearColor& right);
LinearColor operator+(const LinearColor& left, F32 right);
LinearColor operator-(const LinearColor& left, F32 right);
LinearColor operator*(const LinearColor& left, F32 right);
LinearColor operator/(const LinearColor& left, F32 right);
LinearColor operator+(F32 left, const LinearColor& right);
LinearColor operator-(F32 left, const LinearColor& right);
LinearColor operator*(F32 left, const LinearColor& right);
LinearColor& operator+=(LinearColor& left, F32 right);
LinearColor& operator-=(LinearColor& left, F32 right);
LinearColor& operator*=(LinearColor& left, F32 right);
LinearColor& operator/=(LinearColor& left, F32 right);

} // namespace en