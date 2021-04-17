#include "INT.h"
#include "FRAC.h"
#include "algorithm"
#include <iostream>
#include <functional>
#include <vector>

using namespace real;
using namespace std;

static std::vector<std::function<void()>> testCases;

void addIntTest() {
	testCases.push_back([&] {
		INT n;
		assert(n.isZero());
		assert(!n.isPositive());
		assert(!n.isNegative());
		std::cout << "zero test passed" << std::endl;
		});

	testCases.push_back([&] {
		INT one = 1;
		INT one_ = INT("1");
		assert(one == one_);
		assert(!(one != one_));
		std::cout << "one test passed" << std::endl;
		});


	testCases.push_back([&] {
		std::string s1 = "12345";
		INT x1 = INT(s1);
		std::string x1_s = x1.toString();
		std::cout << "x1:" << x1_s << std::endl;
		assert(x1_s == s1);
		std::cout << "simple string constructor test1 passed" << std::endl;
		});

	testCases.push_back([&] {
		INT x = 342343434343;
		INT y = x;
		assert(x == y);
		assert(!(x != y));
		assert((x ^ y) == 0);
		assert((x ^ y).isZero());
		std::cout << "simple string constructor test2 passed" << std::endl;
		});

	testCases.push_back([&] {
		INT x = INT("-342343434343432434234324234");
		assert(x.isNegative());
		assert(!x.isPositive());
		assert(!x.isZero());

		x = -x;
		assert(!x.isNegative());
		assert(x.isPositive());
		assert(!x.isZero());

		auto y = x + (-x);
		assert(y.isZero());
		std::cout << "minus test1 passed" << std::endl;
		});

	testCases.push_back([&] {
		INT x = -3423434343434324343LL;
		assert(x.isNegative());
		assert(!x.isPositive());
		assert(!x.isZero());

		x = -x;
		assert(!x.isNegative());
		assert(x.isPositive());
		assert(!x.isZero());

		auto y = x + (-x);
		assert(y.isZero());
		std::cout << "minus test2 passed" << std::endl;
		});

	testCases.push_back([&] {
		INT x = INT(-3423434343434324343LL);
		assert(x.isNegative());
		assert(!x.isPositive());
		assert(!x.isZero());

		x = -x;
		assert(!x.isNegative());
		assert(x.isPositive());
		assert(!x.isZero());

		auto y = x + (-x);
		assert(y.isZero());
		std::cout << "minus test3 passed" << std::endl;
		});

	testCases.push_back([&] {
		INT x = INT(-3423434343434324343LL);
		x.negate();
		assert(!x.isNegative());
		assert(x.isPositive());
		assert(!x.isZero());

		x = -x;
		assert(x.isNegative());
		assert(!x.isPositive());
		assert(!x.isZero());

		auto y = x + (-x);
		assert(y.isZero());
		std::cout << "minus test4 passed" << std::endl;
		});

	testCases.push_back([&] {
		int p = -34324324234;
		INT x = INT(p);
		bool isOverflow = false;
		assert(x.toInt(isOverflow) == p);
		std::cout << "to int test1 passed" << std::endl;
		});

	testCases.push_back([&] {
		int p = 34324324234;
		INT x = INT(-p);
		bool isOverflow = false;
		assert(x.toInt(isOverflow) == -p);
		std::cout << "to int test2 passed" << std::endl;
		});

	testCases.push_back([&] {
		long long p = -34324324344324234;
		INT x = INT(p);
		bool isOverflow = false;
		assert(x.toLongLong(isOverflow) == p);
		std::cout << "to long long test1 passed" << std::endl;
		});

	testCases.push_back([&] {
		std::string p = "101011010101010101010101101010101011";
		INT x = INT(p, 2);
		std::cout << "x:" << x.toString(2) << std::endl;
		for (int i = 0; i < p.size(); ++i) {
			char bit = x.bit(i) ? '1' : '0';
			assert(bit == p[p.size() - 1 -i]);
		}
		std::cout << "bit test 1 passed" << std::endl;
		});

	testCases.push_back([&] {
		std::string p = "101011010101010101010101101010101011";
		INT x = INT(p, 2);
		std::cout << "x:" << x.toString(2) << std::endl;
		INT y = x - 1;
		std::cout << "y:" << x.toString(2) << std::endl;
		assert(x > y);
		assert(x >= y);
		assert(y < x);
		assert(y <= x);
		assert(x == x);
		assert(!(x == y));
		assert(x != y);
		std::cout << "compare test 1 passed" << std::endl;
		});

	testCases.push_back([&] {
		long long p = 34324324344324234;
		INT x = INT(-p);
		bool isOverflow = false;
		assert(x.toLongLong(isOverflow) == -p);
		std::cout << "to long long test2 passed" << std::endl;
		});

	testCases.push_back([&] {
		std::string s1 = "12345";
		INT x1 = INT(s1);
		std::string x1_s = x1.toString();
		std::cout << "x1:" << x1_s << std::endl;
		assert(x1_s == s1);
		std::cout << "simple string constructor test passed" << std::endl;
		});

	testCases.push_back([&] {
		std::string s6 = "400412878166003164546735534092609172319140330852536603355429981136254443797343719815103359446869847213482227876740797726949481901457564006563326790056796257840594244649848624543981509868730249924";
		INT x6 = INT(s6);
		auto x6_s = x6.toString();
		cout << "x6:" << x6_s << endl;
		assert(x6_s == s6);
		cout << "long string constructor test 6 passed" << endl;
		});

	testCases.push_back([&] {
		INT N = INT("100");
		INT sum;
		for (INT i = 0; i <= N; i = i + 1) {
			sum += i;
		}
		auto sum_s = sum.toString();
		std::cout << "sum:" << sum_s << std::endl;
		assert(sum == N * (N + 1) / 2);
		cout << "sum test 1 passed" << endl;
		});

	testCases.push_back([&] {
		INT N2 = INT("200000");
		INT sum2;
		for (INT i = 0; i <= N2; i = i + 1) {
			sum2 += i;
		}
		auto sum2_s = sum2.toString();
		std::cout << "sum2:" << sum2_s << std::endl;
		assert(sum2 == N2 * (N2 + 1) / 2);
		cout << "sum test 2 passed" << endl;
		});

	testCases.push_back([&] {
		 INT x = INT("999999999999");
		 INT y = x;
		 std::cout << "x:" << x.toString() << std::endl;
		 std::cout << "y:" << y.toString() << std::endl;

		 assert(x++ == y);
		 assert(x == y + 1);

		 std::cout << "x:" << x.toString() << std::endl;
		 std::cout << "y:" << y.toString() << std::endl;

		 assert(--x == y);
		 assert(x == y);

		cout << "incrment and decrement test1 passed\n";
		});

	testCases.push_back([&] {
		INT a = INT("343483463746736473467346736476");
		INT b = INT("364826374736436476374676763746");
		INT c = INT("342349384938483843874839");

		std::cout << "a:" << a.toString() << std::endl;
		std::cout << "b:" << b.toString() << std::endl;
		std::cout << "c:" << c.toString() << std::endl;

		auto s1 = a + (b + c);
		auto s2 = (a + b) + c;
		std::cout << "s1:" << s1.toString() << std::endl;
		std::cout << "s2:" << s2.toString() << std::endl;
		assert(s1 == s2);

		auto p1 = a * (b * c);
		auto p2 = (a * b) * c;
		std::cout << "p1:" << p1.toString() << std::endl;
		std::cout << "p2:" << p2.toString() << std::endl;
		assert(p1 == p2);

		auto m1 = a * (b + c);
		auto m2 = a * b + a * c;
		std::cout << "m1:" << m1.toString() << std::endl;
		std::cout << "m2:" << m2.toString() << std::endl;

		assert(m1 == m2);
		std::cout << "basic plus and multiplcation law test 1 passed" << endl;
		});

	testCases.push_back([&] {
		std::string s7 = "101010101010101101010010101";
		INT x7 = INT(s7, 2);
		auto x7_s = x7.toString(2);
		cout << "x7:" << x7_s << std::endl;
		assert(x7_s == s7);
		cout << "base 2 string construtor test passed\n";

		INT y7 = x7 ^ x7;
		cout << "y7:" << y7.toString() << std::endl;
		assert(y7.isZero());
		cout << "xor test7 passed" << endl;
		});

	testCases.push_back([&] {
		long long p2 = 123456789012345333LL;
		INT x2 = INT(p2);
		auto x2_s = x2.toString();
		std::cout << "x2:" << x2_s << std::endl;
		std::cout << "p2:" << p2 << std::endl;
		assert(std::to_string(p2) == x2_s);
		std::cout << "long long constructor test passed" << std::endl;
		});

	testCases.push_back([&] {
		INT x3 = INT("123456789012345333");
		std::cout << "x3:" << x3.toString() << std::endl;

		std::string s4 = "12345678901234";
		INT x4 = INT(s4);
		auto x4_s = x4.toString();
		std::cout << "x4:" << x4_s << std::endl;
		assert(x4_s == s4);
		cout << "string constructor test4 passed" << std::endl;

		std::string s5 = "32433443423434783432432423423423423423423423423423423423472364723647236476274623764723674623746764237647264723476767236472364782374827384723478324324324243343243243242342432423432434";
		INT x5 = INT(s5);
		std::cout << "x5:" << x5.toString() << std::endl;
		assert(x5.toString() == s5);
		cout << "long string constructor test5 passed" << std::endl;

		std::cout << "x4*x5:" << (x4 * x5).toString() << std::endl;
		std::cout << "x5*x4:" << (x5 * x4).toString() << std::endl;

		assert(x4 * x5 == x5 * x4);
		assert((x4 * x5).toString() == (x5 * x4).toString());
		cout << "multiplication communication law test passed" << std::endl;
		});
	
	testCases.push_back([&] {
		INT x = INT("10011100101001010100100100101001010100101001010101010010101010010011001010001010100110011001", 2);
		INT y = INT("1101111001010101010101010101010010000101011001100100101000111110010010", 2);
		INT x0 = x ^ x;
		std::cout << "x:" << x.toString(2) << std::endl;
		std::cout << "y:" << y.toString(2) << std::endl;
		std::cout << "x0:" << x0.toString(2) << std::endl;
		assert(x0 == INT(0));
		assert((x | x) == x);
		assert((x& x) == x);
		std::cout << "x&y:" << (x & y).toString(2) << std::endl;
		std::cout << "x|y:" << (x | y).toString(2) << std::endl;
		std::cout << "x^y:" << (x ^ y).toString(2) << std::endl;
		std::cout << "bit operation test 1 passed" << std::endl;
		});

	testCases.push_back([&] {
		INT a = INT("342343243647264736476374673647364736476");
		INT b = INT("3434343434334343434343434");

		std::cout << "a:" << a.toString() << std::endl;
		std::cout << "b:" << b.toString() << std::endl;

		INT q, r;
		q = a / b;
		std::cout << "q:" << q.toString() << std::endl;
		r = a % b;
		std::cout << "r:" << r.toString() << std::endl;
		auto c = b * q + r;
		std::cout << "c:" << c.toString() << std::endl;
		assert(a == c);
		std::cout << "/ % test 1 passed" << std::endl;
		});

	testCases.push_back([&] {
		INT a = INT("5489548657467564756744343434346");
		std::cout << "a:" << a.toString() << std::endl;

		INT b = a << 343;
		std::cout << "b:" << b.toString() << std::endl;
		a <<= 343;
		assert(a == b);
		std::cout << "a:" << a.toString() << std::endl;

		b = a >> 249;
		a >>= 249;
		std::cout << "a:" << a.toString() << std::endl;
		std::cout << "b:" << b.toString() << std::endl;
		assert(a == b);

		std::cout << "shift assign test1  passed" << std::endl;
		});

	testCases.push_back([&] {
		INT a = INT("5489548657467564756744343434346");
		INT b = INT("53423434343343489548657467564756744343434346");
		std::cout << "a:" << a.toString() << std::endl;
		std::cout << "b:" << b.toString() << std::endl;

		INT c = a;
		c += b;
		std::cout << "c:" << c.toString() << std::endl;
		assert(c == a + b);

		c = a;
		c -= b;
		std::cout << "c:" << c.toString() << std::endl;
		assert(c == a - b);
		assert(c + b == a);

		c = a;
		c *= b;
		std::cout << "c:" << c.toString() << std::endl;
		assert(c == a * b);

		c = a;
		c /= b;
		std::cout << "c:" << c.toString() << std::endl;
		assert(c == a / b);

		c = a;
		c %= b;
		std::cout << "c:" << c.toString() << std::endl;
		assert(c == a % b);

		std::cout << "assignment test2  passed" << std::endl;
		});

	testCases.push_back([&] {
		INT x = INT("433847982374873847387483478374837438748347837482434243324234234");
		for (int i = 2; i <= 35; ++i) {
			auto str = x.toString(i);
			std::cout << "base" << i << ":" << str << std::endl;
			assert(INT(str, i) == x);
		}
		std::cout << "any base test1  passed" << std::endl;
		});

	testCases.push_back([&] {
		INT x = INT("432434243324234234");
		INT y = x;
		int N = 1000;
		for (int i = 0; i < 1000; ++i) {
			auto x_i = x << i;
			assert(x_i == y);
			y *= 2;
		}
		std::cout << "shift left test1 passed" << std::endl;

		x = y;
		for (int i = 0; i < N; ++i) {
			auto x_i = x >> i;
			assert(x_i == y);
			y /= 2;
		}
		std::cout << "shift right test1 passed" << std::endl;
		});

	testCases.push_back([&] {
		INT x;
		INT y;
		INT a = INT("1334247873434343434343434892374873423423432434343443243434343423423423424334");
		INT b = INT("23423434343434343434343434343434");
		INT q, r;
		divide(a, b, q, r);
		std::cout << "q:" << q.toString() << std::endl;
		std::cout << "r:" << r.toString() << std::endl;
		INT c = b * q + r;
		std::cout << "a:" << a.toString() << std::endl;
		std::cout << "c:" << c.toString() << std::endl;
		assert(a == c);
		std::cout << "big number * and / test1 passed" << std::endl;
		});

	testCases.push_back([&] {
		INT x;
		INT y;
		INT a = INT("1334247873434343434343434892374873423423432434343443243434343423423423424334");
		INT b = INT("9343243424343424324234234343243234343434343434343434343423432423432423443243434");
		for (int i = 0; i < 1; ++i) {
			INT x = (a + i) * b;
			auto x0 = x;
			for (int i = 0; i < 5; ++i) {
				x *= x;
			}

			for (int i = 0; i < 32; ++i) {
				x /= x0;
			}
			cout << "x:" << x.toString() << endl;
			assert(x == 1);
		}
		std::cout << "big number * and / test2 passed" << std::endl;
		});
}

