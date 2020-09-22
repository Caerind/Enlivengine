#pragma once

#include <Enlivengine/System/Preprocessor.hpp>

#if defined(_WIN32) || defined(__WIN32__) || defined(_WIN64)

    // Windows
    #define ENLIVE_PLATFORM_MICROSOFT
    #define ENLIVE_PLATFORM_WINDOWS
    #define ENLIVE_PLATFORM_DESKTOP
    #define ENLIVE_PLATFORM_NAME "Windows"

    #ifdef _WIN64 // VC++ defines both _WIN32 and _WIN64 when compiling for Win64.
        #define ENLIVE_PLATFORM_WIN64
    #else
        #define ENLIVE_PLATFORM_WIN32
    #endif

	#if defined(_M_AMD64) || defined(_AMD64_) || defined(__x86_64__)
		#define ENLIVE_PROCESSOR_X86_64
		#define ENLIVE_PLATFORM_LITTLE_ENDIAN
		#define ENLIVE_PLATFORM_DESCRIPTION "Windows on x64"
	#elif defined(_M_IX86) || defined(_X86_)
		#define ENLIVE_PROCESSOR_X86
		#define ENLIVE_PLATFORM_LITTLE_ENDIAN
		#define ENLIVE_PLATFORM_DESCRIPTION "Windows on X86"
	#elif defined(_M_IA64) || defined(_IA64_)
		#define ENLIVE_PROCESSOR_IA64
		#define ENLIVE_PLATFORM_LITTLE_ENDIAN
		#define ENLIVE_PLATFORM_DESCRIPTION "Windows on IA-64"
	#elif defined(_M_ARM)
		#define ENLIVE_ABI_ARM_WINCE
		#define ENLIVE_PROCESSOR_ARM32
		#define ENLIVE_PLATFORM_LITTLE_ENDIAN
		#define ENLIVE_PLATFORM_DESCRIPTION "Windows on ARM"
	#elif defined(_M_ARM64)
		#define ENLIVE_PROCESSOR_ARM64
		#define ENLIVE_PLATFORM_LITTLE_ENDIAN
		#define ENLIVE_PLATFORM_DESCRIPTION "Windows on ARM64"
	#else
		#error Unknown processor
	#endif

    #if defined(__MINGW32__) || defined(__MINGW64__)
        #define ENLIVE_PLATFORM_MINGW
    #endif

	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif

    #ifndef NOMINMAX
        #define NOMINMAX
    #endif

