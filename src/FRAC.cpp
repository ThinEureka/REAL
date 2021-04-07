
#include "FRAC.h"
#include "INT.h"
#include "algorithm.h"

#include <assert.h> 

using namespace zju04nycs;

std::string FRAC::toString(int base) const {
	if (isZero()) {
		return "0";
	}
	else {
		std::string str;
		if (_sign < 0) {
			str += "-";
		}
		str += _n.toString(base);
		if (_d != INT::one) {
			str += '/';
			str += _d.toString(base);
		}
		return str;
	}
}

void FRAC::setValueWithString(const std::string& str, int base) {
	auto pos = str.find('/');
	if (pos == std::string::npos) {
		_sign = 1;
		_n.setValueWithString(str, base);
		_d.setOne();
		normalize();
	}

	assert(pos < str.size() - 1);
	auto strN = str.substr(0, pos);
	auto strD = str.substr(pos + 1, str.size() - pos - 1);
	_d.setValueWithString(strD, base);
	assert(!_d.isZero());
	_n.setValueWithString(strN, base);
	_sign = 1;
	normalize();
}

FRAC& zju04nycs::plus(const FRAC& v1, const FRAC& v2, FRAC& sum) {
	if (v1.isZero()) {
		sum = v2;
		return sum;
	}

	if (v2.isZero()) {
		sum = v1;
		return sum;
	}

	sum.clear();

	multiply(v1._n, v2._d, sum._c1);
	if (v1._sign < 0) {
		sum._c1.negate();
	}
	multiply(v1._d, v2._n, sum._c2);
	if (v2._sign < 0) {
		sum._c2.negate();
	}
	plus(sum._c1, sum._c2, sum._n);
	multiply(v1._d, v2._d, sum._d);
	sum.normalize();
	return sum;
}

FRAC& zju04nycs::subtract(const FRAC& v1, const FRAC& v2, FRAC& sub) {
	if (v2.isZero()) {
		sub = v1;
		return sub;
	}

	if (v1.isZero()) {
		sub = v2;
		sub.negate();
		return sub;
	}

	sub.clear();
	multiply(v1._n, v2._d, sub._c1);
	if (v1._sign < 0) {
		sub._c1.negate();
	}
	multiply(v1._d, v2._n, sub._c2);
	if (v2._sign > 0) {
		sub._c2.negate();
	}
	plus(sub._c1, sub._c2, sub._n);
	multiply(v1._d, v2._d, sub._d);
	sub.normalize();
	return sub;
}

FRAC& zju04nycs::multiply(const FRAC& v1, const FRAC& v2, FRAC& product) {
	if (v1.isZero() || v2.isZero()) {
		product.setZero();
		return product;
	}

	product._sign = v1._sign > 0 ? v2._sign : -v2._sign;
	multiply(v1._n, v2._n, product._n);
	multiply(v1._d, v2._d, product._d);
	product.normalize();
	return product;
}

FRAC& zju04nycs::divide(const FRAC& v1, const FRAC& v2, FRAC& q) {
	assert(!v2.isZero());
	if (v1.isZero()) {
		q.setZero();
		return q;
	}

	q._sign = v1._sign > 0 ? v2._sign : -v2._sign;
	multiply(v1._n, v2._d, q._n);
	multiply(v1._d, v2._n, q._d);
	q.normalize();
	return q;
}
void FRAC::normalize() {
	if (_n.isZero()) {
		clear();
		return;
	}

	_sign = _n.sign() == _d.sign() ? 1 : -1;
	if (_n.isNegative()) {
		_n.negate();
	}

	if (_d.isNegative()) {
		_d.negate();
	}

	auto& gcd = _c1;
	zju04nycs::gcd(_n, _d, gcd, _c2, _c3, _c4, _c5);
	if (gcd != INT::one) {
		divide(_n, gcd, _c2, _c3);
		_n = _c2;

		divide(_d, gcd, _c2, _c3);
		_d = _c2;
	}
}
