#include "AList.h"
#include "CompStr.h"
#include <iostream>
#include "HashTable.h"
#include "ExpHelper.h"
#include "FileUtil.h"
#include "time.h"
#include "LList.h"
#include "StringKeyHash.h"
#include "algorithm"
#include "iterator"
#include <set>
#define DATA_NUMS 100
#define KEYSIZE 500000
#define EMM_SIZE 9000000
#pragma comment(linker, "/STACK:1024000000,1024000000")
using namespace std;
string iiFName, dirFNames, keywordDir, output, c_prefix, d_prefix, dataSize;
AList<string, CompStr> FileSet(DATA_NUMS);


/*inverted Index Data k,id pairs*/
AList<InvertedNode, CompINode> DataSet(KEYSIZE);
/*id,k pairs Data*/
AList<Node, CompNode> iDKeySet(DATA_NUMS);
AList<AList<string, CompStr>*, CompInt> QuerySet(1000);
string prefixDir = "../../datas/";
void initFile(){
	string dataSize = StringUtil::intToString((DATA_NUMS / 10000)) + "w";
	cout << "dataSize = " << dataSize << endl;
	/*倒排索引数据文件路径*/

	//iiFName = prefixDir + "QuerySet/" + dataSize + "/querySetAll_tt.txt";
	///*conjunctive querySet文件路径*/
	//c_prefix = prefixDir + "QuerySet/" + dataSize + "/conjunctive/";
	///*disjunctive querySet文件路径*/
	//d_prefix = prefixDir + "QuerySet/" + dataSize + "/disjunctive/2/";
	///*输出文件路径*/
	//output = prefixDir + "IEXResult/" + dataSize + "_result.txt";


	/*测试路径*/
	iiFName = prefixDir + "QuerySet_test/1b/querySetAll.txt";
	dirFNames = prefixDir+ "keywords/1b_names.txt";
	keywordDir = prefixDir + "keywords/1b/";
	/*conjunctive querySet文件路径*/
	c_prefix = prefixDir + "QuerySet_test/1b/conjunctive/";
	/*disjunctive querySet文件路径*/
	d_prefix = prefixDir + "QuerySet_test/1b/disjunctive/";
	/*输出文件路径*/
	output = prefixDir + "IEXResult/1b_result.txt";
}

vector<string> vectors_intersection(vector<string> v1, vector<string> v2){
	vector<string> v;
	/*因为保证了有序，所以不需要排序*/
	//sort(v1.begin(), v1.end());
	//sort(v2.begin(), v2.end());
	set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(v));//求交集 
	return v;
}


void genEMMData
(AList<InvertedNode, CompINode> &IDataSet,HashTable<string, InvertedNode, StringKeyHash> *A1, HashTable<string, Node, StringKeyHash> *A2,
HashTable<string, HashTable<string, AList<string, CompStr>*, StringKeyHash>*, StringKeyHash > *EMM){
	//double p = 0.2;
	/*保存当前关键字的内层table*/
	HashTable<string, AList<string, CompStr>*, StringKeyHash> *temp_table;
	InvertedNode iNode1,iNode2;
	Node node1;
	string key1, key2;
	vector<string> fileList,fileList1,fileList2;
	AList<string, CompStr> *keywords;
	AList<string, CompStr> *fileNames;
	/*保存某些文档对应的key值*/
	set<string> keySet; //本地的编译环境使用string&编译不通过
	bool hasConjunctive;
	/*遍历每一个InvertedIndex*/
	for (IDataSet.setStart(); IDataSet.getValue(iNode1);IDataSet.next()){
		key1 = iNode1.keyword;
		fileList1 = iNode1.fileNames;/*key1对应的文档*/
		hasConjunctive = false;
			/*将所有文档对应的关键字去重放到keySet中*/
		for (int i = 0; i < fileList1.size(); i++){
			//找到文档对应的关键字
			A2->get(fileList1[i], node1);
			keywords = node1.conjunctive_keys;
			//插入到不允许重复元素的容器中
			for (keywords->setStart(); keywords->getValue(key2); keywords->next())
				//if (key2.compare(key1) != 0)/*比较两个字符串是否相等,不相等则加入容器*/  
				//这里全加入就行了，因为QuerySet保证了不会求某个关键字和自己的并集
				keySet.insert(key2);				
		}
		/*
		if (keySet.size() <= 0){
			continue;
			cout << "error happened keySet.size() = 0" << endl;
		}*/
		set<string>::iterator it;//本地的编译环境使用string&编译不通过
		temp_table = new HashTable<string, AList<string, CompStr>*, StringKeyHash>(keySet.size() * 2);/*inner hashtable load factor = 0.5*/
		/*遍历每一个与key1有交集的关键字key2*/
		for (it = keySet.begin(); it != keySet.end(); it++){
			if (A1->get(*it, iNode2)){
				fileList2 = iNode2.fileNames;
				/*这里要求fileList1与fileList2的交集*/
				fileList = vectors_intersection(fileList1,fileList2);
				if (fileList.size() != 0){/*如果要添加参数P，是这里*/
					fileNames = StringUtil::vectorToAList(fileList);/*在方法内部new了*/
					temp_table->insert(*it, fileNames);
					hasConjunctive = true;
				}
			}
			else cout << "error happened" << endl;
		}
		if (hasConjunctive){
			EMM->insert(key1, temp_table);
		}
		keySet.clear();
	}
}


