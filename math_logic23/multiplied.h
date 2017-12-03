#pragma once

#include "binary_operation.h"

struct multiplied : binary_operation {
	multiplied(std::shared_ptr<operation> _fst, std::shared_ptr<operation> _snd) : binary_operation(_fst, _snd) {}

	std::string get_symbol() {
		return "*";
	}

	int get_id() {
		return 8;
	}

	std::shared_ptr<operation> get_copy(std::vector <std::shared_ptr<operation>>& ops) {
		return std::make_shared<multiplied>(ops[0], ops[1]);
	}
};