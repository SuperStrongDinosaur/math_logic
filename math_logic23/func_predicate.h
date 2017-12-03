#pragma once

#include "operation.h"

struct func_predicate : operation {
	std::string name;
	std::vector <std::shared_ptr<operation>> operations;

	func_predicate(std::string& _name, std::vector <std::shared_ptr<operation>>& ops) : name(_name), operations(ops) {}

	std::string print() {
		if (operations.size() == 0)
			return name;
		std::string now = name + "(";
		for (int i = 0; i < (int)operations.size() - 1; i++)
			now += operations[i]->print() + ",";
		if (operations.size() > 0)
			now += operations[operations.size() - 1]->print();
		now += ")";
		return now;
	}

	size_t length() {
		size_t now = name.size() + 2;
		for (size_t i = 0; i < operations.size(); i++)
			now += operations[i]->length();
		return now;
	}

	std::shared_ptr<operation> get_nxt(size_t what) {
		if (what < operations.size())
			return operations[what];
		return nullptr;
	}

	std::shared_ptr<operation> copy_yourself(){
		std::vector <std::shared_ptr<operation>> travel;
		for (auto i : operations)
			travel.push_back(i->copy_yourself());
		return get_copy(travel);
	}
};