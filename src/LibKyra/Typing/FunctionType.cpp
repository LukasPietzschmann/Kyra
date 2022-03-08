#include "FunctionType.hpp"

#include <algorithm>
#include <memory>
#include <numeric>
#include <string>
#include <type_traits>
#include <utility>

namespace Kyra {
FunctionType::FunctionType(Type::Ptr return_type, std::vector<Type::Ptr>&& parameters) :
	Type(""), m_return_type(std::move(return_type)), m_parameters(std::move(parameters)) {
	std::string params = std::transform_reduce(
			m_parameters.begin(),
			m_parameters.end(),
			std::string(""),
			[](const std::string& a, const std::string& b) { return a + ", " + b; },
			[](const Type::Ptr& type) {
				return type->get_name();
			}).erase(0, 2);

	m_name = "Function(" + params + ")->" + m_return_type->get_name();
}

Type::Ptr FunctionType::get_return_type() const { return m_return_type; }

const std::vector<Type::Ptr>& FunctionType::get_parameters() const { return m_parameters; }

unsigned long FunctionType::get_arity() const { return m_parameters.size(); }

bool FunctionType::operator==(const Type::Ptr& other) const {
	const auto& casted_other = std::dynamic_pointer_cast<FunctionType>(other);
	if(casted_other == nullptr)
		return false;
	bool are_params_equal = true;
	if(m_parameters.size() != casted_other->get_parameters().size())
		return false;
	for(unsigned long i = 0; i < m_parameters.size(); ++i) {
		if(*m_parameters[i] == casted_other->m_parameters[i])
			continue;
		are_params_equal = false;
		break;
	}

	return *m_return_type == casted_other->m_return_type && are_params_equal;
}

bool FunctionType::is_function() const { return true; }

bool FunctionType::can_be_called_with(const std::vector<Type::Ptr>& params) const {
	if(params.size() != m_parameters.size())
		return false;
	for(size_t i = 0; i < params.size(); ++i) {
		if(*params[i] != m_parameters[i])
			return false;
	}
	return true;
	// return std::equal(params.begin(), params.end(), m_parameters.begin());
}

}