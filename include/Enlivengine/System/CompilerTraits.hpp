#pragma once

#include <Enlivengine/System/CompilerDetection.hpp>

#if defined(__has_feature)
	#define ENLIVE_HAS_FEATURE(x) __has_feature(x)
#else
	#define ENLIVE_HAS_FEATURE(x) 0
#endif

#if defined(__has_builtin)
	#define ENLIVE_HAS_BUILTIN(x) __has_builtin(x)
#else
	#define ENLIVE_HAS_BUILTIN(x) 0
#endif

#if defined(__has_attribute)
	#define ENLIVE_HAS_ATTRIBUTE(attribute) __has_attribute(attribute)
#else
	#define ENLIVE_HAS_ATTRIBUTE(attribute) 0
#endif

#if defined(__has_cpp_attribute) && defined(__cplusplus)
	#define ENLIVE_HAS_CPP_ATTRIBUTE(attribute) __has_cpp_attribute(attribute)
#else
	#define ENLIVE_HAS_CPP_ATTRIBUTE(attribute) 0
#endif

#if defined(__has_extension)
	#define ENLIVE_HAS_EXTENSION(extension) __has_extension(extension)
#else
	#define ENLIVE_HAS_EXTENSION(extension) 0
#endif

#if defined(__has_declspec_attribute)
	#define ENLIVE_HAS_DECLSPEC_ATTRIBUTE(attribute) __has_declspec_attribute(attribute)
#else
	#define ENLIVE_HAS_DECLSPEC_ATTRIBUTE(attribute) 0
#endif

#if defined(__has_warning)
	#define ENLIVE_HAS_WARNING(warning) __has_warning(warning)
#else
	#define ENLIVE_HAS_WARNING(warning) 0
#endif

#if \
  (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)) || \
  defined(__clang__) || \
  (defined(ENLIVE_COMPILER_GNUC) && ENLIVE_COMPILER_VERSION >= ENLIVE_VERSION_ENCODE(3,0,0)) || \
  (defined(ENLIVE_COMPILER_INTEL) && ENLIVE_COMPILER_VERSION >= ENLIVE_VERSION_ENCODE(13,0,0)) || \
  (defined(ENLIVE_COMPILER_IBM) && ENLIVE_COMPILER_VERSION >= ENLIVE_VERSION_ENCODE(10,1,0) && defined(__C99_PRAGMA_OPERATOR))
	#define ENLIVE_PRAGMA(value) _Pragma(#value)
#elif (defined(ENLIVE_COMPILER_MSVC) && ENLIVE_COMPILER_VERSION >= 1500)
	#define ENLIVE_PRAGMA(value) __pragma(value)
#else
	#define ENLIVE_PRAGMA(value)
#endif

#if defined(__clang__)
	#define ENLIVE_DIAGNOSTIC_PUSH _Pragma("clang diagnostic push")
	#define ENLIVE_DIAGNOSTIC_POP _Pragma("clang diagnostic pop")
#elif defined(ENLIVE_COMPILER_INTEL) && ENLIVE_COMPILER_VERSION >= ENLIVE_VERSION_ENCODE(13,0,0)
	#define ENLIVE_DIAGNOSTIC_PUSH _Pragma("warning(push)")
	#define ENLIVE_DIAGNOSTIC_POP _Pragma("warning(pop)")
#elif (defined(ENLIVE_COMPILER_GNUC) && ENLIVE_COMPILER_VERSION >= ENLIVE_VERSION_ENCODE(4,6,0))
	#define ENLIVE_DIAGNOSTIC_PUSH _Pragma("GCC diagnostic push")
	#define ENLIVE_DIAGNOSTIC_POP _Pragma("GCC diagnostic pop")
#elif (defined(ENLIVE_COMPILER_MSVC) && ENLIVE_COMPILER_VERSION >= 1500)
	#define ENLIVE_DIAGNOSTIC_PUSH __pragma(warning(push))
	#define ENLIVE_DIAGNOSTIC_POP __pragma(warning(pop))
#else
	#define ENLIVE_DIAGNOSTIC_PUSH
	#define ENLIVE_DIAGNOSTIC_POP
#endif

#if defined(__cplusplus) && (__cplusplus >= 201402L)
	#define ENLIVE_DEPRECATED(since) [[deprecated("Since " #since)]]
	#define ENLIVE_DEPRECATED_FOR(since, replacement) [[deprecated("Since " #since "; use " #replacement)]]
#elif \
  ENLIVE_HAS_EXTENSION(attribute_deprecated_with_message) || \
  (defined(ENLIVE_COMPILER_GNUC) && ENLIVE_COMPILER_VERSION >= ENLIVE_VERSION_ENCODE(4,5,0)) || \
  (defined(ENLIVE_COMPILER_INTEL) && ENLIVE_COMPILER_VERSION >= ENLIVE_VERSION_ENCODE(13,0,0))
	#define ENLIVE_DEPRECATED(since) __attribute__((__deprecated__("Since " #since)))
	#define ENLIVE_DEPRECATED_FOR(since, replacement) __attribute__((__deprecated__("Since " #since "; use " #replacement)))
#elif \
  ENLIVE_HAS_ATTRIBUTE(deprecated) || \
  (defined(ENLIVE_COMPILER_GNUC) && ENLIVE_COMPILER_VERSION >= ENLIVE_VERSION_ENCODE(3,1,0))
	#define ENLIVE_DEPRECATED(since) __attribute__((__deprecated__))
	#define ENLIVE_DEPRECATED_FOR(since, replacement) __attribute__((__deprecated__))
#elif (defined(ENLIVE_COMPILER_MSVC) && ENLIVE_COMPILER_VERSION >= 1400)
	#define ENLIVE_DEPRECATED(since) __declspec(deprecated("Since " # since))
	#define ENLIVE_DEPRECATED_FOR(since, replacement) __declspec(deprecated("Since " #since "; use " #replacement))
#elif (defined(ENLIVE_COMPILER_MSVC) && ENLIVE_COMPILER_VERSION >= 1310)
	#define ENLIVE_DEPRECATED(since) _declspec(deprecated)
	#define ENLIVE_DEPRECATED_FOR(since, replacement) __declspec(deprecated)
#else
	#define ENLIVE_DEPRECATED(since)
	#define ENLIVE_DEPRECATED_FOR(since, replacement)
#endif

#if defined(ENLIVE_COMPILER_BORLANDC)
	#define ENLIVE_FUNCTION __FUNC__
#elif defined(ENLIVE_COMPILER_CLANG) || defined(ENLIVE_COMPILER_GNUC)
	#define ENLIVE_FUNCTION __PRETTY_FUNCTION__
#elif defined(ENLIVE_COMPILER_MSVC) || defined(ENLIVE_COMPILER_INTEL)
	#define ENLIVE_FUNCTION __FUNCTION__
#else
	#define ENLIVE_FUNCTION __func__ // __func__ has been standardized in C++ 2011
#endif