int main(){
	initFile();
	ofstream fout;
	fout.open(output.c_str(), ios::out);
	//1.插入所有倒排索引数据到GMM中
	double process_time;
	FileUtil::getFiles(dirFNames, FileSet);
	FileUtil::readDatas(FileSet, iDKeySet, keywordDir);
	cout << "read File data success" << endl;

	
	/*读取InvertedNode数据到内存*/
	int elem_count = 0;//记录倒排索引 键值对的个数，暂时用不着
	FileUtil::readIData(iiFName, DataSet, elem_count);
	int tableMaxsize = DataSet.getListSize() * 2;//选择负载引子为0.5
	cout << "GGM maxSize = " << tableMaxsize << " Load Factor = 0.5" << endl;
	HashTable<string, AList<string, CompStr>*, StringKeyHash> *GGM = new HashTable<string, AList<string, CompStr>*, StringKeyHash>(tableMaxsize);
	cout << "get invertedIndex Data success" << endl;

	
	HashTable<string, InvertedNode, StringKeyHash> *key_idtable = new HashTable<string, InvertedNode, StringKeyHash>(DataSet.getListSize()*2);
	HashTable<string, Node, StringKeyHash> *id_keytable = new HashTable<string, Node, StringKeyHash>(iDKeySet.getListSize() * 2);
	InvertedNode inode;
	Node node;
	for (DataSet.setStart(); DataSet.getValue(inode); DataSet.next())
		key_idtable->insert(inode.keyword,inode);
	cout << "insert key->id table success;"<<endl;
	for (iDKeySet.setStart(); iDKeySet.getValue(node); iDKeySet.next())
		id_keytable->insert(node.names, node);
	cout << "insert id->key table success;"<<endl;
	/*gen EMM*/
	HashTable<string, HashTable<string, AList<string, CompStr>*, StringKeyHash>*, StringKeyHash > *EMM 
		= new HashTable<string, HashTable<string, AList<string, CompStr>*, StringKeyHash>*, StringKeyHash >(tableMaxsize);
	time_t EMM_start;
	time_t EMM_end;
	EMM_start = clock();
	genEMMData(DataSet, key_idtable,id_keytable,EMM);
	EMM_end = clock();
	process_time = (double) (EMM_end - EMM_start) / CLOCKS_PER_SEC;
	cout << "EMM init time = " << process_time << " s" << endl;

	/*将InvertedNodes数据读入GMM中*/
	DataSet.setStart();
	int docsSize;
	string keyword;
	vector<string> docs;
	InvertedNode iNode;
	AList<string,CompStr>* docsList;
	time_t insert_start = clock();
	DataSet.getValue(iNode);
	while (DataSet.getValue(iNode)){
		docsSize = iNode.docsize;
		keyword = iNode.keyword;
		docs = iNode.fileNames;
		docsList = new AList<string,CompStr>(docsSize);
		for (unsigned int i = 0; i < docs.size(); i++)
			docsList->append(docs[i]);
		GGM->insert(keyword, docsList);
		DataSet.next();
	}
	time_t insert_end = clock();
	process_time = (double)((insert_end - insert_start) * 1000) / CLOCKS_PER_SEC;
	cout << "GGM init time = " << process_time << " ms" << endl;
	ExpHelper *helper = new ExpHelper(GGM,EMM);
	//3.测试多个关键字的conjunctive
	//获取Query
	AList<string, CompStr> *Result = new AList<string, CompStr>(150);
	string temp_querySet;
	string c_querySet;
	AList<string, CompStr> *Query;
	time_t query_start;
	time_t query_end;
	//string curr_key;
	/*4.测试2个关键字的disjunctive*/

	cout << "disjunctive begin." << endl;
	fout << "----------------disjunctive----------------" << endl;
	for (int i = 10; i <= 100; i += 10){
		temp_querySet = d_prefix + "query_" + StringUtil::intToString(i) + ".txt";
		cout << "temp_querySet = " << temp_querySet << endl;
		//fout << "temp_querySet = " << temp_querySet << endl;
		FileUtil::readQueryData(temp_querySet, QuerySet, 2);
		cout << "QueryCount = " << i << " Read QuerySet Success QuerySet.getListSize() = " << QuerySet.getListSize() << endl;
		//fout << "QueryCount = " << i << " Read QuerySet Success QuerySet.getListSize() = " << QuerySet.getListSize() << endl;
		query_start = clock();
		for (QuerySet.setStart(); QuerySet.getValue(Query); QuerySet.next()){
			Result->clear();
			helper->disQuery_t(Query, Result);
			cout << "Result.size() = " << Result->getListSize() << endl;
			//fout << "Result.size() = " << Result->getListSize() << endl;
		}
		query_end = clock();
		process_time = (double)((query_end - query_start) * 1000) / (CLOCKS_PER_SEC*QuerySet.getListSize());
		cout << "query_Count = " << i << " every single query time = " << process_time << " ms" << endl;
		fout << process_time << endl;
		QuerySet.clear();
	}
		//delete helper;
		system("pause");
}