#elif defined(__APPLE__) && defined(__MACH__)

	// Apple platform, see which one it is
    #include "TargetConditionals.h"

    #if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR

		// iOS
        #define ENLIVE_PLATFORM_APPLE
        #define ENLIVE_PLATFORM_IOS
        #define ENLIVE_PLATFORM_MOBILE
        #define ENLIVE_PLATFORM_NAME "iPhone"
        #define ENLIVE_PLATFORM_POSIX

		#if defined(__arm__)
		    #define ENLIVE_ABI_ARM_APPLE
		    #define ENLIVE_PROCESSOR_ARM32
		    #define ENLIVE_PLATFORM_LITTLE_ENDIAN
		    #define ENLIVE_PLATFORM_DESCRIPTION "iPhone on ARM"
		#elif defined(__aarch64__) || defined(__AARCH64)
		    #define ENLIVE_ABI_ARM64_APPLE
		    #define ENLIVE_PROCESSOR_ARM64
		    #define ENLIVE_PLATFORM_LITTLE_ENDIAN
		    #define ENLIVE_PLATFORM_DESCRIPTION "iPhone on ARM64"
		#elif defined(__i386__)
		    #define ENLIVE_PLATFORM_IPHONE_SIMULATOR
		    #define ENLIVE_PROCESSOR_X86
		    #define ENLIVE_PLATFORM_LITTLE_ENDIAN
		    #define ENLIVE_PLATFORM_DESCRIPTION "iPhone simulator on x86"
		#elif defined(__x86_64) || defined(__amd64)
            #define ENLIVE_PROCESSOR_X86_64
            #define ENLIVE_PLATFORM_LITTLE_ENDIAN
		    #define ENLIVE_PLATFORM_DESCRIPTION "iPhone simulator on x64"
		#else
		    #error Unknown processor
		#endif

    #elif TARGET_OS_MAC

		// MacOS
        #define ENLIVE_PLATFORM_APPLE
        #define ENLIVE_PLATFORM_MACOS
        #define ENLIVE_PLATFORM_DESKTOP
        #define ENLIVE_PLATFORM_NAME "OSX"
        #define ENLIVE_PLATFORM_UNIX
        #define ENLIVE_PLATFORM_POSIX

		#if defined(__i386__) || defined(__intel__)
            #define ENLIVE_PROCESSOR_X86
            #define ENLIVE_PLATFORM_LITTLE_ENDIAN
            #define ENLIVE_PLATFORM_DESCRIPTION "OSX on x86"
        #elif defined(__x86_64) || defined(__amd64)
			#define ENLIVE_PROCESSOR_X86_64
			#define ENLIVE_PLATFORM_LITTLE_ENDIAN
			#define ENLIVE_PLATFORM_DESCRIPTION "OSX on x64"
		#elif defined(__arm__)
			#define ENLIVE_ABI_ARM_APPLE
			#define ENLIVE_PROCESSOR_ARM32
			#define ENLIVE_PLATFORM_LITTLE_ENDIAN
			#define ENLIVE_PLATFORM_DESCRIPTION "OSX on ARM"
		#elif defined(__aarch64__) || defined(__AARCH64)
			#define ENLIVE_ABI_ARM64_APPLE
			#define ENLIVE_PROCESSOR_ARM64
			#define ENLIVE_PLATFORM_LITTLE_ENDIAN
			#define ENLIVE_PLATFORM_DESCRIPTION "OSX on ARM64"
		#elif defined(__POWERPC64__) || defined(__powerpc64__)
			#define ENLIVE_PROCESSOR_POWERPC
			#define ENLIVE_PROCESSOR_POWERPC_64
			#define ENLIVE_PLATFORM_BIG_ENDIAN
			#define ENLIVE_PLATFORM_DESCRIPTION "OSX on PowerPC 64"
		#elif defined(__POWERPC__) || defined(__powerpc__)
            #define ENLIVE_PROCESSOR_POWERPC
			#define ENLIVE_PROCESSOR_POWERPC_32
			#define ENLIVE_PLATFORM_BIG_ENDIAN
			#define ENLIVE_PLATFORM_DESCRIPTION "OSX on PowerPC"
		#else
			#error Unknown processor
		#endif

    #else

	    // Unsupported Apple system
        #error This Apple operating system is not supported

    #endif

