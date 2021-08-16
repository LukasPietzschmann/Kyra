add_library(LibSlg AstLogger.hpp TokenType.hpp Token.hpp Lexer.cpp Lexer.hpp Parser.cpp Parser.hpp
				Value.hpp
				Expressions/AssignmentExpr.hpp Expressions/CallExpr.hpp Expressions/Expression.hpp
				Expressions/UnaryExpr.hpp Expressions/BinaryExpr.hpp Expressions/Literal.hpp Expressions/Variable.hpp
				Expressions/Object.hpp Expressions/Function.hpp Expressions/AccessExpr.hpp Expressions/Forward.hpp
				Statements/BlockStmt.hpp Statements/DeclarationStmt.hpp Statements/ExpressionStmt.hpp Statements/PrintStmt.hpp
				Statements/PrintStmt.hpp Statements/ReturnStmt.hpp Statements/Statement.hpp Statements/Forward.hpp)
set_target_properties(LibSlg PROPERTIES LINKER_LANGUAGE CXX)