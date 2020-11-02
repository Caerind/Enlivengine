#pragma once

// Version of the engine
#define ENLIVE_VERSION_MAJOR 0
#define ENLIVE_VERSION_MINOR 3

// Modules you are using
#define ENLIVE_MODULE_PLATFORM
#define ENLIVE_MODULE_UTILS
#define ENLIVE_MODULE_MATH
#define ENLIVE_MODULE_SDL
#define ENLIVE_MODULE_WINDOW
#define ENLIVE_MODULE_RESOURCES
#define ENLIVE_MODULE_GRAPHICS
#define ENLIVE_MODULE_ANIMATION
#define ENLIVE_MODULE_AUDIO
#define ENLIVE_MODULE_TILED
#define ENLIVE_MODULE_CORE
#define ENLIVE_MODULE_META
#define ENLIVE_MODULE_TOOLS

// Some configurables choices
#define ENLIVE_DEBUG
#define ENLIVE_ENABLE_ASSERT
#define ENLIVE_ENABLE_LOG
#define ENLIVE_ENABLE_PROFILE
#define ENLIVE_ENABLE_IMGUI
#define ENLIVE_ENABLE_GRAPHICS_DEBUG
#define ENLIVE_ENABLE_DEBUG_MEMORY

// Some more specific choices
#define ENLIVE_ENABLE_DEFAULT_LOGGER
#define ENLIVE_ENABLE_DEFAULT_MALLOCATOR // Define a default Mallocator
//#define ENLIVE_NO_RENDER // No render
//#define ENLIVE_ENABLE_DOUBLE_PRECISION // Real = float or double


#define ENLIVE_DEFAULT_UP en::Vector3f::UnitY()
#define ENLIVE_DEFAULT_HANDEDNESS en::Math::Handedness::Right


// Modules mandatory dependencies checks
#if defined(ENLIVE_MODULE_UTILS) && !defined(ENLIVE_MODULE_PLATFORM)
#error Utils module depends on Platform
#endif
#if defined(ENLIVE_MODULE_MATH) && (!defined(ENLIVE_MODULE_UTILS))
#error Math module depends on Utils
#endif
#if defined(ENLIVE_MODULE_SDL) && !defined(ENLIVE_MODULE_PLATFORM)
#error SDL module depends on Platform
#endif
#if defined(ENLIVE_MODULE_WINDOW) && (!defined(ENLIVE_MODULE_UTILS) || !defined(ENLIVE_MODULE_MATH) || !defined(ENLIVE_MODULE_SDL))
#error Window module depends on Utils, Math and SDL
#endif
#if defined(ENLIVE_MODULE_RESOURCES) && (!defined(ENLIVE_MODULE_UTILS))
#error Resources module depends on Resources
#endif
#if defined(ENLIVE_MODULE_GRAPHICS) && (!defined(ENLIVE_MODULE_MATH) || !defined(ENLIVE_MODULE_SDL) || !defined(ENLIVE_MODULE_WINDOW))
#error Graphics module depends on Math, SDL and Window
#endif
#if defined(ENLIVE_MODULE_ANIMATION) && (!defined(ENLIVE_MODULE_GRAPHICS))
#error Animation module depends on Graphics
#endif
#if defined(ENLIVE_MODULE_AUDIO) && (!defined(ENLIVE_MODULE_RESOURCES) || !defined(ENLIVE_MODULE_SDL))
#error Audio module depends on SDL and Resources
#endif
#if defined(ENLIVE_MODULE_TILED) && (!defined(ENLIVE_MODULE_GRAPHICS))
#error Tiled module depends on Graphics
#endif
#if defined(ENLIVE_MODULE_META) && (!defined(ENLIVE_MODULE_PLATFORM) || !defined(ENLIVE_MODULE_UTILS))
#error Meta module depends on Platform and Utils
#endif
#if defined(ENLIVE_MODULE_TOOLS) && (!defined(ENLIVE_MODULE_GRAPHICS) || !defined(ENLIVE_MODULE_WINDOW))
#error Tiled module depends on Graphics and Window
#endif

