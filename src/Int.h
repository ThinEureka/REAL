
#pragma once

//The following code currently only works on machine with little endian.
#ifndef INT_H
#define INT_H

#include <vector>
#include <string>

namespace real {

	class Int;
	bool operator == (const Int& v1, const Int& v2);
	bool operator != (const Int& v1, const Int& v2);	

	Int operator & (const Int& v1, const Int& v2);
	Int operator | (const Int& v1, const Int& v2);
	Int operator ^ (const Int& v1, const Int& v2);

	Int operator >> (const Int& v1, int pos);
	Int operator << (const Int& v1, int pos);

	Int operator +(const Int& v1, const Int& v2);
	Int operator -(const Int& v1, const Int& v2);
	Int operator *(const Int& v1, const Int& v2);
	Int operator /(const Int& v1, const Int& v2);
	Int operator %(const Int& v1, const Int& v2);

	Int& plus(const Int& v1, const Int& v2, Int& sum);
	Int& subtract(const Int& v1, const Int& v2, Int& sub);
	Int& multiply(const Int& v1, const Int& v2, Int& product);
	Int& divide(const Int& v1, const Int& v2, Int& q, Int& r);

class Int {
	public:
		typedef unsigned __int64 typeLink;
		typedef unsigned __int32 typeChunk;
		typedef __int64 typeLinkSigned;
		typedef __int32 typeChunkSigned;

		static const typeLink s_numBitsOfChunk = 32;
		static const typeLink s_borrowChunkValue = 1LL << Int::s_numBitsOfChunk;
		static const Int s_smallInts[101];

		static const Int& zero;
		static const Int& one;

	public:
		Int(){}
		Int(const Int& v) :_chunks(v._chunks), _sign(v._sign){}
		Int(Int&& v) noexcept :_chunks(std::move(v._chunks)), _sign(v._sign) {}

		Int(Int::typeChunk v) {
			*this = v;
		}

		Int(Int::typeChunkSigned v) {
			*this = v;
		}

		Int(Int::typeLinkSigned v) {
			*this = v;
		}

		Int(Int::typeLink v) {
			*this = v;
		}

		explicit Int(const std::string& str, int base = 10) {
			set(str, base);
		}

		Int(int sign, const std::vector<typeChunk>& chunks) : _sign(sign), _chunks(chunks) {
			normalize();
		};

		Int(int sign, std::vector<typeChunk>&& chunks) : _sign(sign), _chunks(std::move(chunks)) {
			normalize();
		};

		int toInt(bool& isOverFlow) const { return toChunkSigned(isOverFlow); }
		int toUint(bool& isOverFlow) const { return toChunk(isOverFlow); }

		long long toLongLong(bool& isOverFlow) const { return toLinkSigned(isOverFlow); }
		unsigned long long toUlonglong(bool& isOverFlow) const { return toLink(isOverFlow); }

		std::string toString(int base = 10) const;
		Int& set(const std::string& str, int base = 10);

		Int& set (int sign, const std::vector<typeChunk>& chunks){
			_sign = sign;
			_chunks = chunks;
			normalize();
		};

		Int& set (int sign, std::vector<typeChunk>&& chunks) {
			_sign = sign;
			_chunks = std::move(chunks);
			normalize();
		};

		Int::typeChunk toChunk(bool& isOverFlow) const {
			isOverFlow = _chunks.size() > 1;
			if (_chunks.size() == 0) {
				return 0;
			}
			return _chunks[0];
		}
		Int::typeChunkSigned toChunkSigned(bool& isOverFlow) const;

		Int::typeLink toLink(bool& isOverFlow) const {
			isOverFlow = _chunks.size() > 2;
			if (_chunks.size() == 0) {
				return 0;
			}

			Int::typeLink result = _chunks[0];
			if (_chunks.size() > 1) {
				Int::typeLink highChunk = _chunks[1];
				highChunk <<= Int::s_numBitsOfChunk;
				result += highChunk;
			}

			return result;
		}

