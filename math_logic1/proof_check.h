#ifndef PROOFCHECK_H
#define PROOFCHECK_H

#include "expressions_parser.h"
#include <unordered_map>
#include <deque>
#include <vector>
#include <string>

class proof_checker {

	vector<shared_ptr<expressions_node>> axioms;

	vector<int> MP_first;
	vector<int> MP_second;
	vector<shared_ptr<expressions_node>> MP_expression;

	unordered_map<string, int> MP_map;
	unordered_multimap<string, shared_ptr<expressions_node>> MP_left;
	unordered_multimap<string, shared_ptr<expressions_node>> MP_all;
	vector<string> axioms_s;

	unordered_map<string, int> suppositions;

	int cnt, cnt_sup;

public:
	proof_checker() {
		cnt = cnt_sup = 0;

		axioms_s.push_back("A->B->A");
		axioms_s.push_back("(A->B)->(A->B->C)->(A->C)");
		axioms_s.push_back("A->B->A&B");
		axioms_s.push_back("A&B->A");
		axioms_s.push_back("A&B->B");
		axioms_s.push_back("A->A|B");
		axioms_s.push_back("B->A|B");
		axioms_s.push_back("(A->C)->(B->C)->(A|B->C)");
		axioms_s.push_back("(A->B)->(A->!B)->!A");
		axioms_s.push_back("!!A->A");

		shared_ptr<expressions_parser> expressions_parser(new class expressions_parser());
		for (string a : axioms_s)
			axioms.push_back(expressions_parser->parse(a));

		int num = 1;
		for (shared_ptr<expressions_node> axiom : axioms) {
			axiom->num = num;
			num++;
		}
	}

	void insert_suppositions(string sup) {
		shared_ptr<expressions_parser> expressions_parser(new class expressions_parser());
		suppositions.insert(make_pair(expressions_parser->parse(sup)->var, cnt_sup++));
	}

	void check(shared_ptr<expressions_node> expr) {
		expr->num = ++cnt;
		if (expr->proof == expressions_node::NP)
			proof(expr);

		if (expr->proof != expressions_node::NP) {


			auto range = MP_left.equal_range(expr->var);
			for (auto i = range.first; i != range.second; i++) {
				MP_first.push_back(i->second->num);
				MP_second.push_back(expr->num);
				MP_map.insert(make_pair(i->second->right->var, MP_second.size() - 1));
			}

			range = MP_all.equal_range(expr->left->var);
			for (auto i = range.first; i != range.second; i++) {
				MP_first.push_back(i->second->num);
				MP_second.push_back(expr->num);
				MP_map.insert(make_pair(expr->right->var, MP_second.size() - 1));
			}
		}
		if (expr->oper == expressions_node::IMPL)
			MP_left.insert(make_pair(expr->left->var, expr));
		MP_all.insert(make_pair(expr->var, expr));
	}

private:
	void proof(shared_ptr<expressions_node> expr) {
		if (suppositions.find(expr->var) != suppositions.end()) {
			expr->frst = suppositions.find(expr->var)->second;
			expr->proof = expressions_node::SP;
			return;
		}
		else {
			for (shared_ptr<expressions_node> axiom : axioms)
				if (is_axiom(expr, axiom))
					return;
			is_MP(expr);
		}
	}

	bool is_axiom(shared_ptr<expressions_node> expr, shared_ptr<expressions_node> ax) {
		unordered_map<string, string> m;
		deque<shared_ptr<expressions_node>> e;
		deque<shared_ptr<expressions_node>> a;

		e.push_back(expr);
		a.push_back(ax);

		while (!a.empty()) {
			shared_ptr<expressions_node> curE = *e.begin();
			e.pop_front();
			shared_ptr<expressions_node> curA = *a.begin();
			a.pop_front();

			if (curA->oper == expressions_node::NONE) {
				if (m.find(curA->var) != m.end()) {
					if ((*m.find(curA->var)).second != (curE->var))
						return false;
				}
				else
					m.insert(make_pair(curA->var, curE->var));
			}
			else {
				if (curA->oper == curE->oper) {
					e.push_back(curE->left);
					a.push_back(curA->left);
					if (curA->oper != expressions_node::NOT) {
						e.push_back(curE->right);
						a.push_back(curA->right);
					}
				}
				else
					return false;
			}
		}
		expr->proof = expressions_node::AXIOM;
		expr->frst = ax->num;
		return true;
	}

	bool is_MP(shared_ptr<expressions_node> expr) {

		if (MP_map.find(expr->var) != MP_map.end()) {
			int i = MP_map.find(expr->var)->second;
			expr->proof = expressions_node::MP;
			expr->frst = MP_first[i];
			expr->snd = MP_second[i];
			return true;
		}
		return false;
	}
};

#endif