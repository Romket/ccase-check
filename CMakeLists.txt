cmake_minimum_required(VERSION 3.26)
project(ccase-check VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_definitions(MAJOR_VERSION=0)
add_compile_definitions(MINOR_VERSION=0)
add_compile_definitions(PATCH_VERSION=1)

if(NOT CMAKE_CXX_COMPILER_ID)
    set(CMAKE_CXX_COMPILER_ID "GNU")
    set(CMAKE_CXX_COMPILER "g++")
    message(STATUS "Compiler not specified: defaulting to GNU g++.")
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
    message(STATUS "Build type not specified: defaulting to Debug.")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)

    if(MSVC)
        add_compile_options(/Zi /Od)

    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        add_compile_options(-g -O0 -Wall -Wno-unused)

    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_compile_options(-g -O0 -Weverything)
    endif()
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(RELEASE)

    if(MSVC)
        add_compile_options(/O2 /DNDEBUG)

    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        add_compile_options(-O3 -DNDEBUG -mwindows)

    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_compile_options(-O3 -DNDEBUG -mwindows)
    endif()
endif()

#add_subdirectory(lib/)

add_executable(${PROJECT_NAME}
    src/main.cpp

    src/cases.cpp
    src/parse_args.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

#target_link_libraries(${PROJECT_NAME} template_cpp_engine)
