#pragma once

#ifndef FRAC_H
#define FRAC_H

#include "Int.h"
#include <assert.h> 

namespace real {

	class Frac;
	Frac operator +(const Frac& v1, const Frac& v2);
	Frac operator -(const Frac& v1, const Frac& v2);
	Frac operator *(const Frac& v1, const Frac& v2);
	Frac operator /(const Frac& v1, const Frac& v2);

	Frac& add(const Frac& v1, const Frac& v2, Frac& sum);
	Frac& subtract(const Frac& v1, const Frac& v2, Frac& sub);
	Frac& multiply(const Frac& v1, const Frac& v2, Frac& product);
	Frac& divide(const Frac& v1, const Frac& v2, Frac& q);

class Frac {
	public:
		Frac() {}
		Frac (const Frac& v): _sign(v._sign), _n(v._n), _d(v._d) {}
		Frac (Frac&& v) noexcept : _sign(v._sign), _n(v._n), _d(v._d){}
		//FRAC(const std::string& str, int base) { set(str, base); }
		Frac(const std::string& str, int base = 10) { set(str, base); }
		Frac(const Int& n, const Int& d) { set(n, d); }
		Frac(long  n, long d) { set(n, d); }
		Frac(const Int& n) { *this = n; }
		Frac(int n) {
			*this = n;
		}

		Frac(unsigned int n) {
			*this = n;
		}

		Frac(long n) {
			*this = n;
		}

		Frac(unsigned long n) {
			*this = n;
		}

		Frac(long long n) {
			*this = n;
		}

		Frac(unsigned long long n) {
			*this = n;
		}

		std::string toString(int base = 10) const;
		Frac& set(const std::string& str, int base = 10);
		Frac& set(const Int& n, const Int& d) {
			assert(!d.isZero());
			_sign = 1;
			_n = n;
			_d = d;
			normalize();
			return *this;
		}

		Frac& set(long long n, long long d) {
			assert(d != 0);
			_sign = 1;
			_n = n;
			_d = d;
			normalize();
			return *this;
		}

	public:
		Frac& operator = (const Frac& v) { _sign = v._sign; _n = v._n; _d = v._d; return *this; }
		Frac& operator = (Frac&& v) noexcept { _sign = v._sign; _n = std::move(v._n), _d = std::move(v._d); return *this; }

		Frac& operator = (const Int& n) {
			_sign = 1;
			_n = n;
			_d.setOne();
			normalize();
			return *this;
		}

		Frac& operator = (int n) {
			_sign = 1;
			_n = n;
			_d.setOne();
			normalize();
			return *this;
		}

		Frac& operator = (unsigned int n) {
			_sign = 1;
			_n = n;
			_d.setOne();
			normalize();
			return *this;
		}

		Frac& operator = (long n) {
			return *this = static_cast<long long>(n);
		}

		Frac& operator = (unsigned long n) {
			return *this = static_cast<unsigned long long>(n);
		}

		Frac& operator = (long long n) {
			_sign = 1;
			_n = n;
			_d.setOne();
			normalize();
			return *this;
		}

		Frac& operator = (unsigned long long n) {
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

		friend int compare(const Frac& v1, const Frac& v2, Int& cache1, Int& cache2) {
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
		Frac& negate() { _sign = -_sign;  return *this; };
		Frac& inverse() { std::swap(_n, _d); return *this; }

	public:
		const Frac operator - () const {
			Frac v = *this;
			return v.negate();
		}

		friend bool operator == (const Frac& v1, const Frac& v2) {
			return v1._sign == v2._sign && v1._n == v2._n && v1._d == v2._d;
		}
		friend bool operator != (const Frac& v1, const Frac& v2) {
			return !(v1 == v2);
		}

		friend bool operator < (const Frac& v1, const Frac& v2) {
			Int cache1, cache2;
			return compare(v1, v2, cache1, cache2) < 0;
		}
		friend bool operator > (const Frac& v1, const Frac& v2) {
			Int cache1, cache2;
			return compare(v1, v2, cache1, cache2) > 0;
		}
		friend bool operator <= (const Frac& v1, const Frac& v2) {
			Int cache1, cache2;
			return compare(v1, v2, cache1, cache2) <= 0;
		}
		friend bool operator >= (const Frac& v1, const Frac& v2) {
			Int cache1, cache2;
			return compare(v1, v2, cache1, cache2) >= 0;
		}

		friend Frac operator +(const Frac& v1, const Frac& v2) {
			Frac sum;
			return add(v1, v2, sum);
		}

		friend Frac operator -(const Frac& v1, const Frac& v2) {
			Frac sub;
			return subtract(v1, v2, sub);
		}
		friend Frac operator *(const Frac& v1, const Frac& v2) {
			Frac product;
			return multiply(v1, v2, product);
		}
		friend Frac operator /(const Frac& v1, const Frac& v2) {
			Frac q;
			return divide(v1, v2, q);
		}
		
		Frac& operator += (const Frac& v1) {
			Frac sum; 
			return *this = add(*this, v1, sum);
		}
		Frac& operator -= (const Frac& v1) {
			Frac sub;
			return *this = subtract(*this, v1, sub);
		}
		Frac& operator *= (const Frac& v1) {
			Frac product;
			return *this = multiply(*this, v1, product);
		}
		Frac& operator /= (const Frac& v1) {
			Frac q;
			return *this = divide(*this, v1, q);
		}

		friend Frac& add(const Frac& v1, const Frac& v2, Frac& sum);
		friend Frac& subtract(const Frac& v1, const Frac& v2, Frac& sub);
		friend Frac& multiply(const Frac& v1, const Frac& v2, Frac& product);
		friend Frac& divide(const Frac& v1, const Frac& v2, Frac& q);

	public:
		const Int& d() const { return _d; }
		const Int& n() const { return _n; }

	private:
		void normalize();

	private:
		int _sign{};
		Int _n;
		Int _d;

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
		Int _c1;
		Int _c2;
		Int _c3;
		Int _c4;
	};
}
#endif
