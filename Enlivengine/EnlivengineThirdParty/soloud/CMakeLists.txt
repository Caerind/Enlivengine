cmake_minimum_required (VERSION 2.8)

project(SoLoud)



# AudioSource
file(GLOB_RECURSE SRC_AUDIOSOURCE "src/audiosource/*")
source_group("audiosource" FILES ${SRC_AUDIOSOURCE})

# Core
file(GLOB_RECURSE SRC_CORE "src/core/*")
source_group("core" FILES ${SRC_CORE})

# Filter
file(GLOB_RECURSE SRC_FILTER "src/filter/*")
source_group("filter" FILES ${SRC_FILTER})

# Backend
set(SRC_BACKEND
	src/backend/sdl2_static/soloud_sdl2_static.cpp
)
source_group("backend" FILES ${SRC_BACKEND})

add_library(SoLoud STATIC
	${SRC_AUDIOSOURCE}
	${SRC_BACKEND}
	${SRC_CORE}
	${SRC_FILTER}
)
target_compile_definitions(SoLoud PUBLIC WITH_SDL2_STATIC)
target_include_directories(SoLoud PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_link_libraries(SoLoud PUBLIC SDL2-static SDL2main)
