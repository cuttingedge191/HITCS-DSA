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
struct AVLTreeNode
{
	DataType key;  //����
	int height;   //�������λ�ø߶�
	AVLTreeNode* leftchild;
	AVLTreeNode* rightchild;
	//����ʼ��
	AVLTreeNode()
	{
		height = 0;
		leftchild = rightchild = NULL;
	}
};

//��غ�������
int Get_Height(AVLTreeNode* a);                 //��ȡ���߶Ⱥ���
int Get_Max(AVLTreeNode* a, AVLTreeNode* b);    //�Ƚ������߶Ȳ����ؽϴ�ֵ
DataType Get_MinNode(AVLTreeNode*& T);          //��ȡAVL����ɾ����������������ҽ��
AVLTreeNode* RightRotate(AVLTreeNode* a);       //LL���������ת�������µĸ����ָ��
AVLTreeNode* LeftRotate(AVLTreeNode* a);        //RR���������ת�������µĸ����ָ��
AVLTreeNode* Left_RightRotate(AVLTreeNode* a);  //LR������ȸ����leftchild�����ٸ��ڵ������������µĸ����ָ��
AVLTreeNode* Right_LeftRotate(AVLTreeNode* a);  //RL������ȸ����rightchild�����ٸ���������������µĸ����ָ��
void Insert(AVLTreeNode*& a, DataType x);       //AVL�������������
void Delete(AVLTreeNode*& a, DataType x);       //AVL��ɾ����������
void PrintSort(AVLTreeNode* T);                 //��Ļ��ʾAVL��������
void PrintAVL(AVLTreeNode*& T);                 //��Ļ��ʾAVL��

//��ȡ���߶Ⱥ���
int Get_Height(AVLTreeNode* a)
{
	if (!a)
		return 0;
	return a->height;
}

//�Ƚ������߶Ȳ����ؽϴ�ֵ
int Get_Max(AVLTreeNode* a, AVLTreeNode* b)
{
	if (Get_Height(a) > Get_Height(b))
		return Get_Height(a);
	return Get_Height(b);
}

//��ȡAVL����ɾ����������������ҽ��
DataType Get_MinNode(AVLTreeNode*& T)
{
	DataType min;
	AVLTreeNode* p;
	if (!T->rightchild)
	{
		p = T;
		min = T->key;
		T = T->leftchild;  //�����̳�
		delete p;
		return min;
	}
	else
		return (Get_MinNode(T->rightchild));
}

//LL���������ת�������µĸ����ָ��
AVLTreeNode* RightRotate(AVLTreeNode* a)
{
	AVLTreeNode* lc = a->leftchild;
	a->leftchild = lc->rightchild;
	lc->rightchild = a;
	//���½��߶�
	a->height = Get_Max(a->leftchild, a->rightchild) + 1;
	lc->height = Get_Max(lc->leftchild, lc->rightchild) + 1;
	return lc;
}

//RR���������ת�������µĸ����ָ��
AVLTreeNode* LeftRotate(AVLTreeNode* a)
{
	AVLTreeNode* rc = a->rightchild;
	a->rightchild = rc->leftchild;
	rc->leftchild = a;
	//���½��߶�
	a->height = Get_Max(a->leftchild, a->rightchild) + 1;
	rc->height = Get_Max(rc->leftchild, rc->rightchild) + 1;
	return rc;
}

//LR������ȸ����leftchild�����ٸ��ڵ������������µĸ����ָ��
AVLTreeNode* Left_RightRotate(AVLTreeNode* a)
{
	a->leftchild = LeftRotate(a->leftchild);
	return RightRotate(a);
}

//RL������ȸ����rightchild�����ٸ���������������µĸ����ָ��
AVLTreeNode* Right_LeftRotate(AVLTreeNode* a)
{
	a->rightchild = RightRotate(a->rightchild);
	return LeftRotate(a);
}

