#pragma once

#include <numeric>
#include <utility>

#include "Type.hpp"

namespace Kyra {
class FunctionType : public Type {
public:
	FunctionType(Type::Repr returnType, std::vector<Type::Repr> parameters);
	~FunctionType() override = default;

	Type::Repr getReturnType() const;
	std::vector<Type::Repr> getParameters() const;
	unsigned long getArity() const;

	bool operator==(const Type::Ptr& other) const override;
	bool isFunction() const override;
	bool canBeCalledWith(const std::vector<Type::Repr>& params) const override;
	bool hasReturnType(const Repr& repr) const override;

private:
	Type::Repr m_returnType;
	std::vector<Type::Repr> m_parameters;
};
}