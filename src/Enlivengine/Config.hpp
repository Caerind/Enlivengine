#pragma once

// Version of the engine
#define ENLIVE_VERSION_MAJOR 0
#define ENLIVE_VERSION_MINOR 3

//////////////////////////////////////////////////////////////////////////////////

// ENLIVE_TOOL is the Editor with docked tools
// ENLIVE_RELEASE is the MainGame

// Compile Tool or Release depending on the target
#ifndef NDEBUG
#define ENLIVE_TOOL
#else
#define ENLIVE_RELEASE
#endif

#if !defined(ENLIVE_TOOL) && !defined(ENLIVE_RELEASE)
#define ENLIVE_RELEASE
#endif
#if defined(ENLIVE_TOOL) && defined(ENLIVE_RELEASE)
#undef ENLIVE_RELEASE
#endif

//////////////////////////////////////////////////////////////////////////////////

// You can also define ENLIVE_DEBUG if you want for ENLIVE_RELEASE
// If you are in ENLIVE_TOOL it is automatic
//#define ENLIVE_DEBUG

#if defined(ENLIVE_TOOL) && !defined(ENLIVE_DEBUG)
#define ENLIVE_DEBUG
#endif

//////////////////////////////////////////////////////////////////////////////////

// Debug auto enable some defines
// You can still define them individually
#ifdef ENLIVE_DEBUG

#define ENLIVE_ENABLE_ASSERT
#define ENLIVE_ENABLE_LOG
#define ENLIVE_ENABLE_PROFILE
#define ENLIVE_ENABLE_IMGUI
#define ENLIVE_ENABLE_GRAPHICS_DEBUG
#define ENLIVE_ENABLE_DEBUG_MEMORY

#endif // ENLIVE_DEBUG

//////////////////////////////////////////////////////////////////////////////////

// Some more specific choices
#define ENLIVE_ENABLE_DEFAULT_LOGGER
#define ENLIVE_ENABLE_DEFAULT_MALLOCATOR // Define a default Mallocator
//#define ENLIVE_NO_RENDER // No render
//#define ENLIVE_ENABLE_DOUBLE_PRECISION // Real = float or double

//////////////////////////////////////////////////////////////////////////////////

// TODO : GLM update
// Engine default axes
#define ENLIVE_DEFAULT_HANDEDNESS en::Math::Handedness::Right
#define ENLIVE_DEFAULT_UP en::Vector3f::UnitY()
#define ENLIVE_DEFAULT_DOWN -en::Vector3f::UnitY()
#define ENLIVE_DEFAULT_RIGHT -en::Vector3f::UnitX()
#define ENLIVE_DEFAULT_LEFT en::Vector3f::UnitX()
#define ENLIVE_DEFAULT_FORWARD en::Vector3f::UnitZ()
#define ENLIVE_DEFAULT_BACKWARD -en::Vector3f::UnitZ()