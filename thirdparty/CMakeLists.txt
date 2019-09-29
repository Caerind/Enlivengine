
set(TPROOT ${PROJECT_SOURCE_DIR}/thirdparty)

# Box2D
set(SRC_BOX2D
  ${TPROOT}/Box2D/Collision/Shapes/b2ChainShape.cpp
  ${TPROOT}/Box2D/Collision/Shapes/b2ChainShape.h
  ${TPROOT}/Box2D/Collision/Shapes/b2CircleShape.cpp
  ${TPROOT}/Box2D/Collision/Shapes/b2CircleShape.h
  ${TPROOT}/Box2D/Collision/Shapes/b2EdgeShape.cpp
  ${TPROOT}/Box2D/Collision/Shapes/b2EdgeShape.h
  ${TPROOT}/Box2D/Collision/Shapes/b2PolygonShape.cpp
  ${TPROOT}/Box2D/Collision/Shapes/b2PolygonShape.h
  ${TPROOT}/Box2D/Collision/Shapes/b2Shape.h
  ${TPROOT}/Box2D/Collision/b2BroadPhase.cpp
  ${TPROOT}/Box2D/Collision/b2BroadPhase.h
  ${TPROOT}/Box2D/Collision/b2CollideCircle.cpp
  ${TPROOT}/Box2D/Collision/b2CollideEdge.cpp
  ${TPROOT}/Box2D/Collision/b2CollidePolygon.cpp
  ${TPROOT}/Box2D/Collision/b2Collision.cpp
  ${TPROOT}/Box2D/Collision/b2Collision.h
  ${TPROOT}/Box2D/Collision/b2Distance.cpp
  ${TPROOT}/Box2D/Collision/b2Distance.h
  ${TPROOT}/Box2D/Collision/b2DynamicTree.cpp
  ${TPROOT}/Box2D/Collision/b2DynamicTree.h
  ${TPROOT}/Box2D/Collision/b2TimeOfImpact.cpp
  ${TPROOT}/Box2D/Collision/b2TimeOfImpact.h
  ${TPROOT}/Box2D/Common/b2BlockAllocator.cpp
  ${TPROOT}/Box2D/Common/b2BlockAllocator.h
  ${TPROOT}/Box2D/Common/b2Draw.cpp
  ${TPROOT}/Box2D/Common/b2Draw.h
  ${TPROOT}/Box2D/Common/b2GrowableStack.h
  ${TPROOT}/Box2D/Common/b2Math.cpp
  ${TPROOT}/Box2D/Common/b2Math.h
  ${TPROOT}/Box2D/Common/b2Settings.cpp
  ${TPROOT}/Box2D/Common/b2Settings.h
  ${TPROOT}/Box2D/Common/b2StackAllocator.cpp
  ${TPROOT}/Box2D/Common/b2StackAllocator.h
  ${TPROOT}/Box2D/Common/b2Timer.cpp
  ${TPROOT}/Box2D/Common/b2Timer.h
  ${TPROOT}/Box2D/Dynamics/Contacts/b2ChainAndCircleContact.cpp
  ${TPROOT}/Box2D/Dynamics/Contacts/b2ChainAndCircleContact.h
  ${TPROOT}/Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.cpp
  ${TPROOT}/Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.h
  ${TPROOT}/Box2D/Dynamics/Contacts/b2CircleContact.cpp
  ${TPROOT}/Box2D/Dynamics/Contacts/b2CircleContact.h
  ${TPROOT}/Box2D/Dynamics/Contacts/b2Contact.cpp
  ${TPROOT}/Box2D/Dynamics/Contacts/b2Contact.h
  ${TPROOT}/Box2D/Dynamics/Contacts/b2ContactSolver.cpp
  ${TPROOT}/Box2D/Dynamics/Contacts/b2ContactSolver.h
  ${TPROOT}/Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.cpp
  ${TPROOT}/Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.h
  ${TPROOT}/Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.cpp
  ${TPROOT}/Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.h
  ${TPROOT}/Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.cpp
  ${TPROOT}/Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.h
  ${TPROOT}/Box2D/Dynamics/Contacts/b2PolygonContact.cpp
  ${TPROOT}/Box2D/Dynamics/Contacts/b2PolygonContact.h
  ${TPROOT}/Box2D/Dynamics/Joints/b2DistanceJoint.cpp
  ${TPROOT}/Box2D/Dynamics/Joints/b2DistanceJoint.h
  ${TPROOT}/Box2D/Dynamics/Joints/b2FrictionJoint.cpp
  ${TPROOT}/Box2D/Dynamics/Joints/b2FrictionJoint.h
  ${TPROOT}/Box2D/Dynamics/Joints/b2GearJoint.cpp
  ${TPROOT}/Box2D/Dynamics/Joints/b2GearJoint.h
  ${TPROOT}/Box2D/Dynamics/Joints/b2Joint.cpp
  ${TPROOT}/Box2D/Dynamics/Joints/b2Joint.h
  ${TPROOT}/Box2D/Dynamics/Joints/b2MotorJoint.cpp
  ${TPROOT}/Box2D/Dynamics/Joints/b2MotorJoint.h
  ${TPROOT}/Box2D/Dynamics/Joints/b2MouseJoint.cpp
  ${TPROOT}/Box2D/Dynamics/Joints/b2MouseJoint.h
  ${TPROOT}/Box2D/Dynamics/Joints/b2PrismaticJoint.cpp
  ${TPROOT}/Box2D/Dynamics/Joints/b2PrismaticJoint.h
  ${TPROOT}/Box2D/Dynamics/Joints/b2PulleyJoint.cpp
  ${TPROOT}/Box2D/Dynamics/Joints/b2PulleyJoint.h
  ${TPROOT}/Box2D/Dynamics/Joints/b2RevoluteJoint.cpp
  ${TPROOT}/Box2D/Dynamics/Joints/b2RevoluteJoint.h
  ${TPROOT}/Box2D/Dynamics/Joints/b2RopeJoint.cpp
  ${TPROOT}/Box2D/Dynamics/Joints/b2RopeJoint.h
  ${TPROOT}/Box2D/Dynamics/Joints/b2WeldJoint.cpp
  ${TPROOT}/Box2D/Dynamics/Joints/b2WeldJoint.h
  ${TPROOT}/Box2D/Dynamics/Joints/b2WheelJoint.cpp
  ${TPROOT}/Box2D/Dynamics/Joints/b2WheelJoint.h
  ${TPROOT}/Box2D/Dynamics/b2Body.cpp
  ${TPROOT}/Box2D/Dynamics/b2Body.h
  ${TPROOT}/Box2D/Dynamics/b2ContactManager.cpp
  ${TPROOT}/Box2D/Dynamics/b2ContactManager.h
  ${TPROOT}/Box2D/Dynamics/b2Fixture.cpp
  ${TPROOT}/Box2D/Dynamics/b2Fixture.h
  ${TPROOT}/Box2D/Dynamics/b2Island.cpp
  ${TPROOT}/Box2D/Dynamics/b2Island.h
  ${TPROOT}/Box2D/Dynamics/b2TimeStep.h
  ${TPROOT}/Box2D/Dynamics/b2World.cpp
  ${TPROOT}/Box2D/Dynamics/b2World.h
  ${TPROOT}/Box2D/Dynamics/b2WorldCallbacks.cpp
  ${TPROOT}/Box2D/Dynamics/b2WorldCallbacks.h
  ${TPROOT}/Box2D/Rope/b2Rope.cpp
  ${TPROOT}/Box2D/Rope/b2Rope.h
  ${TPROOT}/Box2D/Box2D.h
)
source_group("Box2D" FILES ${SRC_BOX2D})

