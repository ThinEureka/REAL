
#pragma once

//The following code currently only works on machine with little endian.
#ifndef INT_H
#define INT_H

#include <vector>
#include <string>
#include <cstdint>

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

	Int& add(const Int& v1, const Int& v2, Int& sum);
	Int& subtract(const Int& v1, const Int& v2, Int& sub);
	Int& multiply(const Int& v1, const Int& v2, Int& product);
	Int& divide(const Int& v1, const Int& v2, Int& q, Int& r);

class Int {
	public:
		typedef std::uint64_t typeLink;
		typedef std::uint32_t typeChunk;
		typedef std::int64_t typeLinkSigned;
		typedef std::int32_t typeChunkSigned;

		static const typeLink s_numBitsOfChunk = 32;
		static const typeLink s_borrowChunkValue = 1LL << Int::s_numBitsOfChunk;
		static const Int s_smallInts[101];

		static const Int& zero;
		static const Int& one;

	public:
		static bool isDigit(char c, int base, int& digitValue);
		static char chunkToDigit(Int::typeChunk chunk, int base);
		static bool isExponentSeprator(char c, int base) {
			if (base <= 10) {
				return c == 'e' || c == 'E' || c == '^';
			}
			else {
				return c == '^';
			}
		}

	public:
		Int();
		Int(const Int& v);
		Int(Int&& v) noexcept;

		Int(unsigned int v) {
			*this = v;
		}

		Int(int v) {
			*this = v;
		}

		Int(unsigned long v) {
			*this = static_cast<unsigned long long>(v);
		}

		Int(long v) {
			*this = static_cast<long long>(v);
		}

		Int(long long v) {
			*this = v;
		}

		Int(unsigned long long v) {
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

		//the destructor is not virtual such that this class
		//is not meant to be inherited
		~Int() {
			cleanCache();
		}

		int toInt(bool& isOverFlow) const { return toChunkSigned(isOverFlow); }
		int toUint(bool& isOverFlow) const { return toChunk(isOverFlow); }

		long long toLongLong(bool& isOverFlow) const { return toLinkSigned(isOverFlow); }
		unsigned long long toUlonglong(bool& isOverFlow) const { return toLink(isOverFlow); }

		std::string toString(int base = 10, Int* cacheR = nullptr, Int* cacheN = nullptr, Int* cacheQ = nullptr ) const;
		Int& set(const std::string& str, int base = 10);

		Int& set (int sign, const std::vector<typeChunk>& chunks){
			_sign = sign;
			_chunks = chunks;
			return normalize();
		};

		Int& set (int sign, std::vector<typeChunk>&& chunks) {
			_sign = sign;
			_chunks = std::move(chunks);
			return normalize();
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
		Int& operator = (unsigned int v) {
			clear();
			if (v != 0) {
				_sign = 1;
				_chunks.push_back(v);
			}
			return *this;
		}

		Int& operator = (int v) {
			clear();
			if (v != 0) {
				_sign = v > 0 ? 1 : -1;
				_chunks.push_back(v);
			}
			return *this;
		}
		
		Int& operator = (unsigned long v) {
			return *this = static_cast<unsigned long long>(v);
		}
		
		Int& operator = (long v) {
			return *this = static_cast<long long>(v);
		}

		Int& operator = (unsigned long long v) {
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

		Int& operator = (long long v) {
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
		size_t numChunks() const { return _chunks.size(); }
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
			v.negate();
			return v;
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
			v &= v2;
			return v;
		}
		friend Int operator | (const Int& v1, const Int& v2) {
			Int v = v1;
			v |= v2;
			return v;
		}
		friend Int operator ^ (const Int& v1, const Int& v2) {
			Int v = v1;
			v ^= v2;
			return v;
		}

		friend Int operator >> (const Int& v1, int pos) {
			Int v = v1;
			v >>= pos;
			return v;
		}

		friend Int operator << (const Int& v1, int pos) {
			Int v = v1;
			v <<= pos;
			return v;
		}

		friend Int operator +(const Int& v1, const Int& v2) {
			Int sum;
			add(v1, v2, sum);
			return sum;
		}

		friend Int operator -(const Int& v1, const Int& v2) {
			Int sub;
			subtract(v1, v2, sub);
			return sub;
		}
		friend Int operator *(const Int& v1, const Int& v2) {
			Int product;
			multiply(v1, v2, product);
			return product;
		}
		friend Int operator /(const Int& v1, const Int& v2) {
			Int q;
			divide(v1, v2, q, q.n1());
			return q;
		}
		friend Int operator %(const Int& v1, const Int& v2) {
			Int q;
			divide(v1, v2, q, q.n1());
			q.swap(q.n1());
			return q;
		}

		// Prefix decrement operator.
		Int& operator --() {
			return *this -= one;
		}

		// Postfix decrement operator. 
		Int operator --(int) {
			Int tmp; 
			tmp.swap(*this);
			subtract(tmp, one, *this);
			return tmp;
		}

		// Prefix increment operator.
		Int& operator ++() {
			return *this += one;
		}

		// Postfix increment operator. 
		Int operator ++(int) {
			Int tmp;
			tmp.swap(*this);
			add (tmp, one, *this);
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
			 add(*this, v1, n1());
			 return this->swap(n1());
		}
		Int& operator -= (const Int& v1) {
			subtract(*this, v1, n1());
			return this->swap(n1());
		}
		Int& operator *= (const Int& v1) {
			multiply(*this, v1, n1());
			return this->swap(n1());
		}
		Int& operator /= (const Int& v1) {
			divide(*this, v1, n1(), n2());
			return this->swap(n1());
		}
		Int& operator %= (const Int& v1) {
			divide(*this, v1, n1(), n2());
			return this->swap(n2());
		}

		friend Int& add(const Int& v1, const Int& v2, Int& sum);
		friend Int& subtract(const Int& v1, const Int& v2, Int& sub);
		friend Int& multiply(const Int& v1, const Int& v2, Int& product);
		friend Int& divide(const Int& v1, const Int& v2, Int& q, Int& r);

		void cleanCache() {
			if (_n1) {
				delete _n1;
				_n1 = nullptr;
			}

			if (_n2) {
				delete _n2;
				_n2 = nullptr;
			}
		}

		Int& swap(Int& v) {
			std::swap(_chunks, v._chunks);
			std::swap(_sign, v._sign);
			return *this;
		}

	private:
		Int& normalize() {
			while (_chunks.size() > 0) {
				if (_chunks[_chunks.size() - 1] != 0) {
					return *this;
				}
				_chunks.pop_back();
			}
			if (_chunks.size() == 0) {
				_sign = 0;
			}
			return *this;
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

		void chunksAdd(const std::vector<Int::typeChunk>& chunks1, const std::vector<Int::typeChunk>& chunks2);
		void chunksSubtract(const std::vector<Int::typeChunk>& chunks1, const std::vector<Int::typeChunk>& chunks2);

		void chunksShiftRight(unsigned int pos);
		void chunksShiftLeft(unsigned int pos);

		Int& n1() {
			if (!_n1) {
				_n1 = new Int();
			}
			return *_n1;
		}
		
		Int& n2() {
			if (!_n2) {
				_n2 = new Int();
			}
			return *_n2;
		}

	private:
		int _sign{ 0 };
		std::vector<typeChunk> _chunks;

		Int* _n1{ nullptr };
		Int* _n2{ nullptr };
	};
}

#endif
