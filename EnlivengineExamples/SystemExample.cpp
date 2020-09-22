#include <iostream>

#include <SFML/Graphics.hpp>

#include <Box2D/Box2D.h>

#include <Enlivengine/System/Preprocessor.hpp>
#include <Enlivengine/System/PlatformDetection.hpp>
#include <Enlivengine/System/CompilerDetection.hpp>
#include <Enlivengine/System/CompilerTraits.hpp>
#include <Enlivengine/System/ByteUnits.hpp>
#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/System/Endianness.hpp>
#include <Enlivengine/System/Debugger.hpp>
#include <Enlivengine/System/Log.hpp>
#include <Enlivengine/System/Time.hpp>
#include <Enlivengine/System/Hash.hpp>
#include <Enlivengine/System/String.hpp>
#include <Enlivengine/System/TypeInfo.hpp>
#include <Enlivengine/System/CallOnExit.hpp>
#include <Enlivengine/System/Meta.hpp>
#include <Enlivengine/System/MetaEnum.hpp>
#include <Enlivengine/System/Assert.hpp>

enum class SomeEnum
{
	A,
	B,
	C
};

enum class FlagEnum
{
	A = 0x01,
	B = 0x02,
	C = 0x04,
	D = 0x08
};

void MetaEnumTest()
{
	std::cout << "-- EnumTest --" << std::endl;
	std::cout << std::endl;

	// SomeEnum
	std::cout << en::Meta::GetEnumTypeName<SomeEnum>() << std::endl;
	std::cout << en::Meta::GetEnumName(SomeEnum::A) << std::endl;
	for (const auto& element : en::Meta::GetEnumNames<SomeEnum>())
	{
		std::cout << element << std::endl;
	}
	std::cout << en::Meta::GetEnumName(en::Meta::EnumCast<SomeEnum>(1)) << std::endl;
	std::cout << en::Meta::GetEnumCount<SomeEnum>() << std::endl;

	std::cout << std::endl;

	// FlagEnum
	std::cout << en::Meta::GetEnumTypeName<FlagEnum>() << std::endl;
	std::cout << en::Meta::GetEnumName(FlagEnum::A) << std::endl;
	for (const auto& element : en::Meta::GetEnumNames<FlagEnum>())
	{
		std::cout << element << std::endl;
	}
	std::cout << en::Meta::GetEnumName(en::Meta::EnumCast<FlagEnum>(1)) << std::endl;
	std::cout << en::Meta::GetEnumCount<FlagEnum>() << std::endl;

	std::cout << std::endl;
	std::cout << "-- -- --" << std::endl;
}


enum class LogChannelClient
{
	Gameplay1 = static_cast<en::U32>(en::LogChannel::Max),
	Gameplay2,
	Gameplay3
};


int main()
{
#ifdef ENLIVE_ENABLE_LOG
	en::LogManager::GetInstance().InitializeClientChannels<LogChannelClient>();
#endif // ENLIVE_ENABLE_LOG

	enAssert(true);
	enAssert(!false);

	{
		en::CallOnExit callWhenScopeEnds([]() { enLogInfo(en::LogChannel::System, "CallOnExit::Call() : Scope is now closed"); });
	}

	enLogInfo(en::LogChannel::System, "ENLIVE_FUNCTION : {}", ENLIVE_FUNCTION);
	enLogInfo(en::LogChannel::System, "ENLIVE_COMPILER_NAME : {}", ENLIVE_COMPILER_NAME);
	enLogInfo(en::LogChannel::System, "ENLIVE_COMPILER_VERSION : {}", ENLIVE_STRINGIFY(ENLIVE_COMPILER_VERSION));
	enLogInfo(en::LogChannel::System, "ENLIVE_COMPILER_STRING : {}", ENLIVE_COMPILER_STRING);
	enLogInfo(en::LogChannel::System, "ENLIVE_PLATFORM_NAME : {}", ENLIVE_PLATFORM_NAME);
	enLogInfo(en::LogChannel::System, "ENLIVE_PLATFORM_DESCRIPTION : {}", ENLIVE_PLATFORM_DESCRIPTION);

	std::cout << "TestColor" << std::endl;

	enLogInfo(LogChannelClient::Gameplay1, "Test 1");
	enLogWarning(LogChannelClient::Gameplay2, "Test 2 : {}", 3);
	enLogError(LogChannelClient::Gameplay3, "Test 3 : {}", "azerty");
	enLogFatal(en::LogChannel::Graphics, "Assert {} {}", __FILE__, __LINE__);

	std::cout << "TestColor" << std::endl;
		
	/*	
	LogInfo(en::LogChannel::System, 9, "A: {}", en::TypeInfo<A>::GetName());
	LogInfo(en::LogChannel::System, 9, "B<A>: {}", en::TypeInfo<B<A>>::GetName());
	LogInfo(en::LogChannel::System, 9, "B<en::U32>: {}", en::TypeInfo<B<en::U32>>::GetName());
	LogInfo(en::LogChannel::System, 9, "B<C>: {}", en::TypeInfo<B<C>>::GetName());
	*/

	MetaEnumTest();

	return 0;
}
