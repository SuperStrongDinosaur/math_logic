#pragma once

#include "binary_operation.h"

struct consequence : binary_operation {
	consequence(std::shared_ptr<operation> _fst, std::shared_ptr<operation> _snd) : binary_operation(_fst, _snd) {}

	std::string get_symbol() {
		return "->";
	}

	size_t length() {
		return 2 + fst->length() + snd->length();
	}

	int get_id() {
		return 1;
	}

	std::shared_ptr<operation> get_copy(std::vector <std::shared_ptr<operation>>& ops) {
		return std::make_shared<consequence>(ops[0], ops[1]);
	}
};
