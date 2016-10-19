#ifndef PROOFCHECK_H
#define PROOFCHECK_H

#include "expressions_parser.h"
#include <unordered_map>
#include <deque>

class proof_checker {

	vector<shared_ptr<expressions_node>> axioms;
	vector<shared_ptr<expressions_node>> expressions;

	vector<int> MP_first;
	vector<int> MP_second;
	vector<shared_ptr<expressions_node>> MP_expression;

	unordered_map<string, int> MP_map;
	vector<string> axioms_s;

public:
	proof_checker() {
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

		shared_ptr<expressions_parser> expressions_parser(new expressions_parser());
		for (string a : axioms_s) 
			axioms.push_back(expressions_parser->parse(a));

		int num = 1;
		for (shared_ptr<expressions_node> axiom : axioms) {
			axiom->num = num;
			num++;
		}
	}

	void check(shared_ptr<expressions_node> expr) {
		expr->num = expressions.size() + 1;
		if (expr->proof == expressions_node::NP) 
			proof(expr);

		if (expr->proof != expressions_node::NP) {
			for (auto i : expressions)
				if (i->proof != expressions_node::NP) {
					addIfMP(i, expr);
					addIfMP(expr, i);
				}
		}
		expressions.push_back(expr);
	}

private:
	void proof(shared_ptr<expressions_node> expr) {
		for (shared_ptr<expressions_node> axiom : axioms)
			if (is_axiom(expr, axiom)) 
				return;
		is_MP(expr);
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
					if ((*m.find(curA->var)).second != (curE->var))  // first
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

		/*for (size_t i = 0; i < MP_expression.size(); i++) {
			if (expr->is_equal(MP_expression[i]->right, expr)) {
				expr->proof = expressions_node::MP;
				expr->frst = MP_first[i];
				expr->snd = MP_second[i];
				return true;
			}
		}*/
		return false;
	}

	void addIfMP(shared_ptr<expressions_node> a, shared_ptr<expressions_node> b) {
		if (a->oper == expressions_node::IMPL && a->is_equal(a->left, b)) {
			MP_first.push_back(b->num);
			MP_second.push_back(a->num);
			MP_map.insert(make_pair(a->right->var, MP_second.size() - 1));
			MP_expression.push_back(a);
		}
	}
};

#endif