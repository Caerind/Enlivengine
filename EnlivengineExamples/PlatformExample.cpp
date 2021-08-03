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
#include <SDL_config.h>

#include <Enlivengine/Resources/PathManager.hpp>
#include <Enlivengine/Engine.hpp>

int main(int argc, char** argv)
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
	enLogInfo(en::LogChannel::System, "SDL detected driver:");
#ifdef SDL_VIDEO_DRIVER_WINDOWS
	enLogInfo(en::LogChannel::System, "SDL_VIDEO_DRIVER_WINDOWS");
#endif
#ifdef SDL_VIDEO_DRIVER_WINRT
	enLogInfo(en::LogChannel::System, "SDL_VIDEO_DRIVER_WINRT");
#endif
#ifdef SDL_VIDEO_DRIVER_X11
	enLogInfo(en::LogChannel::System, "SDL_VIDEO_DRIVER_X11");
#endif
#ifdef SDL_VIDEO_DRIVER_DIRECTFB
	enLogInfo(en::LogChannel::System, "SDL_VIDEO_DRIVER_DIRECTFB");
#endif
#ifdef SDL_VIDEO_DRIVER_COCOA
	enLogInfo(en::LogChannel::System, "SDL_VIDEO_DRIVER_COCOA");
#endif
#ifdef SDL_VIDEO_DRIVER_UIKIT
	enLogInfo(en::LogChannel::System, "SDL_VIDEO_DRIVER_UIKIT");
#endif
#ifdef SDL_VIDEO_DRIVER_WAYLAND
	enLogInfo(en::LogChannel::System, "SDL_VIDEO_DRIVER_WAYLAND");
#endif
#ifdef SDL_VIDEO_DRIVER_MIR
	enLogInfo(en::LogChannel::System, "SDL_VIDEO_DRIVER_MIR");
#endif
#ifdef SDL_VIDEO_DRIVER_ANDROID
	enLogInfo(en::LogChannel::System, "SDL_VIDEO_DRIVER_ANDROID");
#endif
#ifdef SDL_VIDEO_DRIVER_VIVANTE
	enLogInfo(en::LogChannel::System, "SDL_VIDEO_DRIVER_VIVANTE");
#endif
#ifdef USING_GENERATED_CONFIG_H
	enLogInfo(en::LogChannel::System, "USING_GENERATED_CONFIG_H");
#endif

	enLogInfo(en::LogChannel::System, "================================================");
	enLogInfo(en::LogChannel::System, "Config:");
#ifdef ENLIVE_TOOL
	enLogInfo(en::LogChannel::System, "ENLIVE_TOOL");
#endif
#ifdef ENLIVE_RELEASE
	enLogInfo(en::LogChannel::System, "ENLIVE_RELEASE");
#endif
#ifdef ENLIVE_DEBUG
	enLogInfo(en::LogChannel::System, "ENLIVE_DEBUG");
#endif
#ifdef ENLIVE_ENABLE_ASSERT
	enLogInfo(en::LogChannel::System, "ENLIVE_ENABLE_ASSERT");
#endif
#ifdef ENLIVE_ENABLE_LOG
	enLogInfo(en::LogChannel::System, "ENLIVE_ENABLE_LOG");
#endif
#ifdef ENLIVE_ENABLE_PROFILE
	enLogInfo(en::LogChannel::System, "ENLIVE_ENABLE_PROFILE");
#endif
#ifdef ENLIVE_ENABLE_IMGUI
	enLogInfo(en::LogChannel::System, "ENLIVE_ENABLE_IMGUI");
#endif
#ifdef ENLIVE_ENABLE_GRAPHICS_DEBUG
	enLogInfo(en::LogChannel::System, "ENLIVE_ENABLE_GRAPHICS_DEBUG");
#endif
#ifdef ENLIVE_ENABLE_DEBUG_MEMORY
	enLogInfo(en::LogChannel::System, "ENLIVE_ENABLE_DEBUG_MEMORY");
#endif
#ifdef ENLIVE_ENABLE_DEFAULT_LOGGER
	enLogInfo(en::LogChannel::System, "ENLIVE_ENABLE_DEFAULT_LOGGER");
#endif
#ifdef ENLIVE_ENABLE_DEFAULT_MALLOCATOR
	enLogInfo(en::LogChannel::System, "ENLIVE_ENABLE_DEFAULT_MALLOCATOR");
#endif
#ifdef ENLIVE_NO_RENDER
	enLogInfo(en::LogChannel::System, "ENLIVE_NO_RENDER");
#endif
#ifdef ENLIVE_ENABLE_DOUBLE_PRECISION
	enLogInfo(en::LogChannel::System, "ENLIVE_ENABLE_DOUBLE_PRECISION");
#endif

	enLogInfo(en::LogChannel::System, "================================================");
	enLogInfo(en::LogChannel::System, "Paths:");
	if (argc >= 1)
	{
		en::PathManager::SetExecutablePath(argv[0]);
	}
	else
	{
		enLogWarning(en::LogChannel::System, "No arg[0], using empty executable path");
		en::PathManager::SetExecutablePath("");
	}
	enLogInfo(en::LogChannel::System, "ExecutablePath: {}", en::PathManager::GetExecutablePath());
	enLogInfo(en::LogChannel::System, "CurrentPath: {}", en::PathManager::GetCurrentPath());


	enLogInfo(en::LogChannel::System, "================================================");
	enLogInfo(en::LogChannel::System, "Engine init/release:");
	en::Engine::Init(argc, argv);
	enLogInfo(en::LogChannel::System, "Engine::IsInitialized: {}", en::Engine::IsInitialized());
	en::Engine::Release();

	enLogInfo(en::LogChannel::System, "================================================");
	
	return 0;
}
