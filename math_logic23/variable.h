#pragma once

#include "operation.h"

struct variable : operation {
	std::string name;
	variable(const std::string& _name) : name(_name) {}

	std::string print() {
		return name;
	}

	size_t length() {
		return name.size();
	}

	int get_id() {
		return 0;
	}

	std::shared_ptr<operation> get_nxt(size_t what) {
		return nullptr;
	}

	std::shared_ptr<operation> get_copy(std::vector <std::shared_ptr<operation>>& ops) {
		return std::make_shared<variable>(name);
	}

	std::shared_ptr<operation> copy_yourself() {
		return std::make_shared<variable>(name);
	}
};