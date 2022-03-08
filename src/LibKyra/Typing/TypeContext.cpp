#include "TypeContext.hpp"

#include <utility>

namespace Kyra {
TypeContext::TypeContext(TypeContext::Ptr parent) :
	Context<TypeContext, Variable<Type::Ptr>, Type::Ptr>(std::move(parent)) {
	auto nothing = Type::make_ptr<NothingType>();
	auto number = std::make_shared<ClassType>("Number");
	auto boolean = std::make_shared<ClassType>("Bool");
	auto string = std::make_shared<ClassType>("String");

	const auto& add_arithmetic_opers = [&number](const std::shared_ptr<ClassType>& type) {
		type->add_declaration("operator+", {Type::make_ptr<FunctionType>(number, std::vector<Type::Ptr>{type}), false});
		type->add_declaration("operator-", {Type::make_ptr<FunctionType>(number, std::vector<Type::Ptr>{type}), false});
		type->add_declaration("operator*", {Type::make_ptr<FunctionType>(number, std::vector<Type::Ptr>{type}), false});
		type->add_declaration("operator/", {Type::make_ptr<FunctionType>(number, std::vector<Type::Ptr>{type}), false});
	};
	const auto& add_equality_opers = [&boolean](const std::shared_ptr<ClassType>& type) {
		type->add_declaration("operator==",
				{Type::make_ptr<FunctionType>(boolean, std::vector<Type::Ptr>{type}), false});
		type->add_declaration("operator!=",
				{Type::make_ptr<FunctionType>(boolean, std::vector<Type::Ptr>{type}), false});
		type->add_declaration("operator<",
				{Type::make_ptr<FunctionType>(boolean, std::vector<Type::Ptr>{type}), false});
		type->add_declaration("operator<=",
				{Type::make_ptr<FunctionType>(boolean, std::vector<Type::Ptr>{type}), false});
		type->add_declaration("operator>",
				{Type::make_ptr<FunctionType>(boolean, std::vector<Type::Ptr>{type}), false});
		type->add_declaration("operator>=",
				{Type::make_ptr<FunctionType>(boolean, std::vector<Type::Ptr>{type}), false});
	};

	add_arithmetic_opers(number);
	add_equality_opers(number);
	add_equality_opers(boolean);
	add_equality_opers(string);
	string->add_declaration("operator+", {Type::make_ptr<FunctionType>(string, std::vector<Type::Ptr>{string}), false});
	string->add_declaration("operator*", {Type::make_ptr<FunctionType>(string, std::vector<Type::Ptr>{number}), false});

	m_types.try_emplace(nothing->get_name(), nothing);
	m_types.try_emplace(number->get_name(), number);
	m_types.try_emplace(boolean->get_name(), boolean);
	m_types.try_emplace(string->get_name(), string);
}

bool TypeContext::declare_var(const std::string& name, Type::Ptr var_type, bool is_mutable) {
	return declare_var(name, Variable(std::move(var_type), is_mutable));
}
}