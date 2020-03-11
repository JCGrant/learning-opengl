cmake_minimum_required(VERSION 3.0)
project(learnopengl)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(vendor/glfw)

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(vendor/assimp)

option(BUILD_BULLET2_DEMOS OFF)
option(BUILD_CPU_DEMOS OFF)
option(BUILD_EXTRAS OFF)
option(BUILD_OPENGL3_DEMOS OFF)
option(BUILD_UNIT_TESTS OFF)
add_subdirectory(vendor/bullet)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
set(GLAD_LIBRARIES dl)

include_directories(
  vendor/assimp/include/
  vendor/bullet/src/
  vendor/glad/include/
  vendor/glfw/include/
  vendor/glm/
  vendor/stb/
)

file(GLOB VENDORS_SOURCES
  vendor/glad/src/glad.c
)
file(GLOB PROJECT_HEADERS
  src/*.h
)
file(GLOB PROJECT_SOURCES
  src/*.cpp
)
file(GLOB PROJECT_DEPS
  resources
  shaders
)
file(GLOB PROJECT_CONFIGS
  CMakeLists.txt
  .gitignore
  .gitmodules
)

source_group("Vendors" FILES ${VENDORS_SOURCES})
source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Deps"    FILES ${PROJECT_DEPS})

add_definitions(
  -DGLFW_INCLUDE_NONE
  -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\"
)
add_executable(
  ${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
    ${PROJECT_DEPS} ${PROJECT_CONFIGS} ${VENDORS_SOURCES}
)
target_link_libraries(
  ${PROJECT_NAME} assimp glfw
  ${GLFW_LIBRARIES} ${GLAD_LIBRARIES}
  BulletDynamics BulletCollision LinearMath
)
set_target_properties(
  ${PROJECT_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME}
)
add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${PROJECT_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${PROJECT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
  DEPENDS ${PROJECT_DEPS}
)
