#include "INT.h"
#include <assert.h> 

using namespace zju04nycs;

int chunksCompare(const std::vector<INT::typeChunk>& chunks1, const std::vector<INT::typeChunk>& chunks2);
int bitsCompare(const INT& v1, int leadBit1, const INT& v2, int leadBit2, int numberBits);

bool isDigit(char c, int base, int& digitValue);
char chunkToDigit(INT::typeChunk chunk, int base);

const INT INT::s_smallInts[101] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
	10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
	20,	21, 22, 23, 24, 25, 26, 27, 28, 29,
	30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
	40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
	50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
	60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
	70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
	80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
	90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
	100
};

INT::INT() {
	int i = 0;
	++i;
}

INT::INT(const INT& v) :_chunks(v._chunks), _sign(v._sign)
{
	int j = 0;
	++j;
}

INT::INT(INT&& v) noexcept :_chunks(std::move(v._chunks)), _sign(v._sign) 
{
	int g = 0;
	g++;
}

INT::INT(const INT&& v) noexcept :_chunks(std::move(v._chunks)), _sign(v._sign) 
{
	int g = 0;
	g++;
}

INT& INT::operator = (const INT& v) {
	if (this == &v) {
		return *this;
	}
	_chunks = v._chunks;
	_sign = v._sign;
	return *this;
}

INT& INT::operator = (INT&& v) noexcept {
	if (this == &v) {
		return *this;
	}
	_chunks = std::move(v._chunks);
	_sign = v._sign;
	return *this;
}

INT& INT::operator = (const INT&& v) noexcept {
	if (this == &v) {
		return *this;
	}
	_chunks = std::move(v._chunks);
	_sign = v._sign;
	return *this;
}

INT::typeChunkSigned INT::toChunkSigned(bool& isOverFlow) const {
	static const INT::typeChunk signMask = 1 << (INT::s_numBitsOfChunk - 1);
	static const INT::typeChunk valueMask = ~signMask;

	if (_chunks.size() == 0) {
		isOverFlow = false;
		return 0;
	}

	auto tailChunk = _chunks[0];
	INT::typeChunkSigned  result = tailChunk & valueMask;

	if (_chunks.size() == 1) {
		if (_sign > 0) {
			isOverFlow = (signMask & tailChunk);
			return  result;
		}
		else
		{
			if (!(tailChunk & signMask)) {
				isOverFlow = false;
				return -result;
			}
			else {
				isOverFlow = result > 0;
				if (isOverFlow) {
					return -result;
				}
				else {
					INT::typeChunk limitValue = 1 << (s_numBitsOfChunk - 1);
					return static_cast<INT::typeChunkSigned>(limitValue);
				}
			}
		}
	}
	else {
		isOverFlow = true;
		return _sign ? result : -result;
	}

	return result;
}

INT::typeLinkSigned INT::toLinkSigned(bool& isOverFlow) const {
	static const INT::typeChunk signMask = 1 << (INT::s_numBitsOfChunk - 1);
	static const INT::typeChunk valueMask = ~signMask;

	if (_chunks.size() == 0) {
		isOverFlow = false;
		return 0;
	}

	if (_chunks.size() == 1) {
		isOverFlow = false;
		INT::typeLinkSigned result = _chunks[0];
		return _sign > 0 ? result : -result;
	}

	INT::typeLink lowChunk = _chunks[0];
	INT::typeChunk highChunk = _chunks[1];
	INT::typeLinkSigned highChunkMasked = highChunk & valueMask;

	INT::typeLinkSigned result = highChunkMasked << INT::s_numBitsOfChunk;
	result += lowChunk;

	if (_chunks.size() == 2) {
		if (_sign > 0) {
			isOverFlow = highChunk & signMask;
			return result;
		} 
		else {
			if (!(highChunk & signMask)) {
				isOverFlow = false;
				return -result;
			}
			else {
				isOverFlow = result > 0;
				if (isOverFlow) {
					return -result;
				}
				else {
					INT::typeLink limitValue = 1;
					limitValue <<= (s_numBitsOfChunk * 2 - 1);
					return static_cast<INT::typeChunkSigned>(limitValue);
				}
			}
		}
	}
	else {
		isOverFlow = true;
		return _sign ? result : -result;
	}

	return result;
}

