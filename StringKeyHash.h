/**
string类型key的hash方法
**/
#ifndef _STRINGKEYHASH_
#define _STRINGKEYHASH_
#include <iostream>
#include <string>
#include "sha_256.h"
class StringKeyHash
{
public:
	static unsigned int hash(string str)
	{
		return i_sha256(str);
	}
};
#endif