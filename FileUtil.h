#ifndef _FILEUTIL_
#define _FILEUTIL_
#include <fstream>
#include <iostream>
#include <string>
//#include <io.h>
#include "CompStr.h"
#include "Alist.h"
#include "CompINode.h"
#include "StringUtil.h"
#include "CompInt.h"
#include "CompNode.h"
using namespace std;

class FileUtil
{
public:
	/*由于Linux下不可用，改为从文件中读取*/
	static void getFiles(const string path, AList<string, CompStr>& files){
		fstream fData;
		bool notFull = true;
		fData.open(path.c_str(), ios::in);
		if (!fData){
			cout << "open '" << path << "' Data file error!" << endl;
			abort();
		}
		string line = "";
		while (!fData.eof() && notFull && getline(fData, line)){
			if (line != "")
			{
				bool flag = files.append(line);
				if (!flag)
				{
					cout << "Data Set is full!" << endl;
					cout << "the current filename is '" << path << "',the list max size is " << files.getListMaxSize() << endl;
					abort();
					notFull = false;
				};
			}
		}
		fData.close();
	}

	/*将文件中的数据，读入到DataSet中*/
	static void readDatas(AList<string, CompStr>& files, AList<Node, CompNode> &DataSet, string keywordDir){
		int length = files.getListSize();
		string fileNames;
		for (int i = 0; i < length; i++){
			readData((keywordDir + files.getPosValue(i)), DataSet);
		}
	}

	/*读取文件，读取结果存储在Node类型的AList中   Alist类型的keyword*/
	static void readData(const string fileName, AList<Node, CompNode> &DataSet)
	{
		fstream fData;
		bool notFull = true;
		fData.open(fileName.c_str(), ios::in);
		if (!fData)
		{
			cout << "open '" << fileName << "' Data file error!" << endl;
			abort();
		}
		//DataSet.clear();
		string line = "";
		string keyword;
		int spilitCount = 0;
		AList<string, CompStr> *conjList;
		AList<string, CompStr> *disjList;
		while (!fData.eof() && notFull && getline(fData, line))
		{
			if (line != "")
			{
				//4-26日修改，文件名取当前文件名，不显示路径信息;
				int last_delimiter_index = fileName.find_last_of('/');//可能在linux 环境中，需要替换成/
				//cout << "last_delimiter_index = " << last_delimiter_index << endl;
				string fileName_o = fileName.substr(last_delimiter_index + 1, fileName.size());//文件名
				//cout << "fileName_o = " << fileName_o << endl;
				//首先获取line中有多少个分割符","
				spilitCount = StringUtil::spCount(line, ',');
				//AList<string, CompStr> conjList(spilitCount); //AList<int> *pDataSet = new  AList<int>(DataNumber);
				//AList<string, CompStr> disjList(spilitCount);
				conjList = new AList<string, CompStr>(spilitCount);
				disjList = new AList<string, CompStr>(spilitCount);
				StringUtil::splitStringInAList(line, *conjList, ",");
				StringUtil::splitStringInAList(line, *disjList, ",");
				//conjList.print();
				//disjList.print();
				//Node node = Node(fileName_o, 1, line);
				Node node = Node(fileName_o, 1, disjList, conjList);
				//Node(string f_n, int nsize, AList<string, CompStr> * d_ks, AList<string, CompStr> *c_ks)
				if (!DataSet.append(node))
				{
					cout << "Data Set is full!" << endl;
					cout << "the current filename is '" << fileName << "',the list max size is " << DataSet.getListMaxSize() << endl;
					abort();
					notFull = false;
				};
			}
		}
		fData.close();
	}


	//得到某一文件下的所有文件路径
	//static void getFiles(const string path, AList<string, CompStr>& files)
	//{
	//	cout << "path = " << path << endl;
	//	//文件句柄
	//	long long   hFile = 0;
	//	//文件信息
	//	struct _finddata_t fileinfo;
	//	string p;
	//	if ((hFile = _findfirst(p.assign(path).append("/*").c_str(), &fileinfo)) != -1)
	//	{
	//		do
	//		{
	//			//如果是目录,迭代之
	//			//如果不是,加入列表
	//			if ((fileinfo.attrib &  _A_SUBDIR))
	//			{
	//				/*
	//				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
	//					getFiles(p.assign(path).append("/").append(fileinfo.name), files);
	//				*/
	//			}
	//			else
	//			{
	//				files.append(p.assign(path).append("/").append(fileinfo.name));
	//			}
	//		} while (_findnext(hFile, &fileinfo) == 0);
	//		_findclose(hFile);
	//	}
	//}