//AVL�������������
void Insert(AVLTreeNode*& a, DataType x)
{
	if (!a)
	{
		a = new AVLTreeNode;
		a->key = x;
	}
	else if (x < a->key)
	{
		Insert(a->leftchild, x);
		if (Get_Height(a->leftchild) - Get_Height(a->rightchild) == 2)
		{
			if (x < a->leftchild->key)
			{
				a = RightRotate(a);      //LL
				cout << "LL" << endl;
			}
			else
			{
				a = Left_RightRotate(a); //LR
				cout << "LR" << endl;
			}
		}
	}
	else if (x > a->key)
	{
		Insert(a->rightchild, x);
		if (Get_Height(a->rightchild) - Get_Height(a->leftchild) == 2)
		{
			if (x > a->rightchild->key)
			{
				a = LeftRotate(a);       //RR
				cout << "RR" << endl;
			}
			else
			{
				a = Right_LeftRotate(a); //RL
				cout << "RL" << endl;
			}
		}
	}
	else
	{
		cout << "Data ["<< x << "] "<< "already exists!" << endl;
		return;
	}
	a->height = Get_Max(a->leftchild, a->rightchild) + 1;
}

//AVL��ɾ����������
void Delete(AVLTreeNode*& a, DataType x)
{
	if (!a)
	{
		cout << "Can not found data [" << x << "]!" << endl;
		return;
	}
	if (x < a->key)
	{
		Delete(a->leftchild, x);
		if (Get_Height(a->rightchild) - Get_Height(a->leftchild) == 2)
		{
			if (x < a->rightchild->key)
			{
				a = LeftRotate(a);       //RR
				cout << "RR" << endl;
			}
			else
			{
				a = Right_LeftRotate(a); //RL
				cout << "RL" << endl;
			}
		}
	}
	else if (x > a->key)
	{
		Delete(a->rightchild, x);
		if (Get_Height(a->leftchild) - Get_Height(a->rightchild) == 2)
		{
			if (x > a->leftchild->key)
			{
				a = RightRotate(a);      //LL
				cout << "LL" << endl;
			}
			else
			{
				a = Left_RightRotate(a); //LR
				cout << "LR" << endl;
			}
		}
	}
	else if (x == a->key)
	{
		AVLTreeNode* tmp;
		tmp = a;
		if (!a->leftchild)
		{
			a = a->rightchild;
			delete tmp;
		}
		else if (!a->rightchild)
		{
			a = a->leftchild;
			delete tmp;
		}
		else
			a->key = Get_MinNode(a->leftchild);
	}
	if (a)
		a->height = Get_Max(a->leftchild, a->rightchild) + 1;
}

//��Ļ��ʾAVL��������
void PrintSort(AVLTreeNode* T)
{
	if (T == NULL)
		return;
	PrintSort(T->leftchild);
	cout << T->key << " ";
	PrintSort(T->rightchild);
}

//��Ļ��ʾAVL��
void PrintAVL(AVLTreeNode*& T)
{
	cout << "---------------AVL Tree---------------" << endl;
	queue<AVLTreeNode*> q;
	if (!T)
		return;
	int curCount = 1;   //��¼AVL����ǰ���ڶ����н�����
	int levelcount = 1;
	AVLTreeNode* temp;
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
	int n;                  //��¼���ݸ���
	DataType tmp;           //�ݴ�����
	AVLTreeNode* T = NULL;  //����AVL�������ָ��
	ifstream Input;
	Input.open(InputDir);
	cout << "---------------Initialization---------------" << endl;
	if (!Input)
	{
		cout << "File open failed!" << endl;
		system("pause");
		return 0;
	}
	else
	{
		//�����ض������ļ�
		streambuf* stream_buffer_cin = cin.rdbuf();
		streambuf* stream_buffer_file = Input.rdbuf();
		cin.rdbuf(stream_buffer_file);
		cin >> n;
		for (int i = 1; i <= n; ++i)
		{
			cin >> tmp;
			Insert(T, tmp);
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
			Insert(T, tmp);
			system("pause");
		}
		else if (n == 2)
		{
			cout << "Input data to delete:";
			cin >> tmp;
			Delete(T, tmp);
			system("pause");
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