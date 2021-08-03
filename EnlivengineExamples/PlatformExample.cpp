#include <Enlivengine/Platform/Preprocessor.hpp>
#include <Enlivengine/Platform/PlatformDetection.hpp>
#include <Enlivengine/Platform/PlatformTraits.hpp>
#include <Enlivengine/Platform/CompilerDetection.hpp>
#include <Enlivengine/Platform/CompilerTraits.hpp>
#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Platform/Endianness.hpp>
#include <Enlivengine/Platform/Debugger.hpp>
#include <Enlivengine/Platform/Time.hpp>

#include <Enlivengine/Utils/Log.hpp>

int main()
{
	enLogInfo(en::LogChannel::System, "================================================");
	enLogInfo(en::LogChannel::System, "Platform:"); 
	enLogInfo(en::LogChannel::System, "ENLIVE_PLATFORM_NAME : {}", ENLIVE_PLATFORM_NAME);
	enLogInfo(en::LogChannel::System, "ENLIVE_PLATFORM_DESCRIPTION : {}", ENLIVE_PLATFORM_DESCRIPTION);

#ifdef ENLIVE_PLATFORM_MICROSOFT
	enLogInfo(en::LogChannel::System, "ENLIVE_PLATFORM_MICROSOFT");
#endif
#ifdef ENLIVE_PLATFORM_APPLE
	enLogInfo(en::LogChannel::System, "ENLIVE_PLATFORM_APPLE");
#endif

#ifdef ENLIVE_PLATFORM_WINDOWS
	enLogInfo(en::LogChannel::System, "ENLIVE_PLATFORM_WINDOWS");
#endif
#ifdef ENLIVE_PLATFORM_IOS
	enLogInfo(en::LogChannel::System, "ENLIVE_PLATFORM_IOS");
#endif
#ifdef ENLIVE_PLATFORM_MACOS
	enLogInfo(en::LogChannel::System, "ENLIVE_PLATFORM_MACOS");
#endif
#ifdef ENLIVE_PLATFORM_UNIX
	enLogInfo(en::LogChannel::System, "ENLIVE_PLATFORM_UNIX");
#endif
#ifdef ENLIVE_PLATFORM_ANDROID
	enLogInfo(en::LogChannel::System, "ENLIVE_PLATFORM_ANDROID");
#endif
#ifdef ENLIVE_PLATFORM_LINUX
	enLogInfo(en::LogChannel::System, "ENLIVE_PLATFORM_LINUX");
#endif
#ifdef ENLIVE_PLATFORM_FREEBSD
	enLogInfo(en::LogChannel::System, "ENLIVE_PLATFORM_FREEBSD");
#endif
#ifdef ENLIVE_PLATFORM_OPENBSD
	enLogInfo(en::LogChannel::System, "ENLIVE_PLATFORM_OPENBSD");
#endif

#ifdef ENLIVE_PLATFORM_POSIX
	enLogInfo(en::LogChannel::System, "ENLIVE_PLATFORM_POSIX");
#endif
#ifdef ENLIVE_PLATFORM_WIN64
	enLogInfo(en::LogChannel::System, "ENLIVE_PLATFORM_WIN64");
#endif
#ifdef ENLIVE_PLATFORM_WIN32
	enLogInfo(en::LogChannel::System, "ENLIVE_PLATFORM_WIN32");
#endif
#ifdef ENLIVE_PLATFORM_MINGW
	enLogInfo(en::LogChannel::System, "ENLIVE_PLATFORM_MINGW");
#endif
#ifdef ENLIVE_PLATFORM_CYGWIN
	enLogInfo(en::LogChannel::System, "ENLIVE_PLATFORM_CYGWIN");
#endif
#ifdef ENLIVE_PLATFORM_IPHONE_SIMULATOR
	enLogInfo(en::LogChannel::System, "ENLIVE_PLATFORM_IPHONE_SIMULATOR");
#endif

#ifdef ENLIVE_PROCESSOR_X86_64
	enLogInfo(en::LogChannel::System, "ENLIVE_PROCESSOR_X86_64");
#endif
#ifdef ENLIVE_PROCESSOR_X86
	enLogInfo(en::LogChannel::System, "ENLIVE_PROCESSOR_X86");
#endif
#ifdef ENLIVE_PROCESSOR_IA64
	enLogInfo(en::LogChannel::System, "ENLIVE_PROCESSOR_IA64");
#endif
#ifdef ENLIVE_PROCESSOR_ARM
	enLogInfo(en::LogChannel::System, "ENLIVE_PROCESSOR_ARM");
#endif
#ifdef ENLIVE_PROCESSOR_ARM32
	enLogInfo(en::LogChannel::System, "ENLIVE_PROCESSOR_ARM32");
#endif
#ifdef ENLIVE_PROCESSOR_ARM64
	enLogInfo(en::LogChannel::System, "ENLIVE_PROCESSOR_ARM64");
#endif
#ifdef ENLIVE_PROCESSOR_POWERPC
	enLogInfo(en::LogChannel::System, "ENLIVE_PROCESSOR_POWERPC");
#endif
#ifdef ENLIVE_PROCESSOR_POWERPC_32
	enLogInfo(en::LogChannel::System, "ENLIVE_PROCESSOR_POWERPC_32");
#endif
#ifdef ENLIVE_PROCESSOR_POWERPC_64
	enLogInfo(en::LogChannel::System, "ENLIVE_PROCESSOR_POWERPC_64");
#endif

#ifdef ENLIVE_ABI_ARM_WINCE
	enLogInfo(en::LogChannel::System, "ENLIVE_ABI_ARM_WINCE");
#endif
#ifdef ENLIVE_ABI_ARM_APPLE
	enLogInfo(en::LogChannel::System, "ENLIVE_ABI_ARM_APPLE");
#endif
#ifdef ENLIVE_ABI_ARM64_APPLE
	enLogInfo(en::LogChannel::System, "ENLIVE_ABI_ARM64_APPLE");
#endif
#ifdef ENLIVE_ABI_ARM_LINUX
	enLogInfo(en::LogChannel::System, "ENLIVE_ABI_ARM_LINUX");
#endif

#ifdef ENLIVE_PLATFORM_LITTLE_ENDIAN
	enLogInfo(en::LogChannel::System, "ENLIVE_PLATFORM_LITTLE_ENDIAN");
#endif
#ifdef ENLIVE_PLATFORM_BIG_ENDIAN
	enLogInfo(en::LogChannel::System, "ENLIVE_PLATFORM_BIG_ENDIAN");
#endif

#ifdef ENLIVE_PLATFORM_DESKTOP
	enLogInfo(en::LogChannel::System, "ENLIVE_PLATFORM_DESKTOP");
#endif
#ifdef ENLIVE_PLATFORM_MOBILE
	enLogInfo(en::LogChannel::System, "ENLIVE_PLATFORM_MOBILE");
#endif

	enLogInfo(en::LogChannel::System, "================================================");
	enLogInfo(en::LogChannel::System, "Compiler:");
	enLogInfo(en::LogChannel::System, "ENLIVE_COMPILER_NAME : {}", ENLIVE_COMPILER_NAME);
	enLogInfo(en::LogChannel::System, "ENLIVE_COMPILER_VERSION : {}", ENLIVE_STRINGIFY(ENLIVE_COMPILER_VERSION));
	enLogInfo(en::LogChannel::System, "ENLIVE_COMPILER_STRING : {}", ENLIVE_COMPILER_STRING);

#ifdef ENLIVE_COMPILER_CPP11_ENABLED
	enLogInfo(en::LogChannel::System, "ENLIVE_COMPILER_CPP11_ENABLED");
#endif
#ifdef ENLIVE_COMPILER_CPP14_ENABLED
	enLogInfo(en::LogChannel::System, "ENLIVE_COMPILER_CPP14_ENABLED");
#endif
#ifdef ENLIVE_COMPILER_CPP17_ENABLED
	enLogInfo(en::LogChannel::System, "ENLIVE_COMPILER_CPP17_ENABLED");
#endif
#ifdef ENLIVE_COMPILER_CPP20_ENABLED
	enLogInfo(en::LogChannel::System, "ENLIVE_COMPILER_CPP20_ENABLED");
#endif

#ifdef ENLIVE_COMPILER_CLANG
	enLogInfo(en::LogChannel::System, "ENLIVE_COMPILER_CLANG");
#endif
#ifdef ENLIVE_COMPILER_GNUC
	enLogInfo(en::LogChannel::System, "ENLIVE_COMPILER_GNUC");
#endif
#ifdef ENLIVE_COMPILER_BORLANDC
	enLogInfo(en::LogChannel::System, "ENLIVE_COMPILER_BORLANDC");
#endif
#ifdef ENLIVE_COMPILER_INTEL
	enLogInfo(en::LogChannel::System, "ENLIVE_COMPILER_INTEL");
#endif
#ifdef ENLIVE_COMPILER_MSVC
	enLogInfo(en::LogChannel::System, "ENLIVE_COMPILER_MSVC");
#endif
#ifdef ENLIVE_COMPILER_MICROSOFT
	enLogInfo(en::LogChannel::System, "ENLIVE_COMPILER_MICROSOFT");
#endif
#ifdef ENLIVE_COMPILER_CLANG_CL
	enLogInfo(en::LogChannel::System, "ENLIVE_COMPILER_CLANG_CL");
#endif
#ifdef ENLIVE_COMPILER_IBM
	enLogInfo(en::LogChannel::System, "ENLIVE_COMPILER_IBM");
#endif

	enLogInfo(en::LogChannel::System, "================================================");
	enLogInfo(en::LogChannel::System, "CompilerTraits:");

	enLogInfo(en::LogChannel::System, "ENLIVE_FUNCTION : {}", ENLIVE_FUNCTION);

	enLogInfo(en::LogChannel::System, "================================================");
	
	return 0;
}
