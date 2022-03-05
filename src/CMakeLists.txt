add_subdirectory(LibKyra)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

find_package(Readline)
if (READLINE_FOUND)
	include_directories(${Readline_INCLUDE_DIR})
endif ()

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

add_executable(Kyra main.cpp ${PROJECT_SOURCE_DIR}/src/version.h ${BACKWARD_ENABLE})
target_include_directories(Kyra PRIVATE LibKyra/include)

target_link_libraries(Kyra LibKyra cxxopts)

option(IWYU "Adds Include-What-You-Use to the Toolchain" OFF)
if (IWYU)
	find_program(iwyu_path NAMES include-what-you-use iwyu REQUIRED)
	set(iwyu_path_and_options ${iwyu_path} -Xiwyu --check_also="${PROJECT_SOURCE_DIR}/src/LibKyra/Token.hpp")
	set_property(TARGET LibKyra Kyra PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path_and_options})
	set_property(TARGET LibKyra Kyra PROPERTY C_INCLUDE_WHAT_YOU_USE ${iwyu_path_and_options})
endif ()

if (CMAKE_BUILD_TYPE MATCHES DEBUG)
	add_backward(Kyra)
endif (CMAKE_BUILD_TYPE MATCHES DEBUG)

if (READLINE_FOUND)
	target_link_libraries(Kyra ${Readline_LIBRARY})
	add_compile_definitions(HAS_READLINE)
endif (READLINE_FOUND)