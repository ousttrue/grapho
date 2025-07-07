set(TARGET_NAME camera)
add_executable(${TARGET_NAME} main.cpp)
target_link_libraries(${TARGET_NAME} PRIVATE grapho imgui glfw_platform)
