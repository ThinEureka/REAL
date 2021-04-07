
#include "algorithm.h"
#include "INT.h"

using namespace zju04nycs;

INT& zju04nycs::gcd(const INT& m, const INT& n, INT& gcd,
	INT& c1, INT& c2, INT& c3, INT& c4) {
	if (m == INT::one){
		return gcd = n;
	}

	if (n == INT::one) {
		return gcd = m;
	}

	c1 = m;
	c2 = n;
	while (true) {
		divide(c1, c2, c3, c4);
		if (c4.isZero()) {
			return gcd = n;
		}
		c1 = c2;
		c2 = c3;
	}
}
