
include_directories("${PROJECT_SOURCE_DIR}/thirdparty/doctest")

add_executable(EnlivengineTests
	Tests.cpp
	System/Endianness_Tests.cpp
	System/PrimitiveTypes_Tests.cpp
	System/Hash_Tests.cpp
	System/String_Tests.cpp
)

target_link_libraries(EnlivengineTests PRIVATE EnlivengineLibrary)

add_test(NAME Tests COMMAND EnlivengineTests)
	