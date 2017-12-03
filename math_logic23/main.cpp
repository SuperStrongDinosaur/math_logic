#include "task2.h"
#include "task3.h"

int main() {
	int a;
	cin >> a;
	if (a == 2)
		solve2("");
/*	for (int i = 1; i < 27; i++) {
		solve2(to_string(i));
 		cout << i << endl;
	}*/
	else if(a == 3)
		solve3();
	return 0;
}