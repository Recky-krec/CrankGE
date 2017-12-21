cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")

project(CrankGE)

#includes and sources (+dependencies)
file(GLOB_RECURSE myCode "code/src/*" "dependencies/*")

include_directories(code/src/)
include_directories(dependencies/ASSIMP/include/)
include_directories(dependencies/GLEW/include/)
include_directories(dependencies/GLFW/include/)
include_directories(dependencies/GLM/include/)
include_directories(dependencies/STB_IMAGE/include/)

add_executable(${PROJECT_NAME} ${myCode} code/src/Utils/Time.cpp code/src/Utils/Time.h)

################# Detect and add OpenGL ########################
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
if (OPENGL_FOUND)
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
    # or: target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_gl_LIBRARY})
    # target_link_libraries(${EXECUTABLE_NAME} m)  # if you use maths.h
endif()
################################################################

# Linking static/dynamic libs (.a is static, .so is dynamic)
target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/dependencies/GLFW/lib/libglfw3.a)
target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/dependencies/GLEW/lib/libGLEW.a)
target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/dependencies/ASSIMP/lib/libassimp.so)
target_link_libraries(${PROJECT_NAME} pthread dl X11)

