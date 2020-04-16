#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <string>

namespace en
{

class ParserJson
{
	public:
		ParserJson();
		ParserJson(const std::string& filename);

		const std::string& getFilename() const;

	private:
		std::string mFilename;
};

} // namespace en