std::string INT::toString(int base) const {
	assert(base >= 2 && base <= 35);
	std::string str;
	if (_sign < 0) {
		str += '-';
	}

	if (_chunks.size() == 0) {
		str += '0';
		return str;
	}

	if (_chunks.size() == 1 && static_cast<int>(_chunks[0]) < base) {
		//TODO:
		//optimize when size == 1
		str += chunkToDigit(_chunks[0], base);
		return str;
	}

	INT r;
	INT n = *this;
	const INT& d = s_smallInts[base];
	INT q;

	while (n >= d) {
		n = divide(n, d, q, r);
		if (r._chunks.size() == 0) {
			str += '0';
		}
		else {
			str += chunkToDigit(r._chunks[0], base);
		}
	}

	if (n._chunks.size() > 0) {
		str += chunkToDigit(n._chunks[0], base);
	}

	size_t i = 0;
	do{
		size_t targetIndex = i;
		if (_sign < 0) {
			targetIndex = i + 1;
		}
		size_t swapIndex = str.size() - 1 - i;
		if (targetIndex < swapIndex) {
			std::swap(str[targetIndex], str[swapIndex]);
			++i;
		}
		else {
			break;
		}
	} while (true);

	return str;
}

void INT::setValueWithString(const std::string& str, int base) {
	assert(base >= 2 && base <= 35);

	clear();
	if (str.size() == 0) {
		return;
	}

	size_t index = 0;
	enum state {
		init,
		set_digit
	};

	state s = state::init;
	int digitValue;
	int sign = 1;
	while (index < str.size()) {
		char c = str[index++];
		switch (s)
		{
		case init:
			if (c == '-') {
				sign = -1;
				s = set_digit;
			}
			else if (c == '+') {
				sign = 1;
				s = set_digit;
			}
			else if (c == ' ' || c == '\t' || c == '\n') {
				continue;
			}
			else if (isDigit(c, base, digitValue))
			{
				*this *= s_smallInts[base];
				*this += s_smallInts[digitValue];
				s = set_digit;
			}
			break;
		case set_digit:
			if (!isDigit(c, base, digitValue)) {
				_sign = sign;
				normalize();
				return;
			}
			else {
				*this *= s_smallInts[base];
				*this += s_smallInts[digitValue];
			}
			break;
		default:
			break;
		}
	}
	_sign = sign;
	normalize();
}

int INT::bit(size_t pos) const {
	if (pos >= _chunks.size() * INT::s_numBitsOfChunk) {
		return 0;
	}

	size_t chunkIndex = pos / INT::s_numBitsOfChunk;
	size_t bitIndex = pos - chunkIndex * INT::s_numBitsOfChunk;

	return (_chunks[chunkIndex] & (1 << bitIndex)) ? 1 : 0;
}

int INT::leadBit() const {
	if (_chunks.size() == 0) {
		return -1;
	}

	auto leadChunk = _chunks[_chunks.size() - 1];
	size_t leadBitInChunk = INT::s_numBitsOfChunk - 1;
	while (leadBitInChunk != 0) {
		INT::typeChunk mask = 1;
		mask <<= leadBitInChunk;
		if (leadChunk & mask) {
			break;
		}
		else {
			--leadBitInChunk;
		}
	}

	return leadBitInChunk + INT::s_numBitsOfChunk * (_chunks.size()-1);
}

int INT::tailBit() const {
	if (_chunks.size() == 0) {
		return -1;
	}

	size_t chunkIndex = 0;
	INT::typeChunk tailChunk = 0;
	while (chunkIndex < _chunks.size() - 1) {
		tailChunk = _chunks[chunkIndex];
		if (tailChunk != 0) {
			break;
		}
		++chunkIndex;
	}

	int tailBitInChunk = 0;
	while (tailBitInChunk < INT::s_numBitsOfChunk - 1) {
		INT::typeChunk mask = 1;
		mask <<= tailBitInChunk;
		if (tailChunk & mask) {
			break;
		}
		else {
			++tailBitInChunk;
		}
	}

	return tailBitInChunk + INT::s_numBitsOfChunk * (chunkIndex - 1);;
}

void INT::setBitWithoutNormalization(size_t bitPos, bool v) {
	size_t chunkIndex = bitPos / s_numBitsOfChunk;
	size_t bitIndex = bitPos - chunkIndex * s_numBitsOfChunk;

	if (chunkIndex + 1 > _chunks.size()) {
		_chunks.resize(chunkIndex + 1);
	}

	if (v) {
		_chunks[chunkIndex] |= (1 << bitIndex);
	}
	else {
		_chunks[chunkIndex] &= ~(1 << bitIndex);
	}
}

