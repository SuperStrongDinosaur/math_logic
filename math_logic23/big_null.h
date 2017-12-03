#pragma once

#include "operation.h"

struct big_null : operation {
	std::string print() {
		return "0";
	}

	size_t length(){	
		return 1;
	}

	int get_id() {
		return 10;
	}

	std::shared_ptr<operation> get_nxt(size_t what) {
		return nullptr;
	}

	std::shared_ptr<operation> get_copy(std::vector <std::shared_ptr<operation>>& ops) {
		return std::make_shared<big_null>();
	}

	std::shared_ptr<operation> copy_yourself() {
		return std::make_shared<big_null>();
	}
};