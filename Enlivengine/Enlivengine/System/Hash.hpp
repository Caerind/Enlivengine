#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>

#include <cstring>
#include <string>

namespace en
{

class Hash
{
public:
	Hash() = delete;

	static constexpr U32 CRC32(const char* key)
	{
                if (key == nullptr)
                {
                    return 0;
                }
		std::size_t len = length(key);
		if (key == nullptr || len == 0)
		{
			return 0;
		}
		U32 h = 0;
		while (len--)
		{
			h ^= (*key++);
			h = crc32Table[h & 0x0f] ^ (h >> 4);
			h = crc32Table[h & 0x0f] ^ (h >> 4);
		}
		return { ~h }; // TODO : Endianness ?
	}

	static constexpr U32 CRC32(std::string_view key)
	{
		const std::size_t len = key.size();
		if (len == 0)
		{
			return 0;
		}
		U32 h = 0;
		for (U32 i = 0; i < len; ++i)
		{
			h ^= key[i];
			h = crc32Table[h & 0x0f] ^ (h >> 4);
			h = crc32Table[h & 0x0f] ^ (h >> 4);
		}
		return { ~h }; // TODO : Endianness ?
	}

	static constexpr inline U32 CombineHash(U32 hash1, U32 hash2)
	{
		return hash1 ^ (hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2));
	}

private:
	static constexpr size_t length(const char* const key)
	{
		return *key ? 1 + length(key + 1) : 0;
	}

	static constexpr U32 crc32Table[16] =
	{
		0x00000000, 0x1DB71064, 0x3B6E20C8, 0x26D930AC,
		0x76DC4190, 0x6B6B51F4, 0x4DB26158, 0x5005713C,
		0xEDB88320, 0xF00F9344, 0xD6D6A3E8, 0xCB61B38C,
		0x9B64C2B0, 0x86D3D2D4, 0xA00AE278, 0xBDBDF21C
	};
};

} // namespace en
