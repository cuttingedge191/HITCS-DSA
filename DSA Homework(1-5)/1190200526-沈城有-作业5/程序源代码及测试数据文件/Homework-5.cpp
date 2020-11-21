/*
* ��ҵ5 ���ҽṹ�����򷽷�
* ��ҵ��Ŀ1��AVL���������ʵ��
* ѧ�ţ�1190200526
* �����������
*/

#include <iostream>
#include <fstream>
#include <queue>
using namespace std;

#define DataType int
#define InputDir "Input.txt"   //���������ļ�·��

//AVL����㣨�������洢�ṹ��
typedef struct AVLTreeNode
{
	DataType key;  //����
	int BF;   //ƽ������
	AVLTreeNode* leftchild;
	AVLTreeNode* rightchild;
}AVLTree;

//��غ�������
void AVL_LeftRotation(AVLTree*& T);           //AVL��LL��LR��ת��������
void AVL_RightRotation(AVLTree*& T);          //AVL��RR��RL��ת��������
DataType AVL_GetMin(AVLTree*& T);             //��ȡAVL����ɾ����������������ҽ��
void AVL_Insert(AVLTree*& T, DataType data);  //AVL�������������
void AVL_Delete(AVLTree*& T, DataType data);  //AVL��ɾ����������
void PrintSort(AVLTree*& T);                  //��Ļ��ʾAVL��������
void PrintAVL(AVLTree*& T);                   //��Ļ��ʾAVL��

//AVL��LL��LR��ת��������
void AVL_LeftRotation(AVLTree*& T)
{
	AVLTreeNode* tmp1;  
	AVLTreeNode* tmp2;  //�������������ָ��
	tmp1 = T->leftchild;
	//LL��ת
	if (tmp1->BF == 1)
	{
		T->leftchild = tmp1->rightchild;
		tmp1->rightchild = T;
		T->BF = 0;
		T = tmp1;
	}
	//LR��ת
	else
	{
		tmp2 = tmp1->rightchild;
		tmp1->rightchild = tmp2->leftchild;
		tmp2->leftchild = tmp1;
		T->leftchild = tmp2->rightchild;
		tmp2->rightchild = T;
		switch (tmp2->BF)
		{
		case 1: {
			T->BF = -1;
			tmp1->BF = 0;
		}
			  break;
		case 0: T->BF = tmp1->BF = 0;
			break;
		case -1: {
			T->BF = 0;
			tmp1->BF = 1;
		}
		}
		T = tmp2;
	}
	T->BF = 0;
}

//AVL��RR��RL��ת��������
void AVL_RightRotation(AVLTree*& T)
{
	AVLTreeNode* tmp1;
	AVLTreeNode* tmp2;  //�������������ָ��
	tmp1 = T->rightchild;
	//RR��ת
	if (tmp1->BF == 1)
	{
		T->rightchild = tmp1->leftchild;
		tmp1->leftchild = T;
		T->BF = 0;
		T = tmp1;
	}
	//RL��ת
	else
	{
		tmp2 = tmp1->leftchild;
		tmp1->leftchild = tmp2->rightchild;
		tmp2->rightchild = tmp1;
		T->rightchild = tmp2->leftchild;
		tmp2->leftchild = T;
		switch (tmp2->BF)
		{
		case -1: {
			T->BF = -1;
			tmp1->BF = 0;
		}
			  break;
		case 0: T->BF = tmp1->BF = 0;
			break;
		case 1: {
			T->BF = 0;
			tmp1->BF = 1;
		}
		}
		T = tmp2;
	}
	T->BF = 0;
}

//��ȡAVL����ɾ����������������ҽ��
DataType AVL_GetMin(AVLTree*& T)
{
	int min = T->key;
	AVLTree* temp = NULL;
	while (T->rightchild)
	{
		temp = T;
		T = T->rightchild;
	}
	min = T->key;
	if (temp)
		temp->rightchild = NULL;
	delete T;
	return min;
}

//AVL�������������
void AVL_Insert(AVLTree*& T, DataType data)
{
	//������в���Ԫ��
	if (!T)
	{
		T = new AVLTreeNode;
		T->key = data;
		T->leftchild = NULL;
		T->rightchild = NULL;
		T->BF = 0;
	}
	//���������ϲ���
	else if (data < T->key)
	{
		AVL_Insert(T->leftchild, data);
		switch (T->BF)
		{
		case -1:
			T->BF = 0;
			break;
		case 0:
			T->BF = 1;
			break;
		case 1:AVL_LeftRotation(T);
		}
	}
	//���������ϲ���
	else if (data > T->key)
	{
		AVL_Insert(T->rightchild, data);
		switch (T->BF)
		{
		case 1:
			T->BF = 0;
			break;
		case 0:T->BF = -1;
			break;
		case -1:AVL_RightRotation(T);
		}
	}
	//AVL���д��ڴ�Ԫ�أ��������
	else
	{
		cout << "Data:" << data << " already exists!" << endl;
		return;
	}
}

