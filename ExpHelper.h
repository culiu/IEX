/**
实验辅助类
**/
#ifndef _EXPHELPER_
#define _EXPHELPER_
#include "hashtable.h"
#include "StringKeyHash.h"
#include "Alist.h"
#include "CompStr.h"
using namespace std;
#define SAFE_DEL(x) { if (x != NULL) delete x; x = NULL; }
class ExpHelper
{
public:
	ExpHelper(HashTable<string, AList<string, CompStr>*, StringKeyHash> *p_G, HashTable<string, HashTable<string, AList<string, CompStr>*, StringKeyHash>*, StringKeyHash > *p_E){
		p_GMM = p_G;//load factor  = 0.5
		p_EMM = p_E;//load factor  = 0.5
	}

	~ExpHelper(){
		SAFE_DEL(p_GMM);
		SAFE_DEL(p_EMM);
	}
	/*param1：query condition
	  param2：result
	  多个关键字的conjunctive
	*/
	void conQuery(AList<string, CompStr> *Query,AList<string, CompStr> *Result){
		/*取出第一个关键字去GMM中查询结果*/
		string curr;/*当前关键字*/
		string f_n;
		AList<string, CompStr> *p_temp;/*当前关键字对应的文档*/
		Query->setStart();
		Query->getValue(curr);/*第一个关键字*/

		/*如果在这里直接使用Result指针指向f_list则会修改hashtable中的内容，所以只能是一个个的插入元素到Result中*/
		AList<string, CompStr>* f_list;
		p_GMM->get(curr, f_list);
		/*把第一个关键字对应的文档添加进Result中*/
		for (f_list->setStart(); f_list->getValue(f_n); f_list->next())
			Result->append(f_n);
		Query->next();
		/*从第二个关键字到最后一个关键字，判断对应的GMM中的集合元素是否在Result中，如果不在则去掉*/
		if (Result->getListSize() != 0){
			while (Query->getValue(curr)){
				p_GMM->get(curr,p_temp);
				SetConjunction(Result,p_temp);
				Query->next();
			}
		}
	}
	/*保存既在S1中，也在S2中的元素*/
	void SetConjunction(AList<string, CompStr>*p_S1, AList<string, CompStr>*p_S2){
		/*由于传递进来的元素都是排序好的，所以不需要排序*/
		string value1, value2, temp;
		p_S1->setStart();
		p_S2->setStart();
		while (p_S1->getValue(value1) && p_S2->getValue(value2)){
			int compareResult = strcmp(value1.c_str(), value2.c_str());/*it may work*/
			if (compareResult == 0){
				p_S1->next();
				p_S2->next();
			}
			else if (compareResult > 0)
				p_S2->next();
			else
				p_S1->remove(temp);
			
		}
		while (p_S1->getValue(value1))
			/*当前元素与之后的元素都需要删除*/
			p_S1->remove(temp);
	}
	/*从S1中删除同时存在在S2中的元素*/
	void SetRemove(AList<string, CompStr>*p_S1, AList<string, CompStr>*p_S2){
		/*密文需要排序*/
		p_S1->sort();
		p_S2->sort();
		string value1, value2, temp;
		p_S1->setStart();
		p_S2->setStart();
		while (p_S1->getValue(value1) && p_S2->getValue(value2)){
			int compareResult = strcmp(value1.c_str(), value2.c_str());/*it may work*/
			if (compareResult == 0){
				p_S1->remove(temp);
				p_S2->next();
			}
			else if (compareResult > 0)
				p_S2->next();
			else
				p_S1->next();

		}
	}


	/*本方法暂不通用多个关键字，暂只做了2个关键字*/
	void disQuery_t(AList<string, CompStr> *Query, AList<string, CompStr>* Result){
		string k1 = Query->getPosValue(0);
		string k2 = Query->getPosValue(1);
		string temp;
		AList<string, CompStr> *R1,*R2;
		HashTable<string, AList<string, CompStr>*, StringKeyHash> *t_table;
		AList<string, CompStr> *temp_List = new AList<string, CompStr>(150);/*除非参与disjunctive的某一个关键字对应的文档大于这个数，否则不需要修改这个值*/
	
		if (p_GMM->get(k1, R1)){
			//HashTable<string, HashTable<string, AList<string, CompStr>*, StringKeyHash>*, StringKeyHash > *p_E
			p_EMM->get(k1,t_table);
			if (t_table->get(k2, R2)){
				/*k1,k2有交集*/
				/*fix a bug:如果修改指针指向元素的内容，那么他对应的hashtable中的内容也会被修改*/
				temp_List->clear();
				for (R1->setStart(); R1->getValue(temp); R1->next())
					temp_List->append(temp);
				SetRemove(temp_List,R2);
				for (temp_List->setStart(); temp_List->getValue(temp); temp_List->next())
					Result->append(temp);
				p_GMM->get(k2, R2);
				for (R2->setStart(); R2->getValue(temp); R2->next())
					Result->append(temp);
			}//end if(t_table->get(k2, R2))
			else{
				/*如果k1,k2没交集*/
				if (p_GMM->get(k2, R2)){
					for (R1->setStart(); R1->getValue(temp); R1->next())
						Result->append(temp);
					for (R2->setStart(); R2->getValue(temp); R2->next())
						Result->append(temp);

				}//end if(p_GGM->get(k2, R2))
				else{
					for (R1->setStart(); R1->getValue(temp); R1->next())
						Result->append(temp);
				}
				
			}
		}
		else{
			/*如果第一个关键词对应的文档集合为空,不过在本次实验中不会发生*/
			if (p_GMM->get(k2, R1)){
				for (R1->setStart(); R1->getValue(temp); R1->next()){
					Result->append(temp);
				}
			}
		}
	}
	


private:
	HashTable<string, AList<string, CompStr>*, StringKeyHash> *p_GMM;
	HashTable<string, HashTable<string, AList<string, CompStr>*, StringKeyHash>*, StringKeyHash > *p_EMM;
};

#endif