#include <Enlivengine/Math/Noise.hpp>

#include <algorithm> // std::shuffle

namespace en
{

Noise::Noise(U32 seed)
{
	mOctaves = 3;
	mLacunarity = 2.0f;
	mGain = 0.5f;

	for (I32 i = 0; i < 256; i++)
	{
		mPermutations[i] = i;
	}

	setSeed(seed);
}

void Noise::setSeed(U32 seed)
{
	mRandom.setSeed(seed);

	auto itr = std::begin(mPermutations);
	std::shuffle(itr, itr + 256, mRandom.getGenerator());

	for (I32 i = 0; i < 256; i++)
	{
		mPermutations[256 + i] = mPermutations[i];
	}
}

U32 Noise::getSeed() const
{
	return mRandom.getSeed();
}

void Noise::setOctaves(U32 octaves)
{
	mOctaves = octaves;
}

U32 Noise::getOctaves() const
{
	return mOctaves;
}

void Noise::setLacunarity(F32 lacunarity)
{
	mLacunarity = lacunarity;
}

F32 Noise::getLacunarity() const
{
	return mLacunarity;
}

void Noise::setGain(F32 gain)
{
	mGain = gain;
}

F32 Noise::getGain() const
{
	return mGain;
}

F32 Noise::getNoise(F32 x) const
{
	return getNoise(x, 0.0f, 0.0f);
}

F32 Noise::getNoise(F32 x, F32 y) const
{
	return getNoise(x, y, 0.0f);
}

F32 Noise::getNoise(F32 x, F32 y, F32 z) const
{
	F32 result = 0.0f;
	F32 amp = 1.0f;
	for (U32 i = 0; i < mOctaves; i++)
	{
		result += perlin(x, y, z) * amp;
		x *= mLacunarity;
		y *= mLacunarity;
		z *= mLacunarity;
		amp *= mGain;
	}
	return result * 0.5f + 0.5f;
}

F32 Noise::grad(I32 hash, F32 x, F32 y, F32 z)
{
	const I32 h = hash & 15;
	const F32 u = h < 8 ? x : y;
	const F32 v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

F32 Noise::perlin(F32 x, F32 y, F32 z) const
{
	const I32 X = I32(Math::Floor(x)) & 255;
	const I32 Y = I32(Math::Floor(y)) & 255;
	const I32 Z = I32(Math::Floor(z)) & 255;

	x -= (F32)x;
	y -= Math::Floor(y);
	z -= Math::Floor(z);

	const F32 u = Math::Fade(x);
	const F32 v = Math::Fade(y);
	const F32 w = Math::Fade(z);

	const I32 A = mPermutations[X] + Y;
	const I32 AA = mPermutations[A] + Z;
	const I32 AB = mPermutations[A + 1] + Z;
	const I32 B = mPermutations[X + 1] + Y;
	const I32 BA = mPermutations[B] + Z;
	const I32 BB = mPermutations[B + 1] + Z;

	const F32 gaa = grad(mPermutations[AA], x, y, z);
	const F32 gba = grad(mPermutations[BA], x - 1, y, z);
	const F32 lugaagba = Math::Lerp(gaa, gba, u);
	const F32 gab = grad(mPermutations[AB], x, y - 1, z);
	const F32 gbb = grad(mPermutations[BB], x - 1, y - 1, z);
	const F32 lugabgbb = Math::Lerp(gab, gbb, u);

	const F32 gaa2 = grad(mPermutations[AA + 1], x, y, z - 1);
	const F32 gba2 = grad(mPermutations[BA + 1], x - 1, y, z - 1);
	const F32 lugaa2gba2 = Math::Lerp(gaa2, gba2, u);
	const F32 gab2 = grad(mPermutations[AB + 1], x, y - 1, z - 1);
	const F32 gbb2 = grad(mPermutations[BB + 1], x - 1, y - 1, z - 1);
	const F32 lugab2gbb2 = Math::Lerp(gab2, gbb2, u);

	return Math::Lerp(Math::Lerp(lugaagba, lugabgbb, v), Math::Lerp(lugaa2gba2, lugab2gbb2, v), w);
}

} // namespace en
