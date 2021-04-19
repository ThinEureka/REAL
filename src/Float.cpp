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
Float& Float::setInt() {
	if (isZero()) {
		return *this;
	}

	int tailBit = this->tailBit();
	if (tailBit >= 0) {
		return *this;
	}

	int leadBit = this->leadBit();
	if (leadBit < 0) {
		return this->setZero();
	}

	_int >>= -tailBit;
	_baseBitPos -= tailBit;

	return this->normalize();
}
