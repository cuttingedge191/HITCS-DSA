/*
* ��ҵ3 ���νṹ����Ӧ��
* ѧ�ţ�1190200526
* �����������
*/

#include <iostream>
#include <cmath>
#include <malloc.h>
using namespace std;
#define elemtype char
#define maxlength 200 //ջ�����е�Ԫ�ظ�������
#define maxheight 8   //��߶������߶�

//�������Ķ�������洢�ṹ
struct BT_Node
{
	BT_Node* lchild;  //����
	elemtype data;    //����
	BT_Node* rchild;  //�Һ���
};
typedef BT_Node* BTree;

//�ǵݹ����ʹ�õģ�˳��ջ�ṹ
struct stack
{
	BTree elements[maxlength];
	int visit[maxlength] = { 0 };     //����
	int top;
};
typedef struct stack Stack;

//�������ʹ�õĶ���
struct queue
{
	int front;
	int rear;
	BTree elements[maxlength];
};
typedef struct queue Queue;

//�����洢�ṹ������������
void S_MakeNull(Stack& S);
int S_Empty(Stack S);
BTree S_Top(Stack S);
void S_Pop(Stack& S);
void S_Push(BTree x, Stack& S);
int Q_Addone(int i);
void Q_MakeNull(Queue& Q);
int Q_Empty(Queue Q);
int Q_Size(Queue Q);
BTree Q_Front(Queue Q);
void Q_EnQueue(BTree BT, Queue& Q);
void Q_DeQueue(Queue& Q);
//���ܺ�������
void CreateBT(BTree& BT);                                  //����ݹ齨������������
void PrintBT(BTree BT);                                    //��Ļ��ʾ����������
int IsEmpty(BTree BT);                                     //�������ж��Ƿ�Ϊ��
void PreOrder_R(BTree BT);                                 //��������������������ݹ飩
void PreOrder_NR(BTree BT);                                //��������������������ǵݹ�)
void InOrder_R(BTree BT);                                  //��������������������ݹ飩
void InOrder_NR(BTree BT);                                 //��������������������ǵݹ飩
void PostOrder_R(BTree BT);                                //��������������������ݹ飩
void PostOrder_NR(BTree BT);                               //��������������������ǵݹ飩
void LeverOrder(BTree BT);                                 //�����������������
bool IsComplete(BTree BT);                                 //�������ж��Ƿ�Ϊ��ȫ������
int BTWidth(BTree BT);                                     //���������Ⱥ���

void S_MakeNull(Stack& S)
{
	S.top = -1;
}

int S_Empty(Stack S)
{
	if (S.top < 0)
		return 1;
	else
		return 0;
}

BTree S_Top(Stack S)
{
	if (S_Empty(S))
		return NULL;
	else
		return (S.elements[S.top]);
}

void S_Pop(Stack& S)
{
	if (S_Empty(S))
		cout << "Stack is empty!" << endl;
	else S.top = S.top - 1;
}

void S_Push(BTree x, Stack& S)
{
	if (S.top == maxlength - 1)
		cout << "Stack is full!" << endl;
	else
	{
		S.top = S.top + 1;
		S.elements[S.top] = x;
	}
}

int Q_Addone(int i)
{
	return ((i + 1) % maxlength);
}

void Q_MakeNull(Queue& Q)
{
	Q.front = 0;
	Q.rear = maxlength - 1;
}

int Q_Empty(Queue Q)
{
	if (Q_Addone(Q.rear) == Q.front)
		return 1;
	return 0;
}

int Q_Size(Queue Q)
{
	return ((Q.rear - Q.front + maxlength + 1) % maxlength);
}

BTree Q_Front(Queue Q)
{
	if (Q_Empty(Q))
		return NULL;
	else
		return (Q.elements[Q.front]);
}

void Q_EnQueue(BTree BT, Queue& Q)
{
	if (Q_Addone(Q_Addone(Q.rear)) == Q.front)
		cout << "Queue is full!" << endl;
	else
	{
		Q.rear = Q_Addone(Q.rear);
		Q.elements[Q.rear] = BT;
	}
}

