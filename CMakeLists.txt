cmake_minimum_required(VERSION 3.21)

project(Coel VERSION 1.0.0)

include(coel_warnings)
add_library(coel_warnings_target INTERFACE)
set_project_warnings(coel_warnings_target INTERFACE)

add_subdirectory(Coelengine)

add_subdirectory(Examples)
