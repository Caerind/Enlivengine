#pragma once

#include <functional>
#include <type_traits>

#include <Enlivengine/Math/Random.hpp>

namespace en
{

namespace priv
{
	template <typename T>
	struct ConstantFunctor
	{
		explicit ConstantFunctor(T value)
			: value(value)
		{
		}

		T operator()() const
		{
			return value;
		}

		T value;
	};
} // namespace priv

template <typename T>
class Distribution
{
	public:
        Distribution(T constant)
        : mFactory(priv::ConstantFunctor<T>(constant))
		{
		}

        Distribution(std::function<T()> function)
		: mFactory(function)
		{
		}

		T operator()() const
		{
			return mFactory();
		}

	private:
		std::function<T()> mFactory;
};

} // namespace en
