#include "INT.h"
#include <assert.h> 

using namespace zju04nycs;

int chunksCompare(const std::vector<INT::typeChunk>& chunks1, const std::vector<INT::typeChunk>& chunks2);
std::vector<INT::typeChunk> chunksShiftRight(const std::vector<INT::typeChunk>& chunks1, unsigned int pos);
std::vector<INT::typeChunk> chunksShiftLeft(const std::vector<INT::typeChunk>& chunks1, unsigned int pos);

std::vector<INT::typeChunk> chunksPlus(const std::vector<INT::typeChunk>& chunks1, const std::vector<INT::typeChunk>& chunks2);
std::vector<INT::typeChunk> chunksSubtract(const std::vector<INT::typeChunk>& chunks1, const std::vector<INT::typeChunk>& chunks2);

int bitsCompare(const INT& v1, int leadBit1, const INT& v2, int leadBit2, int numberBits);

bool isDigit(char c, int base, int& digitValue);
char chunkToDigit(INT::typeChunk chunk, int base);

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

INT::typeChunkSigned INT::toChunkSigned(bool& isTruncated) const {
	static const INT::typeChunk signMask = 1 << (INT::s_numBitsOfChunk - 1);
	static const INT::typeChunk valueMask = ~signMask;

	if (_chunks.size() == 0) {
		isTruncated = false;
		return 0;
	}

	auto tailChunk = _chunks[0];
	INT::typeChunkSigned  result = tailChunk & valueMask;

	if (_chunks.size() == 1) {
		if (_sign > 0) {
			isTruncated = (signMask & tailChunk);
			return  result;
		}
		else
		{
			if (!(tailChunk & signMask)) {
				isTruncated = false;
				return -result;
			}
			else {
				isTruncated = result > 0;
				if (isTruncated) {
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
		isTruncated = true;
		return _sign ? result : -result;
	}

	return result;
}

INT::typeLinkSigned INT::toLinkSigned(bool& isTruncated) const {
	static const INT::typeChunk signMask = 1 << (INT::s_numBitsOfChunk - 1);
	static const INT::typeChunk valueMask = ~signMask;

	if (_chunks.size() == 0) {
		isTruncated = false;
		return 0;
	}

	if (_chunks.size() == 1) {
		isTruncated = false;
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
			isTruncated = highChunk & signMask;
			return result;
		} 
		else {
			if (!(highChunk & signMask)) {
				isTruncated = false;
				return -result;
			}
			else {
				isTruncated = result > 0;
				if (isTruncated) {
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
		isTruncated = true;
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
	INT d = base;

	while (n >= d) {
		n = divide(n, d, r);
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
	while (index < str.size()) {
		char c = str[index++];
		switch (s)
		{
		case init:
			if (c == '-') {
				_sign = -1;
				s = set_digit;
			}
			else if (c == '+') {
				_sign = 1;
				s = set_digit;
			}
			else if (c == ' ' || c == '\t' || c == '\n') {
				continue;
			}
			else if (isDigit(c, base, digitValue))
			{
				*this *= INT(base);
				*this += INT(digitValue);
				s = set_digit;
			}
			break;
		case set_digit:
			if (!isDigit(c, base, digitValue)) {
				normalize();
				return;
			}
			else {
				*this *= INT(base);
				*this += INT(digitValue);
			}
			break;
		default:
			break;
		}
	}
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

INT zju04nycs::operator & (const INT& v1, const INT& v2) {
	const INT* pMax = &v1;
	const INT* pMin = &v2;
	if (pMin->_chunks.size() > pMax->_chunks.size()) {
		std::swap(pMin, pMax);
	}
	std::vector<INT::typeChunk> chunks;
	chunks.resize(pMin->_chunks.size());
	for (size_t i = 0; i < pMin->_chunks.size(); ++i) {
		chunks[i] = pMax->_chunks[i] & pMin->_chunks[i];
	}

	INT v = INT(v1._sign, std::move(chunks));
	return v;
}

INT zju04nycs::operator | (const INT& v1, const INT& v2) {
	const INT* pMax = &v1;
	const INT* pMin = &v2;
	if (pMin->_chunks.size() > pMax->_chunks.size()) {
		std::swap(pMin, pMax);
	}
	std::vector<INT::typeChunk> chunks = pMax->_chunks;
	for (size_t i = 0; i < pMin->_chunks.size(); ++i) {
		chunks[i] = pMax->_chunks[i] | pMin->_chunks[i];
	}

	INT v = INT(v1._sign, std::move(chunks));
	return v;
}

INT zju04nycs::operator ^ (const INT& v1, const INT& v2) {
	const INT* pMax = &v1;
	const INT* pMin = &v2;
	if (pMin->_chunks.size() > pMax->_chunks.size()) {
		std::swap(pMin, pMax);
	}
	std::vector<INT::typeChunk> chunks = pMax->_chunks;
	for (size_t i = 0; i < pMin->_chunks.size(); ++i) {
		chunks[i] = pMax->_chunks[i] ^ pMin->_chunks[i];
	}

	INT v = INT(v1._sign, std::move(chunks));
	return v;
}

INT zju04nycs::operator >> (const INT& v1, int pos) {
	if (pos == 0 || v1.isZero()) {
		return v1;
	}
	else if (pos > 0) {
		std::vector<INT::typeChunk> chunks = chunksShiftRight(v1._chunks, pos);
		INT result = INT(v1._sign, std::move(chunks));
		return result;
	}
	else {
		std::vector<INT::typeChunk> chunks = chunksShiftLeft(v1._chunks, -pos);
		INT result = INT(v1._sign, std::move(chunks));
		return result;
	}
}

INT zju04nycs::operator << (const INT& v1, int pos) {
	if (pos == 0 || v1.isZero()) {
		return v1;
	}
	else if (pos > 0) {
		std::vector<INT::typeChunk> chunks = chunksShiftLeft(v1._chunks, pos);
		INT result = INT(v1._sign, std::move(chunks));
		return result;
	}
	else {
		std::vector<INT::typeChunk> chunks = chunksShiftRight(v1._chunks, -pos);
		INT result = INT(v1._sign, std::move(chunks));
		return result;
	}
}

INT zju04nycs::operator + (const INT& v1, const INT& v2) {
	if (v1._sign == v2._sign) {
		return INT(v1._sign, chunksPlus(v1._chunks, v2._chunks));
	}
	else {
		int comResult = chunksCompare(v1._chunks, v2._chunks);
		if (comResult == 0) {
			INT zero;
			return zero;
		}
		else {
			auto chunks = comResult > 0 ? chunksSubtract(v1._chunks, v2._chunks) :
				chunksSubtract(v2._chunks, v1._chunks);
			INT sum = INT(comResult > 0 ? v1._sign : -v1._sign, std::move(chunks));
			return sum;
		}
	}
}

INT zju04nycs::operator - (const INT& v1, const INT& v2) {
	if (v1._sign == -v2._sign) {
		return INT(v1._sign, chunksPlus(v1._chunks, v2._chunks));
	}
	else {
		int comResult = chunksCompare(v1._chunks, v2._chunks);
		if (comResult == 0) {
			INT zero;
			return zero;
		} 
		else {
			auto chunks = comResult > 0 ? chunksSubtract(v1._chunks, v2._chunks) :
				chunksSubtract(v2._chunks, v1._chunks);
			INT sum = INT(comResult > 0 ? v1._sign : -v1._sign, std::move(chunks));
			return sum;
		}
	}
}

INT zju04nycs::operator * (const INT& v1, const INT& v2) {
	if (v1.isZero() || v2.isZero()) {
		INT zero;
		return zero;
	}
	else {
		std::vector<INT::typeChunk> chunks;
		chunks.resize(v1._chunks.size() + v2._chunks.size());
		for (size_t i = 0; i < v2._chunks.size(); ++i) {
			INT::typeChunk carry = 0;
			for (size_t j = 0; j < v1._chunks.size(); ++j) {
				INT::typeLink product = INT::typeLink(v1._chunks[j]) * INT::typeLink(v2._chunks[i]) + carry;
				chunks[i + j] += static_cast<INT::typeChunk>(product);
				carry = static_cast<INT::typeChunk>(product >> INT::s_numBitsOfChunk);
				if (j == v1._chunks.size() - 1) {
					chunks[i + v2._chunks.size()] += carry;
				}
			}
		}
		INT v = INT(v1._sign == v2._sign ? 1 : -1, std::move(chunks));
		return v;
	}
}

INT zju04nycs::divide (const INT& v1, const INT& v2,  INT& r) {
	assert(!v2.isZero());
	if (v1.isZero()) {
		r.clear();
		INT zero;
		return zero;
	}

	int comResult = chunksCompare(v1._chunks, v2._chunks);
	if (comResult == 0) {
		INT v = 1;
		v._sign = v1._sign == v2._sign ? 1 : -1;
		r.clear();
		return v;
	}
	else if (comResult < 0){
		r = v1;
		INT zero;
		return zero;
	}
	else {
		INT r;
		INT tmpD;
		const INT* pD = &v2;

		int tail = std::min(v1.tailBit(), v2.tailBit());
		if (tail > 0) {
			r = v1 >> tail;
			tmpD = (*pD) >> tail;
			pD = &tmpD;
		}
		else {
			r = v1;
		}
		INT* pN = &r;

		int leadBitN = pN->leadBit();
		int leadBitD = pD->leadBit();

		INT q;
		q._sign = v1._sign == v2._sign ? 1 : -1;
		//leadBit2 must be less or equal to leadBit1
		while (true) {
			if (bitsCompare(*pN, leadBitN, *pD, leadBitD, leadBitD + 1) >= 0) {
				q.setBitWithoutNormalization(leadBitN - leadBitD, 1);
				bitsSubtract(*pN, leadBitN - leadBitD, *pD, 0, leadBitD + 1);
			}
			else {
				q.setBitWithoutNormalization(leadBitN - leadBitD - 1, 1);
				bitsSubtract(*pN, leadBitN - leadBitD -1, *pD, 0, leadBitD + 1);
			}

			pN->normalize();
			int comResult = chunksCompare(pN->_chunks, pD->_chunks);
			if (comResult <= 0) {
				q.normalize();
				return q;
			}

			leadBitN = pN->leadBit();
			leadBitD = pD->leadBit();
		}
		return q;
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

	for (size_t i = size1 -1; i >= 0; --i) {
		auto b1 = chunks1[i];
		auto b2 = chunks2[i];
		if (b1 != b2) {
			return b1 > b2 ? 1 : -1;
		}
	}
	return 0;
}

std::vector<INT::typeChunk> chunksShiftRight(const std::vector<INT::typeChunk>& chunks1, unsigned int pos) {
	std::vector<INT::typeChunk> chunks;
	if (chunks1.size() == 0) {
		return chunks;
	}

	unsigned int shiftChunks = pos / INT::s_numBitsOfChunk;
	unsigned int shiftBits = pos - shiftChunks * INT::s_numBitsOfChunk;
	unsigned int crossShiftBits = INT::s_numBitsOfChunk - shiftBits;

	if (chunks1.size() <= shiftChunks) {
		return chunks;
	}

	chunks.resize(chunks1.size() - shiftChunks);
	for (size_t i = shiftChunks; i < chunks1.size(); ++i) {
		chunks[i - shiftChunks] = chunks1[i];
	}

	if (shiftBits == 0) {
		return chunks;
	}

	INT::typeChunk mask = (1 << shiftBits) - 1;
	for (size_t i = 0; i < chunks.size(); ++i) {
		chunks[i] >>= shiftBits;
		if (i < chunks.size() - 1) {
			chunks[i] |= ((chunks[i + 1] & mask) << crossShiftBits);
		}
	}

	return chunks;
}

std::vector<INT::typeChunk> chunksShiftLeft(const std::vector<INT::typeChunk>& chunks1, unsigned int pos) {
	std::vector<INT::typeChunk> chunks;
	if (chunks1.size() == 0) {
		return chunks;
	}

	unsigned int shiftChunks = pos / INT::s_numBitsOfChunk;
	unsigned int shiftBits = pos - shiftChunks * INT::s_numBitsOfChunk;
	unsigned int crossShiftBits = INT::s_numBitsOfChunk - shiftBits;

	chunks.resize(chunks1.size() + shiftChunks);
	for (size_t i = 0; i < shiftChunks; ++i) {
		chunks[i] = 0;
	}

	for (size_t i = 0; i < chunks1.size(); ++i) {
		chunks[i + shiftChunks] = chunks1[i];
	}

	if (shiftBits == 0) {
		return chunks;
	}

	INT::typeChunk mask = ((1 << shiftBits) - 1) << crossShiftBits;
	for (size_t i = chunks.size() - 1 ; i >= shiftChunks; --i) {
		INT::typeChunk shiftOutChunk = ((chunks[i] & mask) >> crossShiftBits);
		if (shiftOutChunk > 0) {
			if (i < chunks.size() - 1) {
				chunks[i + 1] |= shiftOutChunk;
			}
			else {
				chunks.push_back(shiftOutChunk);
			}
		}
		chunks[i] <<= shiftBits;
	}

	return chunks;
}

std::vector<INT::typeChunk> chunksPlus(const std::vector<INT::typeChunk>& chunks1,
	const std::vector<INT::typeChunk>& chunks2) {
	auto pChunksLeft = &chunks1;
	auto pChunksRight = &chunks2;
	if (pChunksRight->size() > pChunksLeft->size()) {
		std::swap(pChunksLeft, pChunksRight);
	}

	INT::typeLink carry = 0;
	std::vector<INT::typeChunk> chunks = *pChunksLeft;
	for (size_t i = 0; i < chunks.size(); ++i) {
		INT::typeLink partSum = 0;
		if (i < pChunksRight->size()) {
			partSum = INT::typeLink((*pChunksLeft)[i]) + INT::typeLink((*pChunksRight)[i]) + carry;
		}
		else {
			partSum = INT::typeLink((*pChunksLeft)[i]) + carry;
		}
		chunks[i] = (INT::typeChunk)(partSum);
		carry = partSum >> INT::s_numBitsOfChunk;
	}

	if (carry != 0) {
		chunks.push_back(static_cast<INT::typeChunk>(carry));
	}

	return chunks;
}

std::vector<INT::typeChunk> chunksSubtract(const std::vector<INT::typeChunk>& chunks1, const std::vector<INT::typeChunk>& chunks2) {
	std::vector<INT::typeChunk> chunks;
	chunks.resize(chunks1.size());
	INT::typeLink borrow = 0;
	for (size_t i = 0; i < chunks1.size(); ++i) {
		INT::typeLink chunk1 = chunks1[i];
		INT::typeLink chunk2 = 0;
		if (i < chunks2.size()) {
			chunk2 = chunks2[i];
		}
		if (chunk1 >= chunk2 + borrow) {
			chunks[i] = static_cast<INT::typeChunk>(chunk1 - chunk2 - borrow);
			borrow = 0;
		}
		else {
			chunks[i] = static_cast<INT::typeChunk>(INT::s_borrowChunkValue + chunk1
				- chunk2 - borrow);
			borrow = 1;
		}
	}
	return chunks;
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

 void zju04nycs::bitsSubtract(INT& v1, int tailBit1, const INT& v2, int tailBit2, int numberBits){
	int bitOffset = 0;
	int borrow = 0;
	while (bitOffset < numberBits) {
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

