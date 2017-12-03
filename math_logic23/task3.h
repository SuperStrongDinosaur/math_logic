#pragma once
#include "task2.h"
#define _CRT_SECURE_NO_WARNINGS

using namespace std;

string t[71];

string get_null(int a) {
	string res = "0";
	while (a--)
		res += '\'';
	return res;
}

void replace(string& s, map <char, string>& str) {
	string ans = "";
	for (size_t i = 0; i < s.size(); i++) {
		if (s[i] == '_') 
			ans += str[s[++i]];
		else
			ans += s[i];
	}
	cout << ans << endl;
}

void get_evidence(const string& s, vector <string>& to) {
	cout << s << "\n0=0->0=0->0=0\n";
	cout << "(" << s << ")->(0=0->0=0->0=0)->(" << s << ")\n";
	cout << "(0=0->0=0->0=0)->(" << s << ")\n";
	string s1 = "(" + s + ")";
	for (auto i : to) {
		cout << "(0=0->0=0->0=0)->" << s1 << '\n';
		s1 = "@" + i + s1;
		cout << "(0=0->0=0->0=0)->" << s1 << '\n';
	}
	cout << s1 << '\n';
}

void cout_base() {
	ifstream in;
	in.open("base.txt");
	for (int i = 0; i < 273; i++) {
		string a;
		in >> a;
		cout << a << endl;
	}
	in.close();
	in.open("theorems.txt");
	for (int i = 0; i < 71; i++)
		in >> t[i];
	in.close();
}

void solve3() {
	freopen("output.txt", "w", stdout);
	fstream in;
	int a, b;
	cin >> a >> b;
	if (a <= b) {
		cout << "|-?p(" << get_null(a) << "+p=" << get_null(b) << ")\n";
		cout_base();
		get_evidence("a+0=a", vector<string>({ "a" }));
		vector<string> to({ "b", "a" });
		get_evidence("a+b'=(a+b)'", to);
		get_evidence("a=b->a'=b'", to);
		get_evidence("a'=b'->a=b", to);

		vector<string> op(13);
		in.open("proofl.txt");
		for (auto& s : op)
			in >> s;
		vector<string> before(4);
		for (auto& s : before)
			in >> s;
		in.close();

		map <char, string> mp;
		for (int i = 0; i < b - a; i++) {
			mp['d'] = get_null(a);
			mp['e'] = get_null(b - a - i - 1);
			for (auto j : before)
				replace(j, mp);
		}
		mp['d'] = get_null(a);
		replace(string("@a(a+0=a)->(_d+0=_d)"), mp);
		replace(string("_d+0=_d"), mp);

		for (int i = 0; i < b - a; i++) {
			mp['e'] = get_null(a) + "+" + get_null(i + 1);
			mp['d'] = "(" + get_null(a) + "+" + get_null(i) + ")'";
			mp['f'] = get_null(a + i + 1);
			mp['k'] = get_null(a + i);
			mp['r'] = "(" + get_null(a) + "+" + get_null(i) + ")";
			for (auto j : op)
				replace(j, mp);
		}
		cout << get_null(a) << "+" << get_null(b - a) << "=" << get_null(b) << "->" << "?p(" << get_null(a) << "+p=" << get_null(b) << ")\n";
		cout << "?p(" << get_null(a) << "+p=" << get_null(b) << ")\n";
	}
	else {
		cout << "|-!?p(" << get_null(a) << "+p=" << get_null(b) << ")\n";
		cout_base();
		get_evidence("!(a'=0)", vector<string>({ "a" }));
		cout << "@a!(a'=0)->!((" << get_null(a - b - 1) << "+p)'=0)\n!(" << get_null(a - b - 1) << "+p)'=0\n";

		vector<string> op(14);
		in.open("proofm.txt");
		for (auto& s : op)
			in >> s;
		
		map <char, string> mp;
		for (int i = 0; i < b + 1; i++) {
			mp['r'] = get_null(a - b + i - 1);
			mp['t'] = get_null(i);
			for(size_t i = 0; i < 9; i++)
				replace(op[i], mp);
			if (i != b) {
				mp['r'] = get_null(a - b + i);
				for (size_t i = 9; i < 14; i++)
					replace(op[i], mp);
			}
		}
		mp['b'] = get_null(a) + "+p=" + get_null(b);
		mp['a'] = "(0=0->0=0->0=0)";
		for (int i = 0; i < 71; i++)
			replace(t[i], mp);

		mp['a'] = "!(0=0->0=0->0=0)";
		for (int i = 0; i < 71; i++)
			replace(t[i], mp);

		mp['a'] = "!" + get_null(a) + "+p=" + get_null(b);
		mp['b'] = get_null(a) + "+p=" + get_null(b);
		mp['c'] = "?p(" + get_null(a) + "+p=" + get_null(b) + ")->!(0=0->0=0->0=0)";
		mp['d'] = "?p(" + get_null(a) + "+p=" + get_null(b) + ")->!!(0=0->0=0->0=0)";
		for (size_t i = 0; i < 11; i++) {
			string s;
			in >> s;
			replace(s, mp);
		}
	}
}