void Q_DeQueue(Queue& Q)
{
	if (Q_Empty(Q))
		cout << "Queue is empty!" << endl;
	else
		Q.front = Q_Addone(Q.front);
}

//����ݹ齨������������
void CreateBT(BTree& BT)
{
	elemtype data;
	cin >> data;
	if (data == '#')
		BT = NULL;
	else
	{
		BT = (BTree)malloc(sizeof(BT_Node));
		if (BT == NULL)
		{
			cout << "Memory allocation error!" << endl;
			return;
		}
		BT->data = data;
		CreateBT(BT->lchild);
		CreateBT(BT->rchild);
	}
}

//��Ļ��ʾ����������
void PrintBT(BTree BT)
{
	if (BT == NULL)
		return;
	Queue Q;
	int curCount = 1; //��¼��������ǰ���ڶ����н�����
	int levelcount = 1;
	BTree temp;
	Q_MakeNull(Q);
	Q_EnQueue(BT, Q);
	cout << "Level:" << levelcount << "|";
	while (!Q_Empty(Q))
	{
		--curCount;
		temp = Q_Front(Q);
		Q_DeQueue(Q);
		if (temp != NULL)
			cout << temp->data << " ";
		else
			cout << "#" << " ";
		if (temp)
		{
			if (temp->lchild != NULL)
				Q_EnQueue(temp->lchild, Q);
			else Q_EnQueue(NULL, Q);
			if (temp->rchild != NULL)
				Q_EnQueue(temp->rchild, Q);
			else Q_EnQueue(NULL, Q);
		}
		if (curCount == 0)
		{
			curCount = Q_Size(Q);
			cout << endl;
			++levelcount;
			if (Q_Size(Q))
				cout << "Level:" << levelcount << "|";
		}
	}
}

//�������ж��Ƿ�Ϊ��
int IsEmpty(BTree BT)
{
	if (BT == NULL)
		return 1;
	return 0;
}

//��������������������ݹ飩
void PreOrder_R(BTree BT)
{
	if (!IsEmpty(BT))
	{
		cout << BT->data << " ";
		PreOrder_R(BT->lchild);
		PreOrder_R(BT->rchild);
	}
}

//��������������������ǵݹ飩
void PreOrder_NR(BTree BT)
{
	Stack S;
	S_MakeNull(S);
	BTree pcur = BT;
	while (pcur != NULL || !S_Empty(S))
	{
		cout << pcur->data << " ";
		S_Push(pcur, S);
		pcur = pcur->lchild;//�����������ƶ�
		//��������Ϊ�գ�����ջ��Ԫ�س�ջ��������������Ѱ��ֱ��pCur��Ϊ��
		while (!pcur && !S_Empty(S))
		{
			pcur = S_Top(S);
			S_Pop(S);
			pcur = pcur->rchild;
		}
	}
}

//��������������������ݹ飩
void InOrder_R(BTree BT)
{
	if (!IsEmpty(BT))
	{
		InOrder_R(BT->lchild);
		cout << BT->data << " ";
		InOrder_R(BT->rchild);
	}
}

//��������������������ǵݹ飩
void InOrder_NR(BTree BT)
{
	Stack S;
	BTree pcur = BT;
	S_MakeNull(S);
	while (!IsEmpty(pcur) || !S_Empty(S))
	{
		if (!IsEmpty(pcur))
		{
			S_Push(pcur, S);
			pcur = pcur->lchild;
		}
		else
		{
			pcur = S_Top(S);
			S_Pop(S);
			cout << pcur->data << " ";
			pcur = pcur->rchild;
		}
	}
}

//��������������������ݹ飩
void PostOrder_R(BTree BT)
{
	if (!IsEmpty(BT))
	{
		PostOrder_R(BT->lchild);
		PostOrder_R(BT->rchild);
		cout << BT->data << " ";
	}
}

