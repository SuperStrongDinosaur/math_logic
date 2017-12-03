#pragma once

#include "includes.h"
#include "negation.h"

std::map <size_t, size_t> close_bracket;
size_t nxt = 0;
std::string to_parse;

std::shared_ptr<operation> get_expression();
std::shared_ptr<operation> get_term();

std::shared_ptr<variable> get_variable() {
	bool subst = false;
	if (to_parse[nxt] == '_') {
		nxt++;
		subst = true;
	}
	std::string name = "";
	name += to_parse[nxt];
	nxt++;
	for (; nxt < to_parse.size() && '0' <= to_parse[nxt] && to_parse[nxt] <= '9'; nxt++)
		name += to_parse[nxt];
	if (subst)
		return std::make_shared<substitution>(name);
	return std::make_shared<variable>(name);
}

std::shared_ptr<operation> get_multiplied() {
	std::shared_ptr<operation> that;
	if (nxt < to_parse.size() && to_parse[nxt] == '0') {
		nxt++;
		that = std::make_shared<big_null>();
	}
	else if (nxt < to_parse.size() && to_parse[nxt] == '(') {
		nxt++;
		that = get_term();
		nxt++;
	} else {
		std::string name = "";
		name += to_parse[nxt];
		nxt++;
		for (; nxt < to_parse.size() && '0' <= to_parse[nxt] && to_parse[nxt] <= '9'; nxt++)
			name += to_parse[nxt];
		if (nxt < to_parse.size() && to_parse[nxt] == '(') {
			std::vector <std::shared_ptr<operation>> terms;
			while (to_parse[nxt] != ')') {
				nxt++;
				std::shared_ptr<operation> tmp = get_term();
				terms.push_back(tmp);
			}
			nxt++;
			that = std::make_shared<func>(name, terms);
		}
		else
			that = std::make_shared<variable>(name);
	} while (to_parse[nxt] == '\'') {
		that = std::make_shared<increment>(that);
		nxt++;
	}
	return that;
}

std::shared_ptr<operation> get_summand() {
	std::shared_ptr<operation> tmp = get_multiplied();
	while (nxt < to_parse.size() && to_parse[nxt] == '*') {
		nxt++;
		std::shared_ptr<operation> snd = get_multiplied();
		tmp = std::make_shared<multiplied>(tmp, snd);
	}
	return tmp;
}

std::shared_ptr<operation> get_term() {
	std::shared_ptr<operation> tmp = get_summand();
	while (nxt < to_parse.size() && to_parse[nxt] == '+') {
		nxt++;
		std::shared_ptr<operation> snd = get_summand();
		tmp = std::make_shared<summand>(tmp, snd);
	}
	return tmp;
}

std::shared_ptr<operation> get_predicate() {
	if (nxt < to_parse.size() && 'A' <= to_parse[nxt] && to_parse[nxt] <= 'Z') {
		std::string name = "";
		name += to_parse[nxt];
		nxt++;
		for (; nxt < to_parse.size() && '0' <= to_parse[nxt] && to_parse[nxt] <= '9'; nxt++)
			name += to_parse[nxt];
		std::vector <std::shared_ptr<operation>> terms;
		if (to_parse[nxt] == '(') {
			while (nxt < to_parse.size() && to_parse[nxt] != ')') {
				nxt++;
				std::shared_ptr<operation> tmp = get_term();
				terms.push_back(tmp);
			}
			nxt++;
		}
		return std::make_shared<predicate>(name, terms);
	}
	std::shared_ptr<operation> tmp1 = get_term();
	nxt++;
	std::shared_ptr<operation> tmp2 = get_term();
	return std::make_shared<equals>(tmp1, tmp2);
}

