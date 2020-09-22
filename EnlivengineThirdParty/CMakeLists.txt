
# SFML
set(SFML_BUILD_AUDIO TRUE)
set(SFML_BUILD_GRAPHICS TRUE)
set(SFML_BUILD_NETWORK TRUE)
set(SFML_BUILD_WINDOW TRUE)
set(SFML_DIR "./SFML/cmake/")
set(BUILD_SHARED_LIBS FALSE)
add_subdirectory(SFML)

# Box2D
set(SRC_BOX2D
  Box2D/Collision/Shapes/b2ChainShape.cpp
  Box2D/Collision/Shapes/b2ChainShape.h
  Box2D/Collision/Shapes/b2CircleShape.cpp
  Box2D/Collision/Shapes/b2CircleShape.h
  Box2D/Collision/Shapes/b2EdgeShape.cpp
  Box2D/Collision/Shapes/b2EdgeShape.h
  Box2D/Collision/Shapes/b2PolygonShape.cpp
  Box2D/Collision/Shapes/b2PolygonShape.h
  Box2D/Collision/Shapes/b2Shape.h
  Box2D/Collision/b2BroadPhase.cpp
  Box2D/Collision/b2BroadPhase.h
  Box2D/Collision/b2CollideCircle.cpp
  Box2D/Collision/b2CollideEdge.cpp
  Box2D/Collision/b2CollidePolygon.cpp
  Box2D/Collision/b2Collision.cpp
  Box2D/Collision/b2Collision.h
  Box2D/Collision/b2Distance.cpp
  Box2D/Collision/b2Distance.h
  Box2D/Collision/b2DynamicTree.cpp
  Box2D/Collision/b2DynamicTree.h
  Box2D/Collision/b2TimeOfImpact.cpp
  Box2D/Collision/b2TimeOfImpact.h
  Box2D/Common/b2BlockAllocator.cpp
  Box2D/Common/b2BlockAllocator.h
  Box2D/Common/b2Draw.cpp
  Box2D/Common/b2Draw.h
  Box2D/Common/b2GrowableStack.h
  Box2D/Common/b2Math.cpp
  Box2D/Common/b2Math.h
  Box2D/Common/b2Settings.cpp
  Box2D/Common/b2Settings.h
  Box2D/Common/b2StackAllocator.cpp
  Box2D/Common/b2StackAllocator.h
  Box2D/Common/b2Timer.cpp
  Box2D/Common/b2Timer.h
  Box2D/Dynamics/Contacts/b2ChainAndCircleContact.cpp
  Box2D/Dynamics/Contacts/b2ChainAndCircleContact.h
  Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.cpp
  Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.h
  Box2D/Dynamics/Contacts/b2CircleContact.cpp
  Box2D/Dynamics/Contacts/b2CircleContact.h
  Box2D/Dynamics/Contacts/b2Contact.cpp
  Box2D/Dynamics/Contacts/b2Contact.h
  Box2D/Dynamics/Contacts/b2ContactSolver.cpp
  Box2D/Dynamics/Contacts/b2ContactSolver.h
  Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.cpp
  Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.h
  Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.cpp
  Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.h
  Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.cpp
  Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.h
  Box2D/Dynamics/Contacts/b2PolygonContact.cpp
  Box2D/Dynamics/Contacts/b2PolygonContact.h
  Box2D/Dynamics/Joints/b2DistanceJoint.cpp
  Box2D/Dynamics/Joints/b2DistanceJoint.h
  Box2D/Dynamics/Joints/b2FrictionJoint.cpp
  Box2D/Dynamics/Joints/b2FrictionJoint.h
  Box2D/Dynamics/Joints/b2GearJoint.cpp
  Box2D/Dynamics/Joints/b2GearJoint.h
  Box2D/Dynamics/Joints/b2Joint.cpp
  Box2D/Dynamics/Joints/b2Joint.h
  Box2D/Dynamics/Joints/b2MotorJoint.cpp
  Box2D/Dynamics/Joints/b2MotorJoint.h
  Box2D/Dynamics/Joints/b2MouseJoint.cpp
  Box2D/Dynamics/Joints/b2MouseJoint.h
  Box2D/Dynamics/Joints/b2PrismaticJoint.cpp
  Box2D/Dynamics/Joints/b2PrismaticJoint.h
  Box2D/Dynamics/Joints/b2PulleyJoint.cpp
  Box2D/Dynamics/Joints/b2PulleyJoint.h
  Box2D/Dynamics/Joints/b2RevoluteJoint.cpp
  Box2D/Dynamics/Joints/b2RevoluteJoint.h
  Box2D/Dynamics/Joints/b2RopeJoint.cpp
  Box2D/Dynamics/Joints/b2RopeJoint.h
  Box2D/Dynamics/Joints/b2WeldJoint.cpp
  Box2D/Dynamics/Joints/b2WeldJoint.h
  Box2D/Dynamics/Joints/b2WheelJoint.cpp
  Box2D/Dynamics/Joints/b2WheelJoint.h
  Box2D/Dynamics/b2Body.cpp
  Box2D/Dynamics/b2Body.h
  Box2D/Dynamics/b2ContactManager.cpp
  Box2D/Dynamics/b2ContactManager.h
  Box2D/Dynamics/b2Fixture.cpp
  Box2D/Dynamics/b2Fixture.h
  Box2D/Dynamics/b2Island.cpp
  Box2D/Dynamics/b2Island.h
  Box2D/Dynamics/b2TimeStep.h
  Box2D/Dynamics/b2World.cpp
  Box2D/Dynamics/b2World.h
  Box2D/Dynamics/b2WorldCallbacks.cpp
  Box2D/Dynamics/b2WorldCallbacks.h
  Box2D/Rope/b2Rope.cpp
  Box2D/Rope/b2Rope.h
  Box2D/Box2D.h
)
source_group("Box2D" FILES ${SRC_BOX2D})
	
