#pragma once

#include "operation.h"

struct unary_operation : operation {
	std::shared_ptr<operation> nxt;
	unary_operation(std::shared_ptr<operation> _nxt) : nxt(_nxt) {}

	virtual size_t length() {
		return 1 + nxt->length();
	}

	virtual std::shared_ptr<operation> get_nxt(size_t what) {
		if (what == 0)
			return nxt;
		return nullptr;
	}

	virtual std::shared_ptr<operation> copy_yourself() {
		std::vector <std::shared_ptr<operation>> for_copy;
		for_copy.push_back(nxt->copy_yourself());
		return get_copy(for_copy);
	}
};