cmake_minimum_required(VERSION 3.0)

#------------------------------------------------------------------------------
# Project variables

project(
  rematrix
  VERSION 0.1.0
  LANGUAGES CXX
  )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS off)

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

find_package(Python3 COMPONENTS Interpreter REQUIRED)

#------------------------------------------------------------------------------
# Targets

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR})

add_custom_command(
  OUTPUT ${CMAKE_BINARY_DIR}/resources.cpp
  COMMAND ${CMAKE_SOURCE_DIR}/resources.cpp.sh > ${CMAKE_BINARY_DIR}/resources.cpp
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  DEPENDS
  data/font.fnt
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

#------------------------------------------------------------------------------
# Targets

add_custom_command(
  OUTPUT ${PROJECT_BINARY_DIR}/font_image_size.inc
  COMMAND ${ImageMagick_identify_EXECUTABLE} -format "%w, %h" ${PROJECT_SOURCE_DIR}/data/font.png > ${PROJECT_BINARY_DIR}/font_image_size.inc
  DEPENDS ${PROJECT_SOURCE_DIR}/data/font.png
  VERBATIM
  )

foreach(SHADER vertex fragment)
  add_custom_command(
    OUTPUT ${PROJECT_BINARY_DIR}/${SHADER}_src.inc
    COMMAND sh -c "echo 'R\"('; cat ${PROJECT_SOURCE_DIR}/data/${SHADER}.glsl; echo ')\"'" > ${PROJECT_BINARY_DIR}/${SHADER}_src.inc
    DEPENDS ${PROJECT_SOURCE_DIR}/data/${SHADER}.glsl
    VERBATIM
    )
endforeach()

add_custom_command(
  OUTPUT ${PROJECT_BINARY_DIR}/font.inc
  COMMAND ${Python3_EXECUTABLE} ${PROJECT_SOURCE_DIR}/cmds/font_descr2 < ${PROJECT_SOURCE_DIR}/data/font.fnt > ${PROJECT_BINARY_DIR}/font.inc
  DEPENDS ${PROJECT_SOURCE_DIR}/data/font.fnt
  VERBATIM
  )

add_custom_command(
  OUTPUT ${PROJECT_BINARY_DIR}/font.raw
  COMMAND ${ImageMagick_convert_EXECUTABLE} ${PROJECT_SOURCE_DIR}/data/font.png -depth 8 gray:${PROJECT_BINARY_DIR}/font.raw
  DEPENDS ${PROJECT_SOURCE_DIR}/data/font.png
  VERBATIM
  )

add_custom_command(
  OUTPUT ${PROJECT_BINARY_DIR}/font_image_data.inc
  COMMAND od -v -A n -t u1 -w1 ${PROJECT_BINARY_DIR}/font.raw | sed "s/^ *//;s/\$/,/" > ${PROJECT_BINARY_DIR}/font_image_data.inc
  DEPENDS ${PROJECT_BINARY_DIR}/font.raw
  VERBATIM
  )

add_custom_target(
  resources_inc
  DEPENDS
  ${PROJECT_BINARY_DIR}/font.inc
  ${PROJECT_BINARY_DIR}/font_image_data.inc
  ${PROJECT_BINARY_DIR}/font_image_size.inc
  ${PROJECT_BINARY_DIR}/fragment_src.inc
  ${PROJECT_BINARY_DIR}/vertex_src.inc
  )
