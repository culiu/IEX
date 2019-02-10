#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include "HashNode.h"
#include "prime.h"
//Number of times to iterate through the secondary hash function before giving up
#define MAX_C 100 //����ͻ����
//#include "sha_256.h"
//#include "StringKeyHash.h"
//*****************************************************************
// QuadraticHash����̽��hash
//*****************************************************************




template <typename K, typename V, class KeyHash>
class HashTable
{
private:

	//A Table for storing the values
	//Bucket<HashNode>** m_table;
	HashNode<K, V>** m_table;

	// maxSize is the size of the Hash Table array.
	int maxSize;

	/*size is item count in HashTable*/
	int size;

	/*in this all inserts collision times*/
	int all_collision_times;

	//float loadFactor;

	int *co_index;
	//const static int co_index[MAX_C] = { 0, 1, 3, 6, 10, 15, 21, 28, 36, 45, 55, 66, 78, 91,
	//105, 120, 136, 153, 171, 190, 210, 231, 253, 276, 300, 325, 351, 378, 406, 435,
	//465, 496, 528, 561, 595, 630, 666, 703, 741, 780, 820, 861, 903, 946, 990, 1035, 1081,
	//1128, 1176, 1225, 1275, 1326, 1378, 1431, 1485, 1540, 1596, 1653, 1711, 1770, 1830, 1891,
	//1953, 2016, 2080, 2145, 2211, 2278, 2346, 2415, 2485, 2556, 2628, 2701, 2775, 2850, 2926,
	//3003, 3081, 3160, 3240, 3321, 3403, 3486, 3570, 3655, 3741, 3828, 3916, 4005, 4095, 4186,
	//4278, 4371, 4465, 4560, 4656, 4753, 4851, 4950 };

	/*����*/
	int *used_location;
	/*userd space number  service to used_location*/
	int fence;


	/*return ��n��ĵ�һ������*/
	int smallestPrime(int n)const{
		int a, b;
		FindPrime(n, a, b);
		return b;
	}
	/*swap A[i],A[j]*/
	void iSwap(int A[], int i, int j){
		//if (i == j) return; ����ҵ��֤��i != j;
		/*A[i] = A[i] ^ A[j];
		A[j] = A[i] ^ A[j];
		A[i] = A[i] ^ A[j];*/
		int temp = A[i];
		A[i] = A[j];
		A[j] = temp;
	}
public:

	// Constructs the empty Hash Table object.
	// Array maxSize is set to 13 by default.
	/*tablemaxSize = elementSize / loadFactor */
	HashTable(int tablemaxSize = 13){
		{
			//if (tablemaxSize <= 0) tablemaxSize = 13;
			maxSize = smallestPrime(tablemaxSize);
			size = 0;
			all_collision_times = 0;
			//loadFactor = iniloadFactor;
			m_table = new HashNode<K, V>*[maxSize];//table�ĳ�ʼ��
			for (int i = 0; i < maxSize; i++){
				m_table[i] = NULL;
			}
			/*����̽��λ�ô洢*/
			co_index = new int[MAX_C];
			for (int i = 0; i < MAX_C; i++)
				co_index[i] = (int)(0.5*i + 0.5*i*i);
			/*����used_Location*/
			used_location = new int[maxSize];
			for (int i = 0; i < maxSize; i++){
				used_location[i] = i;
			}
			fence = 0;
		}
	}

	void deletCell(int i)
	{
		delete m_table[i];
		m_table[i] = NULL;//change
		/*����Ҫ���� userd_location[n] = i,�ҵ�nȻ���滻����,��Ϊ��ȷ���������Ǻ�ʱ�ǳ����֣������粻Ҫ��Щ�ռ�*/
		//int i_index = 0;
		///*for (int _index = 0; _index < maxSize; _index++){*/
		//for (int _index = maxSize - 1 - fence; _index < maxSize; _index++){
		//	if (used_location[_index] = i){
		//		i_index = _index;
		//	}
		//}
		//iSwap(used_location, i_index, maxSize - 1 - fence);/*used_location[maxSize - 1 -fence] �ǵ�һ����ʹ�õĿռ�*/
		//fence--;
		size--;
	}
	// De-allocates all memory used for the Hash Table.
	~HashTable(){
		if (m_table != NULL){
			for (int i = 0; i < maxSize; i++){
				//m_table[i] = NULL;
				delete m_table[i];
			}
			delete[] m_table;
		}
		/*delete colision index*/
		delete[] co_index;
		/*delete userd location*/
		delete[] used_location;
		maxSize = 0;
		size = 0;
		all_collision_times = 0;
		fence = 0;
	}
	/*����*/
	int insert_opt(const K &key, const V &value){
		/*randomһ���±� , ��used_location[next_index]����Ԫ��*/
		int next_index = rand() % (maxSize - fence);//[0,maxSize-fence];
		int insert_location = used_location[next_index];
		m_table[insert_location] = new HashNode<K, V>(key, value);
		/*�˴����ʹ�����õķ�ʽ������Ҫһ������ı�������next_index*/
		iSwap(used_location, next_index, maxSize - 1 - fence);/*�������ַ���±���Ϊ����ʹ�á�*/
		fence++;
		size++;
		return insert_location;//����ط����ֵ�Ѿ�����������,�������������.
	}

