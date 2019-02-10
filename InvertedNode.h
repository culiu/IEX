#ifndef _INV_NODE_
#define _INV_NODE_
/*
�����ѯ����ʱ����ʹ�õĵ�������Node
*/
#include "algorithm"
#include "iterator"
#include <vector>
#include "StringUtil.h"
using namespace std;
class InvertedNode
{

public:
	int docsize;	//��Ӧ�ļ�������
	vector<string> fileNames;//�ؼ��ֶ�Ӧ���ļ�vector
	string keyword;//�ؼ���



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

	/*�ϲ��ڵ���Ϣ
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


	/*�ϲ�����keywords������ȥ���ظ��ؼ���
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
		//int docsize;	//��ǰdocs������Ԫ�صĸ���
		//string names;//�����ļ����ļ���
		//string keys;//�ĵ�������keywords�Ĳ���
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
	/*����vector �󲢼�,����strng*/
	string keys_union(vector<string> k1_v, vector<string> k2_v){
		vector<string> vec = vectors_set_union(k1_v, k2_v);
		return StringUtil::vectorToString(vec);
	}

	/*����vector �󲢼�,����vector*/
	vector<string> vectors_set_union(vector<string> v1, vector<string> v2){
		vector<string> v;
		sort(v1.begin(), v1.end());
		sort(v2.begin(), v2.end());
		set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(v));
		return v;
	}
	/*����vector�󽻼�,����vector*/
	vector<string> vectors_intersection(vector<string> v1, vector<string> v2){
		vector<string> v;
		sort(v1.begin(), v1.end());
		sort(v2.begin(), v2.end());
		set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(v));
		return v;
	}


};

#endif
