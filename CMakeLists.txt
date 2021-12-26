cmake_minimum_required(VERSION 3.19)
project(Kyra VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

option(FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/(Apple)Clang only)." ON)
if (${FORCE_COLORED_OUTPUT})
	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
		add_compile_options(-fdiagnostics-color=always)
	elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
		add_compile_options(-fcolor-diagnostics)
	endif ()
endif ()

configure_file("${PROJECT_SOURCE_DIR}/include/config.h.in" "${PROJECT_SOURCE_DIR}/include/config.h")

if (MSVC)
	add_compile_options(/W4)
else ()
	add_compile_options(-Wall -Wextra -pedantic)
endif ()

add_subdirectory(src)

option(COMPILE_TESTS "Compile the project with tests" OFF)
if (COMPILE_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif ()