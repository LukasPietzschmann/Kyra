find_package(LLVM 14 REQUIRED CONFIG)
message(STATUS "Using LLVM Version ${LLVM_PACKAGE_VERSION} from ${LLVM_DIR}")
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})
llvm_map_components_to_libnames(llvm_libs support core irreader native)

add_executable(kyra main.cpp Token.cpp Lexer.cpp Parser.cpp SourceRange.cpp AST.cpp TypeChecker.cpp ASTPrinter.cpp Error.cpp CodeGen.cpp Type.cpp)
target_include_directories(kyra PUBLIC ${LLVM_INCLUDE_DIRS})
target_link_libraries(kyra PUBLIC ${llvm_libs})
