
#pragma once

#ifndef INT_H
#define INT_H

#include <vector>

namespace zju04nycs {
	class INT {
	public:
		typedef unsigned __int64 typeLink;
		typedef unsigned __int32 typeUnit;

		static const typeLink s_lowerBitMask	= 0x00000000FFFFFFFF;
		static const typeLink s_higherBitMask	= 0xFFFFFFFF00000000;
		static const typeLink s_borrowValue		= 0x0000000100000000;
		static const typeLink s_numBitsOfUnit = 32;

	public:
		INT();
		INT(const INT& v);
		INT(INT&& v) noexcept;

	private:
		INT(int sign, std::vector<typeUnit>&& bits): _sign(sign), _bits(std::move(bits))
		{};

		INT& operator = (const INT& v);
		INT& operator = (INT&& v)noexcept;

	public:
		friend bool operator == (const INT& v1, const INT& v2);
		friend bool operator != (const INT& v1, const INT& v2);
		friend INT&& operator +(const INT& v1, const INT& v2);
		friend INT&& operator -(const INT& v1, const INT& v2);
		friend INT&& operator *(const INT& v1, const INT& v2);
		friend INT&& operator /(const INT& v1, const INT& v2);
		friend INT&& operator %(const INT& v1, const INT& v2);

		friend INT&& divide(const INT& v1, const INT& v2, INT* pR);
		friend int compare(const INT& v1, const INT& v2);

		INT& negate() { _sign = -_sign; };


	private:
			int _sign{ 1 };
			std::vector<typeUnit> _bits;
	};
	
}

#endif