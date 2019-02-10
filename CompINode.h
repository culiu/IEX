#ifndef _COMPINODE_
#define _COMPINODE_

#include "InvertedNode.h"
class CompINode{
public:
	/*TODO 此方法暂仅通过比较节点中包含的docsize来进行比较*/
	static int comp(InvertedNode value1, InvertedNode value2){
		return value1.docsize - value2.docsize;
	}
};

#endif