#pragma once

#include <vector>

#include "Type.hpp"

namespace Kyra {
class FunctionType : public Type {
public:
	FunctionType(Type::Ptr return_type, std::vector<Type::Ptr>&& parameters);
	~FunctionType() override = default;

	Type::Ptr get_return_type() const;
	const std::vector<Type::Ptr>& get_parameters() const;
	unsigned long get_arity() const;

	bool operator==(const Type::Ptr& other) const override;
	bool is_function() const override;
	bool can_be_called_with(const std::vector<Type::Ptr>& params) const override;

private:
	Type::Ptr m_return_type;
	std::vector<Type::Ptr> m_parameters;
};
}