//��������������������ǵݹ飩
void PostOrder_NR(BTree BT)
{
	Stack S;
	BTree pcur = BT;
	S_MakeNull(S);
	while (pcur != NULL || !S_Empty(S))
	{
		while (pcur != NULL)
		{
			S_Push(pcur, S);
			S.visit[S.top] = 1;
			pcur = pcur->lchild;
		}
		while (!S_Empty(S) && S.visit[S.top] == 2)  //����������������ʸ�
		{
			pcur = S_Top(S);
			S_Pop(S);
			cout << pcur->data << " ";
			pcur = NULL;
		}
		if (!S_Empty(S))
		{
			S.visit[S.top] = 2;
			pcur = S_Top(S)->rchild;
		}
	}
}

//�����������������
void LeverOrder(BTree BT)
{
	Queue Q;
	Q_MakeNull(Q);
	if (!IsEmpty(BT))
		Q_EnQueue(BT, Q);
	while (!Q_Empty(Q))
	{
		BT = Q_Front(Q);
		cout << BT->data << " ";
		Q_DeQueue(Q);
		if (BT->lchild != NULL)
			Q_EnQueue(BT->lchild, Q);
		if (BT->rchild != NULL)
			Q_EnQueue(BT->rchild, Q);
	}
}

//�������ж��Ƿ�Ϊ��ȫ������
/*
* ���ò������
* ������ʵ���ָ����ֹͣ
* ����Ƿ���δ���ʵĽ�㣨�����Ƿ�Ϊ�գ�
* ���У�������ȫ������
*/
bool IsComplete(BTree BT)
{
	if (BT == NULL)
		return true;
	else
	{
		Queue Q;
		BTree p;
		p = BT;
		Q_MakeNull(Q);
		Q_EnQueue(p, Q);
		while (!Q_Empty(Q))
		{
			p = Q_Front(Q);
			Q_DeQueue(Q);
			if (!p) break;//������ָ����ֹͣѭ��
			Q_EnQueue(p->lchild, Q);//�������
			Q_EnQueue(p->rchild, Q);//�Һ������
		}
		while (!Q_Empty(Q)) 
		{//����ʱ�������Ƿ���δ���ʵ��Ľ��
			p = Q_Front(Q);
			Q_DeQueue(Q);
			if (p) return false;
		}
		return true;
	}
}

//���������Ⱥ���
int BTWidth(BTree BT)
{
	if (BT == NULL)
		return 0;
	Queue Q;
	int curCount = 1; //��¼��������ǰ���ڶ����н�����
	int max = 0;      //��¼���ֵ
	BTree temp;
	Q_MakeNull(Q);
	Q_EnQueue(BT, Q);
	while (!Q_Empty(Q))
	{
		--curCount;
		if (curCount == 0)
		{
			if (Q_Size(Q) > max)
				max = Q_Size(Q);
			curCount = Q_Size(Q);
		}
		temp = Q_Front(Q);
		Q_DeQueue(Q);
		if (temp->lchild != NULL)
			Q_EnQueue(temp->lchild, Q);
		if (temp->rchild != NULL)
			Q_EnQueue(temp->rchild, Q);
	}
	return max;
}

int main(void)
{
	BTree BT;
	bool test;
	int count;
	cout << "Create Binary Tree [PreOrder], # for NULL:";
	CreateBT(BT);
	cout << "Binary tree:" << endl;
	PrintBT(BT);
	cout << "---------------------------------" << endl;
	cout << "PreOrder_R:  |";
	PreOrder_R(BT);
	cout << "\nPreOrder_NR: |";
	PreOrder_NR(BT);
	cout << "\nInOrder_R:   |";
	InOrder_R(BT);
	cout << "\nInOrder_NR:  |";
	InOrder_NR(BT);
	cout << "\nPostOrder_R: |";
	PostOrder_R(BT);
	cout << "\nPostOrder_NR:|";
	PostOrder_NR(BT);
	cout << "\nLeverOrder:  |";
	LeverOrder(BT);
	test = IsComplete(BT);
	cout << "\nComplete binary tree? ";
	if (test)
		cout << "Yes." << endl;
	else
		cout << "No." << endl;
	count = BTWidth(BT);
	cout << "The width of binary tree:" << count << endl;
	system("pause");
	return 0;
}