void addFracTest() {
	testCases.push_back([&] {
		FRAC f;
		std::cout << "f:" << f.toString() << std::endl;
		std::cout << "FRAC default constructor test pass" << std::endl;
		});

	testCases.push_back([&] {
		FRAC f = -3;
		std::cout << "f:" << f.toString() << std::endl;
		FRAC g =  3434323434;
		std::cout << "g:" << g.toString() << std::endl;
		std::cout << "FRAC constructor[int] test pass" << std::endl;
		});

	testCases.push_back([&] {
		FRAC f = -34234323423LL;
		std::cout << "f:" << f.toString() << std::endl;
		FRAC g =  34343343434243243LL;
		std::cout << "g:" << g.toString() << std::endl;
		std::cout << "FRAC constructor[long long] test pass" << std::endl;
		});

	testCases.push_back([&] {
		FRAC f(342432434, 324234234);
		std::cout << "f:" << f.toString() << std::endl;
		std::cout << "FRAC constructor[long long] test pass" << std::endl;
		});

	testCases.push_back([&] {
		INT a = INT("343243434343434343434242348347384734736000");
		INT b = INT("7343434343434343434343800");
		std::cout << "a:" << a.toString() << std::endl;
		std::cout << "b:" << a.toString() << std::endl;
		FRAC f(a, b);
		std::cout << "f:" << f.toString() << std::endl;
		assert(f.d() * a == f.n() * b);
		std::cout << "FRAC [INT,INT]constructor test pass" << std::endl;
		});

	testCases.push_back([&] {
		std::string strA = "1342";
		auto a = INT(strA);
		std::cout << "a:" << a.toString() << std::endl;
		std::string strB = "42";
		auto b = INT(strB);
		std::cout << "b:" << b.toString() << std::endl;
		FRAC f = FRAC(strA + '/' + strB);
		std::cout << "f:" << f.toString() << std::endl;
		assert(a * f.d() == b * f.n());
		std::cout << "FRAC [string,int]constructor test2 pass" << std::endl;
		});

	testCases.push_back([&] {
		std::string strA = "3434234798234723748324782734823748374834";
		auto a = INT(strA);
		std::cout << "a:" << a.toString() << std::endl;
		std::string strB = "987238472673463746347346374637467364763746374673";
		auto b = INT(strB);
		std::cout << "b:" << b.toString() << std::endl;
		FRAC f = FRAC(strA + '/' + strB);
		std::cout << "f:" << f.toString() << std::endl;
		assert(a * f.d() == b * f.n());
		std::cout << "FRAC [string,int]constructor test2 pass" << std::endl;
		});

	testCases.push_back([&] {
		FRAC f = FRAC("-343434343434342/2343422221");
		std::cout << "f:" << f.toString() << std::endl;
		assert(f.isNegative());
		assert(!f.isZero());
		assert(!f.isPositive());
		std::cout << "FRAC negation test1 passe" << std::endl;
		});

	testCases.push_back([&] {
		FRAC f = FRAC("3424343492489328493849384938943434");
		std::cout << "f:" << f.toString() << std::endl;
		FRAC g = FRAC("374827348738478234738783748374837847384784");
		std::cout << "g:" << g.toString() << std::endl;
		auto x = f / g;
		auto y = g / f;
		std::cout << "x:" << x.toString() << std::endl;
		std::cout << "y:" << y.toString() << std::endl;
		assert(x * y == 1);
		assert(x.d() == y.n() && x.n() == y.d());
		x.inverse();
		assert(x == y);

		std::cout << "FRAC divide test1 passe" << std::endl;
		});

	testCases.push_back([&] {
		FRAC q("1/32343439");
		FRAC s = 0;
		FRAC q_i = 1;
		for (int i = 0; i < 3; ++i) {
			s += q_i;
			q_i *= q;
		}
		std::cout << "s:" << s.toString() << std::endl;
		assert(s == (1-q_i)/(1-q));
		std::cout << "FRAC divide test2 passe" << std::endl;
		});

	testCases.push_back([&] {
		auto x = FRAC(1, 2) + FRAC(1, 3);
		std::cout << "x:" << x.toString() << std::endl;
		assert(x == FRAC("5/6"));
		std::cout << "FRAC plus test1 passe" << std::endl;
		});

}

