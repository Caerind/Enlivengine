add_executable(SystemExample SystemExample.cpp)
target_link_libraries(SystemExample sfml-system sfml-window sfml-graphics sfml-network sfml-audio EnlivengineThirdParty EnlivengineLibrary)

add_executable(EngineExample EngineExample.cpp)
target_link_libraries(EngineExample sfml-system sfml-window sfml-graphics sfml-network sfml-audio EnlivengineThirdParty EnlivengineLibrary)