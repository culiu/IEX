#ifndef _COMPINODE_
#define _COMPINODE_

#include "InvertedNode.h"
class CompINode{
public:
	/*TODO �˷����ݽ�ͨ���ȽϽڵ��а�����docsize�����бȽ�*/
	static int comp(InvertedNode value1, InvertedNode value2){
		return value1.docsize - value2.docsize;
	}
};

#endif