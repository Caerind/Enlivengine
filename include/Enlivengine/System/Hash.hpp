#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>

#include <cstring>

#include <meow_hash/meow_hash.h>

// TODO : Constexpr

namespace en
{

class Hash
{
public:
	Hash() = delete;

	static U32 Meow(const char* key)
	{
		return MeowU32From(MeowHash(MeowDefaultSeed, strlen(key), (void*)key), 0);
	}
};

} // namespace en