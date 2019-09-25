
set(INCROOT ${PROJECT_SOURCE_DIR}/include/Enlivengine/System)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Enlivengine/System)

set(SRC
    ${INCROOT}/ByteUnits.hpp
    ${INCROOT}/CompilerDetection.hpp
    ${INCROOT}/CompilerTraits.hpp
    ${INCROOT}/Endianness.hpp
    ${INCROOT}/Macros.hpp
    ${INCROOT}/PlatformDetection.hpp
    ${INCROOT}/PrimitiveTypes.hpp
    ${INCROOT}/DebugTools.hpp
    ${SRCROOT}/DebugTools.cpp
)
source_group("System" FILES ${SRC})

add_library(EnlivengineLibrary ${SRC})
target_include_directories(EnlivengineLibrary PUBLIC ${PROJECT_SOURCE_DIR}/include)

#target_include_directories(EnlivengineLibrary PRIVATE "${PROJECT_SOURCE_DIR}/thirdparty/debugbreak")

