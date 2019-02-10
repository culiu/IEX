#ifndef _ALIST_
#define _ALIST_
#include<cstdlib>
#include <iostream>
template <class Elem, class Comp> // Array-based list implementation
class AList  {
private:
	int maxSize;        // Maximum size of list
	//int listSize;       // Actual number of elements in list
	int fence;          // Position of fence
	bool sorted;
	int findpivot(Elem A[], int i, int j)
	{
		return (i + j) / 2;
	}

	/*
	int partition(Elem A[], int l, int r, Elem& pivot) {
	do {             // Move the bounds inward until they meet
	while (A[++l]< pivot);     // Move l right and
	while ((r != 0) && (A[--r]> pivot)); // r left
	swap(A, l, r);              // Swap out-of-place values
	} while (l < r);              // Stop when they cross
	swap(A, l, r);                // Reverse last, wasted swap
	return l;      // Return first position in right partition
	}
	*/


	int partition(Elem A[], int l, int r, Elem& pivot) {

		do {             // Move the bounds inward until they meet
			while (Comp::comp(A[++l], pivot) < 0);
			while ((r != 0) && Comp::comp(A[--r], pivot) > 0);
			swap(A, l, r);              // Swap out-of-place values
		} while (l < r);              // Stop when they cross
		swap(A, l, r);                // Reverse last, wasted swap
		return l;      // Return first position in right partition
	}

	void qsort(Elem A[], int i, int j) { // Quicksort
		if (j <= i) return; // Don't sort 0 or 1 Elem

		int pivotindex = findpivot(A, i, j);
		swap(A, pivotindex, j);    // Put pivot at end
		// k will be the first position in the right subarray
		int k = partition(A, i - 1, j, A[j]);
		//cout<<k<<endl;
		swap(A, k, j);             // Put pivot in place
		qsort(A, i, k - 1);
		qsort(A, k + 1, j);
	}

	//降序排列
	int partitionDesc(Elem A[], int l, int r, Elem& pivot) {

		do {             // Move the bounds inward until they meet
			while (Comp::comp(A[++l], pivot) > 0);
			while ((r != 0) && Comp::comp(A[--r], pivot) < 0);
			swap(A, l, r);              // Swap out-of-place values
		} while (l < r);              // Stop when they cross
		swap(A, l, r);                // Reverse last, wasted swap
		return l;      // Return first position in right partition
	}
	void qsortDesc(Elem A[], int i, int j) { // Quicksort
		if (j <= i) return; // Don't sort 0 or 1 Elem

		int pivotindex = findpivot(A, i, j);
		swap(A, pivotindex, j);    // Put pivot at end
		// k will be the 0 position in the right subarray
		int k = partitionDesc(A, i - 1, j, A[j]);
		//cout<<k<<endl;
		swap(A, k, j);             // Put pivot in place
		qsortDesc(A, i, k - 1);
		qsortDesc(A, k + 1, j);
	}
	void swap(Elem A[], int k, int j)
	{
		Elem temp = A[k];
		A[k] = A[j];
		A[j] = temp;
	}

public:
	Elem* listArray;    // Array holding list elements
	int listSize;       // Actual number of elements in list
	AList() { // Constructor
		listSize = 0;
		fence = 0;
		listArray = new Elem[maxSize];
		sorted = false;
	}
	AList(int size) { // Constructor
		maxSize = size;
		listSize = 0;
		fence = 0;
		listArray = new Elem[maxSize];
		sorted = false;
	}



	~AList() {
		if (listArray != NULL)
			delete[] listArray;
		listArray = NULL;
	} // Destructor
	void clear() {
		//delete [] listArray;
		listSize = 0;
		fence = 0;
		//listArray = new Elem[maxSize];
	}

	//list销毁方法
	void destory()
	{
		listSize = fence = 0;
		if (listArray != NULL)
			delete[] listArray;
		listArray = NULL;
	}

	int getListSize()
	{
		return listSize;
	}
	int getListMaxSize()
	{
		return maxSize;
	}
	/*
	void setListSize(int size)
	{
	listSize=size;
	}
	*/
	int getCurrentPos()
	{
		return fence;
	}

	bool ReplaceValue(int pos, Elem& e)
	{
		if (pos<0 || pos>listSize - 1) return false;
		listArray[pos] = e;
		return true;
	}
	bool insert(const Elem&);
	bool append(const Elem&);
	bool insertPos(int pos, const Elem&);
	bool deleteElem(Elem&);
	bool remove(Elem&);
	void setStart() { fence = 0; }
	void setEnd()   { fence = listSize; }
	bool prev()
	{
		if (fence == 0)
			return false;
		fence--;
		return true;
	}
	void next()     { if (fence <= listSize) fence++; }
	int leftLength() const  { return fence; }
	int rightLength() const { return listSize - fence; }
	bool setPos(int pos) {
		if ((pos >= 0) && (pos <= listSize)) fence = pos;
		return (pos >= 0) && (pos <= listSize);
	}
	bool getValue(Elem& it) const {
		if (rightLength() == 0) return false;
		else { it = listArray[fence]; return true; }
	}

	//设置list pos位置的值为it
	bool setPosValue(int pos, Elem& it) const {
		if (pos<0 || pos>listSize - 1)
			return false;
		listArray[pos] = it;
		return true;
	}

