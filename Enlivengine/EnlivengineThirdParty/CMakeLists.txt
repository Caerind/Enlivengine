
# SDL
add_subdirectory(SDL)
set_target_properties(SDL2 PROPERTIES FOLDER "Enlivengine/SDL")
set_target_properties(SDL2main PROPERTIES FOLDER "Enlivengine/SDL")
set_target_properties(SDL2-static PROPERTIES FOLDER "Enlivengine/SDL")

# bgfx
add_subdirectory(bgfx)
set_target_properties(bgfx PROPERTIES FOLDER "Enlivengine/bgfx")
set_target_properties(bimg PROPERTIES FOLDER "Enlivengine/bgfx")
set_target_properties(bx PROPERTIES FOLDER "Enlivengine/bgfx")
set_target_properties(astc PROPERTIES FOLDER "Enlivengine/bgfx/3rdparty")
set_target_properties(astc-codec PROPERTIES FOLDER "Enlivengine/bgfx/3rdparty")
set_target_properties(dear-imgui PROPERTIES FOLDER "Enlivengine/bgfx/3rdparty")
set_target_properties(edtaa3 PROPERTIES FOLDER "Enlivengine/bgfx/3rdparty")
set_target_properties(etc1 PROPERTIES FOLDER "Enlivengine/bgfx/3rdparty")
set_target_properties(etc2 PROPERTIES FOLDER "Enlivengine/bgfx/3rdparty")
set_target_properties(fcpp PROPERTIES FOLDER "Enlivengine/bgfx/3rdparty")
set_target_properties(glcpp PROPERTIES FOLDER "Enlivengine/bgfx/3rdparty")
set_target_properties(glslang PROPERTIES FOLDER "Enlivengine/bgfx/3rdparty")
set_target_properties(glsl-optimizer PROPERTIES FOLDER "Enlivengine/bgfx/3rdparty")
set_target_properties(iqa PROPERTIES FOLDER "Enlivengine/bgfx/3rdparty")
set_target_properties(mesa PROPERTIES FOLDER "Enlivengine/bgfx/3rdparty")
set_target_properties(meshoptimizer PROPERTIES FOLDER "Enlivengine/bgfx/3rdparty")
set_target_properties(nvtt PROPERTIES FOLDER "Enlivengine/bgfx/3rdparty")
set_target_properties(pvrtc PROPERTIES FOLDER "Enlivengine/bgfx/3rdparty")
set_target_properties(spirv-cross PROPERTIES FOLDER "Enlivengine/bgfx/3rdparty")
set_target_properties(spirv-tools PROPERTIES FOLDER "Enlivengine/bgfx/3rdparty")
set_target_properties(squish PROPERTIES FOLDER "Enlivengine/bgfx/3rdparty")
set_target_properties(geometryc PROPERTIES FOLDER "Enlivengine/bgfx/tools")
set_target_properties(geometryv PROPERTIES FOLDER "Enlivengine/bgfx/tools")
set_target_properties(shaderc PROPERTIES FOLDER "Enlivengine/bgfx/tools")
set_target_properties(texturec PROPERTIES FOLDER "Enlivengine/bgfx/tools")
set_target_properties(texturev PROPERTIES FOLDER "Enlivengine/bgfx/tools")
set_target_properties(tools PROPERTIES FOLDER "Enlivengine/bgfx/tools")
set_target_properties(example-00-helloworld PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-01-cubes PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-02-metaballs PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-03-raymarch PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-04-mesh PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-05-instancing PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-06-bump PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-07-callback PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-08-update PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-09-hdr PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-10-font PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-11-fontsdf PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-12-lod PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-13-stencil PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-14-shadowvolumes PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-15-shadowmaps-simple PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-16-shadowmaps PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-17-drawstress PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-18-ibl PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-19-oit PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-20-nanovg PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-22-windows PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-23-vectordisplay PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-24-nbody PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-25-c99 PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-26-occlusion PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-27-terrain PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-28-wireframe PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-29-debugdraw PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-30-picking PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-31-rsm PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-32-particles PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-33-pom PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-34-mvs PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-35-dynamic PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-36-sky PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-38-bloom PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-39-assao PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(example-common PROPERTIES FOLDER "Enlivengine/bgfx/examples")
set_target_properties(examples PROPERTIES FOLDER "Enlivengine/bgfx/examples")

# Box2D
file(GLOB_RECURSE SRC_BOX2D "Box2D/*")
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

# IconFontCppHeaders
file(GLOB_RECURSE SRC_ICONFONTCPPHEADERS "IconFontCppHeaders/*")
source_group("IconFontCppHeaders" FILES ${SRC_ICONFONTCPPHEADERS})

# imgui
file(GLOB_RECURSE SRC_IMGUI "imgui/*")
source_group("imgui" FILES ${SRC_IMGUI})

# ImGuizmo
file(GLOB_RECURSE SRC_IMGUIZMO "ImGuizmo/*")
source_group("ImGuizmo" FILES ${SRC_IMGUIZMO})

# ImGuiFileDialog
file(GLOB_RECURSE SRC_IMGUIFILEDIALOG "ImGuiFileDialog/*")
source_group("ImGuiFileDialog" FILES ${SRC_IMGUIFILEDIALOG})

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

# soloud
add_subdirectory(soloud)
set_target_properties(SoLoud PROPERTIES FOLDER "Enlivengine")

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
	${SRC_ICONFONTCPPHEADERS}
	${SRC_IMGUI}
	${SRC_IMGUIZMO}
	${SRC_IMGUIFILEDIALOG}
	${SRC_JSON}
	${SRC_MAGICENUM}
	${SRC_MINIZ}
	${SRC_PUGIXML}
	${SRC_TRACERZ}
)
target_include_directories(EnlivengineThirdParty PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(EnlivengineThirdParty PUBLIC SDL2-static SDL2main bgfx SoLoud)
set_target_properties(EnlivengineThirdParty PROPERTIES FOLDER "Enlivengine")
