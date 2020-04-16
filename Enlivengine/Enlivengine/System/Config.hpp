#pragma once

// Version of the engine
#define ENLIVE_VERSION_MAJOR 0
#define ENLIVE_VERSION_MINOR 2

// Some configurables choices
// If they are not specifically given, the default value depend on the current build
#define ENLIVE_DEBUG
#define ENLIVE_ENABLE_ASSERT
#define ENLIVE_ENABLE_LOG
#define ENLIVE_ENABLE_PROFILE
#define ENLIVE_ENABLE_IMGUI
#define ENLIVE_ENABLE_GRAPHICS_API_DEBUG
//#define ENLIVE_ENABLE_METADATA // TEMP : Disable for LD46


// Some more specific choices
#define ENLIVE_ENABLE_DEFAULT_LOGGER
//#define ENLIVE_ENABLE_DOUBLE_PRECISION // Define Real = float/double
//#define ENLIVE_ENABLE_HASH_COLLISION_DETECTION // Check if hash is found for another different string
//#define ENLIVE_ENABLE_METADATA_CHECKING // Check that the meta data are valid // TEMP : Disable for LD46


// TODO : Move this elsewhere ?
#ifndef ENLIVE_MATH_FORWARD
#define ENLIVE_MATH_FORWARD 0,0,1
#endif

#ifndef ENLIVE_MATH_BACK
#define ENLIVE_MATH_BACK 0,0,-1
#endif

#ifndef ENLIVE_MATH_UP
#define ENLIVE_MATH_UP 0,1,0
#endif

#ifndef ENLIVE_MATH_DOWN
#define ENLIVE_MATH_DOWN 0,-1,0
#endif

#ifndef ENLIVE_MATH_RIGHT
#define ENLIVE_MATH_RIGHT -1,0,0
#endif

#ifndef ENLIVE_MATH_LEFT
#define ENLIVE_MATH_LEFT 1,0,0
#endif
