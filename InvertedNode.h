#ifndef _INV_NODE_
#define _INV_NODE_
/*
处理查询集合时，所使用的倒排索引Node
*/
#include "algorithm"
#include "iterator"
#include <vector>
#include "StringUtil.h"
using namespace std;
class InvertedNode
{

public:
	int docsize;	//对应文件的数量
	vector<string> fileNames;//关键字对应的文件vector
	string keyword;//关键字



	InvertedNode(){
		//names = keys = NULL;
	}

	InvertedNode(string k, int nsize, const vector<string>& files)
	{
		keyword = k;
		docsize = nsize;
		fileNames = files;
	}
	void setDocSize(int nsize){
		docsize = nsize;
	}
	void setFileNames(const vector<string>& files){
		fileNames = files;
	}
	void setKeyword(string k){
		keyword = k;
	}

	/*合并节点信息
	void mergeNode(Node *n1, Node *n2){
	docsize = n1->docsize + n2->docsize;
	names = n1->names + "," + n2->names;
	keys = keys_union(n1->keys, n2->keys);
	}*/

	/*disjunctive Node*/
	void disjunctiveINode(InvertedNode *n1, InvertedNode *n2){
		keyword = n1->keyword + " " + n2->keyword;
		fileNames = vectors_set_union(n1->fileNames, n2->fileNames);
		docsize = fileNames.size();
	}

	/*conjunctive Node*/
	void conjunctiveInode(InvertedNode *n1, InvertedNode *n2){
		keyword = n1->keyword + " " + n2->keyword;
		fileNames = vectors_intersection(n1->fileNames, n2->fileNames);
		docsize = fileNames.size();
	}

	/*void clear(){
		keyword = "";
		fileNames.clear();
		docsize = 0;
	}*/


	/*合并两个keywords，并且去除重复关键字
	*/
	string keys_union(string key1, string key2){
		string result;
		vector<string> k1_v;
		vector<string> k2_v;
		StringUtil::splitString(key1, k1_v, ",");
		StringUtil::splitString(key2, k2_v, ",");
		return keys_union(k1_v, k2_v);

	}



	~InvertedNode()
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
	/*两个vector 求并集,返回strng*/
	string keys_union(vector<string> k1_v, vector<string> k2_v){
		vector<string> vec = vectors_set_union(k1_v, k2_v);
		return StringUtil::vectorToString(vec);
	}

	/*两个vector 求并集,返回vector*/
	vector<string> vectors_set_union(vector<string> v1, vector<string> v2){
		vector<string> v;
		sort(v1.begin(), v1.end());
		sort(v2.begin(), v2.end());
		set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(v));
		return v;
	}
	/*两个vector求交集,返回vector*/
	vector<string> vectors_intersection(vector<string> v1, vector<string> v2){
		vector<string> v;
		sort(v1.begin(), v1.end());
		sort(v2.begin(), v2.end());
		set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(v));
		return v;
	}


};

#endif
