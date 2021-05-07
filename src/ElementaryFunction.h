#pragma once

#ifndef ELEMENTARY_FUNCTION_H
#define ELEMENTARY_FUNCTION_H

#include "Float.h"

namespace real{
	Float& e_series(const Float& x, Float& result, const int* precision = nullptr);

	//cached version, for effciency
	inline Float& e(const Float& x, Float& result, const int* precision = nullptr){
		//There's a large room for optimization, we now take the most simple solution,
		//and leave the efficient implementation to the future.
		return e_series(x, result, precision);
	}
	
	//simple interface 
	inline Float e(const Float& x, const int* precision = nullptr){
		Float result;
		e(x, result, precision);
		return result;
	}

}

#endif

