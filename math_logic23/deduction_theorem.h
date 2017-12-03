#pragma once

#include "parse.h"

using namespace std;

std::shared_ptr<operation> dfs4(std::shared_ptr<operation> op, map <unsigned long long, std::shared_ptr<operation>> &sub) {
	if ((op->get_id() == 13 || op->get_id() == 0) && sub.find(op->hash()) != sub.end()) {
		return sub[op->hash()]->copy_yourself();
	} else {
		vector <std::shared_ptr<operation>> ops;
		for (size_t i = 0; op->get_nxt(i) != nullptr; i++)
			ops.push_back(dfs4(op->get_nxt(i), sub));
		return op->get_copy(ops);
	}
}

std::shared_ptr<operation> substitute(std::shared_ptr<operation> op, vector <pair <std::shared_ptr<operation>, std::shared_ptr<operation>> >& ops) {
	map <unsigned long long, std::shared_ptr<operation>> sub;
	for (size_t i = 0; i < ops.size(); i++)
		sub[ops[i].first->hash()] = ops[i].second;
	return dfs4(op, sub);
}

void delete_objects(vector <pair <std::shared_ptr<operation>, std::shared_ptr<operation>> >& smth) {
	for (size_t i = 0; i < smth.size(); i++)
		smth[i].first.reset();
}

vector<string> evidence_every(98), evidence_exist(106);
vector<pair <verdict, pair <size_t, size_t> >> result_every(98), result_exist(106);
vector<size_t> new_position(1000009);

void run_result(vector <pair <verdict, pair <size_t, size_t> > >& result, bool another, size_t ass_number) {
	size_t length = 98;
	vector<pair<verdict, pair <size_t, size_t> >> pointer = result_every;
	if (another) {
		length = 106;
		pointer = result_exist;
	}
	size_t to_add = result.size();
	for (size_t i = 0; i < length; i++) {
		if (pointer[i].first == ASSUMPTION)
			result.push_back(result[ass_number]);
		else if (pointer[i].first == MODUS_PONANS)
			result.push_back(make_pair(pointer[i].first, make_pair(pointer[i].second.first + to_add, pointer[i].second.second + to_add)));
		else if (pointer[i].first == EVERY || pointer[i].first == EXIST)
			result.push_back(make_pair(pointer[i].first, make_pair(pointer[i].second.first + to_add, pointer[i].second.second)));
		else
			result.push_back(pointer[i]);
	}
}

