#pragma once

#include "variable.h"

struct substitution : variable {
	substitution(const std::string& _name) : variable(_name) {}

	std::shared_ptr<operation> get_copy(std::vector <std::shared_ptr<operation>>& ops) {
		return std::make_shared<substitution>(name);
	}

	std::shared_ptr<operation> copy_yourself() {
		return std::make_shared<substitution>(name);
	}

	int get_id() {
		return 13;
	}
};
