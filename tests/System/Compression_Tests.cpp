#include <Enlivengine/System/Compression.hpp>

#include <doctest/doctest.h>

DOCTEST_TEST_CASE("Compression determinism")
{
	const std::string myStr = "AZERTYUIOPQSDFGHJKLMwxcvbn1234567890";
	//DOCTEST_INFO(myStr);

	std::string myStrEncode = myStr;
	bool encode64Valid = en::Compression::encode64(myStrEncode);
	//DOCTEST_INFO(myStrEncode);
	DOCTEST_CHECK(myStr != myStrEncode);
	bool decode64Valid = en::Compression::decode64(myStrEncode);
	DOCTEST_CHECK(encode64Valid);
	DOCTEST_CHECK(decode64Valid);
	DOCTEST_CHECK(myStr == myStrEncode);

	std::string myStrCompress = myStr;
	bool compressValid = en::Compression::compress(myStrCompress);
	//DOCTEST_INFO(myStrCompress);
	DOCTEST_CHECK(myStr != myStrCompress);
	bool decompressValid = en::Compression::decompress(myStrCompress);
	DOCTEST_CHECK(compressValid);
	DOCTEST_CHECK(decompressValid);
	DOCTEST_CHECK(myStr == myStrCompress);

	std::string myStrCompress64 = myStr;
	bool compress64Valid = en::Compression::compress(myStrCompress64);
	//DOCTEST_INFO(myStrCompress64);
	DOCTEST_CHECK(myStr != myStrCompress64);
	bool decompress64Valid = en::Compression::decompress(myStrCompress64);
	DOCTEST_CHECK(compress64Valid);
	DOCTEST_CHECK(decompress64Valid);
	DOCTEST_CHECK(myStr == myStrCompress64);
}