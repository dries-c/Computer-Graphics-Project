cmake_minimum_required(VERSION 3.24)
project(opengl_project)

set(CMAKE_CXX_STANDARD 17)

link_directories(${CMAKE_SOURCE_DIR}/lib)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

find_package(OpenGL REQUIRED)

# find the required packages
find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")

set(LIBS glfw3 "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")

add_library(GLAD "glad.c" includes/KHR/khrplatform.h src/scene/mesh/texture/stb_image.h)
set(LIBS ${LIBS} GLAD)

include_directories(${CMAKE_SOURCE_DIR}/includes)
add_executable(opengl_project main.cpp src/camera/Camera.cpp src/camera/Camera.h src/scene/model/shader/Shader.cpp src/scene/model/shader/Shader.h src/utils/FileUtils.cpp src/utils/FileUtils.h src/camera/Direction.h src/scene/Scene.cpp src/scene/Scene.h src/scene/model/Model.cpp src/scene/model/Model.h src/scene/mesh/texture/Texture.cpp src/scene/mesh/texture/Texture.h src/scene/mesh/Mesh.cpp src/scene/mesh/Mesh.h src/utils/Color.cpp src/utils/Color.h src/utils/Structs.h src/parser/MazeParser.cpp src/parser/MazeParser.h src/parser/FileMazeParser.cpp src/parser/FileMazeParser.h src/parser/PositionEnum.h)
target_link_libraries(opengl_project ${LIBS})