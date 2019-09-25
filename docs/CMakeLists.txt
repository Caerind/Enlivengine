set(DOXYGEN_EXTRACT_ALL YES)
set(DOXYGEN_BUILTIN_STL_SUPPORT YES)

doxygen_add_docs(docs
    Enlivengine/system/platform_detection.hpp
    Enlivengine/system/compiler_detection.hpp
    Enlivengine/system/compiler_traits.hpp
    Enlivengine/system/primitivetypes.hpp
	Enlivengine/system/byteunits.hpp
	Enlivengine/system/endianness.hpp
	Enlivengine/system/macros.hpp
    "${CMAKE_CURRENT_SOURCE_DIR}/mainpage.md"
    WORKING_DIRECTORY
    "${PROJECT_SOURCE_DIR}/include"
)
