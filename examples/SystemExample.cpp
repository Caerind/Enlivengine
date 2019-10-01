#include <Enlivengine/System/Macros.hpp>
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

#include <cocoa/cocoa.hpp>

#include <Box2D/Box2D.h>

#include <SFML/Graphics.hpp>

int main(int argc, char** argv)
{
	en::LogManager::initialize();

	LogInfo(en::LogChannel::System, 9, "ENLIVE_FUNCTION : %s", ENLIVE_FUNCTION);
	LogInfo(en::LogChannel::System, 9, "ENLIVE_COMPILER_NAME : %s", ENLIVE_COMPILER_NAME);
	LogInfo(en::LogChannel::System, 9, "ENLIVE_COMPILER_VERSION : %s", ENLIVE_STRINGIFY(ENLIVE_COMPILER_VERSION));
	LogInfo(en::LogChannel::System, 9, "ENLIVE_COMPILER_STRING : %s", ENLIVE_COMPILER_STRING);
	LogInfo(en::LogChannel::System, 9, "ENLIVE_PLATFORM_NAME : %s", ENLIVE_PLATFORM_NAME);
	LogInfo(en::LogChannel::System, 9, "ENLIVE_PLATFORM_DESCRIPTION : %s", ENLIVE_PLATFORM_DESCRIPTION);

	en::U32 hash = en::Hash::Meow("Test");
	LogInfo(en::LogChannel::Global, 7, "Hash of \"Test\" : %d", hash);

	LogInfo(en::LogChannel::System, 9, "SHA1 of \"Test\" from cocoa : %s", cocoa::SHA1("Test").str().c_str());

	return 0;
}
