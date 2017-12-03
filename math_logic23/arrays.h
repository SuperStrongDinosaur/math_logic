#pragma once
#include "string"

std::string scheme_axioms[] = { "_a->_b->_a", "(_a->_b)->(_a->_b->_c)->(_a->_c)",
"_a->_b->_a&_b", "_a&_b->_a", "_a&_b->_b",
"_a->_a|_b", "_b->_a|_b", "(_a->_c)->(_b->_c)->(_a|_b->_c)",
"(_a->_b)->(_a->!_b)->!_a", "!!_a->_a" };

std::string axioms[] = { "a=b->a'=b'", "a=b->a=c->b=c", "a'=b'->a=b", "!a'=0", "a+b'=(a+b)'",
"a+0=a", "a*0=0", "a*b'=a*b+a" };

std::string theorems_1[] = { "_a->_a->_a", "_a->(_a->_a)->_a", "(_a->(_a->_a))->(_a->(_a->_a)->_a)->(_a->_a)",
"(_a->(_a->_a)->_a)->(_a->_a)", "_a->_a" };

std::string theorems_2[] = {/*"_a->(_b->_c)", "_a->_b", */"(_a->_b)->(_a->(_b->_c))->(_a->_c)",
"(_a->(_b->_c))->(_a->_c)", "_a->_c" };
