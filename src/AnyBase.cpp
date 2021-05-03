
#include "AnyBase.h"

using namespace real;

void real::anybaseDouble(std::vector<int>& anybase, int base){
	int carry = 0;
	for (size_t index = 0; index < anybase.size(); ++index){
		int& curplace = anybase[index];
		curplace <<= 1;
		curplace += carry;
		if (curplace >= base){
			curplace -= base;
			carry = 1;
		}
		else{
			carry = 0;
		}
	}

	if (carry != 0){
		anybase.push_back(1);
	}
}

void real::anybaseAddOne(std::vector<int>& anybase, int base){
	int carry = 1;
	for (size_t  index = 0; index < anybase.size(); ++index){
		int& curplace = anybase[index];
		curplace += carry;
		if (curplace >= base){
			curplace -= base;
			carry = 1;
		}
		else{
			return;
		}
	}

	if (carry != 0){
		anybase.push_back(1);
	}
}
