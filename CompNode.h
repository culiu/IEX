#ifndef _COMPNODE_
#define _COMPNODE_

#include"Node.h"

class CompNode{
public:
	/*TODO 当Node需要按照某种方式排序时，改这里*/
	static int comp(Node value1, Node value2){
		return value1.docsize - value2.docsize;
	}
};

#endif