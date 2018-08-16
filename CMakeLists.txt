PROJECT(VR_demo)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS "-std=c++14")
set(OpenGL_GL_PREFERENCE "GLVND")

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(glm REQUIRED)

set(GLAD_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/glad/include")
set(GLAD_SRC "${GLAD_INCLUDE_DIR}/../src/glad.c")
add_library("GLAD_LIBRARY" ${GLAD_SRC})
target_include_directories("GLAD_LIBRARY" PRIVATE "${GLAD_INCLUDE_DIR}")

set(INCLUDE_DIRS  ${OPENGL_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS} ${GLAD_INCLUDE_DIR})
set(LIBS ${OPENGL_LIBRARY} ${SDL2_LIBRARIES} ${GLAD_LIBRARY})

#specify include directory
include_directories(INCLUDE_DIRS ${INCLUDE_DIRS} include)

set( SOURCES
    main.cpp
    Application.cpp
    Window.cpp
    camera.cpp
    ShaderProgram.hpp
    ${GLAD_SRC}
)

add_executable( ${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${LIBS} SDL2::SDL2)

#Now copy shaders to build directory
add_custom_target(copy-runtime-files ALL
    COMMAND cmake -E copy_directory ${CMAKE_SOURCE_DIR}/shaders ${CMAKE_BINARY_DIR}/shaders
    COMMAND cmake -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
    DEPENDS ${MY_TARGET})
