cmake_minimum_required(VERSION 3.21)

project(tedit LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

file(GLOB_RECURSE SOURCE_FILES
    CONFIGURE_DEPENDS
    ${SRC_DIR}/*.cpp
)

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

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_precompile_headers(${PROJECT_NAME}
    PRIVATE
    ${SRC_DIR}/pch.h
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${INCLUDE_DIRS}
    ${CURSES_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    ${CURSES_LIBRARIES}
)

if(UNIX AND NOT APPLE)
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()
