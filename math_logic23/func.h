#pragma once

#include "func_predicate.h"

struct func : func_predicate {
	func(std::string _name, std::vector <std::shared_ptr<operation>>& that) : func_predicate(_name, that) {}

	int get_id() {
		return 12;
	}

	std::shared_ptr<operation> get_copy(std::vector <std::shared_ptr<operation>>& ops) {
		return std::make_shared<func>(name, ops);
	}
};