cmake_minimum_required(VERSION 2.8)

set(NAME my_awesome_project)

project(${NAME})

include_directories(src)
include_directories(src/imgui)

add_definitions(-std=c++11)
add_definitions("-Wall")

file(GLOB SOURCE src/*.cc src/imgui/*.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin/")

add_executable(${NAME} src/main.cc ${SOURCE})

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
find_package(X11 REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS} )
include_directories( ${GLEW_INCLUDE_DIRS} )
include_directories( ${GLFW_INLUDE_DIRS} )

target_link_libraries(${NAME} ${OPENGL_LIBRARIES} )
target_link_libraries(${NAME} GLEW ${GLEW_LIBRARIES} )
target_link_libraries(${NAME} glfw3 ${GLFW_LIBRARIES} )
target_link_libraries(${NAME}  ${X11_LIBRARIES} )

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
FIND_LIBRARY(COCOA_LIBRARY Cocoa)
FIND_LIBRARY(IOKIT_LIBRARY IOKit)
FIND_LIBRARY(COREVIDEO_LIBRARY CoreVideo)
FIND_LIBRARY(CARBON_LIBRARY Carbon)

target_link_libraries(${NAME} ${COCOA_LIBRARY})
target_link_libraries(${NAME} ${IOKIT_LIBRARY})
target_link_libraries(${NAME} ${COREVIDEO_LIBRARY})
target_link_libraries(${NAME} ${CARBON_LIBRARY})
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
target_link_libraries(${NAME} -lXxf86vm )
target_link_libraries(${NAME} -lXrandr )
target_link_libraries(${NAME} -lpthread )
target_link_libraries(${NAME} -lXi )
target_link_libraries(${NAME} -ldl )
target_link_libraries(${NAME} -lm )
target_link_libraries(${NAME} -lXinerama )
target_link_libraries(${NAME} -lXcursor)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

target_link_libraries(${NAME})
