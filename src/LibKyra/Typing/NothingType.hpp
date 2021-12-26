#pragma once

#include "Type.hpp"

namespace Kyra {
class NothingType : public Type {
public:
	NothingType() : Type("Nothing") {}
	~NothingType() override = default;
	bool isApplicableForDeclaration() const override { return false; }
	bool canBeAssignedTo(Type::Ptr) const override { return true; }
};
}