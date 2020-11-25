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
struct AVLTreeNode
{
	DataType key;  //数据
	int height;   //结点所在位置高度
	AVLTreeNode* leftchild;
	AVLTreeNode* rightchild;
	//结点初始化
	AVLTreeNode()
	{
		height = 0;
		leftchild = rightchild = NULL;
	}
};

//相关函数声明
int Get_Height(AVLTreeNode* a);                 //获取结点高度函数
int Get_Max(AVLTreeNode* a, AVLTreeNode* b);    //比较两结点高度并返回较大值
DataType Get_MinNode(AVLTreeNode*& T);          //获取AVL树待删除结点左子树的最右结点
AVLTreeNode* RightRotate(AVLTreeNode* a);       //LL情况，右旋转，返回新的根结点指针
AVLTreeNode* LeftRotate(AVLTreeNode* a);        //RR情况，左旋转，返回新的根结点指针
AVLTreeNode* Left_RightRotate(AVLTreeNode* a);  //LR情况，先根结点leftchild左旋再根节点右旋，返回新的根结点指针
AVLTreeNode* Right_LeftRotate(AVLTreeNode* a);  //RL情况，先根结点rightchild右旋再根结点左旋，返回新的根结点指针
void Insert(AVLTreeNode*& a, DataType x);       //AVL树插入操作函数
void Delete(AVLTreeNode*& a, DataType x);       //AVL树删除操作函数
void PrintSort(AVLTreeNode* T);                 //屏幕显示AVL树排序结果
void PrintAVL(AVLTreeNode*& T);                 //屏幕显示AVL树

//获取结点高度函数
int Get_Height(AVLTreeNode* a)
{
	if (!a)
		return 0;
	return a->height;
}

//比较两结点高度并返回较大值
int Get_Max(AVLTreeNode* a, AVLTreeNode* b)
{
	if (Get_Height(a) > Get_Height(b))
		return Get_Height(a);
	return Get_Height(b);
}

//获取AVL树待删除结点左子树的最右结点
DataType Get_MinNode(AVLTreeNode*& T)
{
	DataType min;
	AVLTreeNode* p;
	if (!T->rightchild)
	{
		p = T;
		min = T->key;
		T = T->leftchild;  //左链继承
		delete p;
		return min;
	}
	else
		return (Get_MinNode(T->rightchild));
}

//LL情况，右旋转，返回新的根结点指针
AVLTreeNode* RightRotate(AVLTreeNode* a)
{
	AVLTreeNode* lc = a->leftchild;
	a->leftchild = lc->rightchild;
	lc->rightchild = a;
	//更新结点高度
	a->height = Get_Max(a->leftchild, a->rightchild) + 1;
	lc->height = Get_Max(lc->leftchild, lc->rightchild) + 1;
	return lc;
}

//RR情况，左旋转，返回新的根结点指针
AVLTreeNode* LeftRotate(AVLTreeNode* a)
{
	AVLTreeNode* rc = a->rightchild;
	a->rightchild = rc->leftchild;
	rc->leftchild = a;
	//更新结点高度
	a->height = Get_Max(a->leftchild, a->rightchild) + 1;
	rc->height = Get_Max(rc->leftchild, rc->rightchild) + 1;
	return rc;
}

//LR情况，先根结点leftchild左旋再根节点右旋，返回新的根结点指针
AVLTreeNode* Left_RightRotate(AVLTreeNode* a)
{
	a->leftchild = LeftRotate(a->leftchild);
	return RightRotate(a);
}

//RL情况，先根结点rightchild右旋再根结点左旋，返回新的根结点指针
AVLTreeNode* Right_LeftRotate(AVLTreeNode* a)
{
	a->rightchild = RightRotate(a->rightchild);
	return LeftRotate(a);
}

//AVL树插入操作函数
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

//AVL树删除操作函数
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

//屏幕显示AVL树排序结果
void PrintSort(AVLTreeNode* T)
{
	if (T == NULL)
		return;
	PrintSort(T->leftchild);
	cout << T->key << " ";
	PrintSort(T->rightchild);
}

//屏幕显示AVL树
void PrintAVL(AVLTreeNode*& T)
{
	cout << "---------------AVL Tree---------------" << endl;
	queue<AVLTreeNode*> q;
	if (!T)
		return;
	int curCount = 1;   //记录AVL树当前层在队列中结点个数
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
	int n;                  //记录数据个数
	DataType tmp;           //暂存数据
	AVLTreeNode* T = NULL;  //保存AVL树根结点指针
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
		//输入重定向至文件
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