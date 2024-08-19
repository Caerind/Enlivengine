cmake_minimum_required(VERSION 3.12...3.15)

project(Enlivengine
    VERSION 0.0
    DESCRIPTION "Enlivengine"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    enable_testing()
endif()

# Used to have a good display look in IDE
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_subdirectory(src)

if(EXISTS "${CMAKE_SOURCE_DIR}/tests")
    add_subdirectory(tests)
endif()
if(EXISTS "${CMAKE_SOURCE_DIR}/examples")
    add_subdirectory(examples)
endif()

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT 3DSceneExample)
endif()
