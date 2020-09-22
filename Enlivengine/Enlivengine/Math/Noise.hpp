#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>

#include <FastNoise/FastNoise.h>

namespace en
{

class Noise
{
public:
	enum class NoiseType { Value = 0, ValueFractal, Perlin, PerlinFractal, Simplex, SimplexFractal, WhiteNoise, Cubic, CubicFractal };
	enum class Interpolation { Linear = 0, Hermite, Quintic };
	enum class FractalType { FBM = 0, Billow, RigidMulti };

	Noise(I32 seed = 1337);

	void SetSeed(I32 seed);
	I32 GetSeed() const;

	void SetFrequency(F32 frequency);
	F32 GetFrequency() const;

	void SetInterpolation(Interpolation interpolation);
	Interpolation GetInterpolation() const;

	void SetNoiseType(NoiseType type);
	NoiseType GetNoiseType() const;

	void SetFractalOctaves(I32 octaves);
	I32 GetFractalOctaves() const;

	void SetFractalLacunarity(F32 lacunarity);
	F32 GetFractalLacunarity() const;

	void SetFractalGain(F32 fractalGain);
	F32 GetFractalGain() const;

	void SetFractalType(FractalType fractalType);
	FractalType GetFractalType() const;

	F32 Get(F32 x, F32 y) const;
	F32 Get(F32 x, F32 y, F32 z) const;

private:
	FastNoise mNoise;
};

} // namespace en