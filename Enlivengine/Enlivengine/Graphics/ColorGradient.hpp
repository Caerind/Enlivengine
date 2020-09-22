#pragma once

#include <Enlivengine/System/Assert.hpp>
#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/Math/Color.hpp>
#include <map>

namespace en
{

class ColorGradient
{
public:
	ColorGradient() : mColors()
	{
	}

	Color& operator[](F32 position)
	{
		enAssert(position >= 0.0f && position <= 1.0f);
		return mColors[position];
	}

	const Color& operator[](F32 position) const
	{
		enAssert(position >= 0.0f && position <= 1.0f);
		return mColors.at(position);
	}

	Color GetSampleColor(F32 position) const
	{
		// Make sure the positions 0 and 1 are set
		enAssert(mColors.count(0.0f) && mColors.count(1.0f));

		// Find first entry >= position, return color if entry == position
		auto nextColor = mColors.lower_bound(position);
		if (nextColor->first == position)
			return nextColor->second;

		// Find first entry < position
		auto prevColor = std::prev(nextColor);

		// Interpolate color between 2 entries
		const F32 interpolation = (position - prevColor->first) / (nextColor->first - prevColor->first);
		return BlendColors(prevColor->second, nextColor->second, interpolation);
	}

	static Color BlendColors(const Color& firstColor, const Color& secondColor, F32 interpolation)
	{
		enAssert(interpolation >= 0.f && interpolation <= 1.f);

		const F32 firstPart = 1.f - interpolation;

		return Color(
			static_cast<U8>(firstPart * firstColor.r + interpolation * secondColor.r),
			static_cast<U8>(firstPart * firstColor.g + interpolation * secondColor.g),
			static_cast<U8>(firstPart * firstColor.b + interpolation * secondColor.b),
			static_cast<U8>(firstPart * firstColor.a + interpolation * secondColor.a));
	}

private:
	std::map<F32, Color> mColors;
};

} // namespace en