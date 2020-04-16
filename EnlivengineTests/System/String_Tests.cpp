#include <Enlivengine/System/String.hpp>

#include <doctest/doctest.h>

DOCTEST_TEST_CASE("Strings")
{
	std::string testToLower = "Test of THE TOLOwer FUNctION";
	en::toLower(testToLower);
	DOCTEST_CHECK(testToLower == "test of the tolower function");
	std::string testToUpper = "Test of THE TOUPper FUNctION";
	en::toUpper(testToUpper);
	DOCTEST_CHECK(testToUpper == "TEST OF THE TOUPPER FUNCTION");

	std::string testSplit1 = "hello,world,itsme";
	std::string testSplit1Res;
	DOCTEST_CHECK(en::split(testSplit1, testSplit1Res, ','));
	DOCTEST_CHECK(testSplit1Res == "hello");
	DOCTEST_CHECK(testSplit1 == "world,itsme");
	DOCTEST_CHECK(en::split(testSplit1, testSplit1Res, ','));
	DOCTEST_CHECK(testSplit1Res == "world");
	DOCTEST_CHECK(testSplit1 == "itsme");
	DOCTEST_CHECK(!en::split(testSplit1, testSplit1Res, ','));
	std::string testSplit2 = "hello==world==itsme";
	std::string testSplit2Res;
	DOCTEST_CHECK(en::split(testSplit2, testSplit2Res, "=="));
	DOCTEST_CHECK(testSplit2Res == "hello");
	DOCTEST_CHECK(testSplit2 == "world==itsme");
	DOCTEST_CHECK(en::split(testSplit2, testSplit2Res, "=="));
	DOCTEST_CHECK(testSplit2Res == "world");
	DOCTEST_CHECK(testSplit2 == "itsme");
	DOCTEST_CHECK(!en::split(testSplit2, testSplit2Res, "=="));

	std::string testContains = "Hello world !";
	DOCTEST_CHECK(en::contains(testContains, 'd'));
	DOCTEST_CHECK(!en::contains(testContains, 'a'));
	DOCTEST_CHECK(en::contains(testContains, "o w"));
	DOCTEST_CHECK(!en::contains(testContains, "ed"));

	std::string giantString = "This is the test of this function called : limitSize";
	std::string smallString = "Hello !";
	std::string storeString1;
	std::string storeString2;
	DOCTEST_CHECK(en::limitSize(giantString, storeString1, 10));
	DOCTEST_CHECK(!en::limitSize(smallString, storeString2, 10));
	DOCTEST_CHECK(storeString1 == "This is th");
	DOCTEST_CHECK(storeString2 == "Hello !");
	DOCTEST_CHECK(en::limitSize(giantString, 10));
	DOCTEST_CHECK(!en::limitSize(smallString, 10));
	DOCTEST_CHECK(giantString == "This is th");
	DOCTEST_CHECK(smallString == "Hello !");

	/*
	std::string testTrimWhitespace = "   \t   \t Hello world !   \t   \t   ";
	en::trimWhitespace(testTrimWhitespace);
	DOCTEST_CHECK(testTrimWhitespace == "Hello world !");
	*/

	std::string testBoolString;
	testBoolString = en::toBoolString(true);
	DOCTEST_CHECK(testBoolString == "true");
	testBoolString = en::toBoolString(false);
	DOCTEST_CHECK(testBoolString == "false");

	std::string testToString;
	testToString = en::toString("test");
	DOCTEST_CHECK(testToString == "test");
	testToString = en::toString(true);
	DOCTEST_CHECK(testToString == "1");
	testToString = en::toString(false);
	DOCTEST_CHECK(testToString == "0");
	testToString = en::toString('c');
	DOCTEST_CHECK(testToString == "c");
	testToString = en::toString(2.3f);
	DOCTEST_CHECK(testToString == "2.3");
	testToString = en::toString(234);
	DOCTEST_CHECK(testToString == "234");
	testToString = en::toString(-234);
	DOCTEST_CHECK(testToString == "-234");

	std::string testFromString = en::fromString<std::string>("test");
	DOCTEST_CHECK(testFromString == "test");
	bool testFromStringBool = en::fromString<bool>("true");
	DOCTEST_CHECK(testFromStringBool == true);
	testFromStringBool = en::fromString<bool>("false");
	DOCTEST_CHECK(testFromStringBool == false);
	testFromStringBool = en::fromString<bool>("1");
	DOCTEST_CHECK(testFromStringBool == true);
	testFromStringBool = en::fromString<bool>("0");
	DOCTEST_CHECK(testFromStringBool == false);
	testFromStringBool = en::fromString<bool>("x");
	DOCTEST_CHECK(testFromStringBool == false);
	char testFromStringChar = en::fromString<char>("t");
	DOCTEST_CHECK(testFromStringChar == 't');
	en::F32 testFromStringF32 = en::fromString<en::F32>("2.3");
	DOCTEST_CHECK(testFromStringF32 == 2.3f);
	en::U32 testFromStringU32 = en::fromString<en::U32>("234");
	DOCTEST_CHECK(testFromStringU32 == 234);
	en::I32 testFromStringI32 = en::fromString<en::I32>("-234");
	DOCTEST_CHECK(testFromStringI32 == -234);

	/*
	en::StringId id1 = en::StringId::hash("Test");
	en::StringId id2 = en::StringId::hash("TestTest");
	en::StringId id3 = en::StringId::hashAndStore("TestTestTest");
	en::StringId id4;

	DOCTEST_CHECK(id1.isValid());
	DOCTEST_CHECK(id2.isValid());
	DOCTEST_CHECK(id3.isValid());
	DOCTEST_CHECK(!id4.isValid());

	DOCTEST_CHECK(id1 != id2);
	DOCTEST_CHECK(id1 != id3);
	DOCTEST_CHECK(id1 != id4);
	DOCTEST_CHECK(id2 != id3);
	DOCTEST_CHECK(id2 != id4);
	DOCTEST_CHECK(id3 != id4);

	DOCTEST_CHECK(!id1.isStored());
	DOCTEST_CHECK(!id2.isStored());
	DOCTEST_CHECK(id3.isStored());

	DOCTEST_CHECK(id1.getStringFromStorage() == nullptr);
	DOCTEST_CHECK(id2.getStringFromStorage() == nullptr);
	DOCTEST_CHECK(id3.getStringFromStorage() != nullptr);
	DOCTEST_CHECK(strcmp(id3.getStringFromStorage(), "TestTestTest") == 0);
	*/
}