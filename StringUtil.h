#ifndef _STRINGUTIL_
#define _STRINGUTIL_
#include <iostream>
#include <sstream>
#include <assert.h>
#include <string.h>
#include <vector>
#include "CompStr.h"

using namespace std;

class StringUtil
{
public:

	//将string转为char*
	static char* strToChar(string s)
	{
		//const char* c= s.c_str();
		//int len = strlen(c);
		int length = s.length() + 1;
		char *data = new char[length];
		try{
			for (int i = 0; i<length - 1; i++)
				data[i] = s.c_str()[i];
		}
		catch (exception){
			cout << "复制出现异常" << endl;
		}

		data[length - 1] = '\0';
		return data;
	}

	//int转为string类型
	static string intToString(const int n)
	{
		std::stringstream newstr;
		newstr << n;
		return newstr.str();
	}

	//string 转int
	static int stringToInt(const string s)
	{
		std::stringstream ss;
		int n = -1;
		ss << s;
		ss >> n;
		return n;
	}

	static int charToInt(const char* c)
	{
		return atoi(c);
	}

	static char* intToChar(const int n)
	{
		return strToChar(intToString(n));
	}

	static char* myStrcpy(char* strDest, const char* strSrc)
	{
		assert(strSrc != NULL);
		assert(strDest != NULL);
		int i;
		char * address = strDest;
		for (i = 0; strSrc[i] != '\0'; i++)
		{
			strDest[i] = strSrc[i];
		}
		strDest[i] = '\0';

		return address;
	}


	/*4-25 split string by delimiter then save elem in to vector */
	static void splitString(const std::string& s, std::vector<std::string>& v, const std::string& c)
	{
		std::string::size_type pos1, pos2;
		pos2 = s.find(c);

		///*如果没有分割符*/
		//if (pos2 == std::string::npos){
		//	v.push_back(s);
		//	return;
		//}

		pos1 = 0;
		while (std::string::npos != pos2)
		{
			v.push_back(s.substr(pos1, pos2 - pos1));

			pos1 = pos2 + c.size();
			pos2 = s.find(c, pos1);
		}
		if (pos1 != s.length())
			v.push_back(s.substr(pos1));
	}

	/*4-25 转变VectorToString 默认用,分割*/
	static string vectorToString(vector<string> vec){
		std::ostringstream oss;
		if (!vec.empty()){
			std::copy(vec.begin(), vec.end() - 1, std::ostream_iterator<string>(oss, ","));
			oss << vec.back();
		}
		return oss.str();
	}
	/*转变vector to AList*/
	static AList<string, CompStr>* vectorToAList(vector<string> vec){
		AList <string, CompStr> *result = new AList<string, CompStr>(vec.size());
		for (int i = 0; i < vec.size(); i++)
			result->append(vec[i]);
		return result;
	}

	/*6-24 统计字符串中，某个字符的个数*/
	static int spCount(string str, char c){
		int result = 0;
		for (unsigned int i = 0; i < str.length(); i++){
			if (str[i] == c){
				result++;
			}
		}
		return result;
	}
	/*6-14 split string by delimiter then save elem in to Alist */
	static void splitStringInAList(const std::string& s, AList<string, CompStr> &list, const std::string& c)
	{
		std::string::size_type pos1, pos2;
		pos2 = s.find(c);
		pos1 = 0;
		while (std::string::npos != pos2)
		{

			if (!list.append(s.substr(pos1, pos2 - pos1))){
				cout << "over maxLength = " << list.getListMaxSize() << endl;
				abort();
			}

			pos1 = pos2 + c.size();
			pos2 = s.find(c, pos1);
		}
		if (pos1 != s.length())
		if (!list.append(s.substr(pos1)))
			cout << "over maxLength = " << list.getListMaxSize() << endl;
	}

	/*9-20 split string by delimiter then save elem in to AList */
	static void splitStringIntoAlist(const std::string& s, AList<string, CompStr> &list, const std::string& c)
	{
		std::string::size_type pos1, pos2;
		pos2 = s.find(c);
		pos1 = 0;
		while (std::string::npos != pos2)
		{
			//v.push_back(s.substr(pos1, pos2 - pos1));
			if (!list.append(s.substr(pos1, pos2 - pos1)))
				cout << "error happend list full , listMaxSize = " << list.getListMaxSize() << endl;
			pos1 = pos2 + c.size();
			pos2 = s.find(c, pos1);
		}
		if (pos1 != s.length()){
			//v.push_back(s.substr(pos1));
			if (!list.append(s.substr(pos1)))
				cout << "error happend list full , listMaxSize = " << list.getListMaxSize() << endl;
		}
	}
};

#endif