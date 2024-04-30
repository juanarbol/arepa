cmake_minimum_required(VERSION 3.10)
project(arepa C)

# Compiler options
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-Wall")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Output directory
set(OUT_DIR "${CMAKE_BINARY_DIR}/build")

# Source files
set(SRC src/lexer.c src/parser.c src/token.c)

# Include directory
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

option(BUILD_TESTS "Build the unit tests" ON)

if (BUILD_TESTS)
  # Link test executable to the library
  add_executable(test test/runner.c)
  target_link_libraries(test arepa)
endif()

# Add executable target
add_library(arepa STATIC ${SRC})
target_include_directories(arepa PUBLIC ${INCLUDE_DIR})

# Print debug information
message(STATUS "C compiler: ${CMAKE_C_COMPILER}")
message(STATUS "Output directory: ${OUT_DIR}")
message(STATUS "Executable target: ${TARGET}")