		Int::typeLinkSigned toLinkSigned(bool& isOverFlow) const;

	public:
		Int& operator = (const Int& v);
		Int& operator = (Int&& v) noexcept;
		Int& operator = (Int::typeChunk v) {
			clear();
			if (v != 0) {
				_sign = 1;
				_chunks.push_back(v);
			}
			return *this;
		}

		Int& operator = (Int::typeChunkSigned v) {
			clear();
			if (v != 0) {
				_sign = v > 0 ? 1 : -1;
				_chunks.push_back(v);
			}
			return *this;
		}

		Int& operator = (Int::typeLink v) {
			clear();
			if (v != 0) {
				_sign = 1;
				Int::typeChunk lowChunk = static_cast<Int::typeChunk>(1);
				_chunks.push_back(lowChunk);
				Int::typeChunk highChunk = v >> Int::s_numBitsOfChunk;
				if (highChunk > 0) {
					_chunks.push_back(highChunk);
				}
			}
			return *this;
		}

		Int& operator = (Int::typeLinkSigned v) {
			clear();
			if (v != 0) {
				_sign = v > 0 ? 1 : -1;
				Int::typeLink x = std::abs(v);
				Int::typeChunk lowChunk = static_cast<Int::typeChunk>(x);
				_chunks.push_back(lowChunk);
				Int::typeChunk highChunk = x >> Int::s_numBitsOfChunk;
				if (highChunk > 0) {
					_chunks.push_back(highChunk);
				}
			}
			return *this;
		}

		bool isZero() const { return _sign == 0; }
		Int& clear() { _sign = 0; _chunks.clear(); return *this; }
		Int& setZero() { return clear(); }
		Int& setOne() { _sign = 1; _chunks.clear(); setChunk(0, 1); return *this; }
		bool isPositive() const { return _sign > 0; }
		bool isNegative() const { return _sign < 0;  }

		int bit(size_t pos) const;
		int sign() const { return _sign; }
		Int::typeChunk chunk(size_t chunkIndex) const {
			if (chunkIndex < _chunks.size()) {
				return _chunks[chunkIndex];
			}
			return 0;
		}
		void setBit(size_t bitPos, bool v) {
			setBitWithoutNormalization(bitPos, v);
			normalize();
		}

		int leadBit() const;
		int tailBit() const;

		friend int compare(const Int& v1, const Int& v2);
		friend int absCompare(const Int& v1, const Int& v2);
		Int& negate() {  _sign = -_sign;  return *this; };

	public:
		const Int operator - () const {
			Int v = *this;
			return v.negate();
		}

		friend bool operator == (const Int& v1, const Int& v2);
		friend bool operator != (const Int& v1, const Int& v2) {
			return !(v1 == v2);
		}

		friend bool operator < (const Int& v1, const Int& v2) {
			return compare(v1, v2) < 0;
		}
		friend bool operator > (const Int& v1, const Int& v2) {
			return compare(v1, v2) > 0;
		}
		friend bool operator <= (const Int& v1, const Int& v2) {
			return compare(v1, v2) <= 0;
		}
		friend bool operator >= (const Int& v1, const Int& v2) {
			return compare(v1, v2) >= 0;
		}

		friend Int operator & (const Int& v1, const Int& v2) {
			Int v = v1;
			return v &= v2;
		}
		friend Int operator | (const Int& v1, const Int& v2) {
			Int v = v1;
			return v |= v2;
		}
		friend Int operator ^ (const Int& v1, const Int& v2) {
			Int v = v1;
			return v ^= v2;
		}

		friend Int operator >> (const Int& v1, int pos) {
			Int v = v1;
			return v >>= pos;
		}

		friend Int operator << (const Int& v1, int pos) {
			Int v = v1;
			return v <<= pos;
		}

		friend Int operator +(const Int& v1, const Int& v2) {
			Int sum;
			return plus(v1, v2, sum);
		}

