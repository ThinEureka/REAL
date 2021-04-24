
#include "Frac.h"
#include "Int.h"
#include "algorithm.h"

#include <assert.h> 

using namespace real;

std::string Frac::toString(int base) const {
	if (isZero()) {
		return "0";
	}
	else {
		std::string str;
		if (_sign < 0) {
			str += "-";
		}
		str += _n.toString(base);
		if (_d != Int::one) {
			str += '/';
			str += _d.toString(base);
		}
		return str;
	}
}

Frac& Frac::set(const std::string& str, int base) {
	auto pos = str.find('/');
	if (pos == std::string::npos) {
		_sign = 1;
		_n.set(str, base);
		_d.setOne();
		normalize();
		return *this;
	}

	assert(pos < str.size() - 1);
	auto strN = str.substr(0, pos);
	auto strD = str.substr(pos + 1, str.size() - pos - 1);
	_d.set(strD, base);
	assert(!_d.isZero());
	_n.set(strN, base);
	_sign = 1;
	normalize();
	return *this;
}

Frac& real::add(const Frac& v1, const Frac& v2, Frac& sum) {
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
	add(sum._c1, sum._c2, sum._n);
	multiply(v1._d, v2._d, sum._d);
	sum._sign = 1;
	sum.normalize();
	return sum;
}

Frac& real::subtract(const Frac& v1, const Frac& v2, Frac& sub) {
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
	add(sub._c1, sub._c2, sub._n);
	multiply(v1._d, v2._d, sub._d);
	sub._sign = 1;
	sub.normalize();
	return sub;
}

Frac& real::multiply(const Frac& v1, const Frac& v2, Frac& product) {
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

Frac& real::divide(const Frac& v1, const Frac& v2, Frac& q) {
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

void Frac::normalize() {
	if (_n.isZero()) {
		clear();
		return;
	}

	_sign = _n.sign() == _d.sign() ? _sign : -_sign;
	if (_n.isNegative()) {
		_n.negate();
	}

	if (_d.isNegative()) {
		_d.negate();
	}

	real::gcd(_n, _d, _c1, _c2, _c3, _c4);
	if (_c1 != Int::one) {
		divide(_n, _c1, _c2, _c3);
		_n = _c2;

		divide(_d, _c1, _c2, _c3);
		_d = _c2;
	}
}
