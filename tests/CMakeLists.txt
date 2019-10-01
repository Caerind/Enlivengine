
include_directories("${PROJECT_SOURCE_DIR}/thirdparty/doctest")

set(TESTS_ROOT ${PROJECT_SOURCE_DIR}/tests)

set(TESTS_SYSTEM
    ${TESTS_ROOT}/System/Endianness_Tests.cpp
    ${TESTS_ROOT}/System/Hash_Tests.cpp
    ${TESTS_ROOT}/System/PrimitiveTypes_Tests.cpp
    ${TESTS_ROOT}/System/String_Tests.cpp
)
source_group("System" FILES ${TESTS_SYSTEM})

set(TESTS_MATH
    ${TESTS_ROOT}/Math/Matrix3_Tests.cpp
    ${TESTS_ROOT}/Math/Matrix4_Tests.cpp
    ${TESTS_ROOT}/Math/Random_Tests.cpp
    ${TESTS_ROOT}/Math/Utilities_Tests.cpp
    ${TESTS_ROOT}/Math/Vector2_Tests.cpp
    ${TESTS_ROOT}/Math/Vector3_Tests.cpp
    ${TESTS_ROOT}/Math/Vector4_Tests.cpp
)
source_group("Math" FILES ${TESTS_MATH})

add_executable(EnlivengineTests
	Tests.cpp
	${TESTS_SYSTEM}
	${TESTS_MATH}
)

target_link_libraries(EnlivengineTests OpenGL::GL sfml-system sfml-window sfml-graphics sfml-network sfml-audio EnlivengineThirdParty EnlivengineLibrary)

add_test(NAME Tests COMMAND EnlivengineTests)
	