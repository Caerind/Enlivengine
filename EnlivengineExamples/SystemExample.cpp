#include <iostream>

#include <Box2D/Box2D.h>

#include <Enlivengine/Platform/Preprocessor.hpp>
#include <Enlivengine/Platform/PlatformDetection.hpp>
#include <Enlivengine/Platform/CompilerDetection.hpp>
#include <Enlivengine/Platform/CompilerTraits.hpp>
#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Platform/Endianness.hpp>
#include <Enlivengine/Platform/Debugger.hpp>
#include <Enlivengine/Platform/Time.hpp>

#include <Enlivengine/Utils/ByteUnits.hpp>
#include <Enlivengine/Utils/Log.hpp>
#include <Enlivengine/Utils/Hash.hpp>
#include <Enlivengine/Utils/String.hpp>
#include <Enlivengine/Utils/CallOnExit.hpp>
#include <Enlivengine/Utils/Enums.hpp>
#include <Enlivengine/Utils/Assert.hpp>

#include <Enlivengine/Utils/TypeInfo.hpp>
#include <Enlivengine/Utils/Meta.hpp>

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
	std::cout << en::Enum::GetTypeName<SomeEnum>() << std::endl;
	std::cout << en::Enum::GetValueName(SomeEnum::A) << std::endl;
	for (const auto& element : en::Enum::GetValueNames<SomeEnum>())
	{
		std::cout << element << std::endl;
	}
	std::cout << en::Enum::GetValueName(en::Enum::Cast<SomeEnum>(1)) << std::endl;
	std::cout << en::Enum::GetCount<SomeEnum>() << std::endl;

	std::cout << std::endl;

	// FlagEnum
	std::cout << en::Enum::GetTypeName<FlagEnum>() << std::endl;
	std::cout << en::Enum::GetValueName(FlagEnum::A) << std::endl;
	for (const auto& element : en::Enum::GetValueNames<FlagEnum>())
	{
		std::cout << element << std::endl;
	}
	std::cout << en::Enum::GetValueName(en::Enum::Cast<FlagEnum>(1)) << std::endl;
	std::cout << en::Enum::GetCount<FlagEnum>() << std::endl;

	std::cout << std::endl;
	std::cout << "-- -- --" << std::endl;
}

#ifdef ENLIVE_ENABLE_LOG
enum class LogChannelClient
{
	Gameplay1 = static_cast<en::U32>(en::LogChannel::Max),
	Gameplay2,
	Gameplay3
};
#endif // ENLIVE_ENABLE_LOG

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
	enLogInfo(en::LogChannel::System, "A: {}", en::TypeInfo<A>::GetName());
	enLogInfo(en::LogChannel::System, "B<A>: {}", en::TypeInfo<B<A>>::GetName());
	enLogInfo(en::LogChannel::System, "B<en::U32>: {}", en::TypeInfo<B<en::U32>>::GetName());
	enLogInfo(en::LogChannel::System, "B<C>: {}", en::TypeInfo<B<C>>::GetName());
	*/

	MetaEnumTest();

	return 0;
}
