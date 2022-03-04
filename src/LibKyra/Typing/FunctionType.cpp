#include "FunctionType.hpp"

#include <algorithm>
#include <memory>
#include <numeric>
#include <string>
#include <type_traits>
#include <utility>

namespace Kyra {
FunctionType::FunctionType(Type::Repr return_type, std::vector<Type::Repr> parameters) :
	Type(""), m_return_type(std::move(return_type)), m_parameters(std::move(parameters)) {
	std::string params = std::transform_reduce(
			m_parameters.begin(),
			m_parameters.end(),
			std::string(""),
			[](const std::string& a, const std::string& b) { return a + ", " + b; },
			[](const Type::Repr& type) {
				return type;
			}).erase(0, 2);

	m_name = "Function(" + params + ")->" + m_return_type;
}

Type::Repr FunctionType::get_return_type() const { return m_return_type; }

std::vector<Type::Repr> FunctionType::get_parameters() const { return m_parameters; }

unsigned long FunctionType::get_arity() const { return m_parameters.size(); }

bool FunctionType::operator==(const HasPtrAlias::Ptr& other) const {
	const auto& casted_other = std::dynamic_pointer_cast<FunctionType>(other);
	if(casted_other == nullptr)
		return false;
	bool are_params_equal = true;
	if(m_parameters.size() != casted_other->get_parameters().size())
		return false;
	for(unsigned long i = 0; i < m_parameters.size(); ++i) {
		if(m_parameters[i] == casted_other->m_parameters[i])
			continue;
		are_params_equal = false;
		break;
	}

	return m_return_type == casted_other->m_return_type && are_params_equal;
}

bool FunctionType::is_function() const { return true; }

bool FunctionType::can_be_called_with(const std::vector<Type::Repr>& params) const {
	if(params.size() != m_parameters.size())
		return false;
	return std::equal(params.begin(), params.end(), m_parameters.begin());
}

}