void getAn(FRAC& A, int n, int a) {
	A.setZero();
	static FRAC q;
	static FRAC u;
	static FRAC tmp;
	static FRAC a_2;
	a_2 = a * a;
	for (int i = 1; i <= n; ++i) {
		if (i == 1) {
			u = q.set(1, a);
		}
		else
		{
			q = divide(q, a_2, tmp);
			divide(q, 2 * i - 1, u);
			if (i % 2 == 0) {
				u.negate();
			}
		}
		A = real::plus(A, u, tmp);
	}
}

//(2n-1)q^(2*n-1) > p * 10^k
int getNFor_q_with_p(int k, int q, int p) {
	static INT tmp;
	static INT ten_k;
	ten_k = 1;

	for (int i = 0; i < k; ++i) {
		multiply(ten_k, 10, tmp);
		ten_k = tmp;
	}
	static INT right;
	multiply(ten_k, p, right);

	int n = 1;
	static INT q_2n_minus_one;
	q_2n_minus_one = q;

	static  INT q_2;
	q_2 = q * q;
	static INT _2n_minus_one;
	static INT left;
	do {
		multiply(q_2n_minus_one, q_2, tmp);
		q_2n_minus_one = tmp;
		_2n_minus_one = 2 * n - 1;
		multiply(tmp, _2n_minus_one, left);
		if (left > right) {
			return n;
		}
		++n;
	} while (true);

}