std::shared_ptr<operation> get_unary() {
	if (nxt < to_parse.size() && to_parse[nxt] == '_') {
		nxt++;
		std::string now = "";
		now += to_parse[nxt];
		nxt++;
		for (; nxt < to_parse.size() && '0' <= to_parse[nxt] && to_parse[nxt] <= '9'; nxt++)
			now += to_parse[nxt];
		return std::make_shared<substitution>(now);
	} else if (nxt < to_parse.size() && to_parse[nxt] == '!') {
		nxt++;
		return std::make_shared<my_negation>(get_unary());
	} else if (nxt < to_parse.size() && to_parse[nxt] == '(') {
		if (close_bracket[nxt] + 1 == to_parse.size() || to_parse[close_bracket[nxt] + 1] == '-' || to_parse[close_bracket[nxt] + 1] == '|' || to_parse[close_bracket[nxt] + 1] == '&' || to_parse[close_bracket[nxt] + 1] == ')') {
			nxt++;
			std::shared_ptr<operation> tmp = get_expression();
			nxt++;
			return tmp;
		}
		else
			return get_predicate();
	} else if (nxt < to_parse.size() && to_parse[nxt] == '@') {
		nxt++;
		std::shared_ptr<variable> var = get_variable();
		std::shared_ptr<operation> unary = get_unary();
		return std::make_shared<every>(var, unary);
	} else if (nxt < to_parse.size() && to_parse[nxt] == '?') {
		nxt++;
		std::shared_ptr<variable> var = get_variable();
		std::shared_ptr<operation> unary = get_unary();
		return std::make_shared<exist>(var, unary);
	} else return get_predicate();
}

std::shared_ptr<operation> get_my_conjunction1() {
	std::shared_ptr<operation> tmp = get_unary();
	while (nxt < to_parse.size() && to_parse[nxt] == '&') {
		nxt++;
		std::shared_ptr<operation> snd = get_unary();
		tmp = std::make_shared<my_conjunction>(tmp, snd);
	}
	return tmp;
}

std::shared_ptr<operation> get_my_disjunction1() {
	std::shared_ptr<operation> tmp = get_my_conjunction1();
	while (nxt < to_parse.size() && to_parse[nxt] == '|') {
		nxt++;
		std::shared_ptr<operation> snd = get_my_conjunction1();
		tmp = std::make_shared<my_disjunction>(tmp, snd);
	}
	return tmp;
}

std::shared_ptr<operation> get_expression() {
	std::shared_ptr<operation> tmp = get_my_disjunction1();
	if (nxt + 1 < to_parse.size() && to_parse[nxt] == '-' && to_parse[nxt + 1] == '>') {
		nxt += 2;
		return std::make_shared<consequence>(tmp, get_expression());
	}
	return tmp;
}

void get_nxt_bracket() {
	while (nxt < to_parse.size() && (to_parse[nxt] != '(' && to_parse[nxt] != ')'))
		nxt++;
}

void build_nxt() {
	while (true) {
		get_nxt_bracket();
		if (to_parse[nxt] == '(') {
			size_t now = nxt;
			nxt++;
			build_nxt();
			close_bracket[now] = nxt;
			nxt++;
		}
		else
			return;
	}
}

std::shared_ptr<operation> get_exp(const std::string& str) {
	close_bracket.clear();
	nxt = 0;
	to_parse = str;
	build_nxt();
	nxt = 0;
	return get_expression();
}

std::shared_ptr<operation> get_exp(const std::string& str, int k) {
	size_t buf = k;
	close_bracket.clear();
	nxt = buf;
	build_nxt();
	nxt = buf;
	return get_expression();
}

std::shared_ptr<operation> get_something(std::shared_ptr<operation> where, std::shared_ptr<operation> from, unsigned long long that) {
	if (where->hash() == that)
		return from;
	for (size_t i = 0; where->get_nxt(i) != nullptr; i++)
	{
		std::shared_ptr<operation> res = get_something(where->get_nxt(i), from->get_nxt(i), that);
		if (res != nullptr)
			return res;
	}
	return nullptr;
}

enum verdict {
	MODUS_PONANS,
	EVERY,
	EXIST,
	ASSUMPTION,
	AXIOM_SCHEME,
	AXIOM,
	ERROR
};

enum error_verdict {
	NOTHING,
	NOT_FREE_TO_SUBSTITUTE,
	FREE_ENTRY,
	FREE_IN_ASSUMPTION
};

error_verdict global_error;