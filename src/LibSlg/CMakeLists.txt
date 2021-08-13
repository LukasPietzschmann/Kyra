add_library(LibSlg TokenType.hpp Token.hpp Lexer.cpp Lexer.hpp Parser.cpp Parser.hpp
				Expressions/AssignmentExpr.hpp Expressions/CallExpr.hpp Expressions/ComparisonExpr.hpp
				Expressions/EqualityExpr.hpp Expressions/Expression.hpp Expressions/FactorExpr.hpp Expressions/PrimaryExpr.hpp
				Expressions/TermExpr.hpp Expressions/TermExpr.hpp Expressions/UnaryExpr.hpp Expressions/BinaryExpr.hpp
				Expressions/Literal.hpp Expressions/Variable.hpp Expressions/Object.hpp Expressions/Function.hpp
				Statements/BlockStmt.hpp Statements/DeclarationStmt.hpp Statements/ExpressionStmt.hpp Statements/PrintStmt.hpp
				Statements/PrintStmt.hpp Statements/ReturnStmt.hpp Statements/Statement.hpp Statements/Forward.hpp)
set_target_properties(LibSlg PROPERTIES LINKER_LANGUAGE CXX)