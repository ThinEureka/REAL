#include "Float.h"
#include "assert.h"

using namespace real;

const Float Float::zero = 0;
const Float Float::one = 1;
int Float::s_defaultPrecision = -128;

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

Float& Float::setFloor(int bitPos, bool* isModified) {
	if (_int.sign() >= 0) {
		return this->setInt(isModified);
	}
	else {
		bool isIntModified = false;
		this->truncate(bitPos, &isIntModified);
		if (isIntModified) {
			Float& one = this->f2().set(Int::one, bitPos);
			*this -= one;
		}

		if (isModified) {
			*isModified = isIntModified;
		}
		return *this;
	}
}

Float& Float::setCeil(int bitPos, bool* isModified) {
	if (_int.sign() > 0) {
		bool isIntModified = false;
		this->truncate(bitPos, &isIntModified);
		if (isIntModified) {
			Float& one = this->f2().set(Int::one, bitPos);
			*this += one;
		}

		if (isModified) {
			*isModified = isIntModified;
		}
		return *this;
	}
	else{
		return this->setInt(isModified);
	}
}

Float& Float::truncate(int bitPos, bool* isModified) {
	if (isZero()) {
		if (isModified) {
			*isModified = false;
		}
		return *this;
	}

	int tailBit = this->tailBit();
	if (tailBit >= bitPos) {
		if (isModified) {
			*isModified = false;
		}
		return *this;
	}

	int leadBit = this->leadBit();
	if (leadBit < bitPos) {
		if (isModified) {
			*isModified = true;
		}
		return this->setZero();
	}

	int bitDiff = bitPos - tailBit;
	_int >>= bitDiff;
	_baseBitPos += bitDiff;
	if (isModified) {
		*isModified = true;
	}
	return this->normalize();
}

Float& Float::extend(int bitPos, bool* isModified) {
	if (isZero()) {
		if (isModified) {
			*isModified = false;
		}
		return *this;
	}
	else if (isPositive()) {
		return setCeil(bitPos, isModified);
	}
	else {
		return setFloor(bitPos, isModified);
	}
}

Float& real::add(const Float& v1, const Float& v2, Float& sum) {
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
	sum.f1()._int = pV1->_int;
	sum.f1()._int <<= (tailBit1 - tailBit2);
	add(sum.f1()._int, pV2->_int, sum._int);
	sum.f1().setZero();
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
	sub.f1()._int = pV1->_int;
	sub.f1()._int <<= (tailBit1 - tailBit2);
	subtract(sub.f1()._int, pV2->_int, sub._int);
	return sub.normalize();
}

Float& real::multiply(const Float& v1, const Float& v2, Float& product) {
	multiply(v1._int, v2._int, product._int);
	product._baseBitPos = v1._baseBitPos + v2._baseBitPos;
	return product.normalize();
}

Float& real::divide(const Float& v1, const Float& v2, Float& q, const int* pPrecison, bool isRelativePrecision){
	assert(!v2.isZero());
	if (v1.isZero()) {
		return q.setZero();
	}

	int inversePrecison = pPrecison ? *pPrecison : Float::defaultPrecision();
	if (!isRelativePrecision) {
		inversePrecison -= (v1.leadBit() + 1);
	}

	v2.calculateInverse(q, &inversePrecison, isRelativePrecision);
	multiply(v1, q, q.f1());
	return q.swap(q.f1());
}

Float& Float::calculateInverse(Float& q, const int* pPrecision, bool isRelativePrecision) const {
	assert(!isZero());

	int leadBit = this->leadBit();
	int tailBit = this->tailBit();

	if (leadBit == tailBit) {
		q.set(Int::one, -leadBit);
		if (isNegative()) {
			q.negate();
		}
		return q;
	}

	int precision = pPrecision ? *pPrecision : defaultPrecision();
	if (isRelativePrecision) {
		precision += (-leadBit - 1);
	}

	q.set(Int::one, -leadBit - 1);
	if (isNegative()) {
		q.negate();
	}

	const int N = leadBit;
	while (true) {
		multiply(q, *this, q.f1());
		subtract(Float::one, q.f1(), q.f2());
		assert(q.f2().isPositive());

		if (q.f2().isZero()) {
			q.truncate(precision);
			return q;
		}
		else {
			const int P = q.f2().leadBit();
			const int Q1 = 2*P - N - 1;
			const int Q2 = 2*P + 2 - N;

			if (P + 1 - N < precision - 1) {
				q.truncate(precision - 1);
				return q;
			}
			else {
				add(q.f2(), Float::one, q.f1());
				multiply(q.f1(), q, q.f2());
				q.swap(q.f2());
				q.extend(Q1);
			}
		}
	}
}

