add_subdirectory(LibSlg)

add_executable(Slanguage main.cpp)

target_link_libraries(Slanguage LibSlg)