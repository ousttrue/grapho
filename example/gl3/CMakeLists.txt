set(TARGET_NAME gl3example)
add_executable(${TARGET_NAME} main.cpp)
target_link_libraries(${TARGET_NAME} PRIVATE glew_s grapho glfw_platform)
