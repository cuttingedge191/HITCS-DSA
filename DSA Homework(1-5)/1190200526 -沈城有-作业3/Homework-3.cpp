/*
* ��ҵ3 ���νṹ����Ӧ��
* ѧ�ţ�1190200526
* �����������
*/

#include <iostream>
#include <cmath>
#include <malloc.h>
using namespace std;
#define elemtype int
#define maxlength 100

//�������Ķ�������洢�ṹ
struct BT_Node
{
	BT_Node* lchild;
	elemtype data;
	BT_Node* rchild;
};
typedef BT_Node* BTree;

//�ǵݹ����ʹ�õģ�˳��ջ�ṹ
struct stack
{
	BTree elements[maxlength];
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
BTree Q_Front(Queue Q);
void Q_EnQueue(BTree BT, Queue& Q);
void Q_DeQueue(Queue& Q);
//���ܺ�������
void CreateBT(BTree& BT);                                  //����ݹ齨������������
void PrintBT(BTree BT);                                    //������ʾ����������
int IsEmpty(BTree BT);                                     //�������ж��Ƿ�Ϊ��
void PreOrder_R(BTree BT);                                 //��������������������ݹ飩
void PreOrder_NR(BTree BT);                                //��������������������ǵݹ�)
void InOrder_R(BTree BT);                                  //��������������������ݹ飩
void InOrder_NR(BTree BT);                                 //��������������������ǵݹ飩
void PostOrder_R(BTree BT);                                //��������������������ݹ飩
void PostOrder_NR(BTree BT);                               //��������������������ǵݹ飩
void LeverOrder(BTree BT);                                 //�����������������

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
	cout << "Input data(int), -1 for null [PreOrder]:";
	cin >> data;
	if (data == -1)
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

//������ʾ����������
void PrintBT(BTree BT)
{
	///
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
		pcur = pcur->lchild;
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
		InOrder_R(BT->lchild);
		InOrder_R(BT->rchild);
		cout << BT->data << " ";
	}
}

//��������������������ǵݹ飩
void PostOrder_NR(BTree BT)
{
	///
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

int main(void)
{
	BTree BT;
	CreateBT(BT);
	PrintBT(BT);
	cout << "\nPreOrder_R:";
	PreOrder_R(BT);
	cout << "\nPreOrder_NR:";
	PreOrder_NR(BT);
	cout << "\nInOrder_R:";
	InOrder_R(BT);
	cout << "\nInOrder_NR:";
	InOrder_NR(BT);
	cout << "\nPostOrder_R:";
	PostOrder_R(BT);
	cout << "\nPostOrder_NR:";
	PostOrder_NR(BT);
	cout << "\nLeverOrder:";
	LeverOrder(BT);
	return 0;
}