	bool isEnd(){
		return fence == listSize;
	}

	bool isExist(Elem v)
	{
		Elem data;
		for (setStart(); getValue(data); next())
		if (data == v) return true;
		return false;
	}
	//by rui li

	void print() const {
		int temp = 0;
		std::cout << "< ";
		while (temp < fence) std::cout << listArray[temp++] << " ";
		std::cout << "| ";
		while (temp<listSize) std::cout << listArray[temp++] << " ";
		std::cout << ">\n";
	}

	bool isSorted()
	{
		return sorted;
	}
	void sort(bool isDesc = false) {
		if (isDesc)
		{
			qsortDesc(listArray, 0, listSize - 1);
		}
		else
		{
			qsort(listArray, 0, listSize - 1);
		}
		sorted = true;
	}

	int Position(Elem a)
	{
		if (!sorted)
			return -1;
		int left = 0;
		int right = listSize - 1;
		int mid;
		while (left<right)
		{
			mid = (left + right) / 2;
			if (listArray[mid] == a)
				return mid;
			if (listArray[mid]<a)
				left = mid + 1;
			else
				right = mid - 1;
		}
		return left;
	}

	//得到某一元素的下标
	int getPosition(Elem a)
	{
		//if (!sorted)
		//	return -1; 暂时先去掉---jinkai
		if (listSize == 0)
			return -1;
		int left = 0;
		int right = listSize - 1;
		int mid;
		//while (left<right)
		while (left <= right) //change by jinkai
		{
			mid = (left + right) / 2;
			if (listArray[mid] == a)
				return mid;
			if (listArray[mid]<a)
				left = mid + 1;
			else
				right = mid - 1;
		}
		//return left;
		return -1;//while can not find the element return -1;
	}

	bool removeLast(Elem & it)
	{
		if (listSize == 0) return false;
		it = listArray[--listSize];
		return true;

	}

	void  removeLast()
	{
		if (listSize > 0)
			--listSize;
	}

	bool getFirstValue(Elem & it)
	{
		if (listSize == 0) return false;
		it = listArray[0];
		return true;
	}
	// this function is designed for generating query
	bool removeFirst(Elem & it)
	{
		if (listSize == 0) return false;
		it = listArray[0];
		if (--listSize == 1)
			listArray[0] = listArray[1];
		return true;
	}

	//this function is designed for generating query
	bool removeFirst()
	{
		if (listSize == 0) return false;
		if (--listSize == 1)
			listArray[0] = listArray[1];
		return true;
	}

	bool getLastValue(Elem & it)
	{
		if (listSize == 0) return false;
		it = listArray[listSize - 1];
		return true;

	}



	//得到list对应pos位置的数据内容
	Elem getPosValue(int pos)
	{
		if (pos >= 0 && pos < listSize)
		{
			return listArray[pos];
		}

		//return NULL;
		/*change by liuying*/
		Elem *e = NULL;
		return *e;
	}

	void swampValue(int pos_1, int pos_2)
	{
		if (pos_1 == pos_2) return;
		Elem temp = listArray[pos_1];
		listArray[pos_1] = listArray[pos_2];
		listArray[pos_2] = temp;

	}

	bool getPosValue(int pos, Elem & it)
	{
		if (pos >= 0 && pos < listSize)
		{
			it = listArray[pos];
			return true;
		}
		return false;
	}



};

template <class Elem, class Comp> // Insert at front of right partition
bool AList<Elem, Comp>::insertPos(int pos, const Elem & item) {
	if (listSize == maxSize) return false; // List is full
	for (int i = listSize; i>pos; i--)      // Shift Elems up
		listArray[i] = listArray[i - 1];       //   to make room
	listArray[pos] = item;
	listSize++;                       // Increment list size
	return true;
}

template <class Elem, class Comp> // Insert at front of right partition
bool AList<Elem, Comp>::insert(const Elem & item) {
	if (listSize == maxSize) return false; // List is full
	for (int i = listSize; i>fence; i--)      // Shift Elems up
		listArray[i] = listArray[i - 1];       //   to make room
	listArray[fence] = item;
	listSize++;                       // Increment list size
	return true;
}


template <class Elem, class Comp> // Append Elem to end of the list
bool AList<Elem, Comp>::append(const Elem & item) {
	if (listSize == maxSize)
		return false;
	listArray[listSize++] = item;
	return true;
}

// Remove and return first Elem in right partition
template <class Elem, class Comp>
bool AList<Elem, Comp>::remove(Elem& it) {
	if (rightLength() == 0) return false; // Nothing in right
	it = listArray[fence];                // Copy removed Elem
	for (int i = fence; i<listSize - 1; i++)   // Shift them down
		listArray[i] = listArray[i + 1];
	listSize--;                           // Decrement size
	return true;
}

template <class Elem, class Comp>
bool AList<Elem, Comp>::deleteElem(Elem& it) {
	Elem data;
	for (setStart(); getValue(data); next()){
		if (data == it){
			for (int i = fence; i<listSize - 1; i++)   // Shift them down
				listArray[i] = listArray[i + 1];
			listSize--;                           // Decrement size
			return true;
		}
	}
	return false;
}
#endif
