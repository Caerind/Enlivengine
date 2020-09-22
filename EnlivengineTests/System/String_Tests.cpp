#include <Enlivengine/System/String.hpp>

#include <doctest/doctest.h>

DOCTEST_TEST_CASE("Strings")
{
	std::string testToLower = "Test of THE TOLOwer FUNctION";
	en::ToLower(testToLower);
	DOCTEST_CHECK(testToLower == "test of the tolower function");
	std::string testToUpper = "Test of THE TOUPper FUNctION";
	en::ToUpper(testToUpper);
	DOCTEST_CHECK(testToUpper == "TEST OF THE TOUPPER FUNCTION");

	std::string testIsNumber1F = "Test1";
	std::string testIsNumber2F = "2Test";
	std::string testIsNumber3F = "3Test4";
	std::string testIsNumber4F = "-5Test6";
	std::string testIsNumber5F = "7Test.8";
	DOCTEST_CHECK(!en::IsNumber(testIsNumber1F));
	DOCTEST_CHECK(!en::IsNumber(testIsNumber2F));
	DOCTEST_CHECK(!en::IsNumber(testIsNumber3F));
	DOCTEST_CHECK(!en::IsNumber(testIsNumber4F));
	DOCTEST_CHECK(!en::IsNumber(testIsNumber5F));
	std::string testIsNumber0T = "0";
	std::string testIsNumber1T = "1";
	std::string testIsNumber2T = "22222222222";
	std::string testIsNumber3T = "34";
	std::string testIsNumber4T = "-56";
	std::string testIsNumber5T = "7.8";
	std::string testIsNumber6T = "-9.001";
	DOCTEST_CHECK(en::IsNumber(testIsNumber0T));
	DOCTEST_CHECK(en::IsNumber(testIsNumber1T));
	DOCTEST_CHECK(en::IsNumber(testIsNumber2T));
	DOCTEST_CHECK(en::IsNumber(testIsNumber3T));
	DOCTEST_CHECK(en::IsNumber(testIsNumber4T));
	DOCTEST_CHECK(en::IsNumber(testIsNumber5T));
	DOCTEST_CHECK(en::IsNumber(testIsNumber6T));

	std::string testSplit1 = "hello,world,itsme";
	std::string testSplit1Res;
	DOCTEST_CHECK(en::Split(testSplit1, testSplit1Res, ','));
	DOCTEST_CHECK(testSplit1Res == "hello");
	DOCTEST_CHECK(testSplit1 == "world,itsme");
	DOCTEST_CHECK(en::Split(testSplit1, testSplit1Res, ','));
	DOCTEST_CHECK(testSplit1Res == "world");
	DOCTEST_CHECK(testSplit1 == "itsme");
	DOCTEST_CHECK(!en::Split(testSplit1, testSplit1Res, ','));
	std::string testSplit2 = "hello==world==itsme";
	std::string testSplit2Res;
	DOCTEST_CHECK(en::Split(testSplit2, testSplit2Res, "=="));
	DOCTEST_CHECK(testSplit2Res == "hello");
	DOCTEST_CHECK(testSplit2 == "world==itsme");
	DOCTEST_CHECK(en::Split(testSplit2, testSplit2Res, "=="));
	DOCTEST_CHECK(testSplit2Res == "world");
	DOCTEST_CHECK(testSplit2 == "itsme");
	DOCTEST_CHECK(!en::Split(testSplit2, testSplit2Res, "=="));

	std::string testContains = "Hello world !";
	DOCTEST_CHECK(en::Contains(testContains, 'd'));
	DOCTEST_CHECK(!en::Contains(testContains, 'a'));
	DOCTEST_CHECK(en::Contains(testContains, "o w"));
	DOCTEST_CHECK(!en::Contains(testContains, "ed"));

	std::string giantString = "This is the test of this function called : limitSize";
	std::string smallString = "Hello !";
	std::string storeString1;
	std::string storeString2;
	DOCTEST_CHECK(en::LimitSize(giantString, storeString1, 10));
	DOCTEST_CHECK(!en::LimitSize(smallString, storeString2, 10));
	DOCTEST_CHECK(storeString1 == "This is th");
	DOCTEST_CHECK(storeString2 == "Hello !");
	DOCTEST_CHECK(en::LimitSize(giantString, 10));
	DOCTEST_CHECK(!en::LimitSize(smallString, 10));
	DOCTEST_CHECK(giantString == "This is th");
	DOCTEST_CHECK(smallString == "Hello !");

	/*
	std::string testTrimWhitespace = "   \t   \t Hello world !   \t   \t   ";
	en::trimWhitespace(testTrimWhitespace);
	DOCTEST_CHECK(testTrimWhitespace == "Hello world !");
	*/

	std::string testBoolString;
	testBoolString = en::ToBoolString(true);
	DOCTEST_CHECK(testBoolString == "true");
	testBoolString = en::ToBoolString(false);
	DOCTEST_CHECK(testBoolString == "false");

	std::string testToString;
	testToString = en::ToString("test");
	DOCTEST_CHECK(testToString == "test");
	testToString = en::ToString(true);
	DOCTEST_CHECK(testToString == "1");
	testToString = en::ToString(false);
	DOCTEST_CHECK(testToString == "0");
	testToString = en::ToString('c');
	DOCTEST_CHECK(testToString == "c");
	testToString = en::ToString(2.3f);
	DOCTEST_CHECK(testToString == "2.3");
	testToString = en::ToString(234);
	DOCTEST_CHECK(testToString == "234");
	testToString = en::ToString(-234);
	DOCTEST_CHECK(testToString == "-234");

	std::string testFromString = en::FromString<std::string>("test");
	DOCTEST_CHECK(testFromString == "test");
	bool testFromStringBool = en::FromString<bool>("true");
	DOCTEST_CHECK(testFromStringBool == true);
	testFromStringBool = en::FromString<bool>("false");
	DOCTEST_CHECK(testFromStringBool == false);
	testFromStringBool = en::FromString<bool>("1");
	DOCTEST_CHECK(testFromStringBool == true);
	testFromStringBool = en::FromString<bool>("0");
	DOCTEST_CHECK(testFromStringBool == false);
	testFromStringBool = en::FromString<bool>("x");
	DOCTEST_CHECK(testFromStringBool == false);
	char testFromStringChar = en::FromString<char>("t");
	DOCTEST_CHECK(testFromStringChar == 't');
	en::F32 testFromStringF32 = en::FromString<en::F32>("2.3");
	DOCTEST_CHECK(testFromStringF32 == 2.3f);
	en::U32 testFromStringU32 = en::FromString<en::U32>("234");
	DOCTEST_CHECK(testFromStringU32 == 234);
	en::I32 testFromStringI32 = en::FromString<en::I32>("-234");
	DOCTEST_CHECK(testFromStringI32 == -234);
}