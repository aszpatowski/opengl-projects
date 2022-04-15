cmake_minimum_required(VERSION 3.21)
project(4_zadanie)
set(CMAKE_CXX_STANDARD 14)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(epoxy)
set_package_properties(epoxy PROPERTIES DESCRIPTION "libepoxy")
if(${epoxy} STREQUAL epoxy-NOTFOUND)
    message(FATAL_ERROR "no epoxy, sad")
else()
    message(STATUS "epoxy, good")
endif()
include_directories(${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${epoxy_INCLUDE_DIR})
add_library(
        AGL3Drawable.hpp
        AGL3Window.hpp
        AGL3Window.cpp
        objects/sphere/Sphere.cpp
        objects/tetrahedron/Tetrahedron.cpp
        Camera.cpp
)

add_executable(4_zadanie 4zadanie.cpp)

target_link_libraries(4_zadanie ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${epoxy_LIBRARIES})