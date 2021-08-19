cmake_minimum_required(VERSION 3.19)
project(Slanguage)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

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