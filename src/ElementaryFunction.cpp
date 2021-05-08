#include "ElementaryFunction.h"
#include <vector>

using namespace real;

static Float& e_series(const Float& x, Float& result, const int* precision);
static Float& sin_series(const Float& x, Float& result, const int* precision);

Float real::e(const Float& x, const int* precision){
	Float result;
	e_series(x, result, precision);
	return result;
}

Float& e_series(const Float& x, Float& result, const int* precision){
	int realPrecision = precision ? *precision : Float::defaultPrecision();
	if (x.isZero()){
		result = Float::one;
		return result;
	}

	std::vector<Float*> xn;
	std::vector<Float*> fac;

	xn.push_back(new Float(1));
	fac.push_back(new Float(1));

	size_t i = 0;

	while(true) {
		++i;
		Float& x_i = *(new Float);
		multiply(*xn[i - 1], x, x_i);
		xn.push_back(&x_i);

		Float& fac_i = *(new Float);
		multiply(*fac[i - 1], i, fac_i);
		fac.push_back(&fac_i);

		if (x_i.leadBit() + 1 - fac_i.leadBit() < realPrecision - 1){
			break;
		}
	}

	const size_t n = i - 1;
	size_t leadBit = sizeof(n) * 8 - 1;
	while(leadBit > 0){
		if (n >> leadBit != 0){
			break;
		}
		--leadBit;
	}
	const int unitPrecision = realPrecision -1  - leadBit - 1;

	Float unit;
	add(Float::one, x, result);
	for (int i = 2; i <= n; ++i){
		divide(*xn[i], *fac[i], unit, &unitPrecision, false);
		result += unit;
	}

	for (auto pX : xn){
		delete pX;
	}
	
	for (auto pX : fac){
		delete pX;
	}

	return result;
}

Float real::sin(const Float& x, const int* precision){
	Float result;
	sin_series(x, result, precision);
	return result;
}

Float& sin_series(const Float& x, Float& result, const int* precision){
	int realPrecision = precision ? *precision : Float::defaultPrecision();
	if (x.isZero()){
		result = x;
		return result;
	}

	std::vector<Float*> xn;
	std::vector<Float*> fac;

	xn.push_back(new Float(x));
	fac.push_back(new Float(1));

	const Float x2 = x * x;
	size_t i = 0;

	while(true) {
		++i;
		Float& x_i = *(new Float);
		multiply(*xn[i - 1], x2, x_i);
		xn.push_back(&x_i);

		Float& fac_i = *(new Float);
		multiply(*fac[i - 1], (2*i+1) *(2*i), fac_i);
		fac.push_back(&fac_i);

		if (x_i.leadBit() + 1 - fac_i.leadBit() < realPrecision - 1){
			break;
		}
	}

	const size_t n = i - 1;
	size_t leadBit = sizeof(n) * 8 - 1;
	while(leadBit > 0){
		if (n >> leadBit != 0){
			break;
		}
		--leadBit;
	}
	const int unitPrecision = realPrecision -1  - leadBit - 1;

	Float unit;
	result = x;
	for (int i = 1; i <= n; ++i){
		divide(*xn[i], *fac[i], unit, &unitPrecision, false);
		if (i % 2){
			unit.negate();
		}
		result += unit;
	}

	for (auto pX : xn){
		delete pX;
	}
	
	for (auto pX : fac){
		delete pX;
	}

	return result;
}

