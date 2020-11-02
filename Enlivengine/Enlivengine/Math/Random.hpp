#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_MATH

#include <random>

#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Utils/Assert.hpp>

namespace en
{

class RandomEngine
{
	public:
		RandomEngine();
		RandomEngine(U32 seed);

		template <typename T>
		T Get(T min, T max);

		template <typename T>
		T GetDev(T middle, T deviation);

		bool GetBool();

		void SetSeed(U32 seed);
		U32 GetSeed() const;

		std::mt19937& GetGenerator();

	private:
		std::mt19937 mGenerator;
		U32 mSeed;
};

namespace Random
{

namespace priv
{

RandomEngine& GetDefaultRandomEngine();

} // namespace priv

template <typename T>
T Get(T min, T max)
{
	return priv::GetDefaultRandomEngine().Get<T>(min, max);
}

template <typename T>
T GetDev(T middle, T deviation)
{
	return priv::GetDefaultRandomEngine().GetDev<T>(middle, deviation);
}

bool GetBool();

} // namespace Random

template<typename T>
inline T RandomEngine::Get(T min, T max)
{
	enAssert(false); // Not implemented
	ENLIVE_UNUSED(min);
	ENLIVE_UNUSED(max);
	return T();
}

template<typename T>
inline T RandomEngine::GetDev(T middle, T deviation)
{
	enAssert(false); // Not implemented
	ENLIVE_UNUSED(middle);
	ENLIVE_UNUSED(deviation);
	return T();
}

template<>
inline I32 RandomEngine::Get(I32 min, I32 max)
{
	enAssert(min <= max);
	std::uniform_int_distribution<I32> distribution(min, max);
	return distribution(mGenerator);
}

template<>
inline U32 RandomEngine::Get(U32 min, U32 max)
{
	enAssert(min <= max);
	std::uniform_int_distribution<U32> distribution(min, max);
	return distribution(mGenerator);
}

template<>
inline F32 RandomEngine::Get(F32 min, F32 max)
{
	enAssert(min <= max);
	std::uniform_real_distribution<F32> distribution(min, max);
	return distribution(mGenerator);
}

template<>
inline I32 RandomEngine::GetDev(I32 middle, I32 deviation)
{
	enAssert(deviation >= 0);
	return Get(middle - deviation, middle + deviation);
}

template<>
inline U32 RandomEngine::GetDev(U32 middle, U32 deviation)
{
	return Get(middle - deviation, middle + deviation);
}

template<>
inline F32 RandomEngine::GetDev(F32 middle, F32 deviation)
{
	enAssert(deviation >= 0.0f);
	return Get(middle - deviation, middle + deviation);
}

} // namespace en

#endif // ENLIVE_MODULE_MATH