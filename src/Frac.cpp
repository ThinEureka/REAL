
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

	multiply(v1._n, v2._d, sum.f1()._n);
	if (v1._sign < 0) {
		sum.f1()._n.negate();
	}
	multiply(v1._d, v2._n, sum.f2()._n);
	if (v2._sign < 0) {
		sum.f2()._n.negate();
	}
	add(sum.f1()._n, sum.f2()._n, sum._n);
	multiply(v1._d, v2._d, sum._d);
	sum._sign = 1;
	sum.normalize();

	sum.f1().setZero();
	sum.f2().setZero();

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
	multiply(v1._n, v2._d, sub.f1()._n);
	if (v1._sign < 0) {
		sub.f1()._n.negate();
	}
	multiply(v1._d, v2._n, sub.f2()._n);
	if (v2._sign > 0) {
		sub.f2()._n.negate();
	}
	add(sub.f1()._n, sub.f2()._n, sub._n);
	multiply(v1._d, v2._d, sub._d);
	sub._sign = 1;
	sub.normalize();
	sub.f1().setZero();
	sub.f2().setZero();
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

	Int& _c1 = f1()._n;
	Int& _c2 = f1()._d;
	Int& _c3 = f2()._n;
	Int& _c4 = f2()._d;

	real::gcd(_n, _d, _c1, _c2, _c3, _c4);
	if (_c1 != Int::one) {
		divide(_n, _c1, _c2, _c3);
		_n.swap(_c2);

		divide(_d, _c1, _c2, _c3);
		_d.swap(_c2);
	}

	_c1.setZero();
	_c2.setZero();
	_c3.setZero();
	_c4.setZero();
}
