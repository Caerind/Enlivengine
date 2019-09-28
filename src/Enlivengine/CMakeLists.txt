
set(INCROOT_SYSTEM ${PROJECT_SOURCE_DIR}/include/Enlivengine/System)
set(SRCROOT_SYSTEM ${PROJECT_SOURCE_DIR}/src/Enlivengine/System)
set(SRC_SYSTEM
    ${INCROOT_SYSTEM}/Assert.hpp
    ${INCROOT_SYSTEM}/ByteUnits.hpp
    ${INCROOT_SYSTEM}/CallOnExit.hpp
    ${SRCROOT_SYSTEM}/CallOnExit.cpp
    ${INCROOT_SYSTEM}/CompilerDetection.hpp
    ${INCROOT_SYSTEM}/CompilerTraits.hpp
    ${INCROOT_SYSTEM}/Config.hpp
    ${INCROOT_SYSTEM}/DateTime.hpp
    ${SRCROOT_SYSTEM}/DateTime.cpp
    ${INCROOT_SYSTEM}/Debugger.hpp
    ${SRCROOT_SYSTEM}/Debugger.cpp
    ${INCROOT_SYSTEM}/Endianness.hpp
    ${INCROOT_SYSTEM}/Hash.hpp
    ${SRCROOT_SYSTEM}/Hash.cpp
    ${INCROOT_SYSTEM}/Log.hpp
    ${SRCROOT_SYSTEM}/Log.cpp
    ${INCROOT_SYSTEM}/Macros.hpp
    ${INCROOT_SYSTEM}/ParserIni.hpp
    ${SRCROOT_SYSTEM}/ParserIni.cpp
    ${INCROOT_SYSTEM}/ParserXml.hpp
    ${SRCROOT_SYSTEM}/ParserXml.cpp
    ${INCROOT_SYSTEM}/PlatformDetection.hpp
    ${INCROOT_SYSTEM}/PrimitiveTypes.hpp
    ${INCROOT_SYSTEM}/Signal.hpp
    ${INCROOT_SYSTEM}/String.hpp
    ${SRCROOT_SYSTEM}/String.cpp
    ${INCROOT_SYSTEM}/Time.hpp
    ${SRCROOT_SYSTEM}/Time.cpp
)
source_group("System" FILES ${SRC_SYSTEM})

set(INCROOT_MATH ${PROJECT_SOURCE_DIR}/include/Enlivengine/Math)
set(SRCROOT_MATH ${PROJECT_SOURCE_DIR}/src/Enlivengine/Math)
set(SRC_MATH
	${INCROOT_MATH}/AABB.hpp
	${SRCROOT_MATH}/AABB.cpp
	${INCROOT_MATH}/Distribution.hpp
	${INCROOT_MATH}/Frustum.hpp
	${SRCROOT_MATH}/Frustum.cpp
	${INCROOT_MATH}/Intersection.hpp
	${SRCROOT_MATH}/Intersection.cpp
	${INCROOT_MATH}/Matrix3.hpp
	${INCROOT_MATH}/Matrix4.hpp
	${INCROOT_MATH}/Noise.hpp
	${SRCROOT_MATH}/Noise.cpp
	${INCROOT_MATH}/OBB.hpp
	${SRCROOT_MATH}/OBB.cpp
	${INCROOT_MATH}/Plane.hpp
	${SRCROOT_MATH}/Plane.cpp
	${INCROOT_MATH}/Quaternion.hpp
	${INCROOT_MATH}/Random.hpp
	${SRCROOT_MATH}/Random.cpp
	${INCROOT_MATH}/Ray.hpp
	${SRCROOT_MATH}/Ray.cpp
	${INCROOT_MATH}/Rect.hpp
	${INCROOT_MATH}/Sphere.hpp
	${SRCROOT_MATH}/Sphere.cpp
	${INCROOT_MATH}/Utilities.hpp
	${INCROOT_MATH}/Vector2.hpp
	${INCROOT_MATH}/Vector3.hpp
	${INCROOT_MATH}/Vector4.hpp
)
source_group("Math" FILES ${SRC_MATH})

add_library(EnlivengineLibrary ${SRC_SYSTEM} ${SRC_MATH})
target_include_directories(EnlivengineLibrary PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(EnlivengineLibrary sfml-system sfml-window sfml-graphics sfml-network sfml-audio EnlivengineThirdParty)
target_include_directories(EnlivengineLibrary PUBLIC ${PROJECT_SOURCE_DIR}/thirdparty)