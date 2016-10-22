#define _CRT_SECURE_NO_WARNINGS
#include "proof_check.h"
#include <iostream>

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	freopen("in.txt", "r", stdin);
	freopen("out.txt", "w", stdout);

	shared_ptr<expressions_parser> expressions_parser(new expressions_parser());
	string str, head;
	int curNum = 1;
	proof_checker a;

	getline(cin, head);
	for (size_t i = 2; i < head.length(); i+=2) {
		string cur;
		while (head[i] != '|' && i < head.length())
			cur.push_back(head[i++]);
		a.insert_suppositions(cur);
	}
	cout << head << endl;
	while (getline(cin, str)) {
		shared_ptr<expressions_node> res = expressions_parser->parse(str);

		a.check(res);

		cout << "(" << curNum << ") " << str << " ";
		if (res->proof == expressions_node::AXIOM) 
			cout << "(Сх. акс. " << res->frst << ")\n";
		else if (res->proof == expressions_node::MP)
			cout << "(M. P. " << res->frst << ", " << res->snd << ")\n";
		else if(res->proof == expressions_node::SP)
			cout << "(Предп. " << res->frst + 1 << ")\n";
		else if (res->proof == expressions_node::NP)
			cout << "(Не доказано)\n";

		curNum++;
	}
	return 0;
}