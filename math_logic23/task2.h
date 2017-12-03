#pragma once

#include "deduction_theorem.h"
#include "check_on_axioms.h"
#include "check_on_every_and_exist.h"
#include <fstream>

using namespace std;

void check_evidence(vector <std::shared_ptr<operation>>& evidence, vector <std::shared_ptr<operation>>& assumption, vector <pair <verdict, pair <size_t, size_t> > >& result) {
	map <unsigned long long, pair <size_t, size_t> > mp_evidence, wait_to_activate;
	map <unsigned long long, size_t> set_evidence, mp_assumption;
	vector <vector <unsigned long long> > storage;

	for (size_t i = 0; i < assumption.size(); i++)
		mp_assumption[assumption[i]->hash()] = i;

	for (size_t i = 0; i < evidence.size(); i++) {
		if (i == 530)
			int n = 10;
		global_error = NOTHING;
		if (mp_assumption.find(evidence[i]->hash()) != mp_assumption.end())
			result.push_back(make_pair(ASSUMPTION, make_pair(mp_assumption[evidence[i]->hash()], -1)));
		else if (check_all_scheme_axioms(evidence[i]))
			result.push_back(make_pair(AXIOM_SCHEME, make_pair(check_all_scheme_axioms(evidence[i]), -1)));
		else if (check_all_axioms(evidence[i]))
			result.push_back(make_pair(AXIOM, make_pair(check_all_axioms(evidence[i]), -1)));
		else if (mp_evidence.find(evidence[i]->hash()) != mp_evidence.end())
			result.push_back(make_pair(MODUS_PONANS, make_pair(mp_evidence[evidence[i]->hash()].first, mp_evidence[evidence[i]->hash()].second)));
		else if (check_on_every(evidence[i]) && set_evidence.find(check_on_every(evidence[i])->hash()) != set_evidence.end())
			result.push_back(make_pair(EVERY, make_pair(set_evidence[check_on_every(evidence[i])->hash()], -1)));
		else if (check_on_exist(evidence[i]) && set_evidence.find(check_on_exist(evidence[i])->hash()) != set_evidence.end())
			result.push_back(make_pair(EXIST, make_pair(set_evidence[check_on_exist(evidence[i])->hash()], -1)));
		else
			result.push_back(make_pair(ERROR, make_pair(global_error, -1)));

		if (evidence[i]->get_id() == CONSEQUENCE) {
			unsigned long long hash = evidence[i]->get_nxt(0)->hash();
			if (set_evidence.find(hash) == set_evidence.end()) {
				if (wait_to_activate.find(hash) == wait_to_activate.end()) {
					vector <unsigned long long> st;
					st.push_back(evidence[i]->get_nxt(1)->hash());
					storage.push_back(st);
					wait_to_activate[hash] = make_pair(i, storage.size() - 1);
				}
				else {
					pair <size_t, size_t> that = wait_to_activate[hash];
					storage[that.second].push_back(evidence[i]->get_nxt(1)->hash());
				}
			}
			else mp_evidence[evidence[i]->get_nxt(1)->hash()] = make_pair(i, set_evidence[hash]);
		}
		unsigned long long hash = evidence[i]->hash();
		if (wait_to_activate.find(hash) != wait_to_activate.end()) {
			pair <size_t, size_t> which = wait_to_activate[hash];
			for (size_t j = 0; j < storage[which.second].size(); j++)
				mp_evidence[storage[which.second][j]] = make_pair(which.first, i);
		}
		set_evidence[hash] = i;
	}
}

bool check_verdict(vector <pair <verdict, pair <size_t, size_t> > >& result, fstream& out) {
	for (size_t i = 0; i < result.size(); i++) {
		if (result[i].first == ERROR) {
			out << "Вывод некорректен, начиная с формулы " << i + 1 << ' ';
			switch (result[i].second.first) {
			case FREE_ENTRY:
				out << "[Переменная свободно входит в формулу при применении правил для любого и существует]" << endl;
				break;
			case FREE_IN_ASSUMPTION:
				out << "[Используется правило с квантором по переменной, которая свободно входит в допущение]" << endl;
				break;
			case NOT_FREE_TO_SUBSTITUTE:
				out << "[Терм не свободен для подстановки вместо переменной]";
				break;
			}
			out << endl;
			return false;
		}
	}
	return true;
}

void solve2(string  num) {
	ifstream input, input1;
	input.open("for_proof.txt");
	input1.open("comment_for_proof.txt");
	for (int i = 0; i < 98; i++) {
		input >> evidence_every[i];
		long long a, b, c;
		input1 >> a >> b >> c;
		result_every[i] = make_pair((verdict)a, make_pair((size_t)b, (size_t)c));
	}
	for (int i = 0; i < 106; i++) {
		input >> evidence_exist[i];
		long long a, b, c;
		input1 >> a >> b >> c;
		result_exist[i] = make_pair((verdict)a, make_pair((size_t)b, (size_t)c));
	}
	input.close();
	input1.close();

	string input_file = "input" + num + ".txt";
	string output_file = "output" + num + ".txt";
	fstream in, out;
	in.open(input_file);
	out.open(output_file);

//	freopen(input_file.c_str(), "r", stdin);
//	freopen(output_file.c_str(), "w", stdout);

	string s;
	in >> s;

	string last = "", previous = "";
	int pos = (int)s.size() - 1;
	for (; pos >= 2 && (s[pos - 1] != '-' || s[pos - 2] != '|'); pos--);
	for (int i = 0; i < pos - 2; i++) {
		previous += s[i];
	}
	for (size_t i = pos; i < s.size(); i++)
		last += s[i];
	nxt = 0;
	to_parse = previous;
	while (nxt < previous.size()) {
		std::shared_ptr<operation> op = get_exp(previous, nxt);
		assumption.push_back(op);
		//cerr << assumption.back()->print() << endl;
		if (previous[nxt] == ',')
			nxt++;
	}

	vector <pair <verdict, pair <size_t, size_t> > > result, result1;
	vector <std::shared_ptr<operation>> evidence1, evidence;

	for (size_t i = 0; i < assumption.size() - 1; i++) {
		if (assumption.size() > 0) {
			out << assumption[i]->print();
			if (i != assumption.size() - 2)
				out << ",";
		}
	}
	if (assumption.size() > 0)
		out << "|-" << assumption.back()->print() << "->" << last;
	else out << s;

	while(in >> s) 
		evidence.push_back(get_exp(s));

	take_free_variables_from_assumptions();
	check_evidence(evidence, assumption, result);

	out << endl;
	bool what_to_do = check_verdict(result, out);
	if (what_to_do) {
		for (int i = (int)assumption.size() - 1; i >= 0; i--) {
			deduction_theorem(evidence, result, evidence1, result1, assumption[i]);
			swap(result, result1);
			swap(evidence, evidence1);
		}
		for (auto i : evidence)
			out << i->print() << endl;
	}

	res_quantifier.clear();
	assumptions_free_variables.clear();
	assumption.clear();
	new_position.clear();
	new_position.resize(1000009);
}