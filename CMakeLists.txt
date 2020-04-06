cmake_minimum_required(VERSION 3.10)
set(OpenGL_GL_PREFERENCE GLVND)
project(physics)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Werror=return-type")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


add_executable(physics main.cpp)

include_directories(BEFORE PUBLIC /usr/local/include/reactphysics3d)
target_link_libraries(physics /usr/local/lib/libreactphysics3d.a)

find_package(glfw3 3.3.2 REQUIRED)
target_link_libraries(physics glfw)
find_package(OpenGL REQUIRED)
# These variables have been set by the previous command
target_include_directories(physics PUBLIC ${OPENGL_INCLUDE_DIR})
target_link_libraries(physics ${OPENGL_LIBRARIES})