bool zju04nycs::operator == (const INT& v1, const INT& v2) {
	if (&v1 == &v2) {
		return true;
	}

	if (v1._sign != v2._sign) {
		return false;
	}

	if (v1._chunks.size() != v2._chunks.size()) {
		return false;
	}

	for (size_t i = 0; i < v1._chunks.size(); ++i) {
		if (v1._chunks[i] != v2._chunks[i]) {
			return false;
		}
	}
	return true;
}

int zju04nycs::compare(const INT& v1, const INT& v2) {
	if (&v1 == &v2) {
		return 0;
	}

	if (v1._sign != v2._sign) {
		return v1._sign > v2._sign ? 1 : -1;
	}

	auto chunksResult = chunksCompare(v1._chunks, v2._chunks);
	return v1._sign > 0 ? chunksResult : -chunksResult;
}

INT& INT::operator &= (const INT& v1) {
	if (_chunks.size() > v1._chunks.size()) {
		_chunks.resize(v1._chunks.size());
	}

	for (size_t i = 0; i < _chunks.size(); ++i) {
		_chunks[i] = _chunks[i] & v1._chunks[i];
	}
	normalize();

	return *this;
}

INT& INT::operator |= (const INT& v1) {
	if (v1.isZero()) {
		return *this;
	}

	if (isZero()) {
		return *this = v1;
	}

	if (_chunks.size() < v1._chunks.size()) {
		_chunks.resize(v1._chunks.size());
	}

	for (size_t i = 0; i < _chunks.size(); ++i) {
		_chunks[i] |= v1.chunk(i);
	}
	//do not need normalize

	return *this;
}

INT& INT::operator ^= (const INT& v1) {
	if (v1._chunks.size() > _chunks.size()) {
		_chunks.resize(v1._chunks.size());
	}
	for (size_t i = 0; i < _chunks.size(); ++i) {
		_chunks[i] ^= v1.chunk(i);
	}
	normalize();
	return *this;
}

INT& zju04nycs::plus(const INT& v1, const INT& v2, INT& sum){
	if (v1.isZero()) {
		sum = v2;
		return sum;
	}

	if (v2.isZero()) {
		sum = v1;
		return sum;
	}

	if (v1._sign == v2._sign) {
		sum.chunksPlus(v1._chunks, v2._chunks);
		sum._sign = v1._sign;
		sum.normalize();
		return sum;
	}
	else {
		int comResult = chunksCompare(v1._chunks, v2._chunks);
		if (comResult == 0) {
			sum.clear();
			return sum;
		}
		else {
			if (comResult > 0) {
				sum._sign = v1._sign;
				sum.chunksSubtract(v1._chunks, v2._chunks);
			}
			else {
				sum._sign = v2._sign;
				sum.chunksSubtract(v2._chunks, v1._chunks);
			}
			sum.normalize();
			return sum;
		}
	}
}

INT& zju04nycs::subtract(const INT& v1, const INT& v2, INT& sub) {
	if (v1.isZero()) {
		sub = v2;
		sub.negate();
		return sub;
	}

	if (v2.isZero()) {
		sub = v1;
		return sub;
	}

	sub.clear();
	if (v1._sign == -v2._sign) {
		sub._sign = v1._sign;
		sub.chunksPlus(v1._chunks, v2._chunks);
		sub.normalize();
		return sub;
	}
	else {
		int comResult = chunksCompare(v1._chunks, v2._chunks);
		if (comResult == 0) {
			return sub;
		} 
		else {
			if (comResult > 0) {
				sub._sign = v1._sign;
				sub.chunksSubtract(v1._chunks, v2._chunks);
			} 
			else {
				sub._sign = -v1._sign;
				sub.chunksSubtract(v2._chunks, v1._chunks);
			}
			sub.normalize();
			return sub;
		}
	}
}

INT& zju04nycs::multiply(const INT & v1, const INT & v2, INT & product) {
	product.clear();
	if (v1.isZero() || v2.isZero()) {
		return product;
	}
	else {
		product._sign = v1._sign == v2._sign ? 1 : -1;
		for (size_t i = 0; i < v2._chunks.size(); ++i) {
			INT::typeChunk carry = 0;
			for (size_t j = 0; j < v1._chunks.size(); ++j) {
				INT::typeLink partSum =
					INT::typeLink(product.chunk(i + j)) + INT::typeLink(v1._chunks[j]) * INT::typeLink(v2._chunks[i]) + carry;
				product.setChunk(i+j, static_cast<INT::typeChunk>(partSum));
				carry = static_cast<INT::typeChunk>(partSum >> INT::s_numBitsOfChunk);
				if (carry > 0) {
					if (j == v1._chunks.size() - 1) {
						assert(product.chunk(i + j + 1) == 0);
						product.setChunk(i + j + 1, carry);
					}
				}
			}
		}
		product.normalize();
		return product;
	}
}