std::string& convertToDecimal(std::string& str,  INT& n, INT& d, int N) {
	str.clear();
	static const INT& ten = INT::s_smallInts[10];
	static INT q,r, tmp;

	bool hasAddDot = false;
	int precision = 0;
	do {
		while (n < d) {
			if (!hasAddDot) {
				str += "0.";
				hasAddDot = true;
			}
			else{
				str += "0";
			}
			++precision;
			multiply(n, ten, tmp);
			n = tmp;
		}

		divide(n, d, q, r);
		str += q.toString();
		if (r.isZero()) {
			return str;
		}

		n = r;
		++precision;
		if (precision >= N) {
			return str;
		}

		if (!hasAddDot) {
			str += '.';
			hasAddDot = true;
		}
		multiply(n, ten, tmp);
		n = tmp;
	} while (true);
}

void calculatePiWithFRAC() {
	FRAC An;
	FRAC Am;
	FRAC Pi;
	static FRAC tmp;

	for (int k = 0; k < 100; k += 1) {
		std::cout << "k:" << k << std::endl;
		std::cout << "calculating n for 5..." << std::endl;
		int n = getNFor_q_with_p(k, 5, 16);
		std::cout << "n for 5:" << n << std::endl;

		std::cout << "calculating m for 239..." << std::endl;
		int m = getNFor_q_with_p(k, 239, 4);
		std::cout << "m:" << m << std::endl;

		std::cout << "calucation An for 5..." << n << std::endl;
		getAn(An, n, 5);
		An = multiply(An, 16, tmp);
		//std::cout << "An:" << An.toString() << std::endl;

		std::cout << "calucation An for 239..." << n << std::endl;
		getAn(Am, m, 239);
		Am = multiply(Am, 4, tmp);
		//std::cout << "Am:" << Am.toString() << std::endl;

		std::cout << "calculating Pi..." << std::endl;
		real::subtract(An, Am, Pi);

		//std::cout << "outputing Pi as frac..." << std::endl;
		//std::cout << "Pi(f):" << Pi.toString() << std::endl;

		std::cout << "converting Pi to decimal..." << std::endl;
		std::string strPi;
		INT pi_n = Pi.n();
		INT pi_d = Pi.d();
		convertToDecimal(strPi, pi_n, pi_d, k + 1);
		std::cout << "Pi:" << strPi  <<std::endl;
		/*
		An(A, i, 5);
		std::cout << "i5:" << i << std::endl;
		An(A, i, 239);
		std::cout << "i239:" << i << std::endl;
		*/
	}
	int g = 0;
	std::cin >> g;
}

