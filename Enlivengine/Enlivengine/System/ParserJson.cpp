#include <Enlivengine/System/ParserJson.hpp>

#include <fstream>
#include <iomanip>

namespace en
{

ParserJson::ParserJson()
	: mFilename("")
{
}

ParserJson::ParserJson(const std::string& filename)
	: mFilename(filename)
{
}

const std::string& ParserJson::getFilename() const
{
	return mFilename;
}

} // namespace en