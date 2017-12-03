#pragma once

#include "operation.h"
#include <vector>

struct binary_operation : operation {
	virtual std::string get_symbol() = 0;
	std::shared_ptr<operation> fst;
	std::shared_ptr<operation> snd;
	binary_operation(std::shared_ptr<operation> _fst, std::shared_ptr<operation> _snd) : fst(_fst), snd(_snd) {}

	virtual std::string print() {
		return "(" + fst->print() + get_symbol() + snd->print() + ")";
	}

	virtual size_t length() {
		return 1 + fst->length() + snd->length();
	}

	virtual std::shared_ptr<operation> copy_yourself() {
		std::vector <std::shared_ptr<operation>> for_copy;
		for_copy.push_back(fst->copy_yourself());
		for_copy.push_back(snd->copy_yourself());
		return get_copy(for_copy);
	}

	virtual ~binary_operation() {}

	virtual std::shared_ptr<operation> get_nxt(size_t what) {
		if (what == 0)
			return fst;
		if (what == 1)
			return snd;
		return nullptr;
	}
};