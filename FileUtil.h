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
	/*����Linux�²����ã���Ϊ���ļ��ж�ȡ*/
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

	/*���ļ��е����ݣ����뵽DataSet��*/
	static void readDatas(AList<string, CompStr>& files, AList<Node, CompNode> &DataSet, string keywordDir){
		int length = files.getListSize();
		string fileNames;
		for (int i = 0; i < length; i++){
			readData((keywordDir + files.getPosValue(i)), DataSet);
		}
	}

	/*��ȡ�ļ�����ȡ����洢��Node���͵�AList��   Alist���͵�keyword*/
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
				//4-26���޸ģ��ļ���ȡ��ǰ�ļ���������ʾ·����Ϣ;
				int last_delimiter_index = fileName.find_last_of('/');//������linux �����У���Ҫ�滻��/
				//cout << "last_delimiter_index = " << last_delimiter_index << endl;
				string fileName_o = fileName.substr(last_delimiter_index + 1, fileName.size());//�ļ���
				//cout << "fileName_o = " << fileName_o << endl;
				//���Ȼ�ȡline���ж��ٸ��ָ��","
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


	//�õ�ĳһ�ļ��µ������ļ�·��
	//static void getFiles(const string path, AList<string, CompStr>& files)
	//{
	//	cout << "path = " << path << endl;
	//	//�ļ����
	//	long long   hFile = 0;
	//	//�ļ���Ϣ
	//	struct _finddata_t fileinfo;
	//	string p;
	//	if ((hFile = _findfirst(p.assign(path).append("/*").c_str(), &fileinfo)) != -1)
	//	{
	//		do
	//		{
	//			//�����Ŀ¼,����֮
	//			//�������,�����б�
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


	///*��ȡ�ļ�����ȡ����洢��LeafNode���͵�AList��*/
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
	//			//4-26���޸ģ��ļ���ȡ��ǰ�ļ���������ʾ·����Ϣ;
	//			int last_delimiter_index = fileName.find_last_of('/');
	//			string fileName_o = fileName.substr(last_delimiter_index + 1, fileName.size());//�ļ���
	//			//���Ȼ�ȡline���ж��ٸ��ָ��","
	//			commonSize = StringUtil::spCount(line, ',');//�������ݼ��У��ж��ٸ�,���ж��ٸ��ؼ���
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

	/*���ļ��е����ݣ����뵽DataSet��*/
	/*�����д������һ���Ż�����ʹ�þ�̬��������AList<string,CompStr> &pL_Fterm ���һ����Ա���������Բ�����Ϊ��������*/
	//static void readDatas(AList<string, CompStr>& files, AList<LeafNode*, CompLNode> &DataSet, string keywordDir, AList<string, CompStr> &pL_Fterm){
	//	string fileName;
	//	for (files.setStart(); files.getValue(fileName); files.next())
	//		readData((keywordDir + fileName), DataSet, pL_Fterm);
	//}




	/*���ļ��л�ȡ����*/
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
		string con_size;/*�����ֵ��ʱ���ü��뵽QuerySet��*/
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
				/*�쳣����*/
				cout << "Data Set is full!" << endl;
				cout << "the current filename is '" << fileName << "',the list max size is " << QuerySet.getListMaxSize() << endl;
				abort();
				notFull = false;
			}
			if (QuerySet.getListSize() == QuerySet.getListMaxSize()){
				/*��������*/
				notFull = false;
			}
		}
		/*ɾ�����һ��Ԫ��*/
		if (QuerySet.getListSize() != 1000)
			QuerySet.removeLast();
	}


};


#endif