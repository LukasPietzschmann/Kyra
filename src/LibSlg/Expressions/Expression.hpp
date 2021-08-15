#pragma once

#include "Forward.hpp"

namespace LibSlg {

class ExpressionVisitor {
public:
	virtual void visitAssignmentExpr(AssignmentExpr& assignmentExpr) = 0;
	virtual void visitBinaryExpr(BinaryExpr& binaryExpr) = 0;
	virtual void visitCallExpr(CallExpr& callExpr) = 0;
	virtual void visitFunction(Function& functionExpr) = 0;
	virtual void visitLiteral(Literal& literal) = 0;
	virtual void visitObject(Object& objectExpr) = 0;
	virtual void visitUnaryExpr(UnaryExpr& unaryExpr) = 0;
	virtual void visitVariable(Variable& variableExpr) = 0;
};

class Expression {
public:
	typedef std::shared_ptr<Expression> Ptr;
	virtual void accept(ExpressionVisitor& visitor) = 0;
	template <typename T, class... Args>
	static Ptr makePtr(Args... args) {
		static_assert(std::is_constructible<T, Args...>::value, "Cannot construct object in Expression::makePtr");
		return std::make_shared<T>(args...);
	}
};
}