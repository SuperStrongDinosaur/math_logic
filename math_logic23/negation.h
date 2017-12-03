#pragma once

#include "unary_operation.h"

struct my_negation : unary_operation {
	my_negation(std::shared_ptr<operation> _nxt) : unary_operation(_nxt) {}

	std::string print() {
		return "!" + nxt->print();
	}

	int get_id() {
		return 4;
	}

	std::shared_ptr<operation> get_copy(std::vector <std::shared_ptr<operation>>& ops) {
		return std::make_shared<my_negation>(ops[0]);
	}
};