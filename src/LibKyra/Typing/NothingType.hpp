#pragma once

#include "Type.hpp"

namespace Kyra {
class NothingType : public Type {
public:
	NothingType();
	~NothingType() override = default;
	bool isApplicableForDeclaration() const override;
	bool canBeAssignedTo(Type::Ptr) const override;
};
}