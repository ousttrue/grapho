set(TARGET_NAME pbr)
add_executable(${TARGET_NAME} main.cpp imageloader.cpp drawable.cpp)
target_link_libraries(${TARGET_NAME} PRIVATE glew_s grapho glfw_platform imgui stb)
