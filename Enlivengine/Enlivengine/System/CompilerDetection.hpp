#pragma once

#include <Enlivengine/System/Preprocessor.hpp>

#if defined(__cplusplus)

	// ENLIVE_COMPILER_CPP11_ENABLED
	#if (__cplusplus >= 201103L) // Clang and GCC defines this like so in C++11 mode.
		#define ENLIVE_COMPILER_CPP11_ENABLED
	#elif defined(__GNUC__) && defined(__GXX_EXPERIMENTAL_CXX0X__)
		#define ENLIVE_COMPILER_CPP11_ENABLED
	#elif defined(_MSC_VER) && (_MSC_VER >= 1600)
		#define ENLIVE_COMPILER_CPP11_ENABLED
	#endif

	// ENLIVE_COMPILER_CPP14_ENABLED
	#if (__cplusplus >= 201402L) // Clang and GCC defines this like so in C++14 mode.
		#define ENLIVE_COMPILER_CPP14_ENABLED
	#elif defined(_MSC_VER) && (_MSC_VER >= 1900) // VS2015+ 
		#define ENLIVE_COMPILER_CPP14_ENABLED
	#endif

	// ENLIVE_COMPILER_CPP17_ENABLED
	#if (__cplusplus >= 201703L) 
		#define ENLIVE_COMPILER_CPP17_ENABLED
	#elif defined(_MSVC_LANG) && (_MSVC_LANG >= 201703L) // C++17+
		#define ENLIVE_COMPILER_CPP17_ENABLED
	#endif

	// ENLIVE_COMPILER_CPP20_ENABLED
	// TODO : enable once a C++20 value for the __cplusplus macro has been published
	// #if (__cplusplus >= 202003L)
	//     #define ENLIVE_COMPILER_CPP20_ENABLED
	// #elif defined(_MSVC_LANG) && (_MSVC_LANG >= 202003L) // C++20+
	//     #define ENLIVE_COMPILER_CPP20_ENABLED
	// #endif

#endif

// Clang's GCC-compatible driver.
#if defined(__clang__) && !defined(_MSC_VER)
	#define ENLIVE_COMPILER_CLANG
	#define ENLIVE_COMPILER_VERSION ENLIVE_VERSION_ENCODE(__clang_major__, __clang_minor__, __clang_patchlevel__)
	#define ENLIVE_COMPILER_NAME "clang"
	#define ENLIVE_COMPILER_STRING ENLIVE_COMPILER_NAME __clang_version__

// GCC (a.k.a. GNUC)
#elif defined(__GNUC__) // GCC compilers exist for many platforms.
	#define ENLIVE_COMPILER_GNUC
    #if defined(__GNUC_PATCHLEVEL__)
        #define ENLIVE_COMPILER_VERSION ENLIVE_VERSION_ENCODE(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
    #else
        #define ENLIVE_COMPILER_VERSION ENLIVE_VERSION_ENCODE(__GNUC__, __GNUC_MINOR__, 0)
    #endif
	#define ENLIVE_COMPILER_NAME "GCC"
	#define ENLIVE_COMPILER_STRING ENLIVE_COMPILER_NAME " compiler, version " ENLIVE_STRINGIFY(__GNUC__) "." ENLIVE_STRINGIFY(__GNUC_MINOR__)

// Borland C++
#elif defined(__BORLANDC__)
	#define ENLIVE_COMPILER_BORLANDC
	#define ENLIVE_COMPILER_VERSION __BORLANDC__
	#define ENLIVE_COMPILER_NAME "Borland C"
    #define ENLIVE_COMPILER_STRING ENLIVE_COMPILER_NAME " compiler, version " ENLIVE_STRINGIFY(__BORLANDC__)

// Intel C++
// The Intel Windows compiler masquerades as VC++ and defines _MSC_VER.
// The Intel compiler is based on the EDG compiler front-end.
#elif defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC)
	#define ENLIVE_COMPILER_INTEL
	#if defined(_MSC_VER)
		#define ENLIVE_COMPILER_MSVC
		#define ENLIVE_COMPILER_MICROSOFT
	#endif
	#if defined(__GNUC__)
		#define ENLIVE_COMPILER_GNUC
	#endif
	#if defined(__INTEL_COMPILER)
		#define ENLIVE_COMPILER_VERSION __INTEL_COMPILER
	#elifif defined(__ICL)
		#define ENLIVE_COMPILER_VERSION __ICL
	#elif defined(__ICC)
		#define ENLIVE_COMPILER_VERSION __ICC
	#endif
	#define ENLIVE_COMPILER_NAME "Intel C++"
	#if defined(_MSC_VER)
		#define ENLIVE_COMPILER_STRING  ENLIVE_COMPILER_NAME " compiler, version " ENLIVE_STRINGIFY(ENLIVE_COMPILER_VERSION) ", EDG version " ENLIVE_STRINGIFY(__EDG_VERSION__) ", VC++ version " ENLIVE_STRINGIFY(_MSC_VER)
	#elif defined(__GNUC__)
		#define ENLIVE_COMPILER_STRING  ENLIVE_COMPILER_NAME " compiler, version " ENLIVE_STRINGIFY(ENLIVE_COMPILER_VERSION) ", EDG version " ENLIVE_STRINGIFY(__EDG_VERSION__) ", GCC version " ENLIVE_STRINGIFY(__GNUC__)
	#else
		#define ENLIVE_COMPILER_STRING  ENLIVE_COMPILER_NAME " compiler, version " ENLIVE_STRINGIFY(ENLIVE_COMPILER_VERSION) ", EDG version " ENLIVE_STRINGIFY(__EDG_VERSION__)
	#endif

// Microsoft VC++
#elif defined(_MSC_VER)
	#define ENLIVE_COMPILER_MSVC
	#define ENLIVE_COMPILER_MICROSOFT
	#define ENLIVE_COMPILER_VERSION _MSC_VER
	#define ENLIVE_COMPILER_NAME "Microsoft Visual C++"
    #define ENLIVE_COMPILER_STRING ENLIVE_COMPILER_NAME " compiler, version " ENLIVE_STRINGIFY(ENLIVE_COMPILER_VERSION)

	#if defined(__clang__)
		// Clang's MSVC-compatible driver.
		#define ENLIVE_COMPILER_CLANG_CL
	#endif

// IBM
#elif defined(__xlC__)
	#define ENLIVE_COMPILER_IBM
	#define ENLIVE_COMPILER_VERSION __xlC__
	#define ENLIVE_COMPILER_NAME "IBM XL C"
	#define ENLIVE_COMPILER_STRING "IBM XL C compiler, version " ENLIVE_STRINGIFY(__xlC__)

// Unknown
#else 
	#define ENLIVE_COMPILER_VERSION 0
	#define ENLIVE_COMPILER_NAME "Unknown"

	#error Unknown compiler

#endif