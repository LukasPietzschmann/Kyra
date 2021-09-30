add_library(LibSlg AstLogger.hpp Context.hpp Context.cpp Exceptions.hpp
		TokenType.hpp Token.hpp Lexer.cpp Lexer.hpp Parser.cpp Parser.hpp Interpreter.hpp
		Interpreter.cpp Position.hpp
		Expressions/AssignmentExpr.hpp Expressions/CallExpr.hpp Expressions/Expression.hpp Expressions/UnaryExpr.hpp
		Expressions/BinaryExpr.hpp Expressions/LiteralExpr.hpp Expressions/VariableExpr.hpp Expressions/FunctionExpr.hpp
		Expressions/AccessExpr.hpp Expressions/GroupExpr.hpp Expressions/InstantiationExpr.hpp Expressions/TypeExpr.hpp
		Expressions/Forward.hpp
		Statements/BlockStmt.hpp Statements/DeclarationStmt.hpp Statements/ClassDeclarationStmt.hpp
		Statements/ExpressionStmt.hpp Statements/PrintStmt.hpp Statements/PrintStmt.hpp Statements/ReturnStmt.hpp
		Statements/Statement.hpp Statements/Forward.hpp
		Typing/TypeChecker.hpp Typing/TypeChecker.cpp Typing/Type.hpp Typing/ClassType.hpp Typing/FunctionType.hpp Typing/NativeTypes.hpp
		Typing/TypingErrors.hpp
		Values/Value.hpp Values/Value.cpp Values/Bool.hpp Values/Function.hpp Values/Function.cpp Values/Number.hpp
		Values/Nothing.hpp Values/String.hpp Values/Klass.hpp Values/Klass.cpp Values/Forward.hpp)
set_target_properties(LibSlg PROPERTIES LINKER_LANGUAGE CXX)