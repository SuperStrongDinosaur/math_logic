#ifndef expressions_parser_H
#define expressions_parser_H

#include "expressions_node.h"

class expressions_parser {
	int i;
	string s;

public:
	shared_ptr<expressions_node> parse(string s1) {
		s = delete_whites_paces(s1);
		i = 0;
		shared_ptr<expressions_node> res = parse_expression();
		res->var = s;
		return res;
	}

private:
	shared_ptr<expressions_node> parse_expression() {
		shared_ptr<expressions_node> res = parse_disj();
		if (s[i] == '-') {
			i++;
			if (s[i] == '>') {
				i++;
				shared_ptr<expressions_node> new_res(new expressions_node(expressions_node::EXPR, expressions_node::IMPL));
				new_res->left = res;
				new_res->right = parse_expression();
				res = new_res;
				res->var = ("(" + res->left->var + "->" + res->right->var + ")");
			}
		}
		return res;
	}

	shared_ptr<expressions_node> parse_disj() {
		shared_ptr<expressions_node> res = parse_conj();
		while (s[i] == '|') {
			i++;
			shared_ptr<expressions_node> new_res(new expressions_node(expressions_node::DISJ, expressions_node::OR));
			new_res->left = res;
			new_res->right = parse_conj();
			res = new_res;
			res->var = "(" + res->left->var + "|" + res->right->var + ")";
		}
		return res;
	}

	shared_ptr<expressions_node> parse_conj()  {
		shared_ptr<expressions_node> res = parse_neg();
		while (s[i] == '&') {
			i++;
			shared_ptr<expressions_node> new_res(new expressions_node(expressions_node::CONJ, expressions_node::AND));
			new_res->left = res;
			new_res->right = parse_neg();
			res = new_res;
			res->var = "(" + res->left->var + "&" + res->right->var + ")";
		}
		return res;
	}

	shared_ptr<expressions_node> parse_neg() {
		shared_ptr<expressions_node> res;
		char c = s[i];
		if (c >= 'A' && c <= 'Z') {
			string var = "";
			while ((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
				var += s[i];
				i++;
				c = s[i];
			}
			shared_ptr<expressions_node> res1(new expressions_node(expressions_node::NEG, expressions_node::NONE, var));
			res = res1;
			return res;
		}
		if (s[i] == '(') {
			i++;
			res = parse_expression();
			if (s[i] == ')') {
				i++;
				return res;
			}
		}
		if (s[i] == '!') {
			i++;
			shared_ptr<expressions_node> res1(new expressions_node(expressions_node::NEG, expressions_node::NOT));
			res = res1;
			res->left = parse_neg();
			res->var = "(!" + res->left->var + ")";
			return res;
		}
	}

	static string delete_whites_paces(string s) {
		string res = "";
		for (size_t i = 0; i < s.length(); i++) 
			if (s[i] != ' ') 
				res += s[i];
		return res;
	}
};

#endif