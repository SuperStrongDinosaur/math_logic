#pragma once

#include "unary_operation.h"
#include "variable.h"

struct every : unary_operation {
	std::shared_ptr<variable> var;

	every(std::shared_ptr<variable> _var, std::shared_ptr<operation> _nxt) : unary_operation(_nxt), var(_var) {}

	std::string print() {
		return "@" + var->print() + nxt->print();
	}

	int get_id() {
		return 6;
	}

	std::shared_ptr<operation> get_nxt(size_t what) {
		if (what == 0)
			return var;
		if (what == 1)
			return nxt;
		return nullptr;
	}

	std::shared_ptr<operation> get_copy(std::vector <std::shared_ptr<operation>>& ops) {
		if (ops.size() == 2)
			return std::make_shared<every>(std::make_shared<variable>(ops[0]->print()), ops[1]);
		return std::make_shared<every>(var, ops[0]);
	}
};