#elif defined(__unix__)

	// UNIX system, see which one it is
    #if defined(__ANDROID__)

	    // Android
        #define ENLIVE_PLATFORM_ANDROID
        #define ENLIVE_PLATFORM_MOBILE
		#define ENLIVE_PLATFORM_NAME "Android"
        #define ENLIVE_PLATFORM_UNIX
        #define ENLIVE_PLATFORM_POSIX

		#if defined(__arm__)
			#define ENLIVE_ABI_ARM_LINUX // ARM eabi
			#define ENLIVE_PROCESSOR_ARM32
            #define ENLIVE_PLATFORM_LITTLE_ENDIAN
			#define ENLIVE_PLATFORM_DESCRIPTION "Android on ARM"
		#elif defined(__aarch64__)
			#define ENLIVE_PROCESSOR_ARM64
            #define ENLIVE_PLATFORM_LITTLE_ENDIAN
			#define ENLIVE_PLATFORM_DESCRIPTION "Android on ARM64"
		#elif defined(__i386__)
			#define ENLIVE_PROCESSOR_X86
            #define ENLIVE_PLATFORM_LITTLE_ENDIAN
			#define ENLIVE_PLATFORM_DESCRIPTION "Android on x86"
		#elif defined(__x86_64)
			#define ENLIVE_PROCESSOR_X86_64
            #define ENLIVE_PLATFORM_LITTLE_ENDIAN
			#define ENLIVE_PLATFORM_DESCRIPTION "Android on x64"
		#else
			#error Unknown processor
		#endif

    #elif defined(__linux__) || defined(__linux)

        // Linux
        #define ENLIVE_PLATFORM_LINUX
        #define ENLIVE_PLATFORM_DESKTOP
		#define ENLIVE_PLATFORM_NAME "Linux"
		#define ENLIVE_PLATFORM_UNIX
		#define ENLIVE_PLATFORM_POSIX

		#if defined(__i386__) || defined(__intel__) || defined(_M_IX86)
			#define ENLIVE_PROCESSOR_X86
			#define ENLIVE_PLATFORM_LITTLE_ENDIAN
			#define ENLIVE_PLATFORM_DESCRIPTION "Linux on x86"
		#elif defined(__ARM_ARCH_7A__) || defined(__ARM_EABI__)
			#define ENLIVE_ABI_ARM_LINUX
            #define ENLIVE_PROCESSOR_ARM32
            #define ENLIVE_PLATFORM_LITTLE_ENDIAN
			#define ENLIVE_PLATFORM_DESCRIPTION "Linux on ARM 6/7 32-bits"
		#elif defined(__aarch64__) || defined(__AARCH64)
            #define ENLIVE_PROCESSOR_ARM64
            #define ENLIVE_PLATFORM_LITTLE_ENDIAN
			#define ENLIVE_PLATFORM_DESCRIPTION "Linux on ARM64"
		#elif defined(__x86_64__)
			#define ENLIVE_PROCESSOR_X86_64
			#define ENLIVE_PLATFORM_LITTLE_ENDIAN
			#define ENLIVE_PLATFORM_DESCRIPTION "Linux on x64"
		#elif defined(__powerpc64__)
			#define ENLIVE_PROCESSOR_POWERPC
			#define ENLIVE_PROCESSOR_POWERPC_64
			#define ENLIVE_PLATFORM_BIG_ENDIAN
			#define ENLIVE_PLATFORM_DESCRIPTION "Linux on PowerPC 64"
		#elif defined(__powerpc__)
			#define ENLIVE_PROCESSOR_POWERPC
			#define ENLIVE_PROCESSOR_POWERPC_32
			#define ENLIVE_PLATFORM_BIG_ENDIAN
			#define ENLIVE_PLATFORM_DESCRIPTION "Linux on PowerPC"
		#else
			#error Unknown processor
		#endif

        #if defined(__CYGWIN__)
            #define ENLIVE_PLATFORM_CYGWIN
        #endif

    #elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)

	    // FreeBSD
        #define ENLIVE_PLATFORM_FREEBSD
        #define ENLIVE_PLATFORM_DESKTOP
		#define ENLIVE_PLATFORM_NAME "FreeBSD"
        #define ENLIVE_PLATFORM_UNIX
        #define ENLIVE_PLATFORM_POSIX

        #error FreeBSD is detected but is not supported now

    #elif defined(__OpenBSD__)

	    // OpenBSD
        #define ENLIVE_PLATFORM_OPENBSD
        #define ENLIVE_PLATFORM_DESKTOP
        #define ENLIVE_PLATFORM_NAME "OpenBSD"
        #define ENLIVE_PLATFORM_UNIX
        #define ENLIVE_PLATFORM_POSIX

        #error OpenBSD is detected but is not supported now

    #else

	    // Unsupported UNIX system
        #error This UNIX operating system is not supported

    #endif

#else

	// Unsupported system
    #error This operating system is not supported

#endif

#ifndef ENLIVE_PROCESSOR_ARM
	#if defined(ENLIVE_PROCESSOR_ARM32) || defined(ENLIVE_PROCESSOR_ARM64) || defined(ENLIVE_PROCESSOR_ARM7)
		#define ENLIVE_PROCESSOR_ARM
	#endif
#endif

#if !defined(ENLIVE_STATIC)
	#if defined(ENLIVE_SYSTEM_WINDOWS)
		// Windows compilers need specific (and different) keywords for export and import
		#define ENLIVE_EXPORT __declspec(dllexport)
		#define ENLIVE_IMPORT __declspec(dllimport)
		// For Visual C++ compilers, we also need to turn off this annoying C4251 warning
		#if defined(_MSC_VER)
			#pragma warning(disable: 4251)
		#endif
	#else // Linux, FreeBSD, Mac OS X
		#if __GNUC__ >= 4
			// GCC 4 has special keywords for showing/hidding symbols,
			// the same keyword is used for both importing and exporting
			#define ENLIVE_EXPORT __attribute__ ((__visibility__ ("default")))
			#define ENLIVE_IMPORT __attribute__ ((__visibility__ ("default")))
		#else
			// GCC < 4 has no mechanism to explicitely hide symbols, everything's exported
			#define ENLIVE_EXPORT
			#define ENLIVE_IMPORT
		#endif
	#endif
#else
	// Static build doesn't need import/export macros
	#define ENLIVE_EXPORT
	#define ENLIVE_IMPORT
#endif