# cocoa
set(SRC_COCOA
	${TPROOT}/cocoa/cocoa.hpp
)
source_group("cocoa" FILES ${SRC_COCOA})

# dbgtools
set(SRC_DBGTOOLS
    ${TPROOT}/dbgtools/assert.h
    ${TPROOT}/dbgtools/assert.cpp
    ${TPROOT}/dbgtools/callstack.h
    ${TPROOT}/dbgtools/callstack.cpp
    ${TPROOT}/dbgtools/debugger.h
    ${TPROOT}/dbgtools/debugger.cpp
    ${TPROOT}/dbgtools/hw_breakpoint.h
    ${TPROOT}/dbgtools/hw_breakpoint.cpp
)
source_group("dbgtools" FILES ${SRC_DBGTOOLS})
	
# debug_break
set(SRC_DEBUGBREAK
	${TPROOT}/debug_break/debugbreak.h
)
source_group("debug_break" FILES ${SRC_DEBUGBREAK})
	
# doctest
set(SRC_DOCTEST
	${TPROOT}/doctest/doctest.h
)
source_group("doctest" FILES ${SRC_DOCTEST})

# imgui
set(SRC_IMGUI
	${TPROOT}/imgui/imconfig.h
	${TPROOT}/imgui/imgui.cpp
	${TPROOT}/imgui/imgui.h
	${TPROOT}/imgui/imgui_demo.cpp
	${TPROOT}/imgui/imgui_draw.cpp
	${TPROOT}/imgui/imgui_internal.h
	${TPROOT}/imgui/imgui_stdlib.cpp
	${TPROOT}/imgui/imgui_stdlib.h
	${TPROOT}/imgui/imgui_widgets.cpp
	${TPROOT}/imgui/imstb_rectpack.h
	${TPROOT}/imgui/imstb_textedit.h
	${TPROOT}/imgui/imstb_truetype.h
)
source_group("imgui" FILES ${SRC_IMGUI})

# imgui-sfml
set(SRC_IMGUISFML
	${TPROOT}/imgui-sfml/imgui-SFML.cpp
	${TPROOT}/imgui-sfml/imgui-SFML.h
	${TPROOT}/imgui-sfml/imgui-SFML_export.h
)
source_group("imgui-sfml" FILES ${SRC_IMGUISFML})

# pugixml
set(SRC_PUGIXML
	${TPROOT}/pugixml/pugiconfig.hpp
	${TPROOT}/pugixml/pugixml.hpp
	${TPROOT}/pugixml/pugixml.cpp
)
source_group("pugixml" FILES ${SRC_PUGIXML})

add_library(EnlivengineThirdParty 
	${SRC_BOX2D}
	${SRC_COCOA}
	${SRC_DBGTOOLS}
	${SRC_DEBUGBREAK}
	${SRC_DOCTEST}
	${SRC_IMGUI}
	${SRC_IMGUISFML}
	${SRC_PUGIXML}
)
target_link_libraries(EnlivengineThirdParty sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
target_include_directories(EnlivengineThirdParty PUBLIC ${TPROOT})
