include(FetchContent)

FetchContent_Populate(
  glew
  URL https://sourceforge.net/projects/glew/files/glew/2.1.0/glew-2.1.0.tgz/download
)
add_library(glew_s STATIC ${glew_SOURCE_DIR}/src/glew.c
                          ${glew_SOURCE_DIR}/src/glewinfo.c)
target_include_directories(glew_s PUBLIC ${glew_SOURCE_DIR}/include)
target_compile_definitions(
  glew_s
  PRIVATE GLEW_BUILD _CRT_SECURE_NO_WARNINGS
  PUBLIC GLEW_STATIC)
target_link_libraries(glew_s PUBLIC OpenGL32)

set(BUILD_SHARED_LIBS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.3.8)
FetchContent_MakeAvailable(glfw)

FetchContent_Populate(
  imgui
  # URL https://github.com/ocornut/imgui/archive/refs/tags/v1.91.9b.zip
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.91.0-docking)
set(TARGET_NAME imgui)
add_library(
  ${TARGET_NAME} STATIC
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/imgui_demo.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)
target_include_directories(${TARGET_NAME} PUBLIC ${imgui_SOURCE_DIR}
                                                 ${imgui_SOURCE_DIR}/backends)
target_link_libraries(${TARGET_NAME} PUBLIC glfw)
target_compile_options(${TARGET_NAME} PUBLIC -Wno-nontrivial-memcall)

FetchContent_Populate(
  stb URL https://github.com/nothings/stb/archive/refs/heads/master.zip)
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${stb_SOURCE_DIR})

subdirs(glfw_platform gl3 pbr
        # normalmap
        camera)
