#pragma once

// Version of the engine
#define ENLIVE_VERSION_MAJOR 0
#define ENLIVE_VERSION_MINOR 1

// Some configurables choices
// If they are not specifically given, the default value depend on the current build
#define ENLIVE_BUILD_DEBUG
#define ENLIVE_ENABLE_ASSERT
#define ENLIVE_ENABLE_LOG
#define ENLIVE_ENABLE_PROFILE
#define ENLIVE_ENABLE_IMGUI
#define ENLIVE_ENABLE_GRAPHICS_API_DEBUG


// Some more specific choices
#define ENLIVE_ENABLE_DEFAULT_LOGGER
//#define ENLIVE_ENABLE_DOUBLE_PRECISION
#define ENLIVE_ENABLE_HASH_CRC32_COLLISION_DETECTION