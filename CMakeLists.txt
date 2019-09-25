cmake_minimum_required(VERSION 3.1...3.15)

# Support for 3.1 to 3.15, but > 3.12 is better
if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(Enlivengine VERSION 1.0
                    DESCRIPTION "Enlivengine"
					LANGUAGES CXX)
					
add_library(EnlivengineLib STATIC
	tests/testrunner.cpp
	tests/system/tests_endianness.cpp
	tests/system/tests_primitivetypes.cpp)
target_include_directories(EnlivengineLib PUBLIC
	src
	src/system
	tests
	tests/system)
target_compile_features(EnlivengineLib PUBLIC cxx_std_17)

add_executable(Enlivengine main.cpp)
target_link_libraries(Enlivengine PUBLIC EnlivengineLib) 




