#pragma once

#ifndef FRAC_H
#define FRAC_H

#include "INT.h"

namespace zju04nycs {

	class FRAC;
	FRAC operator & (const FRAC& v1, const FRAC& v2);
	FRAC operator | (const FRAC& v1, const FRAC& v2);
	FRAC operator ^ (const FRAC& v1, const FRAC& v2);

	FRAC operator +(const FRAC& v1, const FRAC& v2);
	FRAC operator -(const FRAC& v1, const FRAC& v2);
	FRAC operator *(const FRAC& v1, const FRAC& v2);
	FRAC operator /(const FRAC& v1, const FRAC& v2);

	FRAC& plus(const FRAC& v1, const FRAC& v2, FRAC& sum);
	FRAC& subtract(const FRAC& v1, const FRAC& v2, FRAC& sub);
	FRAC& multiply(const FRAC& v1, const FRAC& v2, FRAC& product);
	FRAC& divide(const FRAC& v1, const FRAC& v2, FRAC& q);

class FRAC {
	public:
		FRAC() {}
		FRAC (const FRAC& v): _sign(v._sign), _n(v._n), _d(v._d) {}
		FRAC (const FRAC&& v) noexcept : _sign(v._sign), _n(v._n), _d(v._d){}

		std::string toString(int base = 10) const;
		void setValueWithString(const std::string& str, int base = 10);

	public:
		FRAC& operator = (const FRAC& v) { _sign = v._sign; _n = v._n; _d = v._d; }
		FRAC& operator = (FRAC&& v) noexcept;
		FRAC& operator = (const FRAC&& v) noexcept;

		bool isZero() const;
		void clear();
		bool isPositive() const;
		bool isNegative() const;

		friend int compare(const FRAC& v1, const FRAC& v2);
		FRAC& negate() { if (!isZero()) { _sign = -_sign; } return *this; };

	public:
		const FRAC operator - () const {
			FRAC v = *this;
			return v.negate();
		}

		friend bool operator == (const FRAC& v1, const FRAC& v2);
		friend bool operator != (const FRAC& v1, const FRAC& v2) {
			return !(v1 == v2);
		}

		friend bool operator < (const FRAC& v1, const FRAC& v2) {
			return compare(v1, v2) < 0;
		}
		friend bool operator > (const FRAC& v1, const FRAC& v2) {
			return compare(v1, v2) > 0;
		}
		friend bool operator <= (const FRAC& v1, const FRAC& v2) {
			return compare(v1, v2) <= 0;
		}
		friend bool operator >= (const FRAC& v1, const FRAC& v2) {
			return compare(v1, v2) >= 0;
		}

		friend FRAC operator +(const FRAC& v1, const FRAC& v2) {
			FRAC sum;
			return plus(v1, v2, sum);
		}

		friend FRAC operator -(const FRAC& v1, const FRAC& v2) {
			FRAC sub;
			return subtract(v1, v2, sub);
		}
		friend FRAC operator *(const FRAC& v1, const FRAC& v2) {
			FRAC product;
			return multiply(v1, v2, product);
		}
		friend FRAC operator /(const FRAC& v1, const FRAC& v2) {
			FRAC q;
			return divide(v1, v2, q);
		}
		
		FRAC& operator += (const FRAC& v1) {
			FRAC sum; 
			return *this = plus(*this, v1, sum);
		}
		FRAC& operator -= (const FRAC& v1) {
			FRAC sub;
			return *this = subtract(*this, v1, sub);
		}
		FRAC& operator *= (const FRAC& v1) {
			FRAC product;
			return *this = multiply(*this, v1, product);
		}
		FRAC& operator /= (const FRAC& v1) {
			FRAC q;
			return *this = divide(*this, v1, q);
		}

		friend FRAC& plus(const FRAC& v1, const FRAC& v2, FRAC& sum);
		friend FRAC& subtract(const FRAC& v1, const FRAC& v2, FRAC& sub);
		friend FRAC& multiply(const FRAC& v1, const FRAC& v2, FRAC& product);
		friend FRAC& divide(const FRAC& v1, const FRAC& v2, FRAC& q);

	public:
		const INT& d() const { return _d; }
		const INT& n() const { return _n; }

	private:
		void normalize();

	private:
		int _sign;
		INT _n;
		INT _d;
	};
}
#endif
