#ifndef _COMPNODE_
#define _COMPNODE_

#include"Node.h"

class CompNode{
public:
	/*TODO ��Node��Ҫ����ĳ�ַ�ʽ����ʱ��������*/
	static int comp(Node value1, Node value2){
		return value1.docsize - value2.docsize;
	}
};

#endif