		friend Int operator -(const Int& v1, const Int& v2) {
			Int sub;
			return subtract(v1, v2, sub);
		}
		friend Int operator *(const Int& v1, const Int& v2) {
			Int product;
			return multiply(v1, v2, product);
		}
		friend Int operator /(const Int& v1, const Int& v2) {
			Int q, r;
			return divide(v1, v2, q, r);
		}
		friend Int operator %(const Int& v1, const Int& v2) {
			Int q, r;
			divide(v1, v2, q, r);
			return r;
		}

		// Prefix decrement operator.
		Int& operator --() {
			return *this -= one;
		}

		// Postfix decrement operator. 
		Int operator --(int) {
			Int tmp = *this;
			subtract(tmp, one, *this);
			return tmp;
		}

		// Prefix increment operator.
		Int& operator ++() {
			return *this += one;
		}

		// Postfix increment operator. 
		Int operator ++(int) {
			Int tmp = *this;
			plus (tmp, one, *this);
			return tmp;
		}

		Int& operator &= (const Int& v1);
		Int& operator |= (const Int& v1);	
		Int& operator ^= (const Int& v1);

		Int& operator <<= (int pos) {
			if (pos == 0 || isZero()) {
				// do nothing
			}
			else if (pos > 0) {
				chunksShiftLeft(pos);
			}
			else {
				chunksShiftRight(-pos);
				normalize();
			}
			return *this;
		}
		Int& operator >>= (int pos) {
			if (pos == 0 || isZero()) {
				// do nothing
			}
			else if (pos > 0) {
				chunksShiftRight(pos);
				normalize();
			}
			else {
				chunksShiftLeft(-pos);
			}
			return *this;
		}
		
		Int& operator += (const Int& v1) {
			Int sum; 
			return *this = plus(*this, v1, sum);
		}
		Int& operator -= (const Int& v1) {
			Int sub;
			return *this = subtract(*this, v1, sub);
		}
		Int& operator *= (const Int& v1) {
			Int product;
			return *this = multiply(*this, v1, product);
		}
		Int& operator /= (const Int& v1) {
			Int q,r;
			return *this = divide(*this, v1, q, r);
		}
		Int& operator %= (const Int& v1) {
			Int q,r;
			divide(*this, v1, q, r);
			return *this = r;
		}

		friend Int& plus(const Int& v1, const Int& v2, Int& sum);
		friend Int& subtract(const Int& v1, const Int& v2, Int& sub);
		friend Int& multiply(const Int& v1, const Int& v2, Int& product);
		friend Int& divide(const Int& v1, const Int& v2, Int& q, Int& r);

	private:
		void normalize() {
			while (_chunks.size() > 0) {
				if (_chunks[_chunks.size() - 1] != 0) {
					return;
				}
				_chunks.pop_back();
			}
			if (_chunks.size() == 0) {
				_sign = 0;
			}
		}

		void setBitWithoutNormalization(size_t bitPos, bool v);
		friend void bitsSubtract(Int& v1, int leadBit1, int tailBit1, const Int& v2, int tailBit2, int numberBits);

		//chunk operations, the class may not be normalized after
		//carrying out the following operations.
		void setChunk(size_t chunkIndex, Int::typeChunk chunk) {
			if (chunkIndex >= _chunks.size()) {
				_chunks.resize(chunkIndex + 1);
			}
			_chunks[chunkIndex] = chunk;
		}

		void chunksPlus(const std::vector<Int::typeChunk>& chunks1, const std::vector<Int::typeChunk>& chunks2);
		void chunksSubtract(const std::vector<Int::typeChunk>& chunks1, const std::vector<Int::typeChunk>& chunks2);

		void chunksShiftRight(unsigned int pos);
		void chunksShiftLeft(unsigned int pos);

	private:
		int _sign{ 0 };
		std::vector<typeChunk> _chunks;
	};
}

#endif