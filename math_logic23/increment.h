#pragma once

#include "unary_operation.h"

struct increment : unary_operation {
	increment(std::shared_ptr<operation> _nxt) : unary_operation(_nxt) {}

	std::string print() {
		return nxt->print() + "'";
	}

	int get_id() {
		return 11;
	}

	std::shared_ptr<operation> get_copy(std::vector <std::shared_ptr<operation>>& ops) {
		return std::make_shared<increment>(ops[0]);
	}
};
