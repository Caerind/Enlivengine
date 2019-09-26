
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
    ${INCROOT}/Debugger.hpp
    ${SRCROOT}/Debugger.cpp
)
source_group("System" FILES ${SRC})

add_library(EnlivengineLibrary ${SRC})
target_include_directories(EnlivengineLibrary 
	PUBLIC 
		${PROJECT_SOURCE_DIR}/include
	PRIVATE 
		${PROJECT_SOURCE_DIR}/thirdparty/debugbreak
)