const INT& getPower(int a, int n, std::vector<const INT*>& cache) {
	if (n == 0) {
		return INT::one;
	}

	int index = n - 1;
	if (index < cache.size()) {
		return *cache[index];
	}


	for (int i = cache.size(); i < n; ++i) {
		INT* x = nullptr;
		if (i == 0) {
			x = new INT(a);
			cache.push_back(x);
		}
		else
		{
			x = new INT();
			multiply(*cache[i - 1], a, *x);
			cache.push_back(x);
		}
	}

	return *cache[index];
}

const INT& getOddFac(int n, std::vector<const INT*>& cache) {
	auto index = n - 1;
	if (index < cache.size()) {
		return *(cache[index]);
	}
	static INT c1;

	if (n == 1) {
		return INT::one;
	}

	for (int i = cache.size(); i <= index; ++i) {
		if (i == 0) {
			cache.push_back(&INT::one);
		}
		else {
			auto x = new INT();
			multiply(*cache[i - 1], 2 * i + 1, *x);
			cache.push_back(x);
		}
	}
	return *(cache[index]);
}

void getIntAn(INT& P, INT& Q, int n, int a, std::vector<const INT*>& facCache) {
	std::vector<const INT*> cache;
	std::cout << "getOddFac" << std::endl;
	INT Pn = getOddFac(n, facCache);
	INT Qn; 
	std::cout << "getPower" << std::endl;
	INT c =getPower(a * a,  n - 1, cache);
	multiply(c, a, Qn);
	INT U;
	INT tmp;
	INT tmp1;
	P.setZero();

	for (int i = 1; i <= n; ++i) {
		//std::cout << "i:" << i << std::endl;
		const INT& qn = getPower(a*a, n - i, cache);
		real::divide(Pn, 2 * i - 1, tmp, tmp1);

		if (i % 2 == 0) {
			tmp.negate();
		}
		multiply(qn, tmp, tmp1);
		P = real::plus(P, tmp1, tmp);
	}

	multiply(Pn, Qn, Q);
	//release memory: well it's a toy, but it occupies lots of memory when the number is large
	for (auto x : cache) {
		delete x;
	}
	cache.clear();
}

