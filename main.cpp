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
	/*�������������ļ�·��*/

	//iiFName = prefixDir + "QuerySet/" + dataSize + "/querySetAll_tt.txt";
	///*conjunctive querySet�ļ�·��*/
	//c_prefix = prefixDir + "QuerySet/" + dataSize + "/conjunctive/";
	///*disjunctive querySet�ļ�·��*/
	//d_prefix = prefixDir + "QuerySet/" + dataSize + "/disjunctive/2/";
	///*����ļ�·��*/
	//output = prefixDir + "IEXResult/" + dataSize + "_result.txt";


	/*����·��*/
	iiFName = prefixDir + "QuerySet_test/1b/querySetAll.txt";
	dirFNames = prefixDir+ "keywords/1b_names.txt";
	keywordDir = prefixDir + "keywords/1b/";
	/*conjunctive querySet�ļ�·��*/
	c_prefix = prefixDir + "QuerySet_test/1b/conjunctive/";
	/*disjunctive querySet�ļ�·��*/
	d_prefix = prefixDir + "QuerySet_test/1b/disjunctive/";
	/*����ļ�·��*/
	output = prefixDir + "IEXResult/1b_result.txt";
}

vector<string> vectors_intersection(vector<string> v1, vector<string> v2){
	vector<string> v;
	/*��Ϊ��֤���������Բ���Ҫ����*/
	//sort(v1.begin(), v1.end());
	//sort(v2.begin(), v2.end());
	set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(v));//�󽻼� 
	return v;
}


