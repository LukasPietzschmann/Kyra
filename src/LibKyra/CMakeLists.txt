add_library(LibKyra AstLogger.cpp Exceptions.cpp Token.cpp Lexer.cpp Parser.cpp Interpreter.cpp Position.cpp
		Expressions/AssignmentExpr.cpp Expressions/CallExpr.cpp Expressions/UnaryExpr.cpp Expressions/BinaryExpr.cpp
		Expressions/LiteralExpr.cpp Expressions/VariableExpr.cpp Expressions/FunctionExpr.cpp Expressions/AccessExpr.cpp
		Expressions/GroupExpr.cpp Expressions/InstantiationExpr.cpp Expressions/TypeExpr.cpp
		Statements/BlockStmt.cpp Statements/DeclarationStmt.cpp Statements/ClassDeclarationStmt.cpp
		Statements/ExpressionStmt.cpp Statements/PrintStmt.cpp Statements/ReturnStmt.cpp Statements/WhileStmt.cpp
		Statements/IfStmt.cpp
		Typing/TypeChecker.cpp Typing/Type.cpp Typing/ClassType.cpp Typing/FunctionType.cpp Typing/NothingType.cpp
		Typing/TypeProvider.cpp Typing/TypingErrors.cpp Typing/TypeContext.cpp
		Values/Value.cpp Values/Bool.cpp Values/Function.cpp Values/Number.cpp Values/Nothing.cpp Values/String.cpp
		Values/Klass.cpp
		Runtime/Runtime.cpp Runtime/RuntimeContext.cpp)
set_target_properties(LibKyra PROPERTIES LINKER_LANGUAGE CXX)