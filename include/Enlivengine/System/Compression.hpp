#pragma once

#include <string>
#include <Enlivengine/System/PrimitiveTypes.hpp>

namespace en
{

class Compression
{
	public:
		static bool encode64(std::string& data);
		static bool decode64(std::string& data);
		static bool compress(std::string& data);
		static bool decompress(std::string& data);

		static bool compress64(std::string& data);
		static bool decompress64(std::string& data);

	private:
		static const std::string mBase64Table;

		static bool isBase64(U8 c);
};

} // namespace en
