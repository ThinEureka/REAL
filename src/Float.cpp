#include "Float.h"

using namespace real;

const Float Float::zero = 0;
const Float Float::one = 1;

Float& Float::setBit(int bitPos, bool v) {
	if (_int.isZero()) {
		if (!v) {
			return *this;
		}
		_int.setOne();
		_baseBitPos = bitPos;
		return *this;
	}

	if (bitPos < _baseBitPos) {
		if (!v) {
			return *this;
		}

		int bitDiff = _baseBitPos - bitPos;
		_baseBitPos = bitPos;
		_int <<= bitDiff;
		_int.setBit(0, v);
		return *this;
	}

	//here we're sure that bitPos is greater or equal than _baseBitPos
	_int.setBit(static_cast<size_t>(bitPos - _baseBitPos), v);
	normalize();
	return *this;
}

int real::absCompare(const Float& v1, const Float& v2) {
	if (v1.isZero() != v2.isZero()) {
		return v1.isZero() ? -1 : 1;
	}

	if (v1.isZero() && v2.isZero()) {
		return 0;
	}

	int leadBit1 = v1.leadBit();
	int leadBit2 = v2.leadBit();
	if (leadBit1 != leadBit2) {
		return leadBit1 < leadBit2 ? -1 : 1;
	}

	int tailBit1 = v1.tailBit();
	int tailBit2 = v2.tailBit();
	int maxTailBit = std::min(tailBit1, tailBit2);
	for (auto bitPos = leadBit1-1; bitPos >= maxTailBit; --bitPos) {
		auto bit1 = v1.bit(bitPos);
		auto bit2 = v2.bit(bitPos);
		if (bit1 != bit2) {
			return bit1 < bit2 ? -1 : 1;
		}
	}

	if (tailBit1 != tailBit2) {
		//the ont with smaller tail bit has more digits
		return tailBit1 > tailBit2 ? -1 : 1;
	}

	return 0;
}

Float& Float::setFloor() {
	if (_int.sign() >= 0) {
		return this->setInt();
	}
	else{
		bool isInt = true;
		this->setInt(&isInt);
		if (!isInt) {
			*this -= one;
		}
		return *this;
	}
}

Float& Float::setCeil() {
	if (_int.sign() > 0) {
		bool isInt = true;
		this->setInt(&isInt);
		if (!isInt) {
			*this += one;
		}
		return *this;
	}
	else{
		return this->setInt();
	}
}

Float& Float::setInt(bool* isInt) {
	if (isZero()) {
		if (isInt) {
			*isInt = true;
		}
		return *this;
	}

	int tailBit = this->tailBit();
	if (tailBit >= 0) {
		if (isInt) {
			*isInt = true;
		}
		return *this;
	}

	int leadBit = this->leadBit();
	if (leadBit < 0) {
		if (isInt) {
			*isInt = false;
		}
		return this->setZero();
	}

	*isInt = false;
	_int >>= -tailBit;
	_baseBitPos -= tailBit;

	return this->normalize();
}

Float& real::plus(const Float& v1, const Float& v2, Float& sum) {
	if (v1.isZero()) {
		return sum = v2;
	}

	if (v2.isZero()) {
		return sum = v1;
	}

	const Float* pV1 = &v1;
	const Float* pV2 = &v2;
	int tailBit1 = v1.tailBit();
	int tailBit2 = v2.tailBit();

	if (tailBit1 < tailBit2) {
		std::swap(tailBit1, tailBit2);
		std::swap(pV1, pV2);
	}

	//we ensure that pV1 is the one with larger tailBit
	sum._baseBitPos = tailBit2;
	sum._c1 = pV1->_int;
	sum._c1 <<= (tailBit1 - tailBit2);
	plus(sum._c1, pV2->_int, sum._int);
	return sum.normalize();
}

Float& real::subtract(const Float& v1, const Float& v2, Float& sub) {
	if (v1.isZero()) {
		return (sub = v2).negate();
	}

	if (v2.isZero()) {
		return sub = v1;
	}

	const Float* pV1 = &v1;
	const Float* pV2 = &v2;
	int tailBit1 = v1.tailBit();
	int tailBit2 = v2.tailBit();

	if (tailBit1 < tailBit2) {
		std::swap(tailBit1, tailBit2);
		std::swap(pV1, pV2);
	}

	//we ensure that pV1 is the one with larger tailBit
	sub._baseBitPos = tailBit2;
	sub._c1 = pV1->_int;
	sub._c1 <<= (tailBit1 - tailBit2);
	subtract(sub._c1, pV2->_int, sub._int);
	return sub.normalize();
}

Float& real::multiply(const Float& v1, const Float& v2, Float& product) {
	multiply(v1._int, v2._int, product._int);
	product._baseBitPos = v1._baseBitPos + v2._baseBitPos;
	return product.normalize();
}
