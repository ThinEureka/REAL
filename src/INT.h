
#pragma once

//The following code currently only works on machine with little endian.
#ifndef INT_H
#define INT_H

#include <vector>
#include <string>

namespace zju04nycs {

	class INT;
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

	public:
		INT();
		INT(const INT& v);
		INT(INT&& v) noexcept;
		INT(const INT&& v) noexcept;

		INT(INT::typeChunkSigned v) {
			if (v != 0) {
				_sign = v > 0 ? 1 : -1;
				_chunks.push_back(std::abs(v));
			}
		}

		INT(INT::typeLinkSigned v) {
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
		}

		INT(int sign, INT::typeChunk v) {
			if (v != 0) {
				_sign = sign > 0 ? 1 : -1;
				_chunks.push_back(v);
			}
		}

		INT(int sign, INT::typeChunkSigned v) {
			if (v != 0) {
				_sign = sign > 0 ? 1 : - 1;
				_sign = v > 0 ? _sign : -_sign;
				_chunks.push_back(std::abs( v));
			}
		}

		INT(int sign, INT::typeLinkSigned v) {
			if (v != 0) {
				_sign = sign > 0 ? 1 : - 1;
				_sign = v > 0 ? _sign : -_sign;
				INT::typeLink x = std::abs(v);
				INT::typeChunk lowChunk = static_cast<INT::typeChunk>(x);
				_chunks.push_back(lowChunk);
				INT::typeChunk highChunk = x >> INT::s_numBitsOfChunk;
				if (highChunk > 0) {
					_chunks.push_back(highChunk);
				}
			}
		}

		explicit INT(const std::string& str, int base = 10) {
			setValueWithString(str, base);
		}

		INT(int sign, const std::vector<typeChunk>& chunks) : _sign(sign), _chunks(chunks) {
			normalize();
		};

		INT(int sign, std::vector<typeChunk>&& chunks) : _sign(sign), _chunks(std::move(chunks)) {
			normalize();
		};

		int toInt(bool& isTruncated) const { return toChunkSigned(isTruncated); }
		int toUint(bool& isTruncated) const { return toChunk(isTruncated); }

		long long toLongLong(bool& isTruncated) const { return toLinkSigned(isTruncated); }
		unsigned long long toUlonglong(bool& isTruncated) const { return toLink(isTruncated); }

		std::string toString(int base = 10) const;
		void setValueWithString(const std::string& str, int base = 10);

		INT::typeChunk toChunk(bool& isTruncated) const {
			isTruncated = _chunks.size() > 1;
			if (_chunks.size() == 0) {
				return 0;
			}
			return _chunks[0];
		}
		INT::typeChunkSigned toChunkSigned(bool& isTruncated) const;

		INT::typeLink toLink(bool& isTruncated) const {
			isTruncated = _chunks.size() > 2;
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

		INT::typeLinkSigned toLinkSigned(bool& isTruncated) const;

	public:
		INT& operator = (const INT& v);
		INT& operator = (INT&& v) noexcept;
		INT& operator = (const INT&& v) noexcept;

		bool isZero() const { return _chunks.size() == 0; }
		void clear() { _sign = 1; _chunks.clear(); }
		bool isPositive() const { return _sign > 0 && _chunks.size() > 0; }
		bool isNegative() const { return _sign < 0;  }

		int bit(size_t pos) const;
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
		INT& negate() { if (!isZero()) { _sign = -_sign; } return *this; };

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
				_sign = 1;
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
		int _sign{ 1 };
		std::vector<typeChunk> _chunks;
	};
}

#endif