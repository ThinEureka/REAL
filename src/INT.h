
#pragma once

//The following code currently only works on machine with little endian.
#ifndef INT_H
#define INT_H

#include <vector>
#include <string>

namespace real {

	class INT;
	bool operator == (const INT& v1, const INT& v2);
	bool operator != (const INT& v1, const INT& v2);	

	INT operator & (const INT& v1, const INT& v2);
	INT operator | (const INT& v1, const INT& v2);
	INT operator ^ (const INT& v1, const INT& v2);

	INT operator >> (const INT& v1, int pos);
	INT operator << (const INT& v1, int pos);

	INT operator +(const INT& v1, const INT& v2);
	INT operator -(const INT& v1, const INT& v2);
	INT operator *(const INT& v1, const INT& v2);
	INT operator /(const INT& v1, const INT& v2);
	INT operator %(const INT& v1, const INT& v2);

	INT& plus(const INT& v1, const INT& v2, INT& sum);
	INT& subtract(const INT& v1, const INT& v2, INT& sub);
	INT& multiply(const INT& v1, const INT& v2, INT& product);
	INT& divide(const INT& v1, const INT& v2, INT& q, INT& r);

class INT {
	public:
		typedef unsigned __int64 typeLink;
		typedef unsigned __int32 typeChunk;
		typedef __int64 typeLinkSigned;
		typedef __int32 typeChunkSigned;

		static const typeLink s_numBitsOfChunk = 32;
		static const typeLink s_borrowChunkValue = 1LL << INT::s_numBitsOfChunk;
		static const INT s_smallInts[101];

		static const INT& zero;
		static const INT& one;

	public:
		INT(){}
		INT(const INT& v) :_chunks(v._chunks), _sign(v._sign){}
		INT(INT&& v) noexcept :_chunks(std::move(v._chunks)), _sign(v._sign) {}

		INT(INT::typeChunk v) {
			*this = v;
		}

		INT(INT::typeChunkSigned v) {
			*this = v;
		}

		INT(INT::typeLinkSigned v) {
			*this = v;
		}

		INT(INT::typeLink v) {
			*this = v;
		}

		explicit INT(const std::string& str, int base = 10) {
			set(str, base);
		}

		INT(int sign, const std::vector<typeChunk>& chunks) : _sign(sign), _chunks(chunks) {
			normalize();
		};

		INT(int sign, std::vector<typeChunk>&& chunks) : _sign(sign), _chunks(std::move(chunks)) {
			normalize();
		};

		int toInt(bool& isOverFlow) const { return toChunkSigned(isOverFlow); }
		int toUint(bool& isOverFlow) const { return toChunk(isOverFlow); }

		long long toLongLong(bool& isOverFlow) const { return toLinkSigned(isOverFlow); }
		unsigned long long toUlonglong(bool& isOverFlow) const { return toLink(isOverFlow); }

		std::string toString(int base = 10) const;
		INT& set(const std::string& str, int base = 10);

		INT& set (int sign, const std::vector<typeChunk>& chunks){
			_sign = sign;
			_chunks = chunks;
			normalize();
		};

		INT& set (int sign, std::vector<typeChunk>&& chunks) {
			_sign = sign;
			_chunks = std::move(chunks);
			normalize();
		};

		INT::typeChunk toChunk(bool& isOverFlow) const {
			isOverFlow = _chunks.size() > 1;
			if (_chunks.size() == 0) {
				return 0;
			}
			return _chunks[0];
		}
		INT::typeChunkSigned toChunkSigned(bool& isOverFlow) const;

		INT::typeLink toLink(bool& isOverFlow) const {
			isOverFlow = _chunks.size() > 2;
			if (_chunks.size() == 0) {
				return 0;
			}

			INT::typeLink result = _chunks[0];
			if (_chunks.size() > 1) {
				INT::typeLink highChunk = _chunks[1];
				highChunk <<= INT::s_numBitsOfChunk;
				result += highChunk;
			}

			return result;
		}

		INT::typeLinkSigned toLinkSigned(bool& isOverFlow) const;

	public:
		INT& operator = (const INT& v);
		INT& operator = (INT&& v) noexcept;
		INT& operator = (INT::typeChunk v) {
			clear();
			if (v != 0) {
				_sign = 1;
				_chunks.push_back(v);
			}
			return *this;
		}

		INT& operator = (INT::typeChunkSigned v) {
			clear();
			if (v != 0) {
				_sign = v > 0 ? 1 : -1;
				_chunks.push_back(v);
			}
			return *this;
		}

		INT& operator = (INT::typeLink v) {
			clear();
			if (v != 0) {
				_sign = 1;
				INT::typeChunk lowChunk = static_cast<INT::typeChunk>(1);
				_chunks.push_back(lowChunk);
				INT::typeChunk highChunk = v >> INT::s_numBitsOfChunk;
				if (highChunk > 0) {
					_chunks.push_back(highChunk);
				}
			}
			return *this;
		}

		INT& operator = (INT::typeLinkSigned v) {
			clear();
			if (v != 0) {
				_sign = v > 0 ? 1 : -1;
				INT::typeLink x = std::abs(v);
				INT::typeChunk lowChunk = static_cast<INT::typeChunk>(x);
				_chunks.push_back(lowChunk);
				INT::typeChunk highChunk = x >> INT::s_numBitsOfChunk;
				if (highChunk > 0) {
					_chunks.push_back(highChunk);
				}
			}
			return *this;
		}