/*集合S1，S2....Sn,取出S1，Result = S1, 暂时不需要调用*/
//for (int keywordsSize = 2; keywordsSize < 3; keywordsSize++){//TODO 改为5
//	cout << "----------------" << keywordsSize << "----------------" << endl;
//	fout << "----------------" << keywordsSize << "----------------" << endl;
//	c_querySet = c_prefix + StringUtil::intToString(keywordsSize) + "/";
//	for (int i = 0; i <= 100; i += 10){
//		temp_querySet = c_querySet + "query_" + StringUtil::intToString(i) + ".txt";
//		cout << "temp_querySet = " << temp_querySet << endl;
//		//fout << "temp_querySet = " << temp_querySet << endl;
//		FileUtil::readQueryData(temp_querySet, QuerySet, keywordsSize);
//		cout << "QueryCount = " << i << " Read QuerySet Success QuerySet.getListSize() = " << QuerySet.getListSize() << endl;
//		if (QuerySet.getListSize() == 0){
//			fout << "0" << endl;
//			continue;
//		}
//		//fout << "QueryCount = " << i << " Read QuerySet Success QuerySet.getListSize() = " << QuerySet.getListSize() << endl;
//		query_start = clock();
//		for (QuerySet.setStart(); QuerySet.getValue(Query); QuerySet.next()){
//			Result->clear();
//			/*1.取出第一个关键字对应的所有文档添加到Result中,
//			LinkedList只需要在每一组查询之后释放资源并且记录时间，
//			而ArrayList删除元素需要频繁的移动元素.

//			update 1-17 在Seny的代码中使用的是ArrayList,他只在ZMF方案中实现了2个关键字的conjunctive，但是底层Result是用的ArrayList
//			*/
//			helper->conQuery(Query, Result);
//			//cout << "Result.size() = " << Result->getListSize() << endl;
//		}
//		query_end = clock();
//		process_time = (double)((query_end - query_start) * 1000) / (CLOCKS_PER_SEC*QuerySet.getListSize());
//		cout << "query_Count = " << i << " every single query time = " << process_time << " ms" << endl;
//		fout << process_time << endl;
//		QuerySet.clear();
//	}
//}



