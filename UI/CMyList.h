#ifndef CMYLIST_H
#define CMYLIST_H

#define LEN_NODE sizeof(NODE)

typedef struct ListNode
{
	struct ListNode *next;
	void *pData;
}NODE;

typedef NODE * PNode;

class CMyList
{
public:
	CMyList();
	virtual ~CMyList();
	void addNode(void *pData);  //添加节点
	void delNode(int nIndex);   //删除节点
	void begin();			//让当前指针指向链表头
	bool hasNext();			//从当前指针开始是否还有下一个节点
	void * Next();		//让当前指针指向下一个节点
	void * operator[] (int nIndex); //获得指定下标的节点的内容
	void * ReturnNow (); //获得当前节点的内容
	int count();    //获得总个数
	void delAll();      //删除头以外的数据
protected:

	PNode get(const int index); //获取指定位置的数据
private:
	PNode pHead;
	PNode pCurNode;

};
#endif // CMYLIST_H
