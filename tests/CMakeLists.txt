include(FetchContent)
FetchContent_Declare(
		googletest
		GIT_REPOSITORY https://github.com/google/googletest.git
		GIT_TAG e2239ee6043f73722e7aa812a459f54a28552929 #release-1.11.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(Tests exampleTest.cpp)
target_link_libraries(Tests gtest_main LibSlg)

include(googletest)
gtest_discover_tests(Tests)

option(RUN_TESTS_AFTER_BUILD "Run all tests automatically after a successful build" ON)
if (RUN_TESTS_AFTER_BUILD)
	add_custom_command(TARGET Tests COMMAND ../bin/Tests POST_BUILD)
endif ()