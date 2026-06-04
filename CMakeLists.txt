cmake_minimum_required(VERSION 3.21)

project(tedit LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Release)

set(CURSES_NEED_NCURSES TRUE)

include(FetchContent)

find_package(Curses REQUIRED)

# Tree-sitter

add_subdirectory(vendor/tree_sitter_cpp EXCLUDE_FROM_ALL)

target_include_directories(tree-sitter-cpp
    PUBLIC
    ${CMAKE_SOURCE_DIR}/vendor/tree_sitter_cpp/bindings/c
)

# Catch2

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.8.1
)

FetchContent_MakeAvailable(Catch2)

enable_testing()

# Source collection

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

file(GLOB_RECURSE SOURCE_FILES
    CONFIGURE_DEPENDS
    ${SRC_DIR}/*.cpp
)

# Remove Main.cpp from library sources
list(FILTER SOURCE_FILES EXCLUDE REGEX ".*Main\\.cpp$")

file(GLOB_RECURSE ALL_PATHS
    LIST_DIRECTORIES true
    CONFIGURE_DEPENDS
    ${SRC_DIR}/*
)

set(INCLUDE_DIRS ${SRC_DIR})

foreach(path ${ALL_PATHS})
    if(IS_DIRECTORY ${path})
        list(APPEND INCLUDE_DIRS ${path})
    endif()
endforeach()

list(REMOVE_DUPLICATES INCLUDE_DIRS)

# Core library

add_library(tedit_core ${SOURCE_FILES})

target_precompile_headers(tedit_core
    PRIVATE
    ${SRC_DIR}/pch.hpp
)

target_include_directories(tedit_core
    PUBLIC
    ${INCLUDE_DIRS}
    ${CURSES_INCLUDE_DIRS}
)

target_link_libraries(tedit_core
    PUBLIC
    ${CURSES_LIBRARIES}
    tree-sitter
    tree-sitter-cpp
)

# Main executable

add_executable(tedit
    ${SRC_DIR}/Main.cpp
)

target_precompile_headers(tedit
    PRIVATE
    ${SRC_DIR}/pch.hpp
)

target_link_libraries(tedit
    PRIVATE
    tedit_core
)

# Tests

file(GLOB_RECURSE TEST_SOURCE_FILES
    CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/tests/*.cpp
)

add_executable(tedit_tests
    ${TEST_SOURCE_FILES}
)

target_precompile_headers(tedit_tests
    PRIVATE
    ${SRC_DIR}/pch.hpp
)

target_link_libraries(tedit_tests
    PRIVATE
    tedit_core
    Catch2::Catch2WithMain
)

include(CTest)
include(Catch)

add_test(NAME tedit_tests
    COMMAND ${CMAKE_COMMAND} -E env LSAN_OPTIONS=detect_leaks=0 $<TARGET_FILE:tedit_tests>
)

# Warnings + Sanitizers

if(UNIX AND NOT APPLE)

    target_compile_options(tedit_core PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -fsanitize=address
        -fsanitize=undefined
    )

    target_link_options(tedit_core PUBLIC
        -fsanitize=address
        -fsanitize=undefined
    )

endif()