void calculatePiWithINT() {
	FRAC Pi;
	INT Pn, Qn, Pm, Qm;
	INT tmp;
	INT c1, c2, c3, c4;
	std::vector<const INT*> facCache;

	for (int k = 20; k <= 30; k += 10) {
		const clock_t begin_time = clock();
		std::cout << "k:" << k << std::endl;
		std::cout << "calculating n for 5..." << std::endl;
		int n = getNFor_q_with_p(k + 1, 5, 16);
		std::cout << "n for 5:" << n << std::endl;

		std::cout << "calculating m for 239..." << std::endl;
		int m = getNFor_q_with_p(k + 1, 239, 4);
		std::cout << "m:" << m << std::endl;

		std::cout << "calucation An for 5..." << n << std::endl;
		getIntAn(Pn, Qn, n, 5, facCache);
		Pn = multiply(Pn, 16, tmp);
		//std::cout << "An:" << Pn.toString() << "/" << Qn.toString() << std::endl;

		std::cout << "calucation An for 239..." << n << std::endl;
		getIntAn(Pm, Qm, m, 239, facCache);
		Pm = multiply(Pm, 4, tmp);
		//std::cout << "Am:" << Pm.toString() << "/" << Qm.toString() << std::endl;
		//release memory
		std::cout << "release mem for cache" << std::endl;
		for (auto x : facCache) {
			if (x != &INT::one) {
				delete x;
			}
		}
		facCache.clear();

		std::cout << "calculating Pi..." << std::endl;
		multiply(Pn, Qm, c1);
		multiply(Pm, Qn, c2);

		real::subtract(c1, c2, c3);
		multiply(Qn, Qm, c4);

		//std::cout << "outputing Pi as frac..." << std::endl;
		//std::cout << "Pi(f):" << Pi.toString() << std::endl;

		std::cout << "converting Pi to decimal..." << std::endl;
		std::string strPi;
		convertToDecimal(strPi, c3, c4, k + 1);
		std::cout << "Pi(" << k <<"):" << strPi  <<std::endl;

		std::cout << "Using time(" << k << "):" << float(clock() - begin_time) / CLOCKS_PER_SEC << std::endl;
	}
}

