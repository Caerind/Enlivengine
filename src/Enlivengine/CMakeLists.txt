
add_subdirectory(System)


target_link_libraries(EnlivengineLibrary PRIVATE ThirdParty)
target_include_directories(EnlivengineLibrary PUBLIC ${PROJECT_SOURCE_DIR}/thirdparty)