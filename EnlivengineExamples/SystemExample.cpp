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

#include <Enlivengine/Math/Utilities.hpp>

#include <cmath>

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

void MathFunctionsSpeedComparaisonWithStd();

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

	MathFunctionsSpeedComparaisonWithStd();

	return 0;
}


void MathFunctionsSpeedComparaisonWithStd()
{
	en::Time cosStd;
	en::Time cosEn;
	en::Time sinStd;
	en::Time sinEn;
	en::Time tanStd;
	en::Time tanEn;
	en::Time acosStd;
	en::Time acosEn;
	en::Time asinStd;
	en::Time asinEn;
	en::Time atanStd;
	en::Time atanEn;
	en::Time atan2Std;
	en::Time atan2En;

	en::Clock clock;
	en::F32 sumStd = 0.0f;
	en::F32 sumEn = 0.0f;

	clock.Restart();
	for (en::F32 x = 0.0f; x <= 3600.0f; x += 0.01f)
	{
		const en::F32 y = std::sin(x * en::Math::kDegToRad);
		sumStd += y;
	}
	sinStd = clock.Restart();
	for (en::F32 x = 0.0f; x <= 3600.0f; x += 0.01f)
	{
		const en::F32 y = en::Math::Sin(x);
		sumEn += y;
	}
	sinEn = clock.Restart();

	clock.Restart();
	for (en::F32 x = 0.0f; x <= 3600.0f; x += 0.01f)
	{
		const en::F32 y = std::cos(x * en::Math::kDegToRad);
		sumStd += y;
	}
	cosStd = clock.Restart();
	for (en::F32 x = 0.0f; x <= 3600.0f; x += 0.01f)
	{
		const en::F32 y = en::Math::Cos(x);
		sumEn += y;
	}
	cosEn = clock.Restart();

	clock.Restart();
	for (en::F32 x = 0.0f; x <= 3600.0f; x += 0.01f)
	{
		const en::F32 y = std::tan(x * en::Math::kDegToRad);
		sumStd += y;
	}
	tanStd = clock.Restart();
	for (en::F32 x = 0.0f; x <= 3600.0f; x += 0.01f)
	{
		const en::F32 y = en::Math::Tan(x);
		sumEn += y;
	}
	tanEn = clock.Restart();

	clock.Restart();
	for (en::F32 x = -1.0f; x <= 1.0f; x += 0.000001f)
	{
		const en::F32 y = std::asin(x) * en::Math::kRadToDeg;
		sumStd += y;
	}
	asinStd = clock.Restart();
	for (en::F32 x = -1.0f; x <= 1.0f; x += 0.000001f)
	{
		const en::F32 y = en::Math::Asin(x);
		sumEn += y;
	}
	asinEn = clock.Restart();

	clock.Restart();
	for (en::F32 x = -1.0f; x <= 1.0f; x += 0.000001f)
	{
		const en::F32 y = std::acos(x) * en::Math::kRadToDeg;
		sumStd += y;
	}
	acosStd = clock.Restart();
	for (en::F32 x = -1.0f; x <= 1.0f; x += 0.000001f)
	{
		const en::F32 y = en::Math::Acos(x);
		sumEn += y;
	}
	acosEn = clock.Restart();

	clock.Restart();
	for (en::F32 x = -1.0f; x <= 1.0f; x += 0.000001f)
	{
		const en::F32 y = std::atan(x) * en::Math::kRadToDeg;
		sumStd += y;
	}
	atanStd = clock.Restart();
	for (en::F32 x = -1.0f; x <= 1.0f; x += 0.000001f)
	{
		const en::F32 y = en::Math::Atan(x);
		sumEn += y;
	}
	atanEn = clock.Restart();

	clock.Restart();
	for (en::F32 x = -1000.0f; x <= 1000.0f; x += 1.0f)
	{
		for (en::F32 y = -1000.0f; y <= 1000.0f; y += 1.0f)
		{
			if (x != 0.0f && y != 0.0f)
			{
				const en::F32 z = std::atan2(y, x) * en::Math::kRadToDeg;
				sumStd += y;
			}
		}
	}
	atan2Std = clock.Restart();
	for (en::F32 x = -1000.0f; x <= 1000.0f; x += 1.0f)
	{
		for (en::F32 y = -1000.0f; y <= 1000.0f; y += 1.0f)
		{
			if (x != 0.0f && y != 0.0f)
			{
				const en::F32 z = en::Math::Atan2(x, y);
				sumEn += y;
			}
		}
	}
	atan2En = clock.Restart();

	enLogInfo(en::LogChannel::Math, "Sin : {} vs {} => {}", sinStd.AsSeconds(), sinEn.AsSeconds(), sinEn <= sinStd);
	enLogInfo(en::LogChannel::Math, "Cos : {} vs {} => {}", cosStd.AsSeconds(), cosEn.AsSeconds(), cosEn <= cosStd);
	enLogInfo(en::LogChannel::Math, "Tan : {} vs {} => {}", tanStd.AsSeconds(), tanEn.AsSeconds(), tanEn <= tanStd);
	enLogInfo(en::LogChannel::Math, "Asin : {} vs {} => {}", asinStd.AsSeconds(), asinEn.AsSeconds(), asinEn <= asinStd);
	enLogInfo(en::LogChannel::Math, "Acos : {} vs {} => {}", acosStd.AsSeconds(), acosEn.AsSeconds(), acosEn <= acosStd);
	enLogInfo(en::LogChannel::Math, "Atan : {} vs {} => {}", atanStd.AsSeconds(), atanEn.AsSeconds(), atanEn <= atanStd);
	enLogInfo(en::LogChannel::Math, "Atan2 : {} vs {} => {}", atan2Std.AsSeconds(), atan2En.AsSeconds(), atan2En <= atan2Std);
	enLogInfo(en::LogChannel::Math, "Precision : {} vs {} => {}", sumStd, sumEn, en::Math::Equals(sumStd, sumEn));
}