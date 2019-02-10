#ifndef _NODE_
#define _NODE_
/*
树节点的数据域
*/
#include "algorithm"
#include "iterator"
#include "StringUtil.h"
#include "CompStr.h"
#include"AList.h"
using namespace std;
class Node
{

public:
	int docsize;	//当前docs中索引元素的个数
	string names;//保存文件名的集合
	//string keys;//文档集合中keywords的并集
	//vector<string> keys;
	AList<string, CompStr> *disjunctive_keys;
	AList<string, CompStr> *conjunctive_keys;

	Node(){
		docsize = 0;
		names = "";
		disjunctive_keys = NULL;
		conjunctive_keys = NULL;
	}

	Node(string f_n, int nsize, AList<string, CompStr> * d_ks, AList<string, CompStr> *c_ks)
	{
		names = f_n;
		docsize = nsize;
		disjunctive_keys = d_ks;
		conjunctive_keys = c_ks;
	}
	void setDocSize(int nsize){
		docsize = nsize;
	}
	void setNames(string fileNames){
		names = fileNames;
	}
	/*
	void setKeys(string ks){
	keys = ks;
	}*/
	void setDisjunctiveKeys(AList<string, CompStr> * d_ks){
		disjunctive_keys = d_ks;
	}
	void setConjunctiveKeys(AList<string, CompStr> * c_ks){
		conjunctive_keys = c_ks;
	}

	/*合并节点信息    string版
	void mergeNode(Node *n1,Node *n2){
	docsize = n1->docsize + n2->docsize;
	names = n1->names + "," +n2->names;
	keys = keys_union(n1->keys, n2->keys);
	}*/

	/*合并节点信息    vector版
	void mergeNode(Node *n1,Node *n2){
	docsize = n1->docsize + n2->docsize;
	names = n1->names + "," +n2->names;
	keys = keys_union(n1->keys, n2->keys);
	}*/


	/*合并两个keywords，并且去除重复关键字
	string keys_union(string key1, string key2){
	string result;
	vector<string> k1_v ;
	vector<string> k2_v;
	StringUtil::splitString(key1,k1_v,",");
	StringUtil::splitString(key2,k2_v,",");
	return keys_union(k1_v, k2_v);
	}*/

	/*
	Node(char *n, int nsize, char *k){
	docsize = nsize;
	names = new char[strlen(n) +1];
	strcpy(names,n);

	keys = new char[strlen(k) +1];
	strcpy(keys,k);

	//delete n;
	//delete k;
	}
	*/


	~Node()
	{
		//TODO
		//int docsize;	//当前docs中索引元素的个数
		//string names;//保存文件名的集合
		//string keys;//文档集合中keywords的并集
		/*
		if (names != NULL)
		{
		//delete names;
		//names = NULL;
		}
		if (keys != NULL)
		{
		//delete  keys;
		//keys = NULL;
		}
		*/
	}
private:
	/*
	string keys_union(vector<string> k1_v,vector<string> k2_v){
	vector<string> vec = vectors_set_union(k1_v,k2_v);
	return StringUtil::vectorToString(vec);
	}*/
	/*
	vector<string> keys_union(vector<string> k1_v, vector<string> k2_v){
	return vectors_set_union(k1_v,k2_v);
	}*/

	/*两个vector 求并集
	vector<string> vectors_set_union(vector<string> v1, vector<string> v2){
	vector<string> v;
	sort(v1.begin(), v1.end());
	sort(v2.begin(), v2.end());
	set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(v));
	return v;
	}*/


	/*得到两个string类型list的交集个数*/
	int getConjunctiveCount(AList<string, CompStr> &list1, AList<string, CompStr> &list2)
	{
		string value1, value2;
		if (!list1.isSorted())
		{
			list1.sort();
		}
		if (!list2.isSorted())
		{
			list2.sort();
		}
		list1.setStart();
		list2.setStart();
		int count = 0;
		while (list1.getValue(value1) && list2.getValue(value2))
		{
			//int result = strcmp(value1,value2);
			int result = value1.compare(value2);
			if (result == 0)
			{
				count++;
				list1.next();
				list2.next();
			}
			else if (result > 0)
			{
				list2.next();
			}
			else
			{
				list1.next();
			}
		}
		return count;
	}

	/*conjunctive*/
	void getConjunctive(AList<string, CompStr> &list1, AList<string, CompStr> &list2, AList<string, CompStr> &list)
	{
		string value1, value2;
		if (!list1.isSorted())
		{
			list1.sort();
		}
		if (!list2.isSorted())
		{
			list2.sort();
		}
		list1.setStart();
		list2.setStart();
		while (list1.getValue(value1) && list2.getValue(value2))
		{
			//int result = strcmp(value1, value2);
			int result = value1.compare(value2);
			if (result == 0)
			{
				list.append(value1);
				list1.next();
				list2.next();
			}
			else if (result > 0)
			{
				list2.next();
			}
			else
			{
				list1.next();
			}
		}
	}
	/*disjunctive*/
	void getDisjunctive(AList<string, CompStr> &list1, AList<string, CompStr> &list2, AList<string, CompStr> &list){
		string value1, value2;
		if (!list1.isSorted())
		{
			list1.sort();
		}
		if (!list2.isSorted())
		{
			list2.sort();
		}
		list1.setStart();
		list2.setStart();
		while (list1.getValue(value1) && list2.getValue(value2)){
			int result = value1.compare(value2);
			if (result == 0)
			{
				list.append(value1);
				list1.next();
				list2.next();
			}
			else if (result < 0){
				list.append(value1);
				list1.next();
			}
			else
			{
				list.append(value2);
				list2.next();
			}
		}
		//cout << "list1.rightLength() = " << list1.rightLength() << endl;
		while (list1.rightLength()>0)
		{
			list1.getValue(value1);
			list.append(value1);
			list1.next();
		}
		//cout << "list2.rightLength() = " << list2.rightLength() << endl;
		while (list2.rightLength()>0)
		{
			list2.getValue(value2);
			list.append(value2);
			list2.next();
		}

	}

};

#endif