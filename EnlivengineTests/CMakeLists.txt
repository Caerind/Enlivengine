
set(TESTS_SYSTEM_PATH System)
set(TESTS_SYSTEM
    ${TESTS_SYSTEM_PATH}/Array_Tests.cpp
    ${TESTS_SYSTEM_PATH}/Compression_Tests.cpp
    ${TESTS_SYSTEM_PATH}/Endianness_Tests.cpp
    ${TESTS_SYSTEM_PATH}/Hash_Tests.cpp
    ${TESTS_SYSTEM_PATH}/PrimitiveTypes_Tests.cpp
    ${TESTS_SYSTEM_PATH}/String_Tests.cpp
)
source_group("System" FILES ${TESTS_SYSTEM})

set(TESTS_MATH_PATH Math)
set(TESTS_MATH
    ${TESTS_MATH_PATH}/Matrix3_Tests.cpp
    ${TESTS_MATH_PATH}/Matrix4_Tests.cpp
    ${TESTS_MATH_PATH}/Random_Tests.cpp
    ${TESTS_MATH_PATH}/Utilities_Tests.cpp
    ${TESTS_MATH_PATH}/Vector2_Tests.cpp
    ${TESTS_MATH_PATH}/Vector3_Tests.cpp
    ${TESTS_MATH_PATH}/Vector4_Tests.cpp
)
source_group("Math" FILES ${TESTS_MATH})

add_executable(EnlivengineTests
	Tests.cpp
	${TESTS_SYSTEM}
	${TESTS_MATH}
)
target_link_libraries(EnlivengineTests PRIVATE Enlivengine)
set_target_properties(EnlivengineTests PROPERTIES FOLDER "Enlivengine")

add_test(NAME Tests COMMAND EnlivengineTests)
	