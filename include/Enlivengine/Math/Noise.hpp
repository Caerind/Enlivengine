#pragma once

#include <Enlivengine/Math/Random.hpp>
#include <Enlivengine/Math/Utilities.hpp>

namespace en
{

class Noise
{
	public:
		Noise(U32 seed = 1337);

		void setSeed(U32 seed);
		U32 getSeed() const;

		void setOctaves(U32 octaves);
		U32 getOctaves() const;

		void setLacunarity(F32 lacunarity);
		F32 getLacunarity() const;

		void setGain(F32 gain);
		F32 getGain() const;

		F32 getNoise(F32 x) const;
		F32 getNoise(F32 x, F32 y) const;
		F32 getNoise(F32 x, F32 y, F32 z) const;

	private:
		static F32 grad(I32 hash, F32 x, F32 y, F32 z);

		F32 perlin(F32 x, F32 y, F32 z) const;

	private:
		I32 mPermutations[512];
		RandomEngine mRandom;
		U32 mOctaves;
		F32 mLacunarity;
		F32 mGain;
};

} // namespace en