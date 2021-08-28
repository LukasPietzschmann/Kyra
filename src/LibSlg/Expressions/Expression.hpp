#pragma once

#include "Forward.hpp"
#include "../Values/Value.hpp"

namespace LibSlg {

class ExpressionVisitor {
public:
	virtual Value::Ptr visitAccessExpr(AccessExpr& accessExpr) = 0;
	virtual Value::Ptr visitAssignmentExpr(AssignmentExpr& assignmentExpr) = 0;
	virtual Value::Ptr visitBinaryExpr(BinaryExpr& binaryExpr) = 0;
	virtual Value::Ptr visitCallExpr(CallExpr& callExpr) = 0;
	virtual Value::Ptr visitFunction(FunctionExpr& functionExpr) = 0;
	virtual Value::Ptr visitGroupExpr(GroupExpr& groupExpr) = 0;
	virtual Value::Ptr visitInstantiationExpr(InstantiationExpr& instantiationExpr) = 0;
	virtual Value::Ptr visitLiteral(LiteralExpr& literalExpr) = 0;
	virtual Value::Ptr visitUnaryExpr(UnaryExpr& unaryExpr) = 0;
	virtual Value::Ptr visitVariable(VariableExpr& variableExpr) = 0;
};

class Expression {
public:
	typedef std::shared_ptr<Expression> Ptr;
	virtual Value::Ptr accept(ExpressionVisitor& visitor) = 0;
	template <typename T, class... Args>
	static Ptr makePtr(Args... args) {
		static_assert(std::is_constructible<T, Args...>::value, "Cannot construct object in Expression::makePtr");
		return std::make_shared<T>(args...);
	}
protected:
	virtual ~Expression() {};
};
}