	///*读取文件，读取结果存储在LeafNode类型的AList中*/
	//static void readData(const string fileName, AList<LeafNode *, CompLNode> &DataSet, AList<string, CompStr> &fterms)
	//{
	//	fstream fData;
	//	bool notFull = true;
	//	fData.open(fileName.c_str(), ios::in);
	//	if (!fData)
	//	{
	//		cout << "open '" << fileName << "' Data file error!" << endl;
	//		abort();
	//	}
	//	string line = "";
	//	string keyword;
	//	int commonSize = 0;
	//	int ftermSize = 0;
	//	AList<string, CompStr> *keywords;
	//	//AList<string, CompStr> *pTemp = new AList<stringCom
	//	while (!fData.eof() && notFull && getline(fData, line))
	//	{
	//		if (line != "")
	//		{
	//			//4-26日修改，文件名取当前文件名，不显示路径信息;
	//			int last_delimiter_index = fileName.find_last_of('/');
	//			string fileName_o = fileName.substr(last_delimiter_index + 1, fileName.size());//文件名
	//			//首先获取line中有多少个分割符","
	//			commonSize = StringUtil::spCount(line, ',');//本份数据集中，有多少个,就有多少个关键字
	//			keywords = new AList<string, CompStr>(commonSize);
	//			StringUtil::splitStringInAList(line, *keywords, ",");
	//			keywords->sort();
	//			LeafNode *node = new LeafNode(fileName_o, keywords);

	//			if (!DataSet.append(node))
	//			{
	//				cout << "Data Set is full!" << endl;
	//				cout << "the current filename is '" << fileName << "',the list max size is " << DataSet.getListMaxSize() << endl;
	//				abort();
	//				notFull = false;
	//			};
	//		}
	//	}
	//	fData.close();
	//}

	/*将文件中的数据，读入到DataSet中*/
	/*这里的写法存在一种优化，不使用静态方法，把AList<string,CompStr> &pL_Fterm 变成一个成员变量，可以不用作为参数传递*/
	//static void readDatas(AList<string, CompStr>& files, AList<LeafNode*, CompLNode> &DataSet, string keywordDir, AList<string, CompStr> &pL_Fterm){
	//	string fileName;
	//	for (files.setStart(); files.getValue(fileName); files.next())
	//		readData((keywordDir + fileName), DataSet, pL_Fterm);
	//}




	/*从文件中获取数量*/
	static void readDataCount(const string fileName, int &dataCount){
		fstream fData;
		fData.open(fileName.c_str(), ios::in);
		if (!fData)
		{
			cout << "open Data file error!" << endl;
			abort();
		}
		string line;
		if (!fData.eof() && getline(fData, line)){
			if (line != ""){
				dataCount = StringUtil::stringToInt(line);
			}
		}
		fData.close();
	}

	static void readIData(const string fileName, AList<InvertedNode, CompINode> &DataSet, int &elem_count){
		fstream fData;
		cout << "fileName = " << fileName << endl;
		fData.open(fileName.c_str(), ios::in);
		bool notFull = true;
		if (!fData)
		{
			cout << "open Data file error!" << endl;
			abort();
		}
		DataSet.clear();
		string line = "";
		string count;
		string keyword;
		string fileNames;
		vector<string> fileV;
		while (!fData.eof() && notFull && getline(fData, line))
		{
			if (line != "")
			{
				istringstream is(line);
				is >> count >> keyword >> fileNames;
				fileV.clear();
				StringUtil::splitString(fileNames, fileV, ",");
				InvertedNode iNode = InvertedNode(keyword, StringUtil::stringToInt(count), fileV);
				elem_count += StringUtil::stringToInt(count);//BloomFilterSize
				if (!DataSet.append(iNode))
				{
					cout << "Data Set is full!" << endl;
					cout << "the current filename is '" << fileName << "',the list max size is " << DataSet.getListMaxSize() << endl;
					abort();
					notFull = false;
				};
			}
		}
	}

	static void readQueryData(const string fileName, AList<AList<string, CompStr>*, CompInt> &QuerySet, int cur_con_size){
		fstream fData;
		fData.open(fileName.c_str(), ios::in);
		bool notFull = true;
		if (!fData)
		{
			cout << "open Data file error!" << "fileName = " << fileName << endl;
			abort();
		}
		QuerySet.clear();
		string line = "";
		string con_size;/*这个数值暂时不用加入到QuerySet中*/
		string keywords;
		//string fileNames;
		AList<string, CompStr> *curQuery;
		while (!fData.eof() && notFull && getline(fData, line)){
			//if (line != ""){
			istringstream is(line);
			is >> con_size >> keywords;
			curQuery = new AList<string, CompStr>(cur_con_size);
			StringUtil::splitStringIntoAlist(keywords, *curQuery, ",");
			if (!QuerySet.append(curQuery))
			{
				/*异常出口*/
				cout << "Data Set is full!" << endl;
				cout << "the current filename is '" << fileName << "',the list max size is " << QuerySet.getListMaxSize() << endl;
				abort();
				notFull = false;
			}
			if (QuerySet.getListSize() == QuerySet.getListMaxSize()){
				/*正常出口*/
				notFull = false;
			}
		}
		/*删除最后一个元素*/
		if (QuerySet.getListSize() != 1000)
			QuerySet.removeLast();
	}


};


#endif