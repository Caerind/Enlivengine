#pragma once

#include <string>

#include <Enlivengine/System/PrimitiveTypes.hpp>

namespace en
{

class Hash
{
public:
	Hash() = delete;

	static constexpr U32 Combine32(U32 hash1, U32 hash2) { return hash1 ^ (hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2)); }
	static constexpr U64 Combine64(U64 hash1, U64 hash2) { return hash1 ^ (hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2)); }

	// Use this in constexpr context as Meow is not constexpr
	static constexpr U32 SlowHash(const char* str) 
	{ 
		if (str != nullptr)
		{
			U32 h = 89;
			while (*str != 0)
			{
				h = h * 33 + (*str++);
			}
			return h;
		}
		else
		{
			return 0;
		}
	}
	static constexpr U32 SlowHash(std::string_view str) 
	{ 
		const U32 length = static_cast<U32>(str.size());
		if (length > 0)
		{
			U32 h = 89;
			for (U32 i = 0; i < length; ++i)
			{
				h = h * 33 + static_cast<U32>(str[i]);
			}
			return h;
		}
		else
		{
			return 0;
		}
	}
};

} // namespace en