//AVL��ɾ����������
void AVL_Delete(AVLTree*& T, DataType data)
{
	AVLTree* tmp;
	//������δ�ҵ�
	if (!T)
	{
		cout << "Data not found, deletion failed!" << endl;
		return;
	}
	//����������Ѱ��
	else if (data < T->key)
	{
		AVL_Delete(T->leftchild, data);
		switch (T->BF)
		{
		case 1: T->BF = 0;
			break;
		case 0:T->BF = -1;
			break;
		case -1:AVL_RightRotation(T);
		}
	}
	//����������Ѱ��
	else if (data > T->key)
	{
		AVL_Delete(T->rightchild, data);
		switch (T->BF)
		{
		case -1:T->BF = 0;
			break;
		case 0:T->BF = 1;
			break;
		case 1:AVL_LeftRotation(T);
		}
	}
	//AVL���д��ڴ�Ԫ�أ�����ɾ��
	else
	{
		tmp = T;
		if (!T->leftchild)
		{
			T = T->rightchild;
			delete tmp;
		}
		else if (!T->rightchild)
		{
			T = T->leftchild;
			delete tmp;
		}
		else
			T->key = AVL_GetMin(T->leftchild);
	}
	cout << "Deletion complete!" << endl;
}

//��Ļ��ʾAVL��������
void PrintSort(AVLTree*& T)
{
	if (T == NULL)
		return;
	PrintSort(T->leftchild);
	cout << T->key << " ";
	PrintSort(T->rightchild);
}

//��Ļ��ʾAVL��
void PrintAVL(AVLTree*& T)
{
	cout << "---------------AVL Tree---------------" << endl;
	queue<AVLTree*> q;
	if (!T)
		return;
	int curCount = 1;   //��¼AVL����ǰ���ڶ����н�����
	int levelcount = 1;
	AVLTree* temp;
	q.push(T);
	cout << "Level:" << levelcount << "|";
	while (!q.empty())
	{
		--curCount;
		temp = q.front();
		q.pop();
		if (temp)
			cout << temp->key << " ";
		else
			cout << "#" << " ";
		if (temp)
		{
			if (temp->leftchild != NULL)
				q.push(temp->leftchild);
			else q.push(NULL);
			if (temp->rightchild != NULL)
				q.push(temp->rightchild);
			else q.push(NULL);
		}
		if (curCount == 0)
		{
			curCount = q.size();
			cout << endl;
			++levelcount;
			if (q.size())
				cout << "Level:" << levelcount << "|";
		}
	}
}

int main(void)
{
	int n;         //��¼���ݸ���
	DataType tmp;  //�ݴ�����
	ifstream Input;
	AVLTree* T = NULL;
	Input.open(InputDir);
	if (!Input)
	{
		cout << "File open failed!" << endl;
		system("pause");
		return 0;
	}
	else
	{
		streambuf* stream_buffer_cin = cin.rdbuf();
		streambuf* stream_buffer_file = Input.rdbuf();
		cin.rdbuf(stream_buffer_file);
		cin >> n;
		for (int i = 1; i <= n; ++i)
		{
			cin >> tmp;
			AVL_Insert(T, tmp);
		}
		Input.close();
		cin.rdbuf(stream_buffer_cin);
		cout << "File input completed." << endl;
		system("pause");
	}
	while (1)
	{
		system("cls");
		PrintAVL(T);
		cout << "Sorted data:";
		PrintSort(T);
		cout << "\n[1]Insert\t[2]Delete" << endl;
		cout << "Input choice(other characters to quit):";
		cin >> n;
		if (n == 1)
		{
			cout << "Input data to insert:";
			cin >> tmp;
			AVL_Insert(T, tmp);
		}
		else if (n == 2)
		{
			cout << "Input data to delete:";
			cin >> tmp;
			AVL_Delete(T, tmp);
		}
		else
		{
			cout << "Program terminated." << endl;
			system("pause");
			break;
		}
	}
	return 0;
}