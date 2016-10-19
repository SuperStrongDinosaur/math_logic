#ifndef Expression_h
#define Expression_h

#include <iostream>
#include <string>

class expression {
public:

	expression *r, *l;
	int type;
	std::string s;

	expression(expression *l1, expression *r1): r(r1), l(l1){}


};

#endif