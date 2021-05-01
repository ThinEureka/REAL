#include "Int.h"
#include <assert.h> 

using namespace real;

int chunksCompare(const std::vector<Int::typeChunk>& chunks1, const std::vector<Int::typeChunk>& chunks2);
int bitsCompare(const Int& v1, int leadBit1, const Int& v2, int leadBit2, int numberBits);

const Int Int::s_smallInts[101] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
	10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
	20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
	30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
	40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
	50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
	60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
	70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
	80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
	90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
	100
};

const Int& Int::zero = Int::s_smallInts[0];
const Int& Int::one = Int::s_smallInts[1];

Int& Int::operator = (const Int& v) {
	if (this == &v) {
		return *this;
	}
	_chunks = v._chunks;
	_sign = v._sign;
	return *this;
}

Int& Int::operator = (Int&& v) noexcept {
	if (this == &v) {
		return *this;
	}
	_chunks = std::move(v._chunks);
	_sign = v._sign;
	_n1 = v._n1;
	_n2 = v._n2;

	v._n1 = nullptr;
	v._n2 = nullptr;

	return *this;
}

Int::typeChunkSigned Int::toChunkSigned(bool& isOverFlow) const {
	static const Int::typeChunk signMask = 1 << (Int::s_numBitsOfChunk - 1);
	static const Int::typeChunk valueMask = ~signMask;

	if (_chunks.size() == 0) {
		isOverFlow = false;
		return 0;
	}

	auto tailChunk = _chunks[0];
	Int::typeChunkSigned  result = tailChunk & valueMask;

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
					Int::typeChunk limitValue = 1 << (s_numBitsOfChunk - 1);
					return static_cast<Int::typeChunkSigned>(limitValue);
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

Int::typeLinkSigned Int::toLinkSigned(bool& isOverFlow) const {
	static const Int::typeChunk signMask = 1 << (Int::s_numBitsOfChunk - 1);
	static const Int::typeChunk valueMask = ~signMask;

	if (_chunks.size() == 0) {
		isOverFlow = false;
		return 0;
	}

	if (_chunks.size() == 1) {
		isOverFlow = false;
		Int::typeLinkSigned result = _chunks[0];
		return _sign > 0 ? result : -result;
	}

	Int::typeLink lowChunk = _chunks[0];
	Int::typeChunk highChunk = _chunks[1];
	Int::typeLinkSigned highChunkMasked = highChunk & valueMask;

	Int::typeLinkSigned result = highChunkMasked << Int::s_numBitsOfChunk;
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
					Int::typeLink limitValue = 1;
					limitValue <<= (s_numBitsOfChunk * 2 - 1);
					return static_cast<Int::typeChunkSigned>(limitValue);
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

std::string Int::toString(int base, Int* cacheR, Int* cacheN, Int* cacheQ) const {
	assert(base >= 2 && base <= 35);
	if (_sign == 0) {
		return "0";
	}

	std::string str;
	if (_sign < 0) {
		str += '-';
	}

	if (_chunks.size() == 0) {
		str += '0';
		return str;
	}

	if (_chunks.size() == 1 && _chunks[0] < static_cast<Int::typeChunk>(base)) {
		//TODO:
		//optimize when size == 1
		str += chunkToDigit(_chunks[0], base);
		return str;
	}

	Int& r = cacheR ? *cacheR : *(new Int);
	Int& n = cacheR ? *cacheN : *(new Int(*this));
	Int& q = cacheR ? *cacheQ : *(new Int());
	const Int& d = s_smallInts[base];

	while (chunksCompare(n._chunks, d._chunks) >= 0) {
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

	if (!cacheN) delete& n;
	if (!cacheR) delete& r;
	if (!cacheQ) delete& q;

	return str;
}

Int& Int::set(const std::string& str, int base) {
	assert(base >= 2 && base <= 35);

	setZero();
	if (str.size() == 0) {
		return *this;
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
			else if (Int::isDigit(c, base, digitValue))
			{
				*this *= s_smallInts[base];
				*this += s_smallInts[digitValue];
				s = set_digit;
			}
			break;
		case set_digit:
			if (!Int::isDigit(c, base, digitValue)) {
				_sign = sign;
				normalize();
				return *this;
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
	return *this;
}

int Int::bit(size_t pos) const {
	if (pos >= _chunks.size() * Int::s_numBitsOfChunk) {
		return 0;
	}

	size_t chunkIndex = pos / Int::s_numBitsOfChunk;
	size_t bitIndex = pos - chunkIndex * Int::s_numBitsOfChunk;

	return (_chunks[chunkIndex] & (1 << bitIndex)) ? 1 : 0;
}

int Int::leadBit() const {
	if (_chunks.size() == 0) {
		return -1;
	}

	auto leadChunk = _chunks[_chunks.size() - 1];
	size_t leadBitInChunk = Int::s_numBitsOfChunk - 1;
	while (leadBitInChunk != 0) {
		Int::typeChunk mask = 1;
		mask <<= leadBitInChunk;
		if (leadChunk & mask) {
			break;
		}
		else {
			--leadBitInChunk;
		}
	}

	return static_cast<int>(leadBitInChunk + Int::s_numBitsOfChunk * (_chunks.size() - 1));
}

int Int::tailBit() const {
	if (_chunks.size() == 0) {
		return -1;
	}

	size_t chunkIndex = 0;
	Int::typeChunk tailChunk = 0;
	while (chunkIndex < _chunks.size()) {
		tailChunk = _chunks[chunkIndex];
		if (tailChunk != 0) {
			break;
		}
		++chunkIndex;
	}

	int tailBitInChunk = 0;
	while (tailBitInChunk < Int::s_numBitsOfChunk - 1) {
		Int::typeChunk mask = 1;
		mask <<= tailBitInChunk;
		if (tailChunk & mask) {
			break;
		}
		else {
			++tailBitInChunk;
		}
	}

	return static_cast<int>(tailBitInChunk + Int::s_numBitsOfChunk * chunkIndex);
}

void Int::setBitWithoutNormalization(size_t bitPos, bool v) {
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

bool real::operator == (const Int& v1, const Int& v2) {
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

int real::compare(const Int& v1, const Int& v2) {
	if (&v1 == &v2) {
		return 0;
	}

	if (v1._sign != v2._sign) {
		return v1._sign > v2._sign ? 1 : -1;
	}

	auto chunksResult = chunksCompare(v1._chunks, v2._chunks);
	return v1._sign > 0 ? chunksResult : -chunksResult;
}

int real::absCompare(const Int& v1, const Int& v2) {
	if (&v1 == &v2) {
		return 0;
	}

	auto chunksResult = chunksCompare(v1._chunks, v2._chunks);
	return v1._sign > 0 ? chunksResult : -chunksResult;
}

Int& Int::operator &= (const Int& v1) {
	if (_chunks.size() > v1._chunks.size()) {
		_chunks.resize(v1._chunks.size());
	}

	for (size_t i = 0; i < _chunks.size(); ++i) {
		_chunks[i] = _chunks[i] & v1._chunks[i];
	}
	return this->normalize();
}

Int& Int::operator |= (const Int& v1) {
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

Int& Int::operator ^= (const Int& v1) {
	if (v1._chunks.size() > _chunks.size()) {
		_chunks.resize(v1._chunks.size());
	}
	for (size_t i = 0; i < _chunks.size(); ++i) {
		_chunks[i] ^= v1.chunk(i);
	}
	return this->normalize();
}

Int& real::add(const Int& v1, const Int& v2, Int& sum){
	if (v1.isZero()) {
		sum = v2;
		return sum;
	}

	if (v2.isZero()) {
		sum = v1;
		return sum;
	}

	if (v1._sign == v2._sign) {
		sum.chunksAdd(v1._chunks, v2._chunks);
		sum._sign = v1._sign;
		sum.normalize();
		return sum;
	}
	else {
		int comResult = chunksCompare(v1._chunks, v2._chunks);
		if (comResult == 0) {
			sum.setZero();
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

Int& real::subtract(const Int& v1, const Int& v2, Int& sub) {
	if (v1.isZero()) {
		sub = v2;
		sub.negate();
		return sub;
	}

	if (v2.isZero()) {
		sub = v1;
		return sub;
	}

	sub.setZero();
	if (v1._sign == -v2._sign) {
		sub._sign = v1._sign;
		sub.chunksAdd(v1._chunks, v2._chunks);
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

Int& real::multiply(const Int & v1, const Int & v2, Int & product) {
	product.setZero();
	if (v1.isZero() || v2.isZero()) {
		return product;
	}
	else {
		product._sign = v1._sign == v2._sign ? 1 : -1;
		for (size_t i = 0; i < v2._chunks.size(); ++i) {
			Int::typeChunk carry = 0;
			for (size_t j = 0; j < v1._chunks.size(); ++j) {
				Int::typeLink partSum =
					Int::typeLink(product.chunk(i + j)) + Int::typeLink(v1._chunks[j]) * Int::typeLink(v2._chunks[i]) + carry;
				product.setChunk(i+j, static_cast<Int::typeChunk>(partSum));
				carry = static_cast<Int::typeChunk>(partSum >> Int::s_numBitsOfChunk);
				if (carry > 0) {
					if (j == v1._chunks.size() - 1) {
						product.setChunk(i + j + 1, carry);
					}
				}
			}
		}
		product.normalize();
		return product;
	}
}

Int& real::divide (const Int& v1, const Int& v2, Int& q, Int& r) {
	assert(!v2.isZero());
	if (v1.isZero()) {
		q.setZero();
		r = v1;
		return q;
	}

	int comResult = chunksCompare(v1._chunks, v2._chunks);
	if (comResult == 0) {
		q.setOne();
		r.setZero();
		return q;
	}
	else if (comResult < 0){
		q.setZero();
		r = v1;
		return q;
	}
	else {
		r = v1;
		int leadBitR = r.leadBit();

		const Int& d = v2;
		const int leadBitD = d.leadBit();

		q.setZero();
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

int chunksCompare(const std::vector<Int::typeChunk>& chunks1, const std::vector<Int::typeChunk>& chunks2) {
	int size1 = static_cast<int>(chunks1.size());
	int size2 = static_cast<int>(chunks2.size());
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

void Int::chunksShiftRight(unsigned int pos) {
	if (pos == 0) {
		return;
	}

	if (_chunks.size() == 0) {
		return;
	}

	unsigned int shiftChunks = pos / Int::s_numBitsOfChunk;
	unsigned int shiftBits = pos - shiftChunks * Int::s_numBitsOfChunk;
	unsigned int crossShiftBits = Int::s_numBitsOfChunk - shiftBits;

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

	Int::typeChunk mask = (1 << shiftBits) - 1;
	for (size_t i = 0; i < _chunks.size(); ++i) {
		_chunks[i] >>= shiftBits;
		if (i < _chunks.size() - 1) {
			_chunks[i] |= ((_chunks[i + 1] & mask) << crossShiftBits);
		}
	}
}

void Int::chunksShiftLeft(unsigned int pos) {
	if (pos == 0) {
		return;
	}

	if (_chunks.size() == 0) {
		return;
	}

	unsigned int shiftChunks = pos / Int::s_numBitsOfChunk;
	unsigned int shiftBits = pos - shiftChunks * Int::s_numBitsOfChunk;
	unsigned int crossShiftBits = Int::s_numBitsOfChunk - shiftBits;

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

	Int::typeChunk mask = ((1 << shiftBits) - 1) << crossShiftBits;
	for (int i = static_cast<int>(_chunks.size()) - 1 ; i >= static_cast<int>(shiftChunks); --i) {
		Int::typeChunk shiftOutChunk = ((_chunks[i] & mask) >> crossShiftBits);
		if (shiftOutChunk > 0) {
			if (i < static_cast<int>(_chunks.size()) - 1) {
				_chunks[i + 1] |= shiftOutChunk;
			}
			else {
				_chunks.push_back(shiftOutChunk);
			}
		}
		_chunks[i] <<= shiftBits;
	}
}

void Int::chunksAdd(const std::vector<Int::typeChunk>& chunks1, const std::vector<Int::typeChunk>& chunks2) {
	auto pChunksLeft = &chunks1;
	auto pChunksRight = &chunks2;
	if (pChunksRight->size() > pChunksLeft->size()) {
		std::swap(pChunksLeft, pChunksRight);
	}

	Int::typeLink carry = 0;
	_chunks = *pChunksLeft;
	for (size_t i = 0; i < _chunks.size(); ++i) {
		Int::typeLink partSum = 0;
		if (i < pChunksRight->size()) {
			partSum = Int::typeLink((*pChunksLeft)[i]) + Int::typeLink((*pChunksRight)[i]) + carry;
		}
		else {
			partSum = Int::typeLink((*pChunksLeft)[i]) + carry;
		}
		_chunks[i] = static_cast<Int::typeChunk>(partSum);
		carry = partSum >> Int::s_numBitsOfChunk;
	}

	if (carry != 0) {
		_chunks.push_back(static_cast<Int::typeChunk>(carry));
	}
}

void Int::chunksSubtract(const std::vector<Int::typeChunk>& chunks1, const std::vector<Int::typeChunk>& chunks2) {
	_chunks.resize(chunks1.size());
	Int::typeLink borrow = 0;
	for (size_t i = 0; i < chunks1.size(); ++i) {
		Int::typeLink chunk1 = chunks1[i];
		Int::typeLink chunk2 = 0;
		if (i < chunks2.size()) {
			chunk2 = chunks2[i];
		}
		if (chunk1 >= chunk2 + borrow) {
			_chunks[i] = static_cast<Int::typeChunk>(chunk1 - chunk2 - borrow);
			borrow = 0;
		}
		else {
			_chunks[i] = static_cast<Int::typeChunk>(Int::s_borrowChunkValue + chunk1
				- chunk2 - borrow);
			borrow = 1;
		}
	}
}

int bitsCompare(const Int& v1, int leadBit1, const Int& v2, int leadBit2, int numberBits) {
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

 void real::bitsSubtract(Int& v1, int leadBit1, int tailBit1, const Int& v2, int tailBit2, int numberBits){
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

 bool Int::isDigit(char c, int base, int& digitValue) {
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

char Int::chunkToDigit(Int::typeChunk chunk, int base) {
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