		bool isZero() const { return _sign == 0; }
		INT& clear() { _sign = 0; _chunks.clear(); return *this; }
		INT& setZero() { return clear(); }
		INT& setOne() { _sign = 1; _chunks.clear(); setChunk(0, 1); return *this; }
		bool isPositive() const { return _sign > 0; }
		bool isNegative() const { return _sign < 0;  }

		int bit(size_t pos) const;
		int sign() const { return _sign; }
		INT::typeChunk chunk(size_t chunkIndex) const {
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

		friend int compare(const INT& v1, const INT& v2);
		INT& negate() {  _sign = -_sign;  return *this; };

	public:
		const INT operator - () const {
			INT v = *this;
			return v.negate();
		}

		friend bool operator == (const INT& v1, const INT& v2);
		friend bool operator != (const INT& v1, const INT& v2) {
			return !(v1 == v2);
		}

		friend bool operator < (const INT& v1, const INT& v2) {
			return compare(v1, v2) < 0;
		}
		friend bool operator > (const INT& v1, const INT& v2) {
			return compare(v1, v2) > 0;
		}
		friend bool operator <= (const INT& v1, const INT& v2) {
			return compare(v1, v2) <= 0;
		}
		friend bool operator >= (const INT& v1, const INT& v2) {
			return compare(v1, v2) >= 0;
		}

		friend INT operator & (const INT& v1, const INT& v2) {
			INT v = v1;
			return v &= v2;
		}
		friend INT operator | (const INT& v1, const INT& v2) {
			INT v = v1;
			return v |= v2;
		}
		friend INT operator ^ (const INT& v1, const INT& v2) {
			INT v = v1;
			return v ^= v2;
		}

		friend INT operator >> (const INT& v1, int pos) {
			INT v = v1;
			return v >>= pos;
		}

		friend INT operator << (const INT& v1, int pos) {
			INT v = v1;
			return v <<= pos;
		}

		friend INT operator +(const INT& v1, const INT& v2) {
			INT sum;
			return plus(v1, v2, sum);
		}

		friend INT operator -(const INT& v1, const INT& v2) {
			INT sub;
			return subtract(v1, v2, sub);
		}
		friend INT operator *(const INT& v1, const INT& v2) {
			INT product;
			return multiply(v1, v2, product);
		}
		friend INT operator /(const INT& v1, const INT& v2) {
			INT q, r;
			return divide(v1, v2, q, r);
		}
		friend INT operator %(const INT& v1, const INT& v2) {
			INT q, r;
			divide(v1, v2, q, r);
			return r;
		}

		// Prefix decrement operator.
		INT& operator --() {
			return *this -= one;
		}

		// Postfix decrement operator. 
		INT operator --(int) {
			INT tmp = *this;
			subtract(tmp, one, *this);
			return tmp;
		}

		// Prefix increment operator.
		INT& operator ++() {
			return *this += one;
		}

		// Postfix increment operator. 
		INT operator ++(int) {
			INT tmp = *this;
			plus (tmp, one, *this);
			return tmp;
		}

		INT& operator &= (const INT& v1);
		INT& operator |= (const INT& v1);	
		INT& operator ^= (const INT& v1);

		INT& operator <<= (int pos) {
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
		INT& operator >>= (int pos) {
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
		
		INT& operator += (const INT& v1) {
			INT sum; 
			return *this = plus(*this, v1, sum);
		}
		INT& operator -= (const INT& v1) {
			INT sub;
			return *this = subtract(*this, v1, sub);
		}
		INT& operator *= (const INT& v1) {
			INT product;
			return *this = multiply(*this, v1, product);
		}
		INT& operator /= (const INT& v1) {
			INT q,r;
			return *this = divide(*this, v1, q, r);
		}
		INT& operator %= (const INT& v1) {
			INT q,r;
			divide(*this, v1, q, r);
			return *this = r;
		}

		friend INT& plus(const INT& v1, const INT& v2, INT& sum);
		friend INT& subtract(const INT& v1, const INT& v2, INT& sub);
		friend INT& multiply(const INT& v1, const INT& v2, INT& product);
		friend INT& divide(const INT& v1, const INT& v2, INT& q, INT& r);

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
		friend void bitsSubtract(INT& v1, int leadBit1, int tailBit1, const INT& v2, int tailBit2, int numberBits);

		//chunk operations, the class may not be normalized after
		//carrying out the following operations.
		void setChunk(size_t chunkIndex, INT::typeChunk chunk) {
			if (chunkIndex >= _chunks.size()) {
				_chunks.resize(chunkIndex + 1);
			}
			_chunks[chunkIndex] = chunk;
		}

		void chunksPlus(const std::vector<INT::typeChunk>& chunks1, const std::vector<INT::typeChunk>& chunks2);
		void chunksSubtract(const std::vector<INT::typeChunk>& chunks1, const std::vector<INT::typeChunk>& chunks2);

		void chunksShiftRight(unsigned int pos);
		void chunksShiftLeft(unsigned int pos);

	private:
		int _sign{ 0 };
		std::vector<typeChunk> _chunks;
	};
}

#endif