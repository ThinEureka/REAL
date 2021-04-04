
#pragma once

#ifndef INT_H
#define INT_H

#include <vector>
#include <string>

//The following code currently only works on machine with little endian.
namespace zju04nycs {
	class INT {
	public:
		typedef unsigned __int64 typeLink;
		typedef unsigned __int32 typeChunk;
		typedef __int64 typeLinkSinged;
		typedef __int32 typeChunkSigned;

		static const typeLink s_numBitsOfChunk = 32;
		static const typeLink s_borrowChunkValue = 1LL << INT::s_numBitsOfChunk;

	public:
		INT() {};
		INT(int v) {
			if (v != 0) {
				_sign = v > 0 ? 1 : -1;
				_chunks.push_back(v);
			}
		}

		INT(INT::typeLinkSinged v) {
			if (v != 0) {
				_sign = v > 0 ? 1 : -1;
				INT::typeLink x = std::abs(v);

				INT::typeChunk chunk = x;
				_chunks.push_back(chunk);
				INT::typeChunk chunkHigher = x >> INT::s_numBitsOfChunk;
				if (chunkHigher > 0) {
					_chunks.push_back(chunkHigher);
				}
			}
		}

		explicit INT(const std::string& str, int base = 10) {
			setValueWithString(str, 10);
		}

		INT(const INT& v) :_chunks(v._chunks), _sign(v._sign) {};
		INT(INT&& v) noexcept :_chunks(std::move(v._chunks)), _sign(v._sign) {};

		INT(int sign, const std::vector<typeChunk>& bits) : _sign(sign), _chunks(_chunks) {
			normalize();
		};

		INT(int sign, std::vector<typeChunk>&& bits) : _sign(sign), _chunks(std::move(bits)) {
			normalize();
		};

		const std::string&& toString(int base = 10) const;
		void setValueWithString(const std::string& str, int base = 10);

	public:
		INT& operator = (const INT& v) {
			_chunks = v._chunks;
			_sign = v._sign;
			return *this;
		}

		INT& operator = (INT&& v)noexcept {
			_chunks = std::move(v._chunks);
			_sign = v._sign;
			return *this;
		}

		bool isZero() const { return _chunks.size() == 0; }
		void clear() { _sign = 1; _chunks.clear(); }

		int bit(size_t pos) const;
		void setBit(size_t bitPos, bool v) {
			setBitWithoutNormalization(bitPos, v);
			normalize();
		}

		int leadBit() const;
		int tailBit() const;

		friend int compare(const INT& v1, const INT& v2);
		INT& negate() { if (!isZero()) { _sign = -_sign; } return *this; };

	public:
		const INT&& operator - () const {
			INT v = *this;
			if (!v.isZero()) {
				v._sign = -v._sign;
			}
			return std::move(v);
		}

		friend bool operator == (const INT& v1, const INT& v2);
		friend bool operator != (const INT& v1, const INT& v2) {
			return v1 != v2;
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

		friend const INT&& operator & (const INT& v1, const INT& v2);
		friend const INT&& operator | (const INT& v1, const INT& v2);
		friend const INT&& operator ^ (const INT& v1, const INT& v2);

		friend const INT&& operator >> (const INT& v1, int pos);
		friend const INT&& operator << (const INT& v1, int pos);

		friend const INT&& operator +(const INT& v1, const INT& v2);
		friend const INT&& operator -(const INT& v1, const INT& v2);
		friend const INT&& operator *(const INT& v1, const INT& v2);
		friend const INT&& operator /(const INT& v1, const INT& v2) {
			INT r;
			return divide(v1, v2, r);
		}
		friend const INT&& operator %(const INT& v1, const INT& v2) {
			INT r;
			divide(v1, v2, r);
			return std::move(r);
		}
		friend const INT&& divide(const INT& v1, const INT& v2, INT& r);

		INT& operator &= (const INT& v1) {
			return *this = *this & v1;
		}
		INT& operator |= (const INT& v1) {
			return *this = *this | v1;
		}
		INT& operator ^= (const INT& v1) {
			return *this = *this ^ v1;
		}

		INT& operator >>= (int pos) {
			return *this = *this >> pos;
		}
		INT& operator <<= (int pos) {
			return *this = *this << pos;
		}
		
		INT& operator += (const INT& v1) {
			return *this = *this + v1;
		}
		INT& operator -= (const INT& v1) {
			return *this = *this - v1;
		}
		INT& operator *= (const INT& v1) {
			return *this = *this * v1;
		}
		INT& operator /= (const INT& v1) {
			INT r;
			return *this = divide(*this, v1, r);
		}
		INT& operator %= (const INT& v1) {
			INT r;
			divide(*this, v1, r);
			return *this = std::move(r);
		}
		friend const INT&& divide(const INT& v1, const INT& v2, INT& r);

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
		friend void bitsSubtract(INT& v1, int tailBit1, const INT& v2, int tailBit2, int numberBits);

	private:
		int _sign{ 1 };
		std::vector<typeChunk> _chunks;
	};
	
}

#endif