void genEMMData
(AList<InvertedNode, CompINode> &IDataSet,HashTable<string, InvertedNode, StringKeyHash> *A1, HashTable<string, Node, StringKeyHash> *A2,
HashTable<string, HashTable<string, AList<string, CompStr>*, StringKeyHash>*, StringKeyHash > *EMM){
	//double p = 0.2;
	/*���浱ǰ�ؼ��ֵ��ڲ�table*/
	HashTable<string, AList<string, CompStr>*, StringKeyHash> *temp_table;
	InvertedNode iNode1,iNode2;
	Node node1;
	string key1, key2;
	vector<string> fileList,fileList1,fileList2;
	AList<string, CompStr> *keywords;
	AList<string, CompStr> *fileNames;
	/*����ĳЩ�ĵ���Ӧ��keyֵ*/
	set<string> keySet; //���صı��뻷��ʹ��string&���벻ͨ��
	bool hasConjunctive;
	/*����ÿһ��InvertedIndex*/
	for (IDataSet.setStart(); IDataSet.getValue(iNode1);IDataSet.next()){
		key1 = iNode1.keyword;
		fileList1 = iNode1.fileNames;/*key1��Ӧ���ĵ�*/
		hasConjunctive = false;
			/*�������ĵ���Ӧ�Ĺؼ���ȥ�طŵ�keySet��*/
		for (int i = 0; i < fileList1.size(); i++){
			//�ҵ��ĵ���Ӧ�Ĺؼ���
			A2->get(fileList1[i], node1);
			keywords = node1.conjunctive_keys;
			//���뵽�������ظ�Ԫ�ص�������
			for (keywords->setStart(); keywords->getValue(key2); keywords->next())
				//if (key2.compare(key1) != 0)/*�Ƚ������ַ����Ƿ����,��������������*/  
				//����ȫ��������ˣ���ΪQuerySet��֤�˲�����ĳ���ؼ��ֺ��Լ��Ĳ���
				keySet.insert(key2);				
		}
		/*
		if (keySet.size() <= 0){
			continue;
			cout << "error happened keySet.size() = 0" << endl;
		}*/
		set<string>::iterator it;//���صı��뻷��ʹ��string&���벻ͨ��
		temp_table = new HashTable<string, AList<string, CompStr>*, StringKeyHash>(keySet.size() * 2);/*inner hashtable load factor = 0.5*/
		/*����ÿһ����key1�н����Ĺؼ���key2*/
		for (it = keySet.begin(); it != keySet.end(); it++){
			if (A1->get(*it, iNode2)){
				fileList2 = iNode2.fileNames;
				/*����Ҫ��fileList1��fileList2�Ľ���*/
				fileList = vectors_intersection(fileList1,fileList2);
				if (fileList.size() != 0){/*���Ҫ��Ӳ���P��������*/
					fileNames = StringUtil::vectorToAList(fileList);/*�ڷ����ڲ�new��*/
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
	//1.�������е����������ݵ�GMM��
	double process_time;
	FileUtil::getFiles(dirFNames, FileSet);
	FileUtil::readDatas(FileSet, iDKeySet, keywordDir);
	cout << "read File data success" << endl;

	
	/*��ȡInvertedNode���ݵ��ڴ�*/
	int elem_count = 0;//��¼�������� ��ֵ�Եĸ�������ʱ�ò���
	FileUtil::readIData(iiFName, DataSet, elem_count);
	int tableMaxsize = DataSet.getListSize() * 2;//ѡ��������Ϊ0.5
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

	/*��InvertedNodes���ݶ���GMM��*/
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
	//3.���Զ���ؼ��ֵ�conjunctive
	//��ȡQuery
	AList<string, CompStr> *Result = new AList<string, CompStr>(150);
	string temp_querySet;
	string c_querySet;
	AList<string, CompStr> *Query;
	time_t query_start;
	time_t query_end;
	//string curr_key;
	/*4.����2���ؼ��ֵ�disjunctive*/

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

/*����S1��S2....Sn,ȡ��S1��Result = S1, ��ʱ����Ҫ����*/
//for (int keywordsSize = 2; keywordsSize < 3; keywordsSize++){//TODO ��Ϊ5
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
//			/*1.ȡ����һ���ؼ��ֶ�Ӧ�������ĵ���ӵ�Result��,
//			LinkedListֻ��Ҫ��ÿһ���ѯ֮���ͷ���Դ���Ҽ�¼ʱ�䣬
//			��ArrayListɾ��Ԫ����ҪƵ�����ƶ�Ԫ��.

//			update 1-17 ��Seny�Ĵ�����ʹ�õ���ArrayList,��ֻ��ZMF������ʵ����2���ؼ��ֵ�conjunctive�����ǵײ�Result���õ�ArrayList
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
//����disjunctive 60 london,informative
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
//19 high,problems,researc,scie ��
//12 atomic, electron, properties, spectroscopy ��

//ENron���ݼ�
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
//31 east,science,united ��
//151 systems, technology, tools ��
//Enron ���ݼ�
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
//44 addition, considered ��
//55 organic, understand ��
//Enron���ݼ�
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


/*���ݵ����������ϲ���EMM�����ݣ�����д�뵽�ļ��У�
warning:��δ�����ÿһ�����ݼ���ֻ��Ҫ����һ��
warning2:�÷����β���Ҫ���&,���ݲ�֪Ϊʲô.
�����������ã���Ҫ����Ԥ��������С.
*/
//void genEMMDataFileFormat(AList<InvertedNode, CompINode> &DataSet, string fileName){
//	ofstream fout;
//	fout.open(EMM_F_Name.c_str(),ios::out);
//	InvertedNode nodeI, nodeJ;
//	InvertedNode *temp = new InvertedNode();
//	string conjunctive_file;//���������ؼ���conjunctive��file����","�ָ�.
//	
//	int run_count = 0;
//
//	for (int i = 0; i < DataSet.getListSize(); i++){
//		DataSet.getPosValue(i,nodeI);
//		run_count++;
//		if (run_count % 20==0){
//			cout << "otter run count = " << run_count << endl;
//		}
//		/*��������жϴ�*/
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
//			/*��������ؼ��ʵ�conjunctive size != 0 ����Ҫ��¼*/
//			if (temp->docsize != 0){
//			conjunctive_file = StringUtil::vectorToString(temp->fileNames);
//			fout << temp->keyword << " "<<conjunctive_file<< endl;
//			}
//		}
//	}
//	cout << "gen EMM data Success ,fileLocation : " << fileName << endl;
//	fout.close();
//}
