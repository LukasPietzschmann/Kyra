#pragma once

#include <numeric>
#include <utility>

#include "Type.hpp"

namespace LibSlg {
class FunctionType : public Type {
public:
	FunctionType(Type::Ptr returnType, std::vector<Type::Ptr> parameters) :
		Type(""), m_returnType(std::move(returnType)), m_parameters(std::move(parameters)) {
		std::string params = std::transform_reduce(
				m_parameters.begin(), m_parameters.end(), std::string(""),
				[](const std::string& a, const std::string& b) { return a + ", " + b; },
				[](const Type::Ptr& type) {
					return type->getName();
				}).erase(0, 2);

		m_name = "Function(" + params + ")->" + m_returnType->getName();
	}
	~FunctionType() override = default;
	const Type::Ptr& getReturnType() const { return m_returnType; }
	std::vector<Type::Ptr> getParameters() const { return m_parameters; }
	unsigned long getArity() const { return m_parameters.size(); }
	Type::Ptr duplicate() const override { return Type::makePtr<FunctionType>(*this); }

	bool operator==(const Type::Ptr& other) const override {
		const auto& castedOther = std::dynamic_pointer_cast<FunctionType>(other);
		if(castedOther == nullptr)
			return false;
		bool areParamsEqual = true;
		if(m_parameters.size() != castedOther->getParameters().size())
			areParamsEqual = false;
		for(unsigned long i = 0; i < m_parameters.size(); ++i) {
			if(*m_parameters[i] == castedOther->m_parameters[i])
				continue;
			areParamsEqual = false;
			break;
		}

		return *m_returnType == castedOther->m_returnType && areParamsEqual;
	}

protected:
	Type::Ptr m_returnType;
	std::vector<Type::Ptr> m_parameters;
};
}