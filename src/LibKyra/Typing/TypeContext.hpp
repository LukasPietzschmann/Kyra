#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../Context.hpp"
#include "../Variable.hpp"
#include "ClassType.hpp"
#include "FunctionType.hpp"
#include "NothingType.hpp"
#include "Type.hpp"

namespace Kyra {
class TypeContext : public Context<TypeContext, Variable<Type::Ptr>, Type::Ptr> {
public:
	explicit TypeContext(TypeContext::Ptr parent = nullptr) :
		Context<TypeContext, Variable<Type::Ptr>, Type::Ptr>(std::move(parent)) {
		Type::Ptr nothing = Type::make_ptr<NothingType>();
		std::shared_ptr<ClassType> number = std::make_shared<ClassType>("Number");
		std::shared_ptr<ClassType> boolean = std::make_shared<ClassType>("Bool");
		std::shared_ptr<ClassType> string = std::make_shared<ClassType>("String");

		const auto& add_arithmetic_opers = [&number](const std::shared_ptr<ClassType>& type) {
			type->add_declaration("operator+",
					{Type::make_ptr<FunctionType>(number, std::vector<Type::Ptr>{type}), false});
			type->add_declaration("operator-",
					{Type::make_ptr<FunctionType>(number, std::vector<Type::Ptr>{type}), false});
			type->add_declaration("operator*",
					{Type::make_ptr<FunctionType>(number, std::vector<Type::Ptr>{type}), false});
			type->add_declaration("operator/",
					{Type::make_ptr<FunctionType>(number, std::vector<Type::Ptr>{type}), false});
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
		string->add_declaration("operator+",
				{Type::make_ptr<FunctionType>(string, std::vector<Type::Ptr>{string}), false});
		string->add_declaration("operator*",
				{Type::make_ptr<FunctionType>(string, std::vector<Type::Ptr>{number}), false});

		m_types.try_emplace(nothing->get_name(), nothing);
		m_types.try_emplace(number->get_name(), number);
		m_types.try_emplace(boolean->get_name(), boolean);
		m_types.try_emplace(string->get_name(), string);
	}

	using Context::declare_var;
	bool declare_var(const std::string& name, Type::Ptr var_type, bool is_mutable);
};
}