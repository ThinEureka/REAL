#include "ElementaryFunction.h"
#include <vector>
#include "assert.h"

using namespace real;

static Float& exp_series(const Float& x, Float& result, const int* precision);
static Float& sin_series(const Float& x, Float& result, const int* precision);
static Float& cos_series(const Float& x, Float& result, const int* precision);

Float real::exp(const Float& x, const int* precision){
	Float result;
	exp_series(x, result, precision);
	return result;
}

Float& exp_series(const Float& x, Float& result, const int* precision){
	if (x.isZero()){
		result = Float::one;
		return result;
	}

	std::vector<Float*> xn;
	std::vector<Float*> fac;

	xn.push_back(new Float(1));
	fac.push_back(new Float(1));

	size_t i = 0;

	int realPrecision = precision ? *precision : Float::defaultPrecision();
	if (x.isPositive()){
		//we should limit the range of x,  
		const Float three = 3;
		Float y = Float::one;
		Float k = Float::zero;
		while (k < x){
			y *= three;
			k += Float::one;
		}
		realPrecision -= (y.leadBit() + 1);
	}
	else {
		//we should limit the range of x,  
		Int y = x.toInt();
		y.negate();
		bool isOverFlow = false;
		realPrecision += static_cast<int>(y.toChunk(isOverFlow));
	}

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

Float real::cos(const Float& x, const int* precision){
	Float result;
	cos_series(x, result, precision);
	return result;
}

Float& cos_series(const Float& x, Float& result, const int* precision){
	int realPrecision = precision ? *precision : Float::defaultPrecision();
	if (x.isZero()){
		result = Float::one;
		return result;
	}

	std::vector<Float*> xn;
	std::vector<Float*> fac;

	xn.push_back(new Float(1));
	fac.push_back(new Float(1));

	const Float x2 = x * x;
	size_t i = 0;

	while(true) {
		++i;
		Float& x_i = *(new Float);
		multiply(*xn[i - 1], x2, x_i);
		xn.push_back(&x_i);

		Float& fac_i = *(new Float);
		multiply(*fac[i - 1], (2*i-1) *(2*i), fac_i);
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
	result = Float::one;
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

Float real::sqrt(const Float& x, const int* precision){
	Float result;
	assert(!x.isNegative());
	if (x.isZero()){
		result.setZero();
		return result;
	}

	Float& xn = result;
	int realPrecision = precision ? *precision : Float::defaultPrecision();
	const int P = x.leadBit();
	xn.set(Int::one, P/2 + 1);

	assert(xn >= x);
	Float s;
	Float d;
	while(true){
		//s = xn^2
		multiply(xn, xn, s);
		subtract(s, x, d);

		const int Q = d.leadBit();
		const int T = xn.leadBit();
		if (Q + 1 < realPrecision + P/2 + 1){
			break;
		}
		else{
			int lowerBound = 2*Q - T - 2;
			int truncatePrecision = lowerBound - 1;
			s += x;

			d = xn;
			d <<= 1;
			divide(s, d, xn, &truncatePrecision, false);
		}
	}

	return result;
}


