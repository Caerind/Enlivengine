#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_CORE

#include <tracerz/tracerz.h>
#include <fstream>

namespace en
{

class TraceryGenerator
{
private:
	using TraceryGeneratorInternal = tracerz::Grammar<std::mt19937, std::uniform_int_distribution<>>;

public:
	TraceryGenerator() {}

	bool LoadFromFile(const std::string& filename)
	{
		std::ifstream file(filename);
		if (!file)
		{
			return false;
		}
		nlohmann::json jsonInput;
		// Don't know why, but it seems the operator>> is not recognized...
		nlohmann::detail::parser<nlohmann::json>(nlohmann::detail::input_adapter(file)).parse(false, jsonInput);
		if (jsonInput.is_null())
		{
			return false;
		}
		mGrammar = TraceryGeneratorInternal(jsonInput);
		return true;
	}

	void AddBaseModifiers()
	{
		mGrammar.addModifiers(tracerz::getBaseExtendedModifiers());
	}

	void AddEnglishModifiers()
	{
		mGrammar.addModifiers(tracerz::getBaseEngModifiers());
	}

	void AddModifier(const std::string& name, std::function<std::string(const std::string&)> modifier)
	{
		static auto wrap = [](const std::function<std::string(const std::string&)>& fun) 
		{
			std::shared_ptr<tracerz::details::IModifierFn> ptr(new tracerz::details::ModifierFn<const std::string&>(fun));
			return ptr;
		};
		mGrammar.addModifier(name, wrap(modifier));
	}

	void AddGrammar(const std::string& grammar, const std::vector<std::string>& strings)
	{
		mGrammar.getGrammar()[grammar] = strings;
	}

	std::string Generate(const std::string& input)
	{
		return mGrammar.flatten(input);
	}

private:
	TraceryGeneratorInternal mGrammar;
};

} // namespace en

#endif // ENLIVE_MODULE_CORE