#pragma once

#include "parse.h"

using namespace std;

set <pair<string, unsigned long long>> assumptions_free_variables;
vector <std::shared_ptr<operation>> assumption;

static void dfs10(std::shared_ptr<operation> op, set < pair<string, unsigned long long>> &freedom_hash, set <unsigned long long> &hash_set) {
	if (op->get_id() == 5 || op->get_id() == 6) {
		bool truth = hash_set.find(op->get_nxt(0)->hash()) == hash_set.end();

		if (freedom_hash.find(make_pair(op->get_nxt(0)->print(), op->get_nxt(0)->hash())) != freedom_hash.end()) 
			freedom_hash.erase(make_pair(op->get_nxt(0)->print(), op->get_nxt(0)->hash()));

		hash_set.insert(op->get_nxt(0)->hash());
		dfs10(op->get_nxt(1), freedom_hash, hash_set);
		if (truth)
			hash_set.erase(op->get_nxt(0)->hash());
	} else if (op->get_id() == 0) {
		if (hash_set.find(op->hash()) == hash_set.end())
			freedom_hash.insert(make_pair(op->print(), op->hash()));
	} else {
		for (size_t i = 0; op->get_nxt(i) != nullptr; i++)
			dfs10(op->get_nxt(i), freedom_hash, hash_set);
	}
}

static void dfs1(std::shared_ptr<operation> op, set < pair<string, unsigned long long>> &freedom_hash, set <unsigned long long> &hash_set) {
	if (op->get_id() == 5 || op->get_id() == 6) {
		bool truth = hash_set.find(op->get_nxt(0)->hash()) == hash_set.end();
		hash_set.insert(op->get_nxt(0)->hash());
		dfs1(op->get_nxt(1), freedom_hash, hash_set);
		if (truth)
			hash_set.erase(op->get_nxt(0)->hash());
	}
	else if (op->get_id() == 0) {
		if (hash_set.find(op->hash()) == hash_set.end())
			freedom_hash.insert(make_pair(op->print(), op->hash()));
	}
	else {
		for (size_t i = 0; op->get_nxt(i) != nullptr; i++)
			dfs1(op->get_nxt(i), freedom_hash, hash_set);
	}
}

bool check_on_unfreedom(std::shared_ptr<operation> for_check, vector <std::shared_ptr<operation>>& var) {
	set <unsigned long long> hash_set;
	set < pair<string, unsigned long long>> freedom_hash;
	dfs1(for_check, freedom_hash, hash_set);
	for (size_t i = 0; i < var.size(); i++)
		if (freedom_hash.find(make_pair(var[i]->print(), var[i]->hash())) != freedom_hash.end())
			return false;
	return true;
}

std::shared_ptr<operation> check_on_every(std::shared_ptr<operation> for_check) {
	if (for_check->get_id() == CONSEQUENCE) {
		std::shared_ptr<operation> left = for_check->get_nxt(0);
		std::shared_ptr<operation> right = for_check->get_nxt(1);
		if (right->get_id() == EVERYS) {
			std::shared_ptr<operation> var = right->get_nxt(0);
			if (assumptions_free_variables.find(make_pair(var->print(), var->hash())) != assumptions_free_variables.end()) {
				global_error = FREE_IN_ASSUMPTION;
				return nullptr;
			}
			std::shared_ptr<operation> nxt = right->get_nxt(1);
			vector <std::shared_ptr<operation>> that;
			that.push_back(var);
			if (check_on_unfreedom(left, that))
				return std::make_shared<consequence>(left, nxt);
			else
				global_error = FREE_ENTRY;
		}
	}
	return nullptr;
}

std::shared_ptr<operation> check_on_exist(std::shared_ptr<operation> for_check) {
	if (for_check->get_id() == CONSEQUENCE) {
		std::shared_ptr<operation> left = for_check->get_nxt(0);
		std::shared_ptr<operation> right = for_check->get_nxt(1);
		if (left->get_id() == EXISTS) {
			std::shared_ptr<operation> var = left->get_nxt(0);
			if (assumptions_free_variables.find(make_pair(var->print(), var->hash())) != assumptions_free_variables.end()) {
				global_error = FREE_IN_ASSUMPTION;
				return nullptr;
			}
			std::shared_ptr<operation> nxt = left->get_nxt(1);
			vector <std::shared_ptr<operation>> that;
			that.push_back(var);
			if (check_on_unfreedom(right, that))
				return std::make_shared<consequence>(nxt, right);
			else
				global_error = FREE_ENTRY;
		}
	}
	return nullptr;
}

void take_free_variables_from_assumptions() {
//	for (size_t i = 0; i < assumption.size(); i++) {
		set<unsigned long long> hash_set;
		set<pair<string, unsigned long long>> freedom_hash;
		vector<string> free_vars;
		if(assumption.size() > 0)
			dfs10(assumption.back(), freedom_hash, hash_set);
		for (auto hash : freedom_hash)
			assumptions_free_variables.insert(hash);
//	}
}

