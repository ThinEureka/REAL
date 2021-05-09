#pragma once

#ifndef ELEMENTARY_FUNCTION_H
#define ELEMENTARY_FUNCTION_H

#include "Float.h"

namespace real{
	Float e(const Float& x, const int* precision = nullptr);
	Float sin(const Float& x, const int* precision = nullptr);
	Float cos(const Float& x, const int* precision = nullptr);
	Float sqrt(const Float& x, const int* precision = nullptr);
}

#endif

