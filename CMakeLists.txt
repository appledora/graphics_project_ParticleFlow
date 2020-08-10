cmake_minimum_required(VERSION 3.9)
project("Graphics Demo")

set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES main.cpp)
set(CMAKE_PREFIX_PATH "C:/freeglut")

set(FREEGLUT_INCLUDE_DIRS "C:/freeglut/include")
set(FREEGLUT_LIBRARY_DIRS "C:/freeglut/lib/x64")

set(FREEGLUT_LIBRARIES freeglut)
set(OpenGL_LIBRARIES glu32 opengl32)

include_directories(${FREEGLUT_INCLUDE_DIRS})
link_directories(${FREEGLUT_LIBRARY_DIRS})

add_executable(demo ${SOURCE_FILES})
target_link_libraries(demo ${FREEGLUT_LIBRARIES} ${OpenGL_LIBRARIES} )