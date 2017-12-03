#pragma once

#include "parse.h"

using namespace std;

vector <std::shared_ptr<operation>> res_quantifier;

void dfs3(std::shared_ptr<operation> op, unsigned long long that, vector <std::shared_ptr<operation>>& in_quantifier, size_t quantifier_pointer) {
	if (op->hash() == that)
	{
		for (; quantifier_pointer < in_quantifier.size(); quantifier_pointer++)
		{
			res_quantifier.push_back(in_quantifier[quantifier_pointer]);
		}
	}
	else if (op->get_id() == 5 || op->get_id() == 6)
	{
		if (op->get_nxt(0)->hash() != that)
		{
			in_quantifier.push_back(op->get_nxt(0));
			dfs3(op->get_nxt(1), that, in_quantifier, quantifier_pointer);
			if (quantifier_pointer == in_quantifier.size())
			{
				quantifier_pointer--;
			}
			in_quantifier.pop_back();
		}
	}
	else
	{
		for (size_t i = 0; op->get_nxt(i) != nullptr; i++)
		{
			dfs3(op->get_nxt(i), that, in_quantifier, quantifier_pointer);
		}
	}
}

void take_quantifier_vars(std::shared_ptr<operation> op, unsigned long long that) {
	res_quantifier.clear();
	size_t quantifier_pointer = 0;
	vector <std::shared_ptr<operation>> in_quantifier;
	dfs3(op, that, in_quantifier, quantifier_pointer);
}

bool dfs5(std::shared_ptr<operation> op, set<unsigned long long> &for_dfs5) {
	if (op->get_id() == VARIABLE) {
		if (for_dfs5.find(op->hash()) != for_dfs5.end())
			return false;
	}
	else {
		for (size_t i = 0; op->get_nxt(i) != nullptr; i++) {
			bool res = dfs5(op->get_nxt(i), for_dfs5);
			if (!res)
				return false;
		}
	}
	return true;
}

bool check_on_free(std::shared_ptr<operation> op, vector <std::shared_ptr<operation>>& res) {
	if (res.size() == 0)
		return true;
	set<unsigned long long> for_dfs5;
	for (size_t i = 0; i < res.size(); i++)
		for_dfs5.insert(res[i]->hash());
	return dfs5(op, for_dfs5);
}

std::shared_ptr<operation> substitute(std::shared_ptr<operation>, vector <pair <std::shared_ptr<operation>, std::shared_ptr<operation>> >&);

void dfs3(std::shared_ptr<operation> op, unsigned long long that, vector <std::shared_ptr<operation>> &in_quantifier, vector <std::shared_ptr<operation>>  &res_quantifier, size_t &quantifier_pointer) {
	if (op->hash() == that)
		for (; quantifier_pointer < in_quantifier.size(); quantifier_pointer++)
			res_quantifier.push_back(in_quantifier[quantifier_pointer]);
	else if (op->get_id() == EXISTS || op->get_id() == EVERYS) {
		in_quantifier.push_back(op->get_nxt(0));
		dfs3(op->get_nxt(1), that, in_quantifier, res_quantifier, quantifier_pointer);
		if (quantifier_pointer == in_quantifier.size())
			quantifier_pointer--;
		in_quantifier.pop_back();
	}
	else
		for (size_t i = 0; op->get_nxt(i) != nullptr; i++)
			dfs3(op->get_nxt(i), that, in_quantifier, res_quantifier, quantifier_pointer);
}

bool dfs(std::shared_ptr<operation> for_check, std::shared_ptr<operation> axiom, vector <unsigned long long> &for_hash, vector <unsigned long long> &for_hash_axiom) {
	if (axiom->get_id() == VARIABLE || axiom->get_id() == SUBSTITUTION) {
		for_hash.push_back(for_check->hash());
		for_hash_axiom.push_back(axiom->hash());
	}
	else if (axiom->get_id() != for_check->get_id())
		return false;
	else {
		for (size_t i = 0; axiom->get_nxt(i) != nullptr && for_check->get_nxt(i) != nullptr; i++) {
			bool res = dfs(for_check->get_nxt(i), axiom->get_nxt(i), for_hash, for_hash_axiom);
			if (res == false)
				return false;
		}
	}
	return true;
}

