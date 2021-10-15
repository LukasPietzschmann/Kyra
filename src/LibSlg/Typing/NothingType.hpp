#pragma once

#include "Type.hpp"

namespace Slanguage {
class NothingType : public Type {
public:
	NothingType() : Type("Nothing") {}
	~NothingType() override = default;
	bool isApplicableForDeclaration() const override { return false; }
	bool canBeAssignedTo(Type::WeakPtr) const override { return true; }
};
}