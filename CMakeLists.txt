cmake_minimum_required(VERSION 3.11...3.15)

project(EnlivengineProject
    VERSION 0.1
    DESCRIPTION "Enlivengine"
    LANGUAGES CXX)

	
# Only on the main
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

	set(CMAKE_CXX_STANDARD 17)
	set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
	set(CMAKE_CXX_EXTENSIONS OFF)
	
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
	
	include(CTest)
	
	#find_package(Doxygen)
	#if(Doxygen_FOUND)
	#	add_subdirectory(docs)
	#endif()
	
endif()
		

# Add the subdirectories
add_subdirectory(src/Enlivengine)

add_subdirectory(example)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    add_subdirectory(tests)
endif()
