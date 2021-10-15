#pragma once

#include "Type.hpp"

namespace Slanguage {
class NothingType : public Type {
public:
	NothingType() : Type("Nothing") {}
	~NothingType() override = default;
	bool isApplicableForDeclaration() const override { return false; }
	bool canBeAssignedTo(const Type::Ptr&) const override { return true; }
};
}