#pragma once

#include <vector>

#include "Type.hpp"

namespace Kyra {
class FunctionType : public Type {
public:
	FunctionType(Type::Repr return_type, std::vector<Type::Repr> parameters);
	~FunctionType() override = default;

	Type::Repr get_return_type() const;
	std::vector<Type::Repr> get_parameters() const;
	unsigned long get_arity() const;

	bool operator==(const Type::Ptr& other) const override;
	bool is_function() const override;
	bool can_be_called_with(const std::vector<Type::Repr>& params) const override;
	bool has_return_type(const Repr& repr) const override;

private:
	Type::Repr m_return_type;
	std::vector<Type::Repr> m_parameters;
};
}