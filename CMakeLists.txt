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

find_package(Python3 COMPONENTS Interpreter REQUIRED)

#------------------------------------------------------------------------------
# Targets

add_executable(
  rematrix
  buffer.cpp
  main.cpp
  options.cpp
  frame_buffer.cpp
  program.cpp
  rendering_context.cpp
  resources.cpp
  shader.cpp
  texture.cpp
  vertex_array.cpp
  )

add_dependencies(
  rematrix
  resources_inc
  )

target_include_directories(
  rematrix
  PRIVATE
  ${PROJECT_BINARY_DIR}
  )

target_link_libraries(
  rematrix
  PRIVATE
  ${X11_X11_LIB}
  ${OPENGL_LIBRARIES}
  ${GLEW_LIBRARIES}
  )

install(TARGETS rematrix DESTINATION bin)

#------------------------------------------------------------------------------
# Targets

add_custom_command(
  OUTPUT ${PROJECT_BINARY_DIR}/font_image_size.inc
  COMMAND ${Python3_EXECUTABLE} ${PROJECT_SOURCE_DIR}/data/procfont image_size ${PROJECT_BINARY_DIR}/font.png > ${PROJECT_BINARY_DIR}/font_image_size.inc
  DEPENDS ${PROJECT_BINARY_DIR}/font.png ${PROJECT_SOURCE_DIR}/data/procfont
  VERBATIM
  )

foreach(SHADER vertex frag)
  add_custom_command(
    OUTPUT ${PROJECT_BINARY_DIR}/${SHADER}_src.inc
    COMMAND sh -c "echo 'R\"('; cat ${PROJECT_SOURCE_DIR}/data/${SHADER}.glsl; echo ')\"'" > ${PROJECT_BINARY_DIR}/${SHADER}_src.inc
    DEPENDS ${PROJECT_SOURCE_DIR}/data/${SHADER}.glsl
    VERBATIM
    )
endforeach()

add_custom_command(
  OUTPUT ${PROJECT_BINARY_DIR}/font.inc
  COMMAND ${Python3_EXECUTABLE} ${PROJECT_SOURCE_DIR}/data/procfont make_cpp_inc < ${PROJECT_BINARY_DIR}/font.fnt > ${PROJECT_BINARY_DIR}/font.inc
  DEPENDS ${PROJECT_BINARY_DIR}/font.fnt ${PROJECT_SOURCE_DIR}/data/procfont
  VERBATIM
  )

add_custom_command(
  OUTPUT ${PROJECT_BINARY_DIR}/font_image_data.inc
  COMMAND ${Python3_EXECUTABLE} ${PROJECT_SOURCE_DIR}/data/procfont dump_image_inc ${PROJECT_BINARY_DIR}/font.png > ${PROJECT_BINARY_DIR}/font_image_data.inc
  DEPENDS ${PROJECT_BINARY_DIR}/font.png ${PROJECT_SOURCE_DIR}/data/procfont
  VERBATIM
  )

add_custom_command(
  OUTPUT ${PROJECT_BINARY_DIR}/font2.png
  COMMAND ${Python3_EXECUTABLE} ${PROJECT_SOURCE_DIR}/data/procfont transpose_glyphs ${PROJECT_SOURCE_DIR}/data/font2.png ${PROJECT_BINARY_DIR}/font2.png < ${PROJECT_SOURCE_DIR}/data/font2.fnt
  DEPENDS ${PROJECT_SOURCE_DIR}/data/procfont ${PROJECT_SOURCE_DIR}/data/font2.fnt ${PROJECT_SOURCE_DIR}/data/font2.png
  VERBATIM
  )

add_custom_command(
  OUTPUT ${PROJECT_BINARY_DIR}/font2.fnt
  COMMAND ${Python3_EXECUTABLE} ${PROJECT_SOURCE_DIR}/data/procfont transpose_descr < ${PROJECT_SOURCE_DIR}/data/font2.fnt > ${PROJECT_BINARY_DIR}/font2.fnt
  DEPENDS ${PROJECT_SOURCE_DIR}/data/procfont ${PROJECT_SOURCE_DIR}/data/font2.fnt
  VERBATIM
  )

add_custom_command(
  OUTPUT ${PROJECT_BINARY_DIR}/font.png
  COMMAND ${Python3_EXECUTABLE} ${PROJECT_SOURCE_DIR}/data/procfont combine_glyphs ${PROJECT_SOURCE_DIR}/data/font1.png ${PROJECT_BINARY_DIR}/font2.png ${PROJECT_BINARY_DIR}/font.png
  DEPENDS ${PROJECT_BINARY_DIR}/font2.png ${PROJECT_SOURCE_DIR}/data/procfont ${PROJECT_SOURCE_DIR}/data/font1.png
  VERBATIM
  )

add_custom_command(
  OUTPUT ${PROJECT_BINARY_DIR}/font.fnt
  COMMAND ${Python3_EXECUTABLE} ${PROJECT_SOURCE_DIR}/data/procfont combine_descrs ${PROJECT_SOURCE_DIR}/data/font1.fnt ${PROJECT_BINARY_DIR}/font2.fnt > ${PROJECT_BINARY_DIR}/font.fnt
  DEPENDS ${PROJECT_BINARY_DIR}/font2.fnt ${PROJECT_SOURCE_DIR}/data/procfont ${PROJECT_SOURCE_DIR}/data/font1.fnt
  VERBATIM
  )

add_custom_target(
  resources_inc
  DEPENDS
  ${PROJECT_BINARY_DIR}/font.inc
  ${PROJECT_BINARY_DIR}/font_image_data.inc
  ${PROJECT_BINARY_DIR}/font_image_size.inc
  ${PROJECT_BINARY_DIR}/frag_src.inc
  ${PROJECT_BINARY_DIR}/vertex_src.inc
  )
