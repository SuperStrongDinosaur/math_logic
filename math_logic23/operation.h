#pragma once

#include <vector>
#include <string>
#include <memory>

enum operations {
	VARIABLE,
	CONSEQUENCE,
	DISJUNCTION,
	CONJUNCTION,
	NEGATION,
	EXISTS, 
	EVERYS,
	SUMMAND,
	MULTIPLIED,
	EQUALS,
	BIG_NULL,
	PREDICATE, 
	FUNC, 
	SUBSTITUTION,
};

struct operation
{
	virtual std::string print() = 0;
	virtual ~operation()
	{}
	virtual int get_id() = 0;
	virtual size_t length() = 0;
	unsigned long long hash() {
		return get_string_hash(print());
	}

	virtual std::shared_ptr<operation> get_copy(std::vector<std::shared_ptr<operation>>&) = 0;
	virtual std::shared_ptr<operation> copy_yourself() = 0;

	static const unsigned long long A = 175993;
	static const unsigned long long INF = 1000000007;

	static unsigned long long get_string_hash(std::string that)
	{
		unsigned long long hash = 0;
		for (size_t i = 0; i < that.size(); i++)
		{
			hash = ((hash * A) % INF + that[i]) % INF;
		}
		return hash;
	}
	static unsigned long long bin_pow(unsigned a, unsigned b)
	{
		if (b == 0)
		{
			return 1;
		}
		unsigned long long res = bin_pow(a, b / 2);
		if (b % 2 == 1)
		{
			return (((res * res) % INF) * a) % INF;
		}
		else
		{
			return (res * res) % INF;
		}
	}
	virtual std::shared_ptr<operation> get_nxt(size_t what) = 0;
};
