/**
ʵ�鸨����
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
	/*param1��query condition
	  param2��result
	  ����ؼ��ֵ�conjunctive
	*/
	void conQuery(AList<string, CompStr> *Query,AList<string, CompStr> *Result){
		/*ȡ����һ���ؼ���ȥGMM�в�ѯ���*/
		string curr;/*��ǰ�ؼ���*/
		string f_n;
		AList<string, CompStr> *p_temp;/*��ǰ�ؼ��ֶ�Ӧ���ĵ�*/
		Query->setStart();
		Query->getValue(curr);/*��һ���ؼ���*/

		/*���������ֱ��ʹ��Resultָ��ָ��f_list����޸�hashtable�е����ݣ�����ֻ����һ�����Ĳ���Ԫ�ص�Result��*/
		AList<string, CompStr>* f_list;
		p_GMM->get(curr, f_list);
		/*�ѵ�һ���ؼ��ֶ�Ӧ���ĵ���ӽ�Result��*/
		for (f_list->setStart(); f_list->getValue(f_n); f_list->next())
			Result->append(f_n);
		Query->next();
		/*�ӵڶ����ؼ��ֵ����һ���ؼ��֣��ж϶�Ӧ��GMM�еļ���Ԫ���Ƿ���Result�У����������ȥ��*/
		if (Result->getListSize() != 0){
			while (Query->getValue(curr)){
				p_GMM->get(curr,p_temp);
				SetConjunction(Result,p_temp);
				Query->next();
			}
		}
	}
	/*�������S1�У�Ҳ��S2�е�Ԫ��*/
	void SetConjunction(AList<string, CompStr>*p_S1, AList<string, CompStr>*p_S2){
		/*���ڴ��ݽ�����Ԫ�ض�������õģ����Բ���Ҫ����*/
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
			/*��ǰԪ����֮���Ԫ�ض���Ҫɾ��*/
			p_S1->remove(temp);
	}
	/*��S1��ɾ��ͬʱ������S2�е�Ԫ��*/
	void SetRemove(AList<string, CompStr>*p_S1, AList<string, CompStr>*p_S2){
		/*������Ҫ����*/
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


	/*�������ݲ�ͨ�ö���ؼ��֣���ֻ����2���ؼ���*/
	void disQuery_t(AList<string, CompStr> *Query, AList<string, CompStr>* Result){
		string k1 = Query->getPosValue(0);
		string k2 = Query->getPosValue(1);
		string temp;
		AList<string, CompStr> *R1,*R2;
		HashTable<string, AList<string, CompStr>*, StringKeyHash> *t_table;
		AList<string, CompStr> *temp_List = new AList<string, CompStr>(150);/*���ǲ���disjunctive��ĳһ���ؼ��ֶ�Ӧ���ĵ������������������Ҫ�޸����ֵ*/
	
		if (p_GMM->get(k1, R1)){
			//HashTable<string, HashTable<string, AList<string, CompStr>*, StringKeyHash>*, StringKeyHash > *p_E
			p_EMM->get(k1,t_table);
			if (t_table->get(k2, R2)){
				/*k1,k2�н���*/
				/*fix a bug:����޸�ָ��ָ��Ԫ�ص����ݣ���ô����Ӧ��hashtable�е�����Ҳ�ᱻ�޸�*/
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
				/*���k1,k2û����*/
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
			/*�����һ���ؼ��ʶ�Ӧ���ĵ�����Ϊ��,�����ڱ���ʵ���в��ᷢ��*/
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