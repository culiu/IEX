#ifndef _HASHNODE_
#define _HASHNODE_
#include <string>
#include <iostream>
using namespace std;
template <typename K, typename V>
class HashNode{
public:
	K _key;
	V _value;
	HashNode(const K &key, const V &value) :
		_key(key), _value(value)
	{
	}

	K getKey() const
	{
		return _key;
	}

	V getValue() const
	{
		return _value;
	}

	void setValue(V value)
	{
		_value = value;
	}


	///*判断两个hashNode是否相等：仅判断key是否相等*/
	////bool operator==(const HashNode& hn){
	////	return this->key == hn.key;
	////}


	//friend ostream &operator<<(ostream &output, const HashNode& hn){
	//	output << "key:" << hn.key << " enc_left_child:" << hn.enc_left_child << " enc_right_child:" << hn.enc_right_child;
	//	return output;
	//}
};
#endif