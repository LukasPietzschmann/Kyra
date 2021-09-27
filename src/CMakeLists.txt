add_subdirectory(LibSlg)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

find_package(Readline)
include_directories(${Readline_INCLUDE_DIR})

include(FetchContent)
FetchContent_Declare(
		cxxopts
		GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
		GIT_TAG 302302b30839505703d37fb82f536c53cf9172fa # v2.2.1
)
FetchContent_MakeAvailable(cxxopts)

string(TOUPPER "${CMAKE_BUILD_TYPE}" NORMALIZED_BUILD_TYPE)
if (NORMALIZED_BUILD_TYPE STREQUAL "DEBUG")
	FetchContent_Declare(
			backward-cpp
			GIT_REPOSITORY https://github.com/bombela/backward-cpp.git
			GIT_TAG 3bb9240cb15459768adb3e7d963a20e1523a6294 # v1.6
	)
	FetchContent_MakeAvailable(backward-cpp)
endif (NORMALIZED_BUILD_TYPE STREQUAL "DEBUG")

add_executable(Slanguage main.cpp ${PROJECT_SOURCE_DIR}/include/config.h ${BACKWARD_ENABLE})

target_include_directories(Slanguage PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(Slanguage LibSlg cxxopts)

if (CMAKE_BUILD_TYPE MATCHES DEBUG)
	add_backward(Slanguage)
endif (CMAKE_BUILD_TYPE MATCHES DEBUG)

if (READLINE_FOUND)
	target_link_libraries(Slanguage ${Readline_LIBRARY})
	add_compile_definitions(HAS_READLINE)
endif (READLINE_FOUND)