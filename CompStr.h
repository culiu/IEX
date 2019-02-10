/**
string ×Ö·û´®±È½ÏÀà
**/
#ifndef _COMPSTRING_
#define _COMPSTRING_
#include <iostream>
#include <string>

class CompStr
{
public:

	static int comp(std::string value1, std::string value2)
	{
		//int result = value1 - value2; // value1.compare(value2);
		if (value1 == value2)
		{
			return 0;
		}
		else if (value1 > value2)
		{
			return 1;
		}
		else {
			return -1;
		}
		//return result;
	}
};

#endif