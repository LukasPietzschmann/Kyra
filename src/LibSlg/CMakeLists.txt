add_library(LibSlg AstLogger.hpp AstJsonPrinter.hpp AstJsonPrinter.cpp Context.hpp Context.cpp Exceptions.hpp
				TokenType.hpp Token.hpp Lexer.cpp Lexer.hpp Parser.cpp Parser.hpp
				Value.hpp Interpreter.hpp Interpreter.cpp
				Expressions/AssignmentExpr.hpp Expressions/CallExpr.hpp Expressions/Expression.hpp
				Expressions/UnaryExpr.hpp Expressions/BinaryExpr.hpp Expressions/LiteralExpr.hpp Expressions/VariableExpr.hpp
		Expressions/ObjectExpr.hpp Expressions/FunctionExpr.hpp Expressions/AccessExpr.hpp Expressions/GroupExpr.hpp
				Expressions/Forward.hpp
				Statements/BlockStmt.hpp Statements/DeclarationStmt.hpp Statements/ExpressionStmt.hpp Statements/PrintStmt.hpp
				Statements/PrintStmt.hpp Statements/ReturnStmt.hpp Statements/Statement.hpp Statements/Forward.hpp)
set_target_properties(LibSlg PROPERTIES LINKER_LANGUAGE CXX)