# debug_break
set(SRC_DEBUGBREAK
	debug_break/debugbreak.h
)
source_group("debug_break" FILES ${SRC_DEBUGBREAK})

# dirent
set(SRC_DIRENT
	dirent/dirent.h
)
source_group("dirent" FILES ${SRC_DIRENT})
	
# doctest
set(SRC_DOCTEST
	doctest/doctest.h
)
source_group("doctest" FILES ${SRC_DOCTEST})

# Dyma
set(SRC_DYMA
	Dyma/Dyma.cpp
	Dyma/Dyma.hpp
)
source_group("Dyma" FILES ${SRC_DYMA})

# entt
set(SRC_ENTT
	entt/entt.hpp
)
source_group("entt" FILES ${SRC_ENTT})

# FastNoise
set(SRC_FASTNOISE
	FastNoise/FastNoise.cpp
	FastNoise/FastNoise.h
)
source_group("FastNoise" FILES ${SRC_FASTNOISE})

# fmt
set(SRC_FMT
	fmt/chrono.h
	fmt/color.h
	fmt/compile.h
	fmt/core.h
	fmt/format.cc
	fmt/format.h
	fmt/format-inl.h
	fmt/locale.h
	fmt/os.cc
	fmt/os.h
	fmt/ostream.h
	fmt/posix.h
	fmt/printf.h
	fmt/ranges.h
)
source_group("fmt" FILES ${SRC_FMT})

# IconFontCppHeaders
set(SRC_ICONFONTCPPHEADERS
	IconFontCppHeaders/IconsFontAwesome5.h
)
source_group("IconFontCppHeaders" FILES ${SRC_ICONFONTCPPHEADERS})

# imgui
set(SRC_IMGUI
	imgui/imconfig.h
	imgui/imgui.cpp
	imgui/imgui.h
	imgui/imgui_demo.cpp
	imgui/imgui_draw.cpp
	imgui/imgui_internal.h
	imgui/imgui_stdlib.cpp
	imgui/imgui_stdlib.h
	imgui/imgui_widgets.cpp
	imgui/imstb_rectpack.h
	imgui/imstb_textedit.h
	imgui/imstb_truetype.h
)
source_group("imgui" FILES ${SRC_IMGUI})

# ImGuiFileDialog
set(SRC_IMGUIFILEDIALOG
	ImGuiFileDialog/ImGuiFileDialog.cpp
	ImGuiFileDialog/ImGuiFileDialog.h
)
source_group("ImGuiFileDialog" FILES ${SRC_IMGUIFILEDIALOG})

# imgui-node-editor
set(SRC_IMGUINODEEDITOR
	imgui-node-editor/crude_json.cpp
	imgui-node-editor/crude_json.h
	imgui-node-editor/imgui_bezier_math.h
	imgui-node-editor/imgui_bezier_math.inl
	imgui-node-editor/imgui_canvas.cpp
	imgui-node-editor/imgui_canvas.h
	imgui-node-editor/imgui_extra_math.h
	imgui-node-editor/imgui_extra_math.inl
	imgui-node-editor/imgui_node_editor.cpp
	imgui-node-editor/imgui_node_editor.h
	imgui-node-editor/imgui_node_editor_api.cpp
	imgui-node-editor/imgui_node_editor_internal.h
	imgui-node-editor/imgui_node_editor_internal.inl
)
source_group("imgui-node-editor" FILES ${SRC_IMGUINODEEDITOR})

# imgui-sfml
set(SRC_IMGUISFML
	imgui-sfml/imgui-SFML.cpp
	imgui-sfml/imgui-SFML.h
	imgui-sfml/imgui-SFML_export.h
)
source_group("imgui-sfml" FILES ${SRC_IMGUISFML})

# json
set(SRC_JSON
	json/json.hpp
)
source_group("json" FILES ${SRC_JSON})

# magic_enum
set(SRC_MAGICENUM
	magic_enum/magic_enum.hpp
)
source_group("magic_enum" FILES ${SRC_MAGICENUM})

# miniz
set(SRC_MINIZ
	miniz/miniz.cpp
	miniz/miniz.hpp
)
source_group("miniz" FILES ${SRC_MINIZ})

# pugixml
set(SRC_PUGIXML
	pugixml/pugiconfig.hpp
	pugixml/pugixml.hpp
	pugixml/pugixml.cpp
)
source_group("pugixml" FILES ${SRC_PUGIXML})

add_library(EnlivengineThirdParty STATIC
	${SRC_BOX2D}
	${SRC_DEBUGBREAK}
	${SRC_DIRENT}
	${SRC_DOCTEST}
	${SRC_DYMA}
	${SRC_ENTT}
	${SRC_FASTNOISE}
	${SRC_FMT}
	${SRC_ICONFONTCPPHEADERS}
	${SRC_IMGUI}
	${SRC_IMGUIFILEDIALOG}
	${SRC_IMGUINODEEDITOR}
	${SRC_IMGUISFML}
	${SRC_JSON}
	${SRC_MAGICENUM}
	${SRC_MINIZ}
	${SRC_PUGIXML}
)
target_include_directories(EnlivengineThirdParty PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(EnlivengineThirdParty OpenGL::GL sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
set_target_properties(EnlivengineThirdParty PROPERTIES FOLDER "Enlivengine")
