#pragma once

#include "binary_operation.h"

struct summand : binary_operation {
	summand(std::shared_ptr<operation> _fst, std::shared_ptr<operation> _snd) : binary_operation(_fst, _snd) {}

	std::string get_symbol() {
		return "+";
	}

	int get_id() {
		return 7;
	}

	std::shared_ptr<operation> get_copy(std::vector <std::shared_ptr<operation>>& ops) {
		return std::make_shared<summand>(ops[0], ops[1]);
	}
};
