
add_subdirectory(SDL)
set_target_properties(SDL2 PROPERTIES FOLDER "Enlivengine")
set_target_properties(SDL2main PROPERTIES FOLDER "Enlivengine")
set_target_properties(SDL2-static PROPERTIES FOLDER "Enlivengine")

add_subdirectory(bgfx)

# Box2D
file(GLOB_RECURSE SRC_DEBUGBREAK "Box2D/*")
source_group("Box2D" FILES ${SRC_BOX2D})
	
# debug_break
file(GLOB_RECURSE SRC_DEBUGBREAK "debug_break/*")
source_group("debug_break" FILES ${SRC_DEBUGBREAK})

# doctest
file(GLOB_RECURSE SRC_DOCTEST "doctest/*")
source_group("doctest" FILES ${SRC_DOCTEST})

# Dyma
file(GLOB_RECURSE SRC_DYMA "Dyma/*")
source_group("Dyma" FILES ${SRC_DYMA})

# entt
file(GLOB_RECURSE SRC_ENTT "entt/*")
source_group("entt" FILES ${SRC_ENTT})

# FastNoise
file(GLOB_RECURSE SRC_FASTNOISE "FastNoise/*")
source_group("FastNoise" FILES ${SRC_FASTNOISE})

# fmt
file(GLOB_RECURSE SRC_FMT "fmt/*")
source_group("fmt" FILES ${SRC_FMT})

# json
file(GLOB_RECURSE SRC_JSON "json/*")
source_group("json" FILES ${SRC_JSON})

# magic_enum
file(GLOB_RECURSE SRC_MAGICENUM "magic_enum/*")
source_group("magic_enum" FILES ${SRC_MAGICENUM})

# miniz
file(GLOB_RECURSE SRC_MINIZ "miniz/*")
source_group("miniz" FILES ${SRC_MINIZ})

# pugixml
file(GLOB_RECURSE SRC_PUGIXML "pugixml/*")
source_group("pugixml" FILES ${SRC_PUGIXML})

# tracerz
file(GLOB_RECURSE SRC_TRACERZ "tracerz/*")
source_group("tracerz" FILES ${SRC_TRACERZ})

add_library(EnlivengineThirdParty STATIC
	${SRC_BOX2D}
	${SRC_DEBUGBREAK}
	${SRC_DOCTEST}
	${SRC_DYMA}
	${SRC_ENTT}
	${SRC_FASTNOISE}
	${SRC_FMT}
	${SRC_JSON}
	${SRC_MAGICENUM}
	${SRC_MINIZ}
	${SRC_PUGIXML}
	${SRC_TRACERZ}
)
target_include_directories(EnlivengineThirdParty PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(EnlivengineThirdParty PUBLIC SDL2-static SDL2main bgfx dear-imgui)
set_target_properties(EnlivengineThirdParty PROPERTIES FOLDER "Enlivengine")
