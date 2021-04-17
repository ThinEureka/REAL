#include "Int.h"
#include "Frac.h"
#include "algorithm"
#include <iostream>
#include <functional>
#include <vector>

using namespace real;
using namespace std;

static std::vector<std::function<void()>> testCases;

void addIntTest() {
	testCases.push_back([&] {
		Int n;
		assert(n.isZero());
		assert(!n.isPositive());
		assert(!n.isNegative());
		std::cout << "zero test passed" << std::endl;
		});

	testCases.push_back([&] {
		Int one = 1;
		Int one_ = Int("1");
		assert(one == one_);
		assert(!(one != one_));
		std::cout << "one test passed" << std::endl;
		});


	testCases.push_back([&] {
		std::string s1 = "12345";
		Int x1 = Int(s1);
		std::string x1_s = x1.toString();
		std::cout << "x1:" << x1_s << std::endl;
		assert(x1_s == s1);
		std::cout << "simple string constructor test1 passed" << std::endl;
		});

	testCases.push_back([&] {
		Int x = 342343434343;
		Int y = x;
		assert(x == y);
		assert(!(x != y));
		assert((x ^ y) == 0);
		assert((x ^ y).isZero());
		std::cout << "simple string constructor test2 passed" << std::endl;
		});

	testCases.push_back([&] {
		Int x = Int("-342343434343432434234324234");
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
		Int x = -3423434343434324343LL;
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
		Int x = Int(-3423434343434324343LL);
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
		Int x = Int(-3423434343434324343LL);
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
		int p = static_cast<int>(-34324324234);
		Int x = Int(p);
		bool isOverflow = false;
		assert(x.toInt(isOverflow) == p);
		std::cout << "to int test1 passed" << std::endl;
		});

	testCases.push_back([&] {
		int p = static_cast<int>(34324324234);
		Int x = Int(-p);
		bool isOverflow = false;
		assert(x.toInt(isOverflow) == -p);
		std::cout << "to int test2 passed" << std::endl;
		});

	testCases.push_back([&] {
		long long p = -34324324344324234;
		Int x = Int(p);
		bool isOverflow = false;
		assert(x.toLongLong(isOverflow) == p);
		std::cout << "to long long test1 passed" << std::endl;
		});

	testCases.push_back([&] {
		std::string p = "101011010101010101010101101010101011";
		Int x = Int(p, 2);
		std::cout << "x:" << x.toString(2) << std::endl;
		for (size_t i = 0; i < p.size(); ++i) {
			char bit = x.bit(i) ? '1' : '0';
			assert(bit == p[p.size() - 1 -i]);
		}
		std::cout << "bit test 1 passed" << std::endl;
		});

	testCases.push_back([&] {
		std::string p = "101011010101010101010101101010101011";
		Int x = Int(p, 2);
		std::cout << "x:" << x.toString(2) << std::endl;
		Int y = x - 1;
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
		Int x = Int(-p);
		bool isOverflow = false;
		assert(x.toLongLong(isOverflow) == -p);
		std::cout << "to long long test2 passed" << std::endl;
		});

	testCases.push_back([&] {
		std::string s1 = "12345";
		Int x1 = Int(s1);
		std::string x1_s = x1.toString();
		std::cout << "x1:" << x1_s << std::endl;
		assert(x1_s == s1);
		std::cout << "simple string constructor test passed" << std::endl;
		});

	testCases.push_back([&] {
		std::string s6 = "400412878166003164546735534092609172319140330852536603355429981136254443797343719815103359446869847213482227876740797726949481901457564006563326790056796257840594244649848624543981509868730249924";
		Int x6 = Int(s6);
		auto x6_s = x6.toString();
		cout << "x6:" << x6_s << endl;
		assert(x6_s == s6);
		cout << "long string constructor test 6 passed" << endl;
		});

	testCases.push_back([&] {
		Int N = Int("100");
		Int sum;
		for (Int i = 0; i <= N; i = i + 1) {
			sum += i;
		}
		auto sum_s = sum.toString();
		std::cout << "sum:" << sum_s << std::endl;
		assert(sum == N * (N + 1) / 2);
		cout << "sum test 1 passed" << endl;
		});

	testCases.push_back([&] {
		Int N2 = Int("200000");
		Int sum2;
		for (Int i = 0; i <= N2; i = i + 1) {
			sum2 += i;
		}
		auto sum2_s = sum2.toString();
		std::cout << "sum2:" << sum2_s << std::endl;
		assert(sum2 == N2 * (N2 + 1) / 2);
		cout << "sum test 2 passed" << endl;
		});

	testCases.push_back([&] {
		 Int x = Int("999999999999");
		 Int y = x;
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
		Int a = Int("343483463746736473467346736476");
		Int b = Int("364826374736436476374676763746");
		Int c = Int("342349384938483843874839");

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
		Int x7 = Int(s7, 2);
		auto x7_s = x7.toString(2);
		cout << "x7:" << x7_s << std::endl;
		assert(x7_s == s7);
		cout << "base 2 string construtor test passed\n";

		Int y7 = x7 ^ x7;
		cout << "y7:" << y7.toString() << std::endl;
		assert(y7.isZero());
		cout << "xor test7 passed" << endl;
		});

	testCases.push_back([&] {
		long long p2 = 123456789012345333LL;
		Int x2 = Int(p2);
		auto x2_s = x2.toString();
		std::cout << "x2:" << x2_s << std::endl;
		std::cout << "p2:" << p2 << std::endl;
		assert(std::to_string(p2) == x2_s);
		std::cout << "long long constructor test passed" << std::endl;
		});

	testCases.push_back([&] {
		Int x3 = Int("123456789012345333");
		std::cout << "x3:" << x3.toString() << std::endl;

		std::string s4 = "12345678901234";
		Int x4 = Int(s4);
		auto x4_s = x4.toString();
		std::cout << "x4:" << x4_s << std::endl;
		assert(x4_s == s4);
		cout << "string constructor test4 passed" << std::endl;

		std::string s5 = "32433443423434783432432423423423423423423423423423423423472364723647236476274623764723674623746764237647264723476767236472364782374827384723478324324324243343243243242342432423432434";
		Int x5 = Int(s5);
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
		Int x = Int("10011100101001010100100100101001010100101001010101010010101010010011001010001010100110011001", 2);
		Int y = Int("1101111001010101010101010101010010000101011001100100101000111110010010", 2);
		Int x0 = x ^ x;
		std::cout << "x:" << x.toString(2) << std::endl;
		std::cout << "y:" << y.toString(2) << std::endl;
		std::cout << "x0:" << x0.toString(2) << std::endl;
		assert(x0 == Int(0));
		assert((x | x) == x);
		assert((x& x) == x);
		std::cout << "x&y:" << (x & y).toString(2) << std::endl;
		std::cout << "x|y:" << (x | y).toString(2) << std::endl;
		std::cout << "x^y:" << (x ^ y).toString(2) << std::endl;
		std::cout << "bit operation test 1 passed" << std::endl;
		});

	testCases.push_back([&] {
		Int a = Int("342343243647264736476374673647364736476");
		Int b = Int("3434343434334343434343434");

		std::cout << "a:" << a.toString() << std::endl;
		std::cout << "b:" << b.toString() << std::endl;

		Int q, r;
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
		Int a = Int("5489548657467564756744343434346");
		std::cout << "a:" << a.toString() << std::endl;

		Int b = a << 343;
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
		Int a = Int("5489548657467564756744343434346");
		Int b = Int("53423434343343489548657467564756744343434346");
		std::cout << "a:" << a.toString() << std::endl;
		std::cout << "b:" << b.toString() << std::endl;

		Int c = a;
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
		Int x = Int("433847982374873847387483478374837438748347837482434243324234234");
		for (int i = 2; i <= 35; ++i) {
			auto str = x.toString(i);
			std::cout << "base" << i << ":" << str << std::endl;
			assert(Int(str, i) == x);
		}
		std::cout << "any base test1  passed" << std::endl;
		});

	testCases.push_back([&] {
		Int x = Int("432434243324234234");
		Int y = x;
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
		Int x;
		Int y;
		Int a = Int("1334247873434343434343434892374873423423432434343443243434343423423423424334");
		Int b = Int("23423434343434343434343434343434");
		Int q, r;
		divide(a, b, q, r);
		std::cout << "q:" << q.toString() << std::endl;
		std::cout << "r:" << r.toString() << std::endl;
		Int c = b * q + r;
		std::cout << "a:" << a.toString() << std::endl;
		std::cout << "c:" << c.toString() << std::endl;
		assert(a == c);
		std::cout << "big number * and / test1 passed" << std::endl;
		});

	testCases.push_back([&] {
		Int x;
		Int y;
		Int a = Int("1334247873434343434343434892374873423423432434343443243434343423423423424334");
		Int b = Int("9343243424343424324234234343243234343434343434343434343423432423432423443243434");
		for (int i = 0; i < 1; ++i) {
			Int x = (a + i) * b;
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
		Frac f;
		std::cout << "f:" << f.toString() << std::endl;
		std::cout << "Frac default constructor test pass" << std::endl;
		});

	testCases.push_back([&] {
		Frac f = -3;
		std::cout << "f:" << f.toString() << std::endl;
		Frac g =  3434323434;
		std::cout << "g:" << g.toString() << std::endl;
		std::cout << "Frac constructor[int] test pass" << std::endl;
		});

	testCases.push_back([&] {
		Frac f = -34234323423LL;
		std::cout << "f:" << f.toString() << std::endl;
		Frac g =  34343343434243243LL;
		std::cout << "g:" << g.toString() << std::endl;
		std::cout << "Frac constructor[long long] test pass" << std::endl;
		});

	testCases.push_back([&] {
		Frac f(342432434, 324234234);
		std::cout << "f:" << f.toString() << std::endl;
		std::cout << "Frac constructor[long long] test pass" << std::endl;
		});

	testCases.push_back([&] {
		Int a = Int("343243434343434343434242348347384734736000");
		Int b = Int("7343434343434343434343800");
		std::cout << "a:" << a.toString() << std::endl;
		std::cout << "b:" << a.toString() << std::endl;
		Frac f(a, b);
		std::cout << "f:" << f.toString() << std::endl;
		assert(f.d() * a == f.n() * b);
		std::cout << "Frac [Int,Int]constructor test pass" << std::endl;
		});

	testCases.push_back([&] {
		std::string strA = "1342";
		auto a = Int(strA);
		std::cout << "a:" << a.toString() << std::endl;
		std::string strB = "42";
		auto b = Int(strB);
		std::cout << "b:" << b.toString() << std::endl;
		Frac f = Frac(strA + '/' + strB);
		std::cout << "f:" << f.toString() << std::endl;
		assert(a * f.d() == b * f.n());
		std::cout << "Frac [string,int]constructor test2 pass" << std::endl;
		});

	testCases.push_back([&] {
		std::string strA = "3434234798234723748324782734823748374834";
		auto a = Int(strA);
		std::cout << "a:" << a.toString() << std::endl;
		std::string strB = "987238472673463746347346374637467364763746374673";
		auto b = Int(strB);
		std::cout << "b:" << b.toString() << std::endl;
		Frac f = Frac(strA + '/' + strB);
		std::cout << "f:" << f.toString() << std::endl;
		assert(a * f.d() == b * f.n());
		std::cout << "Frac [string,int]constructor test2 pass" << std::endl;
		});

	testCases.push_back([&] {
		Frac f = Frac("-343434343434342/2343422221");
		std::cout << "f:" << f.toString() << std::endl;
		assert(f.isNegative());
		assert(!f.isZero());
		assert(!f.isPositive());
		std::cout << "Frac negation test1 passe" << std::endl;
		});

	testCases.push_back([&] {
		Frac f = Frac("3424343492489328493849384938943434");
		std::cout << "f:" << f.toString() << std::endl;
		Frac g = Frac("374827348738478234738783748374837847384784");
		std::cout << "g:" << g.toString() << std::endl;
		auto x = f / g;
		auto y = g / f;
		std::cout << "x:" << x.toString() << std::endl;
		std::cout << "y:" << y.toString() << std::endl;
		assert(x * y == 1);
		assert(x.d() == y.n() && x.n() == y.d());
		x.inverse();
		assert(x == y);

		std::cout << "Frac divide test1 passe" << std::endl;
		});

	testCases.push_back([&] {
		Frac q("1/32343439");
		Frac s = 0;
		Frac q_i = 1;
		for (int i = 0; i < 3; ++i) {
			s += q_i;
			q_i *= q;
		}
		std::cout << "s:" << s.toString() << std::endl;
		assert(s == (1-q_i)/(1-q));
		std::cout << "Frac divide test2 passe" << std::endl;
		});

	testCases.push_back([&] {
		auto x = Frac(1, 2) + Frac(1, 3);
		std::cout << "x:" << x.toString() << std::endl;
		assert(x == Frac("5/6"));
		std::cout << "Frac plus test1 passe" << std::endl;
		});

}

void getAn(Frac& A, int n, int a) {
	A.setZero();
	static Frac q;
	static Frac u;
	static Frac tmp;
	static Frac a_2;
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
	static Int tmp;
	static Int ten_k;
	ten_k = 1;

	for (int i = 0; i < k; ++i) {
		multiply(ten_k, 10, tmp);
		ten_k = tmp;
	}
	static Int right;
	multiply(ten_k, p, right);

	int n = 1;
	static Int q_2n_minus_one;
	q_2n_minus_one = q;

	static  Int q_2;
	q_2 = q * q;
	static Int _2n_minus_one;
	static Int left;
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

std::string& convertToDecimal(std::string& str,  Int& n, Int& d, int N) {
	str.clear();
	static const Int& ten = Int::s_smallInts[10];
	static Int q,r, tmp;

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

void calculatePiWithFrac() {
	Frac An;
	Frac Am;
	Frac Pi;
	static Frac tmp;

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
		Int pi_n = Pi.n();
		Int pi_d = Pi.d();
		convertToDecimal(strPi, pi_n, pi_d, k + 1);
		std::cout << "Pi:" << strPi  <<std::endl;
		/*
		An(A, i, 5);
		std::cout << "i5:" << i << std::endl;
		An(A, i, 239);
		std::cout << "i239:" << i << std::endl;
		*/
	}
}

const Int& getPower(int a, int n, std::vector<const Int*>& cache) {
	if (n == 0) {
		return Int::one;
	}

	int index = n - 1;
	if (index < static_cast<int>(cache.size())) {
		return *cache[index];
	}


	for (int i = static_cast<int>(cache.size()); i < n; ++i) {
		Int* x = nullptr;
		if (i == 0) {
			x = new Int(a);
			cache.push_back(x);
		}
		else
		{
			x = new Int();
			multiply(*cache[i - 1], a, *x);
			cache.push_back(x);
		}
	}

	return *cache[index];
}

const Int& getOddFac(int n, std::vector<const Int*>& cache) {
	auto index = n - 1;
	if (index < static_cast<int>(cache.size())) {
		return *(cache[index]);
	}
	static Int c1;

	if (n == 1) {
		return Int::one;
	}

	for (int i = static_cast<int>(cache.size()); i <= index; ++i) {
		if (i == 0) {
			cache.push_back(&Int::one);
		}
		else {
			auto x = new Int();
			multiply(*cache[i - 1], 2 * i + 1, *x);
			cache.push_back(x);
		}
	}
	return *(cache[index]);
}

void getIntAn(Int& P, Int& Q, int n, int a, std::vector<const Int*>& facCache) {
	std::vector<const Int*> cache;
	std::cout << "getOddFac" << std::endl;
	Int Pn = getOddFac(n, facCache);
	Int Qn; 
	std::cout << "getPower" << std::endl;
	Int c =getPower(a * a,  n - 1, cache);
	multiply(c, a, Qn);
	Int U;
	Int tmp;
	Int tmp1;
	P.setZero();

	for (int i = 1; i <= n; ++i) {
		//std::cout << "i:" << i << std::endl;
		const Int& qn = getPower(a*a, n - i, cache);
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

void calculatePiWithInt() {
	Frac Pi;
	Int Pn, Qn, Pm, Qm;
	Int tmp;
	Int c1, c2, c3, c4;
	std::vector<const Int*> facCache;

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
			if (x != &Int::one) {
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

void calculate_e_withInt() {
	for (int k = 200000; k <= 10000000; k += 100000) {
		const clock_t begin_time = clock();
		int n = 1;
		Int fac_n = Int::one;
		Int fac_n_minus_one = Int::one;
		Int* fac = &fac_n;
		Int ten_p = Int::one;
		Int c;
		const Int ten = 10;
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
		Int u = Int::one;
		Int s;
		for (int i = n; i >= 2; --i) {
			s = real::plus(s, u, c);
			if (i > 2) {
				u = multiply(u, i, c);
			}
		}

		std::cout << "converting e to frac..." << std::endl;
		Frac e(s, *fac);
		Int x = e.n();
		Int y = e.d();

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

void addPiTestWithInt() {
	testCases.push_back([&] {
		calculatePiWithInt();
		});
}

void addPiTestWithFrac() {
	testCases.push_back([&] {
		calculatePiWithInt();
		});
}
void addIntTest_calculate_e(){
	testCases.push_back([&] {
		calculate_e_withInt();
		});
}
int main()
{
	//general test cases for Int class
	addIntTest();

	//general test cases for Frac class
	addFracTest();

	//the test case that calculates pi with pure Int operation
	addPiTestWithInt();

	//the test case that calulates pi with Frac operation
	//addPiTestWithFrac();

	//the test case that calculates e with Int operation
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