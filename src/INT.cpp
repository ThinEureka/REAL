#include "INT.h"

using namespace zju04nycs;

int bitsCompare(const std::vector<INT::typeUnit> bits1, const std::vector<INT::typeUnit> bits2);
std::vector<INT::typeUnit>&& bitsPlus(const std::vector<INT::typeUnit> bits1, const std::vector<INT::typeUnit> bits2);
std::vector<INT::typeUnit>&& bitsSubtract(const std::vector<INT::typeUnit> bits1, const std::vector<INT::typeUnit> bits2);

void trimLeadingZero(std::vector<INT::typeUnit>& bits);

INT::INT() {
	_bits.push_back(0);
}

INT::INT(const INT& v) :_bits(v._bits), _sign(v._sign){
}

INT::INT(INT&& v) noexcept:_bits(std::move(v._bits)), _sign(v._sign) {
}

INT& INT::operator = (const INT& v){
	_bits = v._bits;
	_sign = v._sign;
	return *this;
}

INT& INT::operator = (INT&& v) noexcept {
	_bits = std::move(v._bits);
	_sign = v._sign;
	return *this;
}

bool zju04nycs::operator == (const INT& v1, const INT& v2) {
	if (&v1 == &v2) {
		return true;
	}

	if (v1._sign != v2._sign) {
		return true;
	}

	if (v1._bits.size() != v2._bits.size()) {
		return false;
	}

	for (size_t i = 0; i < v1._bits.size(); ++i) {
		if (v1._bits[i] != v2._bits[i]) {

			return false;
		}
	}

	return true;
}

bool zju04nycs::operator != (const INT& v1, const INT& v2) {
	return !(v1 == v2);
}

int zju04nycs::compare(const INT& v1, const INT& v2) {
	if (v1._sign != v2._sign) {
		return v1._sign - v2._sign;
	}

	auto bitsResult = bitsCompare(v1._bits, v2._bits);
	return v1._sign > 0 ? bitsResult : -bitsResult;
}

INT&& zju04nycs::operator + (const INT& v1, const INT& v2) {
	if (v1._sign == v2._sign) {
		const INT* pLeft = &v1;
		const INT* pRight = &v2;
		return std::move(INT(v1._sign, bitsPlus(v1._bits, v2._bits)));
	} else {
		int comResult = bitsCompare(v1._bits, v2._bits);
		if (comResult == 0) {
			INT zero;
			return std::move(zero);
		} else {
			auto bits = comResult > 0 ? bitsSubtract(v1._bits, v2._bits) :
				bitsSubtract(v2._bits, v1._bits);
			INT sum = INT(comResult > 0 ? v1._sign : -v1._sign, std::move(bits));
			return std::move(sum);
		}
	}
}

INT&& zju04nycs::operator - (const INT& v1, const INT& v2) {
	if (v1._sign == -v2._sign) {
		const INT* pLeft = &v1;
		const INT* pRight = &v2;
		return std::move(INT(v1._sign, bitsPlus(v1._bits, v2._bits)));
	} else {
		int comResult = bitsCompare(v1._bits, v2._bits);
		if (comResult == 0) {
			INT zero;
			return std::move(zero);
		} else {
			auto bits = comResult > 0 ? bitsSubtract(v1._bits, v2._bits) :
				bitsSubtract(v2._bits, v1._bits);
			INT sum = INT(comResult > 0 ? v1._sign : -v1._sign, std::move(bits));
			return std::move(sum);
		}
	}
}

int bitsCompare(const std::vector<INT::typeUnit> bits1, const std::vector<INT::typeUnit> bits2) {
	int size1 = bits1.size();
	int size2 = bits2.size();
	if (size1 != size2) {
		return  size1 - size2;
	}

	for (size_t i = 0; i < size1; ++i) {
		auto b1 = bits1[i];
		auto b2 = bits2[i];
		if (b1 != b2) {
			return b1 - b2;
		}
	}
	return 0;
}

std::vector<INT::typeUnit>&& bitsPlus(const std::vector<INT::typeUnit> bits1,
	const std::vector<INT::typeUnit> bits2) {
	auto pBitsLeft = &bits1;
	auto pBitsRight = &bits2;
	if (pBitsRight->size() > pBitsLeft->size()) {
		std::swap(pBitsLeft, pBitsRight);
	}

	INT::typeLink carry = 0;
	std::vector<INT::typeUnit> bits;
	bits.resize(bits1.size());
	for (size_t i = 0; i < pBitsRight->size(); ++i) {
		INT::typeLink partSum = INT::typeLink((*pBitsLeft)[i]) + INT::typeLink((*pBitsRight)[i]) + carry;
		bits[i] = (INT::typeUnit)(partSum);
		carry = partSum >> INT::s_numBitsOfUnit;
	}

	if (carry != 0) {
		bits.push_back(carry);
	}
	return std::move(bits);
}

std::vector<INT::typeUnit>&& bitsSubtract(const std::vector<INT::typeUnit> bits1, const std::vector<INT::typeUnit> bits2) {
	std::vector<INT::typeUnit> bits;
	bits.resize(bits1.size());
	INT::typeLink borrow = 0;
	for (size_t i = 0; i < bits.size(); ++i) {
		INT::typeLink partLeft = bits1[i];
		INT::typeLink partRight = bits2[i];
		if (partLeft >= partRight + borrow) {
			bits[i] = partLeft - partRight - borrow;
			borrow = 0;
		}
		else {
			bits[i] = INT::s_borrowValue + partLeft
				- partRight - borrow;
			borrow = 1;
		}
	}

	trimLeadingZero(bits);
	return std::move(bits);
}

void trimLeadingZero(std::vector<INT::typeUnit>& bits) {
	while (bits.size() > 0) {
		if (bits[bits.size() - 1] != 0) {
			return;
		}
		bits.pop_back();
	}
}