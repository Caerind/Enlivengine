#pragma once

#include <random>

#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/System/Assert.hpp>

namespace en
{

class RandomEngine
{
	public:
		RandomEngine();
		RandomEngine(U32 seed);

		template <typename T>
		T get(T min, T max);

		template <typename T>
		T getDev(T middle, T deviation);

		bool getBool();

		void setSeed(U32 seed);
		U32 getSeed() const;

		std::mt19937& getGenerator();

	private:
		std::mt19937 mGenerator;
		U32 mSeed;
};

namespace Random
{

namespace priv
{

RandomEngine& getRandomEngine();

} // namespace priv

template <typename T>
T get(T min, T max);

template <typename T>
T getDev(T middle, T deviation);

bool getBool();

template<typename T>
T get(T min, T max)
{
	return priv::getRandomEngine().get<T>(min, max);
}

template<typename T>
T getDev(T middle, T deviation)
{
	return priv::getRandomEngine().getDev<T>(middle, deviation);
}

} // namespace Random

template<typename T>
inline T RandomEngine::get(T min, T max)
{
	enAssert(false); // Not implemented
	ENLIVE_UNUSED(min);
	ENLIVE_UNUSED(max);
	return T();
}

template<typename T>
inline T RandomEngine::getDev(T middle, T deviation)
{
	enAssert(false); // Not implemented
	ENLIVE_UNUSED(middle);
	ENLIVE_UNUSED(deviation);
	return T();
}

template<>
inline I32 RandomEngine::get(I32 min, I32 max)
{
	enAssert(min <= max);
	std::uniform_int_distribution<I32> distribution(min, max);
	return distribution(mGenerator);
}

template<>
inline U32 RandomEngine::get(U32 min, U32 max)
{
	enAssert(min <= max);
	std::uniform_int_distribution<U32> distribution(min, max);
	return distribution(mGenerator);
}

template<>
inline F32 RandomEngine::get(F32 min, F32 max)
{
	enAssert(min <= max);
	std::uniform_real_distribution<F32> distribution(min, max);
	return distribution(mGenerator);
}

template<>
inline I32 RandomEngine::getDev(I32 middle, I32 deviation)
{
	enAssert(deviation >= 0);
	return get(middle - deviation, middle + deviation);
}

template<>
inline U32 RandomEngine::getDev(U32 middle, U32 deviation)
{
	return get(middle - deviation, middle + deviation);
}

template<>
inline F32 RandomEngine::getDev(F32 middle, F32 deviation)
{
	enAssert(deviation >= 0.0f);
	return get(middle - deviation, middle + deviation);
}

} // namespace en