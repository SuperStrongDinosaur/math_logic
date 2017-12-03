#pragma once

#include "func_predicate.h"

struct predicate : func_predicate {
	predicate(std::string _name, std::vector <std::shared_ptr<operation>>& that) : func_predicate(_name, that) {}

	int get_id() {
		return 11;
	}

	std::shared_ptr<operation> get_copy(std::vector <std::shared_ptr<operation>>& ops) {
		return std::make_shared<predicate>(name, ops);
	}
};
