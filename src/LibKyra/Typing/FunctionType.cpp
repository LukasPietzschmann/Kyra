#include "FunctionType.hpp"

namespace Kyra {
FunctionType::FunctionType(Type::Repr returnType, std::vector<Type::Repr> parameters) :
	Type(""), m_returnType(std::move(returnType)), m_parameters(std::move(parameters)) {
	std::string params = std::transform_reduce(
			m_parameters.begin(),
			m_parameters.end(),
			std::string(""),
			[](const std::string& a, const std::string& b) { return a + ", " + b; },
			[](const Type::Repr& type) {
				return type;
			}).erase(0, 2);

	m_name = "Function(" + params + ")->" + m_returnType;
}

Type::Repr FunctionType::getReturnType() const { return m_returnType; }

std::vector<Type::Repr> FunctionType::getParameters() const { return m_parameters; }

unsigned long FunctionType::getArity() const { return m_parameters.size(); }

bool FunctionType::operator==(const HasPtrAlias::Ptr& other) const {
	const auto& castedOther = std::dynamic_pointer_cast<FunctionType>(other);
	if(castedOther == nullptr)
		return false;
	bool areParamsEqual = true;
	if(m_parameters.size() != castedOther->getParameters().size())
		return false;
	for(unsigned long i = 0; i < m_parameters.size(); ++i) {
		if(m_parameters[i] == castedOther->m_parameters[i])
			continue;
		areParamsEqual = false;
		break;
	}

	return m_returnType == castedOther->m_returnType && areParamsEqual;
}

bool FunctionType::isFunction() const { return true; }

bool FunctionType::canBeCalledWith(const std::vector<Type::Repr>& params) const {
	if(params.size() != m_parameters.size())
		return false;
	return std::equal(params.begin(), params.end(), m_parameters.begin());
}

bool FunctionType::hasReturnType(const Type::Repr& repr) const { return m_returnType == repr; }
}