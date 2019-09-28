
set(INCROOT ${PROJECT_SOURCE_DIR}/include/Enlivengine/System)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Enlivengine/System)

set(SRC
    ${INCROOT}/Assert.hpp
    ${INCROOT}/ByteUnits.hpp
    ${INCROOT}/CallOnExit.hpp
    ${SRCROOT}/CallOnExit.cpp
    ${INCROOT}/CompilerDetection.hpp
    ${INCROOT}/CompilerTraits.hpp
    ${INCROOT}/Config.hpp
    ${INCROOT}/DateTime.hpp
    ${SRCROOT}/DateTime.cpp
    ${INCROOT}/Debugger.hpp
    ${SRCROOT}/Debugger.cpp
    ${INCROOT}/Endianness.hpp
    ${INCROOT}/Hash.hpp
    ${SRCROOT}/Hash.cpp
    ${INCROOT}/Log.hpp
    ${SRCROOT}/Log.cpp
    ${INCROOT}/Macros.hpp
    ${INCROOT}/ParserIni.hpp
    ${SRCROOT}/ParserIni.cpp
    ${INCROOT}/ParserXml.hpp
    ${SRCROOT}/ParserXml.cpp
    ${INCROOT}/PlatformDetection.hpp
    ${INCROOT}/PrimitiveTypes.hpp
    ${INCROOT}/Signal.hpp
    ${INCROOT}/String.hpp
    ${SRCROOT}/String.cpp
    ${INCROOT}/Time.hpp
    ${SRCROOT}/Time.cpp
)
source_group("System" FILES ${SRC})

add_library(EnlivengineLibrary ${SRC})
target_include_directories(EnlivengineLibrary PUBLIC ${PROJECT_SOURCE_DIR}/include)
