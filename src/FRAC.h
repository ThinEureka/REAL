#pragma once

#ifndef FRAC_H
#define FRAC_H

#include "INT.h"
#include <assert.h> 

namespace zju04nycs {

	class FRAC;
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
		//FRAC(const std::string& str, int base) { set(str, base); }
		FRAC(const std::string& str, int base = 10) { set(str, base); }
		FRAC(const INT& n, const INT& d) { set(n, d); }
		FRAC(long  n, long d) { set(n, d); }
		FRAC(const INT& n) { *this = n; }
		FRAC(int n) {
			*this = n;
		}

		FRAC(unsigned int n) {
			*this = n;
		}

		FRAC(long long n) {
			*this = n;
		}

		FRAC(unsigned long long n) {
			*this = n;
		}

		std::string toString(int base = 10) const;
		FRAC& set(const std::string& str, int base = 10);
		FRAC& set(const INT& n, const INT& d) {
			assert(!d.isZero());
			_sign = 1;
			_n = n;
			_d = d;
			normalize();
			return *this;
		}

		FRAC& set(long long n, long long d) {
			assert(d != 0);
			_sign = 1;
			_n = n;
			_d = d;
			normalize();
			return *this;
		}

	public:
		FRAC& operator = (const FRAC& v) { _sign = v._sign; _n = v._n; _d = v._d; return *this; }
		FRAC& operator = (FRAC&& v) noexcept { _sign = v._sign; _n = std::move(v._n), _d = std::move(v._d); return *this; }
		FRAC& operator = (const FRAC&& v) noexcept { _sign = v._sign; _n = std::move(v._n), _d = std::move(v._d); return *this; }

		FRAC& operator = (const INT& n) {
			_sign = 1;
			_n = n;
			_d.setOne();
			normalize();
			return *this;
		}

		FRAC& operator = (int n) {
			_sign = 1;
			_n = n;
			_d.setOne();
			normalize();
			return *this;
		}

		FRAC& operator = (long long n) {
			_sign = 1;
			_n = n;
			_d.setOne();
			normalize();
			return *this;
		}

		FRAC& operator = (unsigned int n) {
			_sign = 1;
			_n = n;
			_d.setOne();
			normalize();
			return *this;
		}

		FRAC& operator = (unsigned long long n) {
			_sign = 1;
			_n = n;
			_d.setOne();
			normalize();
			return *this;
		}

		bool isZero() const { return _sign == 0; }
		bool isPositive() const { return _sign > 0; }
		bool isNegative() const { return _sign < 0; }

		void clear() { _sign = 0; _n.setZero(); _d.setOne(); }
		void setZero() { clear(); }
		void setOne() { _sign = 1; _n.setOne(); _d.setOne(); }

		friend int compare(const FRAC& v1, const FRAC& v2, INT& cache1, INT& cache2) {
			if (v1._sign != v2._sign) {
				return v1._sign > v2._sign ? 1 : -1;
			}
			else {
				multiply(v1._n, v2._d, cache1);
				multiply(v1._d, v2._n, cache2);
				auto comResult = compare(cache1, cache2);
				return v1._sign >= 0 ? comResult : -comResult;
			}
		}
		FRAC& negate() { _sign = -_sign;  return *this; };
		FRAC& inverse() { std::swap(_n, _d); return *this; }

	public:
		const FRAC operator - () const {
			FRAC v = *this;
			return v.negate();
		}

		friend bool operator == (const FRAC& v1, const FRAC& v2) {
			return v1._sign == v2._sign && v1._n == v2._n && v1._d == v2._d;
		}
		friend bool operator != (const FRAC& v1, const FRAC& v2) {
			return !(v1 == v2);
		}

		friend bool operator < (const FRAC& v1, const FRAC& v2) {
			INT cache1, cache2;
			return compare(v1, v2, cache1, cache2) < 0;
		}
		friend bool operator > (const FRAC& v1, const FRAC& v2) {
			INT cache1, cache2;
			return compare(v1, v2, cache1, cache2) > 0;
		}
		friend bool operator <= (const FRAC& v1, const FRAC& v2) {
			INT cache1, cache2;
			return compare(v1, v2, cache1, cache2) <= 0;
		}
		friend bool operator >= (const FRAC& v1, const FRAC& v2) {
			INT cache1, cache2;
			return compare(v1, v2, cache1, cache2) >= 0;
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
		int _sign{};
		INT _n;
		INT _d;

		// We do dirty works to reduce dynamic memory allocation.
		// As long as we do not create or delete FRACS, by limiting ourselves
		// to the cached interfaces for all calculation(for example we
		// use function multilpy insead of operator *), unnecessary dynamic memory 
		// allocation can be avoided, 
		// So if dynamica memory allocation is the bottle neck for 
		// calculation performance, you may perfer using the verbose and 
		// complcated cache interfaces.
		// Maybe we can find a better solution in the future.
		// But for now, such a dirty compromise is made. 
		INT _c1;
		INT _c2;
		INT _c3;
		INT _c4;
	};
}
#endif
