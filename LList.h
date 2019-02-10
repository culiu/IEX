// This is the file to include in your code if you want access to the
// complete LList template class

// First, get the declaration for the base list class

//#include "list.h"
#ifndef _LLIST_
#define _LLIST_
#include "Link.h"
#include <iostream>
using namespace std;
// Linked list implementation
template <class Elem>
class LList{

public:
	Link<Elem> * head;       // Pointer to list header
	Link<Elem> * tail;       // Pointer to last Elem in list 
	Link<Elem> * fence;      // Last element on left side
	Link<Elem> * memPos;
	// Size of right partition
	int listSize;

	void init() {           // Intialization routine
		fence = tail = head = new Link<Elem>;
		listSize = 0;
	}
	void removeall() {   // Return link nodes to free store
		while (head != NULL) {
			fence = head;
			head = head->next;
			delete fence;
		}
	}

	LList(int size) { init(); }
	LList(){ init(); }
	~LList() { removeall(); }  // Destructor
	void clear() { removeall(); init(); }
	bool insert(Elem&);
	bool append(Elem&);
	bool remove(Elem&);
	bool removeElem(Elem&);
	Link<Elem> * removeHeadLink()
	{
		if (head->next == NULL) return NULL;
		fence = head->next;
		head->next = fence->next;
		listSize--;
		return fence;
	}
	void setStart()
	{
		fence = head;
	}
	void setEnd()
	{
		fence = tail;
	}
	void prev();
	void next() {
		if (fence != tail)
			fence = fence->next;

	}

	int getListSize() const { return listSize; }

	bool setPos(int pos);
	bool getValue(Elem &it) const {//7月10日 改一个&符号
		if (fence->next == NULL) return false;
		it = fence->next->element;
		return true;
	}
	void remberPos()
	{
		memPos = fence;
	}
	void setMemPos()
	{
		fence = memPos;
	}

	Link<Elem> * removeNode()
	{
		if (fence->next == NULL) return NULL;
		Link<Elem> * pTemp = fence->next;
		fence->next = pTemp->next;
		listSize--;
		return  pTemp;
	}

	bool removeNode(Link<Elem> * & p)
	{
		if (fence->next == NULL) return false;
		p = fence->next;
		fence->next = p->next;
		listSize--;
		return  true;
	}

	bool appendNode(Link<Elem>* p)
	{
		if (p == NULL) return false;
		p->next = NULL;
		tail->next = p;
		tail = p;
		listSize++;
		return true;
	}



	bool insertNode(Link<Elem> * p)
	{
		if (p == NULL) return false;
		p->next = fence->next;
		fence->next = p;
		listSize++;
		if (tail == fence) tail = p;
		return true;
	}

	void insertFirstPlace(Elem & item)
	{
		head->next = new Link<Elem>(item, head->next);
		if (tail == head) tail = head->next;
		listSize++;
	}

	bool removeHeadValue(Elem & item)
	{
		if (head->next == NULL) return false;
		Link<Elem> * p_temp = head->next;
		item = p_temp->element;
		head->next = p_temp->next;
		if (fence == p_temp) fence = head;
		if (tail == p_temp) tail = head;
		delete p_temp;
		listSize--;
		return true;
	}

};
template <class Elem>
bool LList<Elem>::insert(Elem &item) {
	fence->next = new Link<Elem>(item, fence->next);
	if (tail == fence) tail = fence->next;  // New tail
	listSize++;
	return true;
}

// Append Elem to end of the list
template <class Elem>
bool LList<Elem>::append(Elem &item) {
	tail->next = new Link<Elem>(item, NULL);
	if (tail->next == NULL)
		return false;
	tail = tail->next;
	listSize++;
	return true;
}

// Remove and return first Elem in right partition
template <class Elem>
bool LList<Elem>::remove(Elem &it) {
	if (fence->next == NULL) return false; // Empty right
	it = fence->next->element;       // Remember value
	Link<Elem> * ltemp = fence->next; // Remember link node
	fence->next = ltemp->next;       // Remove from list
	if (tail == ltemp) tail = fence; // Reset tail
	delete ltemp;                    // Reclaim space
	ltemp = NULL;
	listSize--;
	return true;
}

template <class Elem>
void LList<Elem>::prev() {
	Link<Elem>* temp = head;
	if (fence == head) return; // No previous Elem
	while (temp->next != fence) temp = temp->next;
	fence = temp;
}

// Set the size of left partition to pos
template <class Elem>
bool LList<Elem>::setPos(int pos) {
	if ((pos < 0) || (pos > listSize)) return false;
	fence = head;
	for (int i = 0; i<pos; i++) fence = fence->next;
	return true;
}

#endif