/* test backup
cout << "---------------------test disjunctive--------------------" << endl;
result.clear();
//测试disjunctive 60 london,informative
//months, future


//helper->disjunctiveQueryO("london", "informative", result);
//cout << "begin result.Size() = " << result.size() << endl;
//for (int i = 0; i < result.size(); i++){
//	cout << result[i] << ",";
//}

//sort(result.begin(), result.end());
//vector<string>::iterator iter = unique(result.begin(), result.end());
//result.erase(iter, result.end());
//cout << endl;
//cout << "after result.Size() = " << result.size() << endl;
//for (int i = 0; i < result.size(); i++){
//	cout << result[i] << ",";
//}
//cout << endl;


cout << "------------------------key size = 4--------------------------" << endl;

AList<string, CompStr> *Query = new AList<string, CompStr>(4);
result.clear();
//19 high,problems,researc,scie 对
//12 atomic, electron, properties, spectroscopy 对

//ENron数据集
//70 Center, Cplace, Creceiving, Cturn
//56 Cbenefit, Cform, Cquestion, Creal

//old
//65 development,graduate,test,work
Query->append("Cbenefit");
Query->append("Cform");
Query->append("Cquestion");
Query->append("Creal");
helper->proQuery(*Query, result);
cout << "conjunctive result.Size() = " << result.size() << endl;
for (int i = 0; i < result.size(); i++){
cout << result[i] << ",";
}
result.clear();
cout << endl;

cout << "------------------------key size = 3--------------------------" << endl;
AList<string, CompStr> *Query1 = new AList<string, CompStr>(3);
result.clear();
//31 east,science,united 对
//151 systems, technology, tools 对
//Enron 数据集
//60 Cconference, Chttp, Cmeeting
//73 Cassociate, Chour, Csign


//30 Caccept, Ccost, Clive

//77 process,properties,types
Query1->append("Cassociate");
Query1->append("Chour");
Query1->append("Csign");

//Query->append("div");
helper->proQuery(*Query1, result);
cout << "conjunctive result.Size() = " << result.size() << endl;
for (int i = 0; i < result.size(); i++){
cout << result[i] << ",";
}
result.clear();

////////////////////////////2

cout << "------------------------key size = 2--------------------------" << endl;
AList<string, CompStr> *Query2 = new AList<string, CompStr>(2);
result.clear();
//44 addition, considered 对
//55 organic, understand 对
//Enron数据集
//93 Csummary, Cwilliam


//48 Ccritical, Cpick
//447 Cagree, Cdays
Query2->append("Cagree");
Query2->append("Cdays");

helper->proQuery(*Query2, result);
cout << "conjunctive result.Size() = " << result.size() << endl;
for (int i = 0; i < result.size(); i++){
cout << result[i] << ",";
}
result.clear();
//Ccritical0001011011010101|a004461.txt#2

cout << "---------------------------old---------------------" << endl;
helper->mainConQuery("Ccritical","Cpick",result);
cout << endl;
cout << "old conjunctive result.Size() = " << result.size() << endl;
for (int i = 0; i < result.size(); i++){
cout << result[i] << ",";
}
*/


/*根据倒排索引集合产生EMM的数据，并且写入到文件中，
warning:这段代码在每一份数据集中只需要运行一次
warning2:该方法形参需要添加&,但暂不知为什么.
本方法已弃用，主要用于预判索引大小.
*/
//void genEMMDataFileFormat(AList<InvertedNode, CompINode> &DataSet, string fileName){
//	ofstream fout;
//	fout.open(EMM_F_Name.c_str(),ios::out);
//	InvertedNode nodeI, nodeJ;
//	InvertedNode *temp = new InvertedNode();
//	string conjunctive_file;//保存两个关键字conjunctive的file，以","分割.
//	
//	int run_count = 0;
//
//	for (int i = 0; i < DataSet.getListSize(); i++){
//		DataSet.getPosValue(i,nodeI);
//		run_count++;
//		if (run_count % 20==0){
//			cout << "otter run count = " << run_count << endl;
//		}
//		/*添加条件判断处*/
//		//if (nodeI.docsize <5){
//		//	//cout << "i docSize = " <<nodeI.docsize<< endl;
//		//	continue;
//		//}
//		for (int j = i + 1; j < DataSet.getListSize(); j++){
//			DataSet.getPosValue(j,nodeJ);
//			//if (nodeJ.docsize <5){
//			//	continue;
//			//}
//			temp->conjunctiveInode(&nodeI,&nodeJ);
//			/*如果两个关键词的conjunctive size != 0 才需要记录*/
//			if (temp->docsize != 0){
//			conjunctive_file = StringUtil::vectorToString(temp->fileNames);
//			fout << temp->keyword << " "<<conjunctive_file<< endl;
//			}
//		}
//	}
//	cout << "gen EMM data Success ,fileLocation : " << fileName << endl;
//	fout.close();
//}
