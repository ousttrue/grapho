set(TARGET_NAME glfw_platform)
add_library(${TARGET_NAME} STATIC glfw_platform.cpp)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(${TARGET_NAME} PUBLIC glfw)