	int insert(const K &key, const V &value){
		int hash_ = KeyHash::hash(key);
		int _index = 0;
		for (int i = 0; i < MAX_C; i++){
			_index = abs((hash_ + co_index[i]) % maxSize);
			if (m_table[_index] == NULL){
				//cout << "inner _index = " << _index << endl;
				m_table[_index] = new HashNode<K, V>(key, value);
				//if (m_table[_index] == NULL)
				//cout << "Insert HashTable Error!" << endl;
				/*�����������ҵ�ֵΪ_index��userd_loc�е��±�λ�ã����������һ�����н���*/
				//if (_index>maxSize -1 - fence){
				//	cout << "���������........." << endl;
				//}
				int loc_index = _index;
				while (used_location[loc_index] != _index)
					loc_index = used_location[loc_index];
				size++;
				iSwap(used_location, loc_index, maxSize - 1 - fence);
				fence++;
				//all_collision_times += i;/*��ͻ��������*/
				return _index;
			}
			if (m_table[_index]->getKey() == key){
				m_table[_index]->setValue(value);
				return _index;
			}
		}
		cout << "warning!!!!!!insert error!" << "current key = " << key;
		return -1;
	}

	//int insert(const K &key, const V &value){
	//	int hash_ = KeyHash::hash(key);
	//	int _index = 0;
	//	for (int i = 0; i < MAX_C; i++){
	//		_index = abs((hash_ + co_index[i]) % maxSize);
	//		if (m_table[_index] == NULL){
	//			m_table[_index] = new HashNode<K, V>(key, value);
	//			//if (m_table[_index] == NULL)
	//				//cout << "Insert HashTable Error!" << endl;
	//			size++;
	//			//all_collision_times += i;/*��ͻ��������*/
	//			return _index;
	//		}
	//		/*
	//			else if (m_table[_index]->getKey() == key){
	//			cout << "�����ظ���key.����value" << endl;
	//			m_table[_index]->setValue(value);
	//			return _index;
	//		}
	//		*/
	//	}
	//	cout << "warning!!!!!!insert error!";
	//	return -1;//������ﷵ�ص���false,�������Ҫ������,����Ҫ�жϵ�ǰԪ�ز�����Ƿ������ӹ���
	//}


	/*����key����Ԫ�أ����һ�ȡ��Ӧ�ĳ�ͻ����*/
	bool get_c(const K key, V &value, int &c_time){
		int hash_ = KeyHash::hash(key);
		int _index = 0;
		for (int i = 0; i < MAX_C; i++){
			_index = abs((hash_ + co_index[i]) % maxSize);
			if (m_table[_index] == NULL){
				c_time = -1;
				cout << "elems not found" << endl;
				return false;
			}
			if (m_table[_index]->getKey() == key){
				value = m_table[_index]->getValue();
				c_time = i;
				return true;
			}
		}
		/*δ�ҵ���Ԫ��*/
		cout << "elems not found" << endl;
		c_time = -1;
		return false;
	}

	// Returns an HashNode from the Hash Table by key.
	// If the item isn't found, return Flase;

	bool get(int Loc, V &value)
	{
		if (Loc <0) return false;
		/*DEBUG*/
		if (m_table[Loc] == NULL){
			cout << "hashtables Loc = " << Loc << "value = NULL error" << endl;
		}
		/*DEBUG*/
		value = m_table[Loc]->getValue();
		return true;
	}

	bool get(const K key,V &value){
		int hash_ = KeyHash::hash(key);
		int _index = 0;
		for (int i = 0; i < MAX_C; i++){
			_index = abs((hash_ + co_index[i]) % maxSize);
			if (m_table[_index] == NULL)
				return false;
			if (m_table[_index]->getKey() == key){
				value = m_table[_index]->getValue();
				return true;
			}
		}
		/*δ�ҵ���Ԫ��*/
		cout << "elems not found" << endl;
		return false;
	}

	/*��֪ ��ͻ���� c_time �����õķ���*/
	void get_opt(const K key, V &value, int c_time){
		int hash = KeyHash::hash(key);//��һ�����Ҫ����Ȼ���벻ͨ��.
		int i = abs((hash + co_index[c_time]) % maxSize);
		value = m_table[i]->getValue();
		//return true;
	}

	void printAll(ostream &fout, const K key1, const K key2){
		for (int i = 0; i < maxSize; i++){
			if (m_table[i] != NULL){
				if (m_table[i]->getKey().find(key1) != string::npos || m_table[i]->getKey().find(key2) != string::npos)
					fout << i << "   :key = " << m_table[i]->getKey() << " value = " << m_table[i]->getValue() << endl;
			}
		}
	}



	// Returns the number of locations in the Hash Table.
	int getmaxSize(){
		return maxSize;
	}

	// Returns the number of Items in the Hash Table.
	int getSize(){
		return size;
	}
	/*all collision occur frequency in this HashTable*/
	int getCollistionTimes(){
		return all_collision_times;
	}
	/*current load ratio*/
	float getCurLoadFactor() const{
		float result = (float)size / (float)maxSize;
		return result;
	}
};

#endif