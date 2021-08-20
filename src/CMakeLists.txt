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

add_executable(Slanguage main.cpp ${PROJECT_SOURCE_DIR}/include/config.h)

target_include_directories(Slanguage PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(Slanguage LibSlg cxxopts)

if (READLINE_FOUND)
	target_link_libraries(Slanguage ${Readline_LIBRARY})
	add_compile_definitions(HAS_READLINE)
endif (READLINE_FOUND)