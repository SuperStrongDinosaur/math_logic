#ifndef expressions_node_H
#define expressions_node_H

#include <string>
#include <unordered_set>
#include <memory>

using namespace std;

class expressions_node {

public:

	enum Type { EXPR, DISJ, CONJ, NEG };
	enum Oper { NONE, NOT, AND, OR, IMPL };
	enum Proof { AXIOM, MP, NP };

	Type t;
	Oper oper;
	Proof proof;

	string var;
	shared_ptr<expressions_node> left, right;

	int num;
	int frst, snd;

	expressions_node() {
		proof = NP;
	}

	expressions_node(Type t1, Oper oper1, string s1) {
		proof = NP;
		t = t1;
		oper = oper1;
		var = s1;
	}

	expressions_node(Type t1, Oper oper1) {
		proof = NP;
		t = t1;
		oper = oper1;
	}

	static bool is_equal(shared_ptr<expressions_node> e1, shared_ptr<expressions_node> e2) {
		if (e1->oper == e2->oper) {
			if (e1->oper == NONE) 
				return (e1->var == e2->var);
			if (e1->oper == NOT) 
				return is_equal(e1->left, e2->left);
			return is_equal(e1->left, e2->left) && is_equal(e1->right, e2->right);
		}
		return false;
	}
};

#endif
