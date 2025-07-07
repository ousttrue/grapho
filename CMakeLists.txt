cmake_minimum_required(VERSION 3.10)
project(grapho)

set(CMAKE_CXX_STANDARD 20)

# for compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# for build/.gitignore
if(NOT PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  file(
    GENERATE
    OUTPUT .gitignore
    CONTENT "*")
endif()

subdirs(src example)
