#pragma once

#include <utility>

#include "Type.hpp"

namespace LibSlg {
class FunctionType : public Type {
public:
	FunctionType(bool isMutable, Ptr returnType, std::vector<Type::Ptr> parameters) :
		Type("Function", isMutable), m_returnType(std::move(returnType)), m_parameters(std::move(parameters)) {}
	~FunctionType() override = default;
	const Type::Ptr& getReturnType() const { return m_returnType; }
	std::vector<Type::Ptr> getParameters() const { return m_parameters; }
	Type::Ptr duplicate() const override { return Type::makePtr<FunctionType>(*this); }

protected:
	Type::Ptr m_returnType;
	std::vector<Type::Ptr> m_parameters;
};
}