
set(TPROOT ${PROJECT_SOURCE_DIR}/thirdparty)

set(SRC_COCOA
	${TPROOT}/cocoa/cocoa.hpp
)
source_group("cocoa" FILES ${SRC_COCOA})

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
	
set(SRC_DEBUGBREAK
	${TPROOT}/debug_break/debugbreak.h
)
source_group("debug_break" FILES ${SRC_DEBUGBREAK})
	
set(SRC_DOCTEST
	${TPROOT}/doctest/doctest.h
)
source_group("doctest" FILES ${SRC_DOCTEST})

set(SRC_PUGIXML
	${TPROOT}/pugixml/pugiconfig.hpp
	${TPROOT}/pugixml/pugixml.hpp
	${TPROOT}/pugixml/pugixml.cpp
)
source_group("pugixml" FILES ${SRC_PUGIXML})

add_library(ThirdParty 
	${SRC_COCOA}
	${SRC_DBGTOOLS}
	${SRC_DEBUGBREAK}
	${SRC_DOCTEST}
	${SRC_PUGIXML}
)
target_include_directories(ThirdParty PUBLIC ${PROJECT_SOURCE_DIR}/thirdparty)
