
#include "algorithm.h"
#include "INT.h"

using namespace real;

INT& real::gcd(const INT& m, const INT& n, INT& gcd,
	INT& c1, INT& c2, INT& c3) {
	if (m == INT::one){
		return gcd.setOne();
	}

	if (n == INT::one) {
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

		c1 = gcd;
		gcd = c3;
	}
}
