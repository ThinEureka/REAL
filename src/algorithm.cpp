
#include "algorithm.h"
#include "Int.h"

using namespace real;

Int& real::gcd(const Int& m, const Int& n, Int& gcd,
	Int& c1, Int& c2, Int& c3) {
	if (m == Int::one){
		return gcd.setOne();
	}

	if (n == Int::one) {
		return gcd.setOne();
	}

	if (m == n) {
		return gcd = m;
	}

	if (m < n) {
		c1 = n;
		gcd = m;
	}
	else
	{
		c1 = m;
		gcd = n;
	}

	while (true) {
		divide(c1, gcd, c2, c3);
		if (c3.isZero()) {
			return gcd;
		}

		c1.swap(gcd);
		gcd.swap(c3);
	}
}
