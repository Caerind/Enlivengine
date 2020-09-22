#pragma once

#include <string>
#include <vector>

#include <Enlivengine/System/PrimitiveTypes.hpp>

namespace en
{

class Compression
{
public: // Base64
	static constexpr const char kBase64Table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	static constexpr const char kBase64PadCharacter = '=';
	static bool IsBase64(U8 c);
	static bool Encode64(const std::vector<U8>& input, std::string& output);
	static bool Decode64(const std::string& input, std::vector<U8>& output);
	static bool Encode64String(const std::string& input, std::string& output);
	static bool Decode64String(const std::string& input, std::string& output);

public:
	enum class CompressionLevel
	{
		NoCompression,
		Default,
		BestSpeed,
		BestCompression,
		UberCompression
	};
	static bool CompressZlib(const std::vector<U8>& input, std::vector<U8>& output);
	static bool DecompressZlib(const std::vector<U8>& input, std::vector<U8>& output);
};

} // namespace en