bool check_on_coincidence(vector <unsigned long long> &for_hash, vector <unsigned long long> &for_hash_axiom) {
	map <unsigned long long, unsigned long long> mp_hash;
	for (size_t i = 0; i < for_hash_axiom.size(); i++) {
		if (mp_hash.find(for_hash_axiom[i]) == mp_hash.end())
			mp_hash[for_hash_axiom[i]] = for_hash[i];
		else
			if (mp_hash[for_hash_axiom[i]] != for_hash[i])
				return false;
	}
	return true;
}

bool check_on_axiom_11_12(std::shared_ptr<operation> for_check, bool which, vector <unsigned long long> &for_hash, vector <unsigned long long> &for_hash_axiom) {
	if (for_check->get_id() != 1)
		return false;
	else {
		std::shared_ptr<operation> left = for_check->get_nxt(0);
		std::shared_ptr<operation> right = for_check->get_nxt(1);
		int id = 6;
		if (which == 1)	{
			swap(left, right);
			id = 5;
		}
		if (left->get_id() != id)
			return false;
		std::shared_ptr<operation> var = left->get_nxt(0);
		std::shared_ptr<operation> that = left->get_nxt(1);
		for_hash.clear();
		for_hash_axiom.clear();
		bool res = dfs(right, that, for_hash, for_hash_axiom);
		if (!res)
			return false;
		std::shared_ptr<operation> smth = get_something(that, right, var->hash());
		take_quantifier_vars(that, var->hash());
		vector<pair<std::shared_ptr<operation>, std::shared_ptr<operation>> > sub;
		sub.push_back(make_pair(var, smth));
		shared_ptr<operation> check(substitute(that, sub));
		if (check->hash() != right->hash())
			return false;
		if (!check_on_free(smth, res_quantifier))	{
			global_error = NOT_FREE_TO_SUBSTITUTE	;
			return false;
		}
	}
	return true;
}

bool check_on_axiom_a9(std::shared_ptr<operation> op) {
	if (op->get_id() != 1)
		return false;
	std::shared_ptr<operation> left = op->get_nxt(0);
	std::shared_ptr<operation> right = op->get_nxt(1);
	if (left->get_id() != 3)
		return false;
	std::shared_ptr<operation> left_left = left->get_nxt(0);
	std::shared_ptr<operation> left_right = left->get_nxt(1);
	if (left_right->get_id() != 6)
		return false;
	std::shared_ptr<operation> var = left_right->get_nxt(0);
	std::shared_ptr<operation> tmp = left_right->get_nxt(1);
	if (tmp->get_id() != 1)
		return false;
	std::shared_ptr<operation> left1 = tmp->get_nxt(0);
	std::shared_ptr<operation> left2 = tmp->get_nxt(1);
	if (left1->hash() != right->hash())
		return false;
	vector <pair <std::shared_ptr<operation>, std::shared_ptr<operation>> > sub;
	shared_ptr<operation> null(std::make_shared<big_null>());
	shared_ptr<operation> inc(std::make_shared<increment>(var));
	sub.push_back(make_pair(var, null));
	if (substitute(right, sub)->hash() != left_left->hash())
		return false;
	sub[0].second = inc;
	if (substitute(right, sub)->hash() != left2->hash())
		return false;
	return true;
}

bool check_on_axiom(std::shared_ptr<operation> for_check, std::shared_ptr<operation> axiom, vector <unsigned long long> &for_hash, vector <unsigned long long> &for_hash_axiom) {
	for_hash.clear();
	for_hash_axiom.clear();
	bool res = dfs(for_check, axiom, for_hash, for_hash_axiom);
	if (!res)
		return false;
	res = check_on_coincidence(for_hash, for_hash_axiom);
	if (!res)
		return false;
	return true;
}

size_t check_all_scheme_axioms(std::shared_ptr<operation> that) {
	vector <unsigned long long> for_hash, for_hash_axiom;
	for (size_t i = 0; i < 10; i++)
		if (check_on_axiom(that, get_exp(scheme_axioms[i]), for_hash, for_hash_axiom))
			return i + 1;
	if (check_on_axiom_11_12(that, 0, for_hash, for_hash_axiom))
		return 11;
	if (check_on_axiom_11_12(that, 1, for_hash, for_hash_axiom))
		return 12;
	if (check_on_axiom_a9(that))
		return 13;
	return 0;
}

size_t check_all_axioms(std::shared_ptr<operation> that) {
	for (size_t i = 0; i < 8; i++)
		if (get_exp(axioms[i])->hash() == that->hash())
			return i + 1;
	return 0;
}