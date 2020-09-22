#include <Enlivengine/Math/Noise.hpp>

#include <Enlivengine/Math/Random.hpp>
#include <Enlivengine/Math/Utilities.hpp>

#include <Enlivengine/System/Assert.hpp>

namespace en
{

Noise::Noise(I32 seed)
	: mNoise(seed)
{
}

void Noise::SetSeed(I32 seed)
{
	if (seed != mNoise.GetSeed())
	{
		mNoise.SetSeed(seed);
	}
}

I32 Noise::GetSeed() const
{
	return static_cast<I32>(mNoise.GetSeed());
}

void Noise::SetFrequency(F32 frequency)
{
	mNoise.SetFrequency(frequency);
}

F32 Noise::GetFrequency() const
{
	return static_cast<F32>(mNoise.GetFrequency());
}

void Noise::SetInterpolation(Interpolation interpolation)
{
	switch (interpolation)
	{
	case Interpolation::Linear: mNoise.SetInterp(FastNoise::Linear); break;
	case Interpolation::Hermite: mNoise.SetInterp(FastNoise::Hermite); break;
	case Interpolation::Quintic: mNoise.SetInterp(FastNoise::Quintic);  break;
	default: enAssert(false); break;
	}
}

Noise::Interpolation Noise::GetInterpolation() const
{
	switch (mNoise.GetInterp())
	{
	case FastNoise::Linear: return Interpolation::Linear;
	case FastNoise::Hermite: return Interpolation::Hermite;
	case FastNoise::Quintic: return Interpolation::Quintic;
	default: enAssert(false); break;
	}
	return Interpolation::Linear;
}

void Noise::SetNoiseType(NoiseType type)
{
	switch (type)
	{
	case NoiseType::Value: mNoise.SetNoiseType(FastNoise::Value); break;
	case NoiseType::ValueFractal: mNoise.SetNoiseType(FastNoise::ValueFractal); break;
	case NoiseType::Perlin: mNoise.SetNoiseType(FastNoise::Perlin); break;
	case NoiseType::PerlinFractal: mNoise.SetNoiseType(FastNoise::PerlinFractal); break;
	case NoiseType::Simplex: mNoise.SetNoiseType(FastNoise::Simplex); break;
	case NoiseType::SimplexFractal: mNoise.SetNoiseType(FastNoise::SimplexFractal); break;
	case NoiseType::WhiteNoise: mNoise.SetNoiseType(FastNoise::WhiteNoise); break;
	case NoiseType::Cubic: mNoise.SetNoiseType(FastNoise::Cubic); break;
	case NoiseType::CubicFractal: mNoise.SetNoiseType(FastNoise::CubicFractal); break;
	default: enAssert(false); break;
	}
}

Noise::NoiseType Noise::GetNoiseType() const
{
	switch (mNoise.GetNoiseType())
	{
	case FastNoise::Value: return NoiseType::Value;
	case FastNoise::ValueFractal: return NoiseType::ValueFractal;
	case FastNoise::Perlin: return NoiseType::Perlin;
	case FastNoise::PerlinFractal: return NoiseType::PerlinFractal;
	case FastNoise::Simplex: return NoiseType::Simplex;
	case FastNoise::SimplexFractal: return NoiseType::SimplexFractal;
	case FastNoise::WhiteNoise: return NoiseType::WhiteNoise;
	case FastNoise::Cubic: return NoiseType::Cubic;
	case FastNoise::CubicFractal: return NoiseType::CubicFractal;
	default: enAssert(false); break;
	}
	return NoiseType::Value;
}

void Noise::SetFractalOctaves(I32 octaves)
{
	mNoise.SetFractalOctaves(octaves);
}

I32 Noise::GetFractalOctaves() const
{
	return static_cast<I32>(mNoise.GetFractalOctaves());
}

void Noise::SetFractalLacunarity(F32 lacunarity)
{
	mNoise.SetFractalLacunarity(lacunarity);
}

F32 Noise::GetFractalLacunarity() const
{
	return static_cast<F32>(mNoise.GetFractalLacunarity());
}

void Noise::SetFractalGain(F32 fractalGain)
{
	mNoise.SetFractalGain(fractalGain);
}

F32 Noise::GetFractalGain() const
{
	return static_cast<F32>(mNoise.GetFractalGain());
}

void Noise::SetFractalType(FractalType fractalType)
{
	switch (fractalType)
	{
	case FractalType::FBM: mNoise.SetFractalType(FastNoise::FBM); break;
	case FractalType::Billow: mNoise.SetFractalType(FastNoise::Billow); break;
	case FractalType::RigidMulti: mNoise.SetFractalType(FastNoise::RigidMulti); break;
	default: enAssert(false); break;
	}
}

Noise::FractalType Noise::GetFractalType() const
{
	switch (mNoise.GetFractalType())
	{
	case FastNoise::FBM: return FractalType::FBM;
	case FastNoise::Billow: return FractalType::Billow;
	case FastNoise::RigidMulti: return FractalType::RigidMulti;
	default: enAssert(false); break;
	}
	return FractalType::FBM;
}

F32 Noise::Get(F32 x, F32 y) const
{
	return mNoise.GetNoise(x, y);
}

F32 Noise::Get(F32 x, F32 y, F32 z) const
{
	return mNoise.GetNoise(x, y, z);
}

} // namespace en
