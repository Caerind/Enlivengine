
source_group("" FILES PlatformExample.cpp)
add_executable(PlatformExample PlatformExample.cpp)
target_link_libraries(PlatformExample PRIVATE Enlivengine)
set_target_properties(PlatformExample PROPERTIES FOLDER "Enlivengine/Examples")

source_group("" FILES SystemExample.cpp)
add_executable(SystemExample SystemExample.cpp)
target_link_libraries(SystemExample PRIVATE Enlivengine)
set_target_properties(SystemExample PROPERTIES FOLDER "Enlivengine/Examples")

source_group("" FILES EngineExample.cpp)
add_executable(EngineExample EngineExample.cpp)
target_link_libraries(EngineExample PRIVATE Enlivengine)
set_target_properties(EngineExample PROPERTIES FOLDER "Enlivengine/Examples")

source_group("" FILES MathExample.cpp)
add_executable(MathExample MathExample.cpp)
target_link_libraries(MathExample PRIVATE Enlivengine)
set_target_properties(MathExample PROPERTIES FOLDER "Enlivengine/Examples")

source_group("" FILES 3DSceneExample.cpp)
add_executable(3DSceneExample 3DSceneExample.cpp)
target_link_libraries(3DSceneExample PRIVATE Enlivengine)
set_target_properties(3DSceneExample PROPERTIES FOLDER "Enlivengine/Examples")

