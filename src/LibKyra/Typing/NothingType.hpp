#pragma once

#include "Type.hpp"

namespace Kyra {
class NothingType : public Type {
public:
	NothingType();
	~NothingType() override = default;
	bool is_applicable_for_declaration() const override;
	bool can_be_assigned_to(Type::Ptr) const override;
};
}