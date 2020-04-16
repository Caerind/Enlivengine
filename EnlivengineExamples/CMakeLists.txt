
source_group("" FILES SystemExample.cpp)
add_executable(SystemExample SystemExample.cpp)
target_link_libraries(SystemExample PRIVATE Enlivengine)
set_target_properties(SystemExample PROPERTIES FOLDER "Enlivengine/EnlivengineExamples")

source_group("" FILES EngineExample.cpp)
add_executable(EngineExample EngineExample.cpp)
target_link_libraries(EngineExample PRIVATE Enlivengine)
set_target_properties(EngineExample PROPERTIES FOLDER "Enlivengine/EnlivengineExamples")