std::string Float::toString(const int* pDigit, int base, std::string* pStr, std::vector<int>* pAnybase, Float* cacheP) const{
	std::string dummyStr;
	std::string& str = pStr ? *pStr : dummyStr;
	str.clear();
	if (isZero()) {
		str += '0';
		return str;
	}

	Float& p = cacheP ? *cacheP : *(new Float);
	if (_baseBitPos >= 0){
		this->toInt(p.f1(), p._int);
		p._int.toString(base, &str, pAnybase);
		p.setZero();
		if (!cacheP) delete &p;
		return str;
	}

	//here we're sure the integer case has been precoessed
	std::vector<int>& anybase = pAnybase ? *pAnybase : *(new std::vector<int>);
	anybase.clear();

	if (isNegative()){
		str += '-';
	}

	this->toInt(p.f1(), p._int);
	p._int.toAnybase(anybase, base);

	for (auto it = anybase.rbegin(); it != anybase.rend(); ++it){
		str += Int::chunkToDigit(static_cast<Int::typeChunk>(*it), base);
	}

	str += '.';
	subtract(*this, p.f1(), p.f2());
	p.swap(p.f2());

	int digit = pDigit ? *pDigit : -s_defaultPrecision;
	int shiftTimes = 0;

	// *= only use f1()
	p.f2() = base;
	const Float& fBase = p.f2(); 

	while (shiftTimes < digit && p.tailBit() < 0){
		p *= fBase;
		shiftTimes++;
	}

	p.toInt(p.f1(), p.f2()._int);
	p.f2()._int.toAnybase(anybase, base);

	int paddingZero = shiftTimes - static_cast<int>(anybase.size()); 

	for (int i = 0; i < paddingZero; ++i){
		str += '0';
	}

	for (auto it = anybase.rbegin(); it != anybase.rend(); ++it){
		str += Int::chunkToDigit(static_cast<Int::typeChunk>(*it), base);
	}

	p.setZero();
	p.f2().setZero();

	if (!cacheP) delete &p;
	if (!pAnybase) delete &anybase;

	return str;
}

Float& Float::set(const std::string& str, int base, const int* pPrecision, bool isRelativePrecision) {
	assert(base >= 2 && base <= 35);
	enum state{
		begin,
		mantissaBegin,
		mantissaDigit,
		fracBegin,
		fracDigit,
		exponentBegin,
		exponentDigit,
		end,
	};
	const char* pos = str.c_str();
	const char* endPos = str.c_str() + str.size();
	if (pos == endPos) {
		return setZero();
	}

	int digitValue = 0;
	state s = begin;

	int sign = 1;
	Int& mantissa = _int;
	mantissa.setZero();
	_baseBitPos = 0;
	int exponent = 0;
	int pointPos = 0;
	int exponentSign = 1;

	while (s != end && pos != endPos) {
		bool fallThrough = false;
		char c = *pos;
		switch (s) {
		case begin:
		{
			if (c == '-') {
				sign = -1;
				s = mantissaDigit;
				pos++;
				break;
			}
			else if (c == '.') {
				s = fracBegin;
				pos++;
				break;
			}
			else if (Int::isDigit(c, base, digitValue)) {
				//do not increase pos
				//fall down to intBegin
				s = mantissaDigit;
				fallThrough = true;
			}
			else {
				s = end;
				break;
			}
		}
		case mantissaDigit:
		{
			if (fallThrough || Int::isDigit(c, base, digitValue)) {
				mantissa *= Int::s_smallInts[base];
				mantissa += Int::s_smallInts[digitValue];
				pos++;
				break;
			}
			else if (c == '.') {
				pos++;
				s = fracDigit;
				break;
			}
			else if (Int::isExponentSeprator(c, base)) {
				pos++;
				s = exponentBegin;
				break;
			}
			else {
				s = end;
				break;
			}
		}
		case fracBegin: {
			if (Int::isDigit(c, base, digitValue)) {
				fallThrough = true;
				s = fracDigit;
			}
			else if (Int::isExponentSeprator(c, base)) {
				pos++;
				s = exponentBegin;
				break;
			}
			else {
				s = end;
				break;
			}
		}
		case fracDigit: {
			if (fallThrough || Int::isDigit(c, base, digitValue)) {
				mantissa *= Int::s_smallInts[base];
				mantissa += Int::s_smallInts[digitValue];
				pointPos++;
				pos++;
				break;
			}
			else if (Int::isExponentSeprator(c, base)) {
				pos++;
				s = exponentBegin;
				break;
			}
			else {
				s = end;
				break;
			}
		}
		case exponentBegin:
		{
			if (c == '-') {
				exponentSign = -1;
				s = exponentDigit;
				pos++;
				break;
			}
			else if (Int::isDigit(c, base, digitValue)) {
				//do not increase pos
				//fall down to intBegin
				s = exponentDigit;
				fallThrough = true;
			}
			else {
				s = end;
				break;
			}
		}
		case exponentDigit:
		{
			if (fallThrough || Int::isDigit(c, base, digitValue)) {
				exponent = exponent * base + digitValue;
				pos++;
				break;
			}
			else {
				s = end;
				break;
			}
		}
		}
	}

	if (exponentSign < 0) {
		exponent = -exponent;
	}

	exponent -= pointPos;

	//exponent >= 0
	if (exponent >= 0) {
		while (exponent-- > 0) {
			mantissa *= Int::s_smallInts[base];
		}
		_baseBitPos = 0;
		this->normalize();
		if (sign < 0) {
			this->negate();
		}

		//in this case precision is relative to bit 0,
		//so precision is the same whether isRelativePrecision is true or not
		//it means tht unless precison is greather than zero, the result is 
		//precise
		int precision = pPrecision ? *pPrecision : Float::defaultPrecision();

		return this->truncate(precision);
	}

	//exponent < 0
	Int& exp = f2()._int;
	exp.setOne();
	while (exponent++ < 0) {
		exp *= Int::s_smallInts[base];
	}
	f2()._baseBitPos = 0;
	f2().normalize();

	int precision = pPrecision ? *pPrecision : Float::defaultPrecision();

	if (isRelativePrecision) {
		precision -= (f2().leadBit() + 1);
	}

	divide(*this, f2(), f1(), &precision, false);

	this->swap(f1());
	if (sign < 0) {
		this->negate();
	}
	return *this;
}

