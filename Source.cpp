#define _CRT_SECURE_NO_WARNINGS
#include "proof_check.h"
#include <iostream>

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	freopen("good6.in", "r", stdin);
	freopen("out.txt", "w", stdout);

	shared_ptr<expressions_parser> expressions_parser(new expressions_parser());
	string str;
	int curNum = 1;
	proof_checker a;

	while (getline(cin, str)) {
		shared_ptr<expressions_node> res = expressions_parser->parse(str);

		a.check(res);

		cout << "(" << curNum << ") " << str << " ";
		if (res->proof == expressions_node::AXIOM) 
			cout << "(Сх. акс. " << res->frst << ")\n";
		if (res->proof == expressions_node::MP)
			cout << "(M. P. " << res->frst << ", " << res->snd << ")\n";
		if (res->proof == expressions_node::NP)
			cout << "(Не доказано)\n";

		curNum++;
	}
	return 0;
}