INT& zju04nycs::divide (const INT& v1, const INT& v2, INT& q, INT& r) {
	assert(!v2.isZero());
	if (v1.isZero()) {
		q.clear();
		r = v1;
		return q;
	}

	int comResult = chunksCompare(v1._chunks, v2._chunks);
	if (comResult == 0) {
		//q = 1;
		q.clear();
		q.setChunk(0, 1);
		q.normalize();

		r.clear();
		return q;
	}
	else if (comResult < 0){
		q.clear();
		r = v1;
		return q;
	}
	else {
		r = v1;
		int leadBitR = r.leadBit();

		const INT& d = v2;
		const int leadBitD = d.leadBit();

		q.clear();
		q._sign = v1._sign == v2._sign ? 1 : -1;
		//leadBit2 must be less or equal to leadBit1
		while (true) {
			if (bitsCompare(r, leadBitR, d, leadBitD, leadBitD + 1) >= 0) {
				q.setBitWithoutNormalization(leadBitR - leadBitD, 1);
				bitsSubtract(r, leadBitR, leadBitR - leadBitD, d, 0, leadBitD + 1);
			}
			else {
				q.setBitWithoutNormalization(leadBitR - leadBitD - 1, 1);
				bitsSubtract(r, leadBitR, leadBitR - leadBitD -1, d, 0, leadBitD + 1);
			}

			r.normalize();
			int comResult = chunksCompare(r._chunks, d._chunks);
			if (comResult < 0) {
				q.normalize();
				return q;
			}

			leadBitR = r.leadBit();
		}
	}
}

int chunksCompare(const std::vector<INT::typeChunk>& chunks1, const std::vector<INT::typeChunk>& chunks2) {
	int size1 = chunks1.size();
	int size2 = chunks2.size();
	if (size1 != size2) {
		return  size1 > size2 ? 1 : -1;
	}

	if (size1 == 0) {
		return 0;
	}

	for (size_t i = size1; i >= 1; --i) {
		auto b1 = chunks1[i -1];
		auto b2 = chunks2[i -1];
		if (b1 != b2) {
			return b1 > b2 ? 1 : -1;
		}
	}
	return 0;
}

void INT::chunksShiftRight(unsigned int pos) {
	if (pos == 0) {
		return;
	}

	if (_chunks.size() == 0) {
		return;
	}

	unsigned int shiftChunks = pos / INT::s_numBitsOfChunk;
	unsigned int shiftBits = pos - shiftChunks * INT::s_numBitsOfChunk;
	unsigned int crossShiftBits = INT::s_numBitsOfChunk - shiftBits;

	if (_chunks.size() <= shiftChunks) {
		_chunks.clear();
		return;
	}

	for (size_t i = shiftChunks; i < _chunks.size(); ++i) {
		_chunks[i - shiftChunks] = _chunks[i];
	}

	_chunks.resize(_chunks.size() - shiftChunks);
	if (shiftBits == 0) {
		return;
	}

	INT::typeChunk mask = (1 << shiftBits) - 1;
	for (size_t i = 0; i < _chunks.size(); ++i) {
		_chunks[i] >>= shiftBits;
		if (i < _chunks.size() - 1) {
			_chunks[i] |= ((_chunks[i + 1] & mask) << crossShiftBits);
		}
	}
}

void INT::chunksShiftLeft(unsigned int pos) {
	if (pos == 0) {
		return;
	}

	if (_chunks.size() == 0) {
		return;
	}

	unsigned int shiftChunks = pos / INT::s_numBitsOfChunk;
	unsigned int shiftBits = pos - shiftChunks * INT::s_numBitsOfChunk;
	unsigned int crossShiftBits = INT::s_numBitsOfChunk - shiftBits;

	auto oldSize = _chunks.size();
	auto newSize = oldSize + shiftChunks;
	_chunks.resize(newSize);
	for (size_t i = 0; i < oldSize; ++i) {
		auto index = newSize - 1 - i;
		_chunks[index] = _chunks[index - shiftChunks];
	}

	for (size_t i = 0; i < shiftChunks; ++i) {
		_chunks[i] = 0;
	}

	if (shiftBits == 0) {
		return;
	}

	INT::typeChunk mask = ((1 << shiftBits) - 1) << crossShiftBits;
	for (int i = _chunks.size() - 1 ; i >= static_cast<int>(shiftChunks); --i) {
		INT::typeChunk shiftOutChunk = ((_chunks[i] & mask) >> crossShiftBits);
		if (shiftOutChunk > 0) {
			if (i < _chunks.size() - 1) {
				_chunks[i + 1] |= shiftOutChunk;
			}
			else {
				_chunks.push_back(shiftOutChunk);
			}
		}
		_chunks[i] <<= shiftBits;
	}
}