void calculate_e_withINT() {
	for (int k = 200000; k <= 10000000; k += 100000) {
		const clock_t begin_time = clock();
		int n = 1;
		INT fac_n = INT::one;
		INT fac_n_minus_one = INT::one;
		INT* fac = &fac_n;
		INT ten_p = INT::one;
		INT c;
		const INT ten = 10;
		std::cout << "k:" << k << std::endl;
		std::cout << "calculating 10 to power k + 2..." << std::endl;
		for (int i = 0; i < k + 2; ++i) {
			ten_p = multiply(ten_p, ten, c);
		}

		std::cout << "calculating n ..." << std::endl;
		while (fac_n < ten_p) {
			++n;
			fac_n_minus_one = fac_n;
			multiply(fac_n_minus_one, n, fac_n);
			if (fac_n >= ten_p) {
				multiply(fac_n_minus_one, n - 1, c);
				if (c >= ten_p) {
					fac = &fac_n_minus_one;
					n--;
				}
				break;
			}
		}
		std::cout << "n:" << n << std::endl;

		std::cout << "calculating e ..." << std::endl;
		INT u = INT::one;
		INT s;
		for (int i = n; i >= 2; --i) {
			s = real::plus(s, u, c);
			if (i > 2) {
				u = multiply(u, i, c);
			}
		}

		std::cout << "converting e to frac..." << std::endl;
		FRAC e(s, *fac);
		INT x = e.n();
		INT y = e.d();

		std::cout << "converting e to decimal..." << std::endl;
		std::string strE;
		convertToDecimal(strE, x, y, k + 1);
		if (strE[0] == '0') {
			strE[0] = '2';
		}
		std::cout << "e(" << k <<"):" << strE  <<std::endl;

		std::cout << "Using time(" << k << "):" << float(clock() - begin_time) / CLOCKS_PER_SEC << std::endl;
	}
}