void deduction_theorem(vector <std::shared_ptr<operation>>& evidence, vector <pair <verdict, pair <size_t, size_t> > >& result, vector <std::shared_ptr<operation>>& evidence1, vector <pair <verdict, pair <size_t, size_t> > >& result1, std::shared_ptr<operation> formula) {
	for (size_t i = 0; i < evidence1.size(); i++)
		evidence1[i].reset();
	evidence1.clear();
	result1.clear();
	for (size_t i = 0; i < evidence.size(); i++) {
		if (result[i].first == MODUS_PONANS){
			vector<pair <std::shared_ptr<operation>, std::shared_ptr<operation>> > ops;
			ops.push_back(make_pair(std::make_shared<substitution>("a"), formula));
			ops.push_back(make_pair(std::make_shared<substitution>("b"), evidence[result[i].second.second]));
			ops.push_back(make_pair(std::make_shared<substitution>("c"), evidence[i]));
			for (size_t i = 0; i < 3; i++)	{
				shared_ptr<operation> res(get_exp(theorems_2[i]));
				evidence1.push_back(substitute(res, ops));
			}
			result1.push_back(make_pair(AXIOM_SCHEME, make_pair(1, -1)));
			result1.push_back(make_pair(MODUS_PONANS, make_pair(evidence1.size() - 3, new_position[result[i].second.second])));
			result1.push_back(make_pair(MODUS_PONANS, make_pair(evidence1.size() - 2, new_position[result[i].second.first])));
			delete_objects(ops);
		}
		else if (result[i].first == EVERY) {
			run_result(result1, false, new_position[result[i].second.first]);
			vector <pair <std::shared_ptr<operation>, std::shared_ptr<operation>> > ops;
			ops.push_back(make_pair(std::make_shared<substitution>("a"), formula));
			ops.push_back(make_pair(std::make_shared<substitution>("b"), evidence[i]->get_nxt(0)));
			ops.push_back(make_pair(std::make_shared<variable>("d"), evidence[i]->get_nxt(1)->get_nxt(0)));
			ops.push_back(make_pair(std::make_shared<substitution>("c"), evidence[i]->get_nxt(1)->get_nxt(1)));
			for (int i = 0; i < 98; i++) {
				shared_ptr<operation> op(get_exp(evidence_every[i]));
				evidence1.push_back(substitute(op, ops));
			}
		} else if (result[i].first == EXIST) {
			run_result(result1, true, new_position[result[i].second.first]);
			vector <pair <std::shared_ptr<operation>, std::shared_ptr<operation>> > ops;
			ops.push_back(make_pair(std::make_shared<substitution>("a"), formula));
			ops.push_back(make_pair(std::make_shared<substitution>("c"), evidence[i]->get_nxt(1)));
			ops.push_back(make_pair(std::make_shared<variable>("d"), evidence[i]->get_nxt(0)->get_nxt(0)));
			ops.push_back(make_pair(std::make_shared<substitution>("b"), evidence[i]->get_nxt(0)->get_nxt(1)));
			for (int i = 0; i < 106; i++) {
				if (i == 105) {
					int n = 0;
					n = 1;
				}
				shared_ptr<operation> op(get_exp(evidence_exist[i]));
				evidence1.push_back(substitute(op, ops));
			}
		}
		else if (result[i].first == ASSUMPTION && evidence[i]->hash() == formula->hash()) {
			vector <pair <std::shared_ptr<operation>, std::shared_ptr<operation>> > ops;
			ops.push_back(make_pair(std::make_shared<substitution>("a"), formula));
			for (size_t i = 0; i < 5; i++) {
				shared_ptr<operation> res(get_exp(theorems_1[i]));
				evidence1.push_back(substitute(res, ops));
			}
			result1.push_back(make_pair(AXIOM_SCHEME, make_pair(1, -1)));
			result1.push_back(make_pair(AXIOM_SCHEME, make_pair(1, -1)));
			result1.push_back(make_pair(AXIOM_SCHEME, make_pair(2, -1)));
			result1.push_back(make_pair(MODUS_PONANS, make_pair(evidence1.size() - 3, evidence1.size() - 5)));
			result1.push_back(make_pair(MODUS_PONANS, make_pair(evidence1.size() - 2, evidence1.size() - 4)));
			delete_objects(ops);
		} else if (result[i].first == AXIOM_SCHEME || result[i].first == AXIOM || result[i].first == ASSUMPTION) {
			evidence1.push_back(evidence[i]->copy_yourself());
			shared_ptr<operation> res(get_exp(scheme_axioms[0]));
			vector <pair <std::shared_ptr<operation>, std::shared_ptr<operation>> > ops;
			ops.push_back(make_pair(std::make_shared<substitution>("a"), evidence[i]));
			ops.push_back(make_pair(std::make_shared<substitution>("b"), formula));
			std::shared_ptr<operation> res1 = substitute(res, ops);
			evidence1.push_back(res1);
			evidence1.push_back(res1->get_nxt(1)->copy_yourself());
			result1.push_back(make_pair(result[i].first, make_pair(result[i].second.first, -1)));
			result1.push_back(make_pair(AXIOM_SCHEME, make_pair(1, -1)));
			result1.push_back(make_pair(MODUS_PONANS, make_pair(evidence1.size() - 2, evidence1.size() - 3)));
			delete_objects(ops);
		}
		new_position[i] = evidence1.size() - 1;
	}
}