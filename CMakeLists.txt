cmake_minimum_required(VERSION 3.0)

#------------------------------------------------------------------------------
# Project variables

project(rematrix)

set(REMATRIX_MAJOR_VERSION 0)
set(REMATRIX_MINOR_VERSION 1)
set(REMATRIX_PATCH_VERSION 0)
set(REMATRIX_VERSION
  "${REMATRIX_MAJOR_VERSION}.${REMATRIX_MINOR_VERSION}.${REMATRIX_PATCH_VERSION}")

#------------------------------------------------------------------------------
# Find packages

find_package(X11 REQUIRED)
include_directories(${X11_X11_INCLUDE_PATH})

find_package(OpenGL COMPONENTS GLX OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

find_path(GLM_INCLUDE_DIR glm/glm.hpp)
include_directories(${GLM_INCLUDE_DIR})

find_package(ImageMagick COMPONENTS convert identify REQUIRED)

# find_program(JQ_EXEC jq)

#------------------------------------------------------------------------------
# Compiler-specific

if(${CMAKE_CXX_COMPILER_ID} MATCHES "^(GNU)|(Clang)$")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()

#------------------------------------------------------------------------------
# Targets

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR})

add_custom_command(
  OUTPUT ${CMAKE_BINARY_DIR}/resources.cpp
  COMMAND ${CMAKE_SOURCE_DIR}/resources.cpp.sh > ${CMAKE_BINARY_DIR}/resources.cpp
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  DEPENDS
  data/font.json
  data/font.png
  data/fragment.glsl
  data/vertex.glsl
  )

add_executable(rematrix
  buffer.cpp
  main.cpp
  program.cpp
  rendering_context.cpp
  shader.cpp
  texture.cpp
  vertex_array.cpp
  ${CMAKE_BINARY_DIR}/resources.cpp
  )
target_link_libraries(rematrix
  ${X11_X11_LIB} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES}
  )

#------------------------------------------------------------------------------
# Installation

install(TARGETS rematrix DESTINATION bin)