void addPiTestWithINT() {
	testCases.push_back([&] {
		calculatePiWithINT();
		});
}

void addPiTestWithFRAC() {
	testCases.push_back([&] {
		calculatePiWithINT();
		});
}
void addIntTest_calculate_e(){
	testCases.push_back([&] {
		calculate_e_withINT();
		});
}
int main()
{
//	addRealTest();

	//the test case that calculates pi with REAL class
//	addPiTestWithREAL();

	//general test cases for INT class
	addIntTest();

	//general test cases for FRAC class
	addFracTest();

	//the test case that calculates pi with pure INT operation
	addPiTestWithINT();

	//the test case that calulates pi with FRAC operation
	//addPiTestWithFRAC();

	//the test case that calculates e with INT operation
	//addIntTest_calculate_e();

	bool reverseOrder = true;

	if (reverseOrder) {
		for (auto it = testCases.rbegin(); it != testCases.rend(); ++it) {
			std::cout << "***************************************************" << std::endl;
			(*it)();
			std::cout << "###################################################" << std::endl << std::endl;
		}
	}
	else {
		for (auto it = testCases.begin(); it != testCases.end(); ++it) {
			std::cout << std::endl << "***************************************************" << std::endl;
			(*it)();
			std::cout << "###################################################" << std::endl << std::endl;
		}
	}
	return 0;
	

}