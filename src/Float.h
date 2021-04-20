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

	const Float operator >> (const Float& v1, int pos);
	const Float operator << (const Float& v1, int pos);

	const Float operator +(const Float& v1, const Float& v2);
	const Float operator -(const Float& v1, const Float& v2);
	const Float operator *(const Float& v1, const Float& v2);
	const Float operator /(const Float& v1, const Float& v2);

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
		Float(Float&& v) noexcept : _int(std::move(v._int)), _baseBitPos(v._baseBitPos), _c1(v._c1) {}

		Float(const Int& v, int baseBitPos = 0) : _int(v), _baseBitPos(baseBitPos) {
			normalize();
		}

		Float(Int&& v, int baseBitPos = 0) noexcept : _int(v), _baseBitPos(baseBitPos) {
			normalize();
		}

		Float(int v, int baseBitPos = 0) : _int(v), _baseBitPos(baseBitPos) { normalize(); }

		Float(long long v, int baseBitPos = 0) : _int(v), _baseBitPos(baseBitPos) { normalize(); }

		Float(unsigned int v, int baseBitPos = 0) : _int(v), _baseBitPos(baseBitPos) { normalize(); }

		Float(unsigned long long v, int baseBitPos = 0) : _int(v), _baseBitPos(baseBitPos) { normalize(); }

		explicit Float(const std::string& str, int base = 10) { set(str, base); }

		//the destructor is not virtual such that this class
		//is not meant to be inherited
		~Float() {
			cleanCache();
		}

		const Float floor() const {
			Float f = *this;
			return f.setFloor();
		}

		const Float ceil() const { 
			auto f = *this;
			f.setCeil();
			return f;
		}

		const Float intValue() const {
			Float f = *this;
			return f.setInt();
		}

		Float& setFloor(bool* isModified = nullptr);
		Float& setCeil(bool* isModified = nullptr);

		Float& setInt(bool* isModified = nullptr) {
			return this->truncate(0, isModified);
		}
		Float& truncate(int bitPos, bool* isModified);

		const Int toInt() const {
			Int n;
			if (_baseBitPos <= 0) {
				n.setZero();
				return n;
			}

			n = _int;
			return n <<= _baseBitPos;
		}

		const std::string toString(int base = 10) const;
		Float& set(const std::string& str, int base = 10);

		Float& set(const Int& n, int baseBitPos) {
			_int = n;
			_baseBitPos = baseBitPos;
			return this->normalize();
		}

		Float& set(Int&& n, int baseBitPos) {
			_int = n;
			_baseBitPos = baseBitPos;
			return this->normalize();
		}

	public:
		Float& operator = (const Float& v) {
			_int = v._int;
			_baseBitPos = v._baseBitPos;
			return *this;
		}
		Float& operator = (Float&& v) noexcept {
			_int = std::move(v._int);
			_baseBitPos = v._baseBitPos;
			return *this;
		}

		Float& operator = (const Int& v) {
			_int = v;
			_baseBitPos = 0;
			normalize();
			return *this;
		}
		Float& operator = (Int&& v) noexcept {
			_int = v;
			_baseBitPos = 0;
			normalize();
			return *this;
		}

		Float& operator = (int v) {
			_int = v;
			_baseBitPos = 0;
			normalize();
			return *this;
		}
		Float& operator = (long long v) {
			_int = v;
			_baseBitPos = 0;
			normalize();
			return *this;
		}

		Float& operator = (unsigned int v) {
			_int = v;
			_baseBitPos = 0;
			normalize();
			return *this;
		}
		Float& operator = (unsigned long long v) {
			_int = v;
			_baseBitPos = 0;
			normalize();
			return *this;
		}

		bool isZero() const {
			return _int.isZero();
		}
		Float& clear() {
			_int.setZero();
			_baseBitPos = 0;
			return *this;
		}
		Float& setZero() {
			clear();
			return *this;
		}
		Float& setOne() { _int.setOne(); _baseBitPos = 0; return *this; }
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

			int result = absCompare(v1, v2);
			return v1.sign() > 0 ? result : -result;
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

		friend const Float operator >> (const Float& v1, int pos) {
			Float v = v1;
			return v >>= pos;
		}

		friend const Float operator << (const Float& v1, int pos) {
			Float v = v1;
			return v <<= pos;
		}

		friend const Float operator +(const Float& v1, const Float& v2) {
			Float sum;
			return plus(v1, v2, sum);
		}

		friend const Float operator -(const Float& v1, const Float& v2) {
			Float sub;
			return subtract(v1, v2, sub);
		}
		friend const Float operator *(const Float& v1, const Float& v2) {
			Float product;
			return multiply(v1, v2, product);
		}
		friend const Float operator /(const Float& v1, const Float& v2) {
			Float q, r;
			return divide(v1, v2, q, r);
		}
		friend const Float operator %(const Float& v1, const Float& v2) {
			Float q, r;
			divide(v1, v2, q, r);
			return r;
		}

		Float& operator <<= (int pos) {
			if (_int.isZero() || pos == 0) {
				return *this;
			}
;
			_baseBitPos += pos;
			return *this;
		}
		Float& operator >>= (int pos) {
			if (_int.isZero() || pos == 0) {
				return *this;
			}
;
			_baseBitPos -= pos;
			return *this;
		}

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

		void cleanCache() {
			if (_c1) {
				delete _c1;
				_c1 = nullptr;
			}
		}

	private:
		Float& normalize() {
			int tailBit = _int.tailBit();
			if (tailBit > 0) {
				_int >>= tailBit;
				_baseBitPos += tailBit;
			}
			else
			{
				_int.clear();
				_baseBitPos = 0;
			}
			return *this;
		}

		Int& c1() {
			if (!_c1) {
				_c1 = new Int();
			}
			return *_c1;
		}

	private:
		Int _int;
		int _baseBitPos;

		//caches
		Int* _c1{ nullptr };
	};
}

#endif
