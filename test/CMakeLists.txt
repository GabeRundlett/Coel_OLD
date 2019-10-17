include_directories("${ENGINE_INCLUDE_PATH}")
include_directories("${MATH_INCLUDE_PATH}")

file(GLOB_RECURSE TEST_SOURCES "src/*.cpp" "src/*.hpp")
add_executable(test ${TEST_SOURCES})
target_link_libraries(test general math engine)
