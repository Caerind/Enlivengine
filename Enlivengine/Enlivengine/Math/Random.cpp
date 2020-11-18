#include <Enlivengine/Math/Random.hpp>

#include <Enlivengine/Platform/Time.hpp>

namespace en
{

RandomEngine::RandomEngine()
{
	SetSeed(static_cast<U32>(Time::Now().AsMilliseconds()));
}

RandomEngine::RandomEngine(U32 seed)
{
	SetSeed(seed);
}

bool RandomEngine::GetBool()
{
	return Get(0, 1) == 1;  
}

void RandomEngine::SetSeed(U32 seed)
{
	mSeed = seed;
	mGenerator.seed(seed);
}

U32 RandomEngine::GetSeed() const
{
	return mSeed;
}

std::mt19937& RandomEngine::GetGenerator()
{
	return mGenerator;
}

RandomEngine& Random::priv::GetDefaultRandomEngine()
{
	static RandomEngine randomEngine;
	return randomEngine;
}

bool Random::GetBool()
{
	return priv::GetDefaultRandomEngine().GetBool();
}

} // namespace en
