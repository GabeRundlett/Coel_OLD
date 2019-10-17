cmake_minimum_required(VERSION 3.6)

project(Coelengine)

set(ENGINE_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/enigne/src")
set(MATH_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/math/src")

add_subdirectory(engine)
add_subdirectory(math)
add_subdirectory(test)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT test)
