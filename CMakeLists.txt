project(Kyra)

cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

add_compile_options(-Wall -Wextra)

option (FORCE_COLORED_OUTPUT "Always produce ANSI-colored output" OFF)
if (${FORCE_COLORED_OUTPUT})
	if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
		add_compile_options(-fdiagnostics-color=always)
	elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
		add_compile_options(-fcolor-diagnostics)
	endif ()
endif ()

option(USE_MOLD_LINKER "Use mold instead of the default linker" OFF)
if (${USE_MOLD_LINKER})
	message(STATUS "Using mold instead of ld")
	if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" AND ${CMAKE_CXX_COMPILER_VERSION} LESS_EQUAL 12.1.0)
		add_link_options(-B/usr/libexec/mold)
	endif ()
	add_link_options(-fuse-ld=mold)
endif ()

include_directories(Utils)
add_subdirectory(Compiler)