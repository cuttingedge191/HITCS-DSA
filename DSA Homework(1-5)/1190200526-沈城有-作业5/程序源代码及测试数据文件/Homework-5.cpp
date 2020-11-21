/*
* 作业5 查找结构与排序方法
* 作业题目1：AVL树的设计与实现
* 学号：1190200526
* 姓名：沈城有
*/

#include <iostream>
#include <fstream>
#include <queue>
using namespace std;

#define DataType int
#define InputDir "Input.txt"   //测试数据文件路径

//AVL树结点（左右链存储结构）
typedef struct AVLTreeNode
{
	DataType key;  //数据
	int BF;   //平衡因子
	AVLTreeNode* leftchild;
	AVLTreeNode* rightchild;
}AVLTree;

//相关函数声明
void AVL_LeftRotation(AVLTree*& T);           //AVL树LL、LR旋转操作函数
void AVL_RightRotation(AVLTree*& T);          //AVL树RR、RL旋转操作函数
DataType AVL_GetMin(AVLTree*& T);             //获取AVL树待删除结点左子树的最右结点
void AVL_Insert(AVLTree*& T, DataType data);  //AVL树插入操作函数
void AVL_Delete(AVLTree*& T, DataType data);  //AVL树删除操作函数
void PrintSort(AVLTree*& T);                  //屏幕显示AVL树排序结果
void PrintAVL(AVLTree*& T);                   //屏幕显示AVL树

//AVL树LL、LR旋转操作函数
void AVL_LeftRotation(AVLTree*& T)
{
	AVLTreeNode* tmp1;  
	AVLTreeNode* tmp2;  //保存待调整结点的指针
	tmp1 = T->leftchild;
	//LL旋转
	if (tmp1->BF == 1)
	{
		T->leftchild = tmp1->rightchild;
		tmp1->rightchild = T;
		T->BF = 0;
		T = tmp1;
	}
	//LR旋转
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

//AVL树RR、RL旋转操作函数
void AVL_RightRotation(AVLTree*& T)
{
	AVLTreeNode* tmp1;
	AVLTreeNode* tmp2;  //保存待调整结点的指针
	tmp1 = T->rightchild;
	//RR旋转
	if (tmp1->BF == 1)
	{
		T->rightchild = tmp1->leftchild;
		tmp1->leftchild = T;
		T->BF = 0;
		T = tmp1;
	}
	//RL旋转
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

//获取AVL树待删除结点左子树的最右结点
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

//AVL树插入操作函数
void AVL_Insert(AVLTree*& T, DataType data)
{
	//向空树中插入元素
	if (!T)
	{
		T = new AVLTreeNode;
		T->key = data;
		T->leftchild = NULL;
		T->rightchild = NULL;
		T->BF = 0;
	}
	//在左子树上插入
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
	//在右子树上插入
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
	//AVL树中存在此元素，无需插入
	else
	{
		cout << "Data:" << data << " already exists!" << endl;
		return;
	}
}

//AVL树删除操作函数
void AVL_Delete(AVLTree*& T, DataType data)
{
	AVLTree* tmp;
	//空树或未找到
	if (!T)
	{
		cout << "Data not found, deletion failed!" << endl;
		return;
	}
	//在左子树上寻找
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
	//在右子树上寻找
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
	//AVL树中存在此元素，进行删除
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

//屏幕显示AVL树排序结果
void PrintSort(AVLTree*& T)
{
	if (T == NULL)
		return;
	PrintSort(T->leftchild);
	cout << T->key << " ";
	PrintSort(T->rightchild);
}

//屏幕显示AVL树
void PrintAVL(AVLTree*& T)
{
	cout << "---------------AVL Tree---------------" << endl;
	queue<AVLTree*> q;
	if (!T)
		return;
	int curCount = 1;   //记录AVL树当前层在队列中结点个数
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
	int n;         //记录数据个数
	DataType tmp;  //暂存数据
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