void INT::chunksPlus(const std::vector<INT::typeChunk>& chunks1, const std::vector<INT::typeChunk>& chunks2) {
	auto pChunksLeft = &chunks1;
	auto pChunksRight = &chunks2;
	if (pChunksRight->size() > pChunksLeft->size()) {
		std::swap(pChunksLeft, pChunksRight);
	}

	INT::typeLink carry = 0;
	_chunks = *pChunksLeft;
	for (size_t i = 0; i < _chunks.size(); ++i) {
		INT::typeLink partSum = 0;
		if (i < pChunksRight->size()) {
			partSum = INT::typeLink((*pChunksLeft)[i]) + INT::typeLink((*pChunksRight)[i]) + carry;
		}
		else {
			partSum = INT::typeLink((*pChunksLeft)[i]) + carry;
		}
		_chunks[i] = static_cast<INT::typeChunk>(partSum);
		carry = partSum >> INT::s_numBitsOfChunk;
	}

	if (carry != 0) {
		_chunks.push_back(static_cast<INT::typeChunk>(carry));
	}
}

void INT::chunksSubtract(const std::vector<INT::typeChunk>& chunks1, const std::vector<INT::typeChunk>& chunks2) {
	_chunks.resize(chunks1.size());
	INT::typeLink borrow = 0;
	for (size_t i = 0; i < chunks1.size(); ++i) {
		INT::typeLink chunk1 = chunks1[i];
		INT::typeLink chunk2 = 0;
		if (i < chunks2.size()) {
			chunk2 = chunks2[i];
		}
		if (chunk1 >= chunk2 + borrow) {
			_chunks[i] = static_cast<INT::typeChunk>(chunk1 - chunk2 - borrow);
			borrow = 0;
		}
		else {
			_chunks[i] = static_cast<INT::typeChunk>(INT::s_borrowChunkValue + chunk1
				- chunk2 - borrow);
			borrow = 1;
		}
	}
}

int bitsCompare(const INT& v1, int leadBit1, const INT& v2, int leadBit2, int numberBits) {
	int leadOffset = 0;
	while (leadOffset < numberBits) {
		int bit1 = v1.bit(leadBit1 - leadOffset);
		int bit2 = v2.bit(leadBit2 - leadOffset);
		if (bit1 != bit2) {
			return bit1 > bit2 ? 1 : -1;
		}
		++leadOffset;
	}

	return 0;
}

 void zju04nycs::bitsSubtract(INT& v1, int leadBit1, int tailBit1, const INT& v2, int tailBit2, int numberBits){
	int bitOffset = 0;
	int borrow = 0;
	while (tailBit1 + bitOffset <= leadBit1) {
		int bit1 = v1.bit(tailBit1 + bitOffset);
		int bit2 = v2.bit(tailBit2 + bitOffset);
		if (bit1 >= bit2 + borrow) {
			v1.setBitWithoutNormalization(tailBit1 + bitOffset, bit1 - bit2 - borrow);
			borrow = 0;
		}
		else {
			v1.setBitWithoutNormalization(tailBit1 + bitOffset, bit1 + 2 - bit2 - borrow);
			borrow = 1;
		}

		++bitOffset;
	}
}

 bool isDigit(char c, int base, int& digitValue) {
	 digitValue = 0;
	 if (c >= '0' && c <= '9') {
		 digitValue = c - '0';
		 if (digitValue < base) {
			 return true;
		 }
	 }
	 else if (c >= 'a' && c <= 'z') {
		 digitValue = 10 + c - 'a';
		 if (digitValue < base) {
			 return true;
		 }
	 }
	 else if (c >= 'A' && c <= 'Z') {
		 digitValue = 10 + c - 'A';
		 if (digitValue < base) {
			 return true;
		 }
	 }

	 return false;
}

char chunkToDigit(INT::typeChunk chunk, int base) {
	if (chunk >= 0 && static_cast<int>(chunk) < base) {
		if (chunk >= 0 && chunk < 10) {
			return '0' + chunk;
		}
		else
		{
			return chunk - 10 + 'A';
		}
	}
	return '#';
}

