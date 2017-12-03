#pragma once

#include "binary_operation.h"

struct my_conjunction : binary_operation {
	my_conjunction(std::shared_ptr<operation> _fst, std::shared_ptr<operation> _snd) : binary_operation(_fst, _snd) {}

	std::string get_symbol() {
		return "&";
	}

	int get_id() {
		return 3;
	}

	std::shared_ptr<operation> get_copy(std::vector <std::shared_ptr<operation>>& ops) {
		return std::make_shared<my_conjunction>(ops[0], ops[1]);
	}
};
