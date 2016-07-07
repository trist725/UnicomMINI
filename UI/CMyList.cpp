/**********************************************************************

*文件名：
		CMyList.cpp
*文件描述：
		CMyList类的封装
*创建者：
		黄彬

*创建时间：
2011-6-8

******************************************************************/


#include <stdlib.h>
#include <string.h>
#include "CMyList.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyList::CMyList()
{
	pHead=(PNode)malloc(LEN_NODE);
	if (pHead==NULL)
	{
		//cout<<"创建失败"<<endl;
	}
	else
	{
		pHead->pData=NULL;
		pHead->next=NULL;
		pCurNode = pHead;
		//cout<<"创建成功"<<endl;
	}


}

CMyList::~CMyList()
{
	PNode p1;
	int free_count=0;

	if (pHead==NULL)
	{
		//cout<<"释放失败"<<endl;
	}
	else
	{
		p1=pHead->next;
		while(p1!=NULL)
		{
			pHead->next=p1->next;
			free(p1);
			p1=pHead->next;
			free_count++;
		}
		free(pHead);
		//cout<<"释放成功"<<free_count<<"个节点"<<endl;
	}

}

/*
功能：
将数据保存到链表的尾部
参数：
Data 需要保存的缓冲区
返回值：

*/
void CMyList::addNode(void *pData)
{
	PNode p1,p2;

	if (pHead==NULL)
	{
		return;
	}

	p2= (PNode) malloc(LEN_NODE);
	memset(p2,0,LEN_NODE);
	if (p2==NULL)
	{
		return;
	}

	p2->pData=pData;
	p2->next=NULL;
	if (pHead->next==NULL)
	{
		pHead->next=p2;
		pCurNode = p2;
		return;
	}
	p1=pHead->next;
	while(p1->next!=NULL)
	{
		p1=p1->next;
	}
	p1->next=p2;
	pCurNode = p2;

}


void CMyList::delNode(int nIndex)
{
	PNode p1,p2;
	int i=1;

	if (pHead==NULL)
	{
		return;
	}
	p1=pHead;
	for(i=0;i<nIndex;i++)
	{
		if (p1->next==NULL)
		{
			if (i<nIndex)
			{
				return;
			}
			else if (i==nIndex)
			{
				break;
			}

		}

		p2=p1;
		p1=p1->next;
	}
	p2->next=p1->next;
	free(p1);
}


/*
功能：
让当前指针指向链表头
参数：
返回值：
*/
void CMyList::begin()
{
	pCurNode =pHead;
}

/*
功能：
从当前指针开始是否还有下一个节点
参数：
返回值：true 存在 flase 不存在
*/
bool CMyList::hasNext()
{
	if (pCurNode->next !=NULL)
	{
		return true;
	}
	else return false;
}

/*
功能：
让当前指针指向下一个节点
参数：
返回值：指向下一个节点
*/

void * CMyList::Next()
{
	if (pCurNode->next !=NULL)
	{
		pCurNode =pCurNode->next;
		return pCurNode->pData;
	}
	else return NULL;
}



/*
功能：
返回链表的节点的个数
参数：
返回值：
 节点个数
*/

int CMyList::count()
{
	PNode p1;
	int count=0;

	if (pHead==NULL)
	{
		return 0;
	}

	p1=pHead->next;
	while(p1!=NULL)
	{
		p1=p1->next;
		count++;
	}
	return count;
}



/*
功能：
返回指定位置的节点
参数：
Index 节点在链表中的下标

返回值：
 下标对应的节点
*/

PNode CMyList::get(const int index)
{
	PNode p1;
	int i=0;

	if (pHead==NULL)
	{
		return NULL;
	}

	if (index <= 0)
	{
		return NULL;
	}

	p1=pHead->next;
	for(i=1;i<index;i++)
	{
		if (p1->next==NULL)
		{
			return NULL;
		}
		p1=p1->next;
	}
	return p1;

}


/*
功能：
返回指定下标节点的内容
参数：
Index 节点在链表中的下标

返回值：
 下标对应的节点
*/
void * CMyList::operator[] (int nIndex)
{

	pCurNode=get(nIndex);

	if (pCurNode != NULL)
	{
		return pCurNode->pData;
	}
	return NULL;
}


/*
功能：
返回当前节点的内容
参数：
返回值：
*/

void * CMyList::ReturnNow ()
{
	if (pCurNode != NULL)
	{
		return pCurNode->pData;
	}
	return NULL;
}


void CMyList::delAll()
{
    PNode p1;
	//int free_count=0;

	if (pHead==NULL)
	{
		//cout<<"释放失败"<<endl;
	}
	else
	{
		p1=pHead->next;
		while(p1!=NULL)
		{
			pHead->next=p1->next;
			free(p1);
			p1=pHead->next;
			//free_count++;
		}
		//cout<<"释放成功"<<free_count<<"个节点"<<endl;
	}
}

