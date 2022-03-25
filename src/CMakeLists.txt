set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

find_package(Readline)
if (READLINE_FOUND)
	include_directories(${Readline_INCLUDE_DIR})
endif ()

include(FetchContent)
FetchContent_Declare(
		cxxopts
		GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
		GIT_TAG c74846a891b3cc3bfa992d588b1295f528d43039 # v3.0.0
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

add_subdirectory(LibKyra)
add_executable(Kyra main.cpp ${BACKWARD_ENABLE})
target_include_directories(Kyra PRIVATE LibKyra/include)
target_link_libraries(Kyra LibKyra cxxopts)

if (READLINE_FOUND)
	target_link_libraries(Kyra ${Readline_LIBRARY})
	add_compile_definitions(HAS_READLINE)
endif (READLINE_FOUND)

if (NORMALIZED_BUILD_TYPE STREQUAL "DEBUG")
	add_backward(Kyra)
endif (NORMALIZED_BUILD_TYPE STREQUAL "DEBUG")

option(IWYU "Adds Include-What-You-Use to the Toolchain" OFF)
if (IWYU)
	find_program(iwyu_path NAMES include-what-you-use iwyu REQUIRED)
	set(iwyu_path_and_options ${iwyu_path} -Xiwyu --check_also="${PROJECT_SOURCE_DIR}/src/LibKyra/Token.hpp")
	set_property(TARGET LibKyra Kyra PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path_and_options})
	set_property(TARGET LibKyra Kyra PROPERTY C_INCLUDE_WHAT_YOU_USE ${iwyu_path_and_options})
endif ()