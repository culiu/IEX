
// Singly-linked list node

#ifndef _LINK_
#define _LINK_
#include <string>
template <class Elem>
class Link {
public:
	Elem element;      // Value for this node
	Link *next;        // Pointer to next node in list
	//Link(TreeNode* & elemval, Link* nextval = NULL)
	//{
	//	element = elemval;  next = nextval;
	//}
	Link(Elem eleval, Link* nextval = NULL){
		element = eleval;
		next = nextval;
	}

	Link(Link* nextval = NULL) { next = nextval; }
};
# endif