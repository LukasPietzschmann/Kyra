add_subdirectory(LibSlg)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

find_package(Readline)
include_directories(${Readline_INCLUDE_DIR})

add_executable(Slanguage main.cpp)

target_link_libraries(Slanguage LibSlg)

if (READLINE_FOUND)
	target_link_libraries(Slanguage ${Readline_LIBRARY})
	add_compile_definitions(HAS_READLINE)
endif (READLINE_FOUND)