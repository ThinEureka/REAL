#pragma once

//The following code currently only works on machine with little endian.
#ifndef FLOAT_H
#define FLOAT_H

#include "Int.h"
#include <string>

namespace real {
	class Float;

	bool operator == (const Float& v1, const Float& v2);
	bool operator != (const Float& v1, const Float& v2);

	Float operator & (const Float& v1, const Float& v2);
	Float operator | (const Float& v1, const Float& v2);
	Float operator ^ (const Float& v1, const Float& v2);

	Float operator >> (const Float& v1, int pos);
	Float operator << (const Float& v1, int pos);

	Float operator +(const Float& v1, const Float& v2);
	Float operator -(const Float& v1, const Float& v2);
	Float operator *(const Float& v1, const Float& v2);
	Float operator /(const Float& v1, const Float& v2);
	Float operator %(const Float& v1, const Float& v2);

	Float& plus(const Float& v1, const Float& v2, Float& sum);
	Float& subtract(const Float& v1, const Float& v2, Float& sub);
	Float& multiply(const Float& v1, const Float& v2, Float& product);
	Float& divide(const Float& v1, const Float& v2, int precision);

	class Float {
	public:
		static const Float zero;
		static const Float one;

	public:
		Float() : _baseBitPos(0) {}
		Float(const Float& v) : _int(v._int), _baseBitPos(v._baseBitPos) {}
		Float(Float&& v) noexcept : _int(std::move(v._int)), _baseBitPos(v._baseBitPos) {}

		Float(Int v) : _int(v), _baseBitPos(0) { normalize(); }
		Float(Int&& v) noexcept : _int(v), _baseBitPos(0) { normalize(); }

		Float(int v) : _int(v), _baseBitPos(0) { normalize(); }

		Float(long long v) : _int(v), _baseBitPos(0) { normalize(); }

		Float(unsigned int v) : _int(v), _baseBitPos(0) { normalize(); }

		Float(unsigned long long v) : _int(v), _baseBitPos(0) { normalize(); }

		explicit Float(const std::string& str, int base = 10) { set(str, base); }

		Int floor() const;
		Int ceil() const;

		std::string toString(int base = 10) const;
		Float& set(const std::string& str, int base = 10);

	public:
		Float& operator = (const Float& v) {
			_int = v._int;
			_baseBitPos = v._baseBitPos;
		}
		Float& operator = (Float&& v) noexcept {
			_int = v._int;
			_baseBitPos = v._baseBitPos;
		}

		Float& operator = (const Int& v) {
			_int = v;
			_baseBitPos = 0;
			normalize();
		}
		Float& operator = (Int&& v) noexcept {
			_int = v;
			_baseBitPos = 0;
			normalize();
		}

		Float& operator = (int v) {
			_int = v;
			_baseBitPos = 0;
			normalize();
		}
		Float& operator = (long long v) {
			_int = v;
			_baseBitPos = 0;
			normalize();
		}

		Float& operator = (unsigned int v) {
			_int = v;
			_baseBitPos = 0;
			normalize();
		}
		Float& operator = (unsigned long long v) {
			_int = v;
			_baseBitPos = 0;
			normalize();
		}

		bool isZero() const {
			return _int.isZero();
		}
		Float& clear() {
			_int.setZero();
			_baseBitPos = 0;
		}
		Float& setZero() {
			clear();
		}
		Float& setOne() { _int.setOne(); _baseBitPos = 0; }
		bool isPositive() const { return _int.isPositive(); }
		bool isNegative() const { return _int.isNegative(); }

		int bit(int pos) const { 
			if (_int.isZero()) 
			{
				return 0; 
			}
			else
			{
				if (pos < _baseBitPos) {
					return 0;
				}
				//pos must be greater or equal to _baseBitPos, 
				//thus pos - baseBitPos >= 0
				return _int.bit(static_cast<size_t>(pos - _baseBitPos));
			}
		}
		int sign() const { return _int.sign(); }

		Float& setBit(int bitPos, bool v);
		// we need to address the issue with _int == 0, in which case the 
		//leadBit and tailBit is actually not defined.
		int leadBit() const { return _int.leadBit() + _baseBitPos; }
		int tailBit() const { return _baseBitPos; }

		friend int compare(const Float& v1, const Float& v2) {
			if (v1.sign() != v2.sign()) {
				return v1.sign() > v2.sign() ? 1 : -1;
			}

			return absCompare(v1, v2);
		}
		friend int absCompare(const Float& v1, const Float& v2);
		Float& negate() { _int.negate(); return *this; }

	public:
		const Float operator - () const {
			Float v = *this;
			return v.negate();
		}

		friend bool operator == (const Float& v1, const Float& v2) {
			return  v1._baseBitPos == v2._baseBitPos && v1._int == v2._int;
		}
		friend bool operator != (const Float& v1, const Float& v2) {
			return !(v1 == v2);
		}

		friend bool operator < (const Float& v1, const Float& v2) {
			return compare(v1, v2) < 0;
		}
		friend bool operator > (const Float& v1, const Float& v2) {
			return compare(v1, v2) > 0;
		}
		friend bool operator <= (const Float& v1, const Float& v2) {
			return compare(v1, v2) <= 0;
		}
		friend bool operator >= (const Float& v1, const Float& v2) {
			return compare(v1, v2) >= 0;
		}

		friend Float operator >> (const Float& v1, int pos) {
			Float v = v1;
			return v >>= pos;
		}

		friend Float operator << (const Float& v1, int pos) {
			Float v = v1;
			return v <<= pos;
		}

		friend Float operator +(const Float& v1, const Float& v2) {
			Float sum;
			return plus(v1, v2, sum);
		}

		friend Float operator -(const Float& v1, const Float& v2) {
			Float sub;
			return subtract(v1, v2, sub);
		}
		friend Float operator *(const Float& v1, const Float& v2) {
			Float product;
			return multiply(v1, v2, product);
		}
		friend Float operator /(const Float& v1, const Float& v2) {
			Float q, r;
			return divide(v1, v2, q, r);
		}
		friend Float operator %(const Float& v1, const Float& v2) {
			Float q, r;
			divide(v1, v2, q, r);
			return r;
		}

		Float& operator <<= (int pos);
		Float& operator >>= (int pos);

		Float& operator += (const Float& v1) {
			Float sum;
			return *this = plus(*this, v1, sum);
		}
		Float& operator -= (const Float& v1) {
			Float sub;
			return *this = subtract(*this, v1, sub);
		}
		Float& operator *= (const Float& v1) {
			Float product;
			return *this = multiply(*this, v1, product);
		}
		Float& operator /= (const Float& v1) {
			Float q, r;
			return *this = divide(*this, v1, q, r);
		}

		friend Float& plus(const Float& v1, const Float& v2, Float& sum);
		friend Float& subtract(const Float& v1, const Float& v2, Float& sub);
		friend Float& multiply(const Float& v1, const Float& v2, Float& product);
		friend Float& divide(const Float& v1, const Float& v2, Float& q, Float& r);

	private:
		void normalize() {
			int tailBit = _int.tailBit();
			if (tailBit > 0) {
				_int >>= tailBit;
				_baseBitPos -= tailBit;
			}
			else
			{
				_int.clear();
				_baseBitPos = 0;
			}
		}

	private:
		Int _int;
		int _baseBitPos;
	};
}

#endif
