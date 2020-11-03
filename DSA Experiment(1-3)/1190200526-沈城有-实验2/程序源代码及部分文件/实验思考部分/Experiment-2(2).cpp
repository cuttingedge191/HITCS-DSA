/*
* 实验2 树型结构及其应用（思考部分）
* 实验项目：树形结构及其应用
* 实验题目：哈夫曼编码与译码方法
* 学号：1190200526
* 姓名：沈城有
*/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <string>
#include <cmath>
using namespace std;
#define InputDir "Input.txt"      //英文文本路径
#define CompDir "Comp.ht"         //编码文本保存路径
#define N 256                     //最大叶子数（字符种类数）
#define Max_K 32                  //最大k值
#define M Max_K * N - 1           //最大结点总数

//最小堆数据结构实现
typedef struct heap
{
	int data[M + 1][2];  //编号+频率
	int n;
}Heap;

//K叉树孩子链表
typedef struct childnode
{
	int child;
	childnode* next;
}HTC;

//K叉哈夫曼树结构定义
typedef struct K_htnode
{
	int parent;
	int freq;
	HTC head;  //孩子链表表头
}HTNode;

//字符编码表
typedef struct codingtablec
{
	int num;
	char ch;
	string code;
}CodeTab_C;

//单词编码表
typedef struct codingtables
{
	int num;
	string word;
	string ch;
}CodeTab_S;

//数据结构相关基本操作声明
void InitHeap(Heap& heap);
bool HeapEmpty(Heap heap);
bool HeapFull(Heap heap);
void HeapInsert(Heap& heap, int num, int freq);
void HeapDeleteMin(Heap& heap, int& num);
//功能实现相关函数声明
int ReadFromFile(char ch);                             //从文件中读取并统计信息
void CreateHeap(char ch, Heap& heap);                  //创建初始堆
HTC* HTCNodeInit(int k);                               //初始化K叉树孩子链表          
int HTNodeInit(char ch);                               //用统计数据初始化生成结点
void SelectMin(Heap& heap, int k, int num[]);          //选取k个最小权值（利用堆）
void CreateHT(Heap heap, char ch, int& k);             //构造哈夫曼树
void ProcessCodeTable(char ch);                        //根据哈夫曼树生成并保存编码表
void ShowInfo(char ch);                                //显示统计信息及编码表
void EncodeFile(void);                                 //根据编码生成压缩文件
void CalculateRate(int countt, int k);                 //计算压缩率

void InitHeap(Heap& heap)
{
	heap.n = 0;
}

bool HeapEmpty(Heap heap)
{
	return (!heap.n);
}

bool HeapFull(Heap heap)
{
	return (heap.n == N - 1);
}

void HeapInsert(Heap& heap, int num, int freq)
{
	int i;
	if (!HeapFull(heap))
	{
		i = ++heap.n;
		while ((i != 1) && (freq < heap.data[i / 2][1]))
		{
			heap.data[i][0] = heap.data[i / 2][0];
			heap.data[i][1] = heap.data[i / 2][1];
			i /= 2;
		}
	}
	heap.data[i][0] = num;
	heap.data[i][1] = freq;
}

void HeapDeleteMin(Heap& heap, int& num)
{
	int parent = 1, child = 2;
	int temp[2];
	if (!HeapEmpty(heap))
	{
		num = heap.data[1][0];
		temp[0] = heap.data[heap.n][0];
		temp[1] = heap.data[heap.n--][1];
		while (child <= heap.n)
		{
			if ((child < heap.n) && (heap.data[child][1] > heap.data[child + 1][1]))
				++child;
			if (temp[1] <= heap.data[child][1])
				break;
			heap.data[parent][0] = heap.data[child][0];
			heap.data[parent][1] = heap.data[child][1];
			parent = child;
			child *= 2;
		}
	}
	else
	{
		cout << "Heap is empty!" << endl;
		return;
	}
	heap.data[parent][0] = temp[0];
	heap.data[parent][1] = temp[1];
}

HTNode HTree[M];           //K叉哈夫曼树
map<char, int> resultc;    //统计字符用
map<string, int> results;  //统计单词用
CodeTab_C CT_C[N];         //存储字符编码表
CodeTab_S CT_S[N];         //存储单词编码表

//从文件中读取并统计信息
int ReadFromFile(char ch)
{
	ifstream OpenFile(InputDir);
	char temp;
	int countt = 0;
	if (ch == '1')
	{
		map<char, int>::iterator it;
		if (!OpenFile)
		{
			cout << "File open failed!" << endl;
			return 0;
		}
		while (OpenFile.get(temp))
		{
			if (temp == '\n')
				temp = '#';  //转换换行符便于显示
			it = resultc.find(temp);
			if (it != resultc.end())
				++it->second;
			else
				resultc.insert(pair<char, int>(temp, 1));
		}
		OpenFile.close();
		for (it = resultc.begin(); it != resultc.end(); ++it)
			countt += it->second;
		cout << "File read and analysis completed." << endl;
		return countt;
	}
	return 0;
	/*else
	{
		map<string, int>::iterator it;
		string s;
		int flag = 0;
		if (!OpenFile)
		{
			cout << "File open failed!" << endl;
			return 0;
		}
		while (OpenFile.get(temp))
		{
			if (flag && s.size() != 0)
			{
				it = results.find(s);
				if (it != results.end())
					++it->second;
				else
					results.insert(pair<string, int>(s, 1));
			}
			if (temp == '\n')
				flag = 1;
			else if(temp == ' ')
		}
		OpenFile.close();
		cout << "File read and analysis completed." << endl;
		return countt;
	}*/  //单词部分
}

//创建初始堆
void CreateHeap(char ch, Heap& heap)
{
	if (ch == '1')
	{
		map<char, int>::iterator it;
		int max = resultc.size();
		int i;
		for (i = 0, it = resultc.begin(); i < max, it != resultc.end(); ++i, ++it)
		{
			CT_C[i].ch = it->first;
			CT_C[i].num = i;
		}
		for (i = 0; i < max; ++i)
		{
			it = resultc.find(CT_C[i].ch);
			HeapInsert(heap, CT_C[i].num, it->second);
		}
	}
	else
	{
		///
	}
}

//初始化K叉树孩子链表
HTC* HTCNodeInit(int k)
{
	HTC* pcur;
	HTC* q;
	q = (HTC*)malloc(sizeof(HTC));  //记录第一个结点
	if (q == NULL)
	{
		cout << "Memory allocation failed!" << endl;
		return NULL;
	}
	q->child = -1;
	pcur = q;
	for (int i = 2; i <= k; ++i)
	{
		pcur->next = (HTC*)malloc(sizeof(HTC));
		if (pcur->next == NULL)
		{
			cout << "Memory allocation failed!" << endl;
			return NULL;
		}
		pcur->next->child = -1;
		pcur = pcur->next;
	}
	pcur->next = NULL;
	return q;
}

//用统计数据初始化生成结点
int HTNodeInit(char ch)
{
	int k;
	do {
		cout << "Input k" << "(<=" << Max_K << "):";
		cin >> k;
	} while (k > Max_K);
	if (ch == '1')
	{
		map<char, int>::iterator it;
		int i = 0;
		for (it = resultc.begin(); it != resultc.end(); ++it)
		{
			HTree[i].freq = it->second;
			HTree[i].parent = -1;
			HTree[i].head.next = HTCNodeInit(k);
			++i;
		}
		for (; i < M; ++i)
		{
			HTree[i].parent = -1;
			HTree[i].head.next = HTCNodeInit(k);
		}
	}
	else
	{
		///
	}
	return k;
}

//选取k个最小权值（利用堆）
void SelectMin(Heap& heap, int k, int num[])
{
	for (int i = 0; i < k; ++i)
		HeapDeleteMin(heap, num[i]);
}

//构造哈夫曼树
void CreateHT(Heap heap, char ch, int& k)
{
	int i, a;
	int p[Max_K];
	HTC* pcur;
	if (ch == '1')
	{
		int maxn = resultc.size();
		k = HTNodeInit(ch);
		CreateHeap(ch, heap);
		i = maxn;
		if ((maxn - 1) % (k - 1))  //补充几个无意义的点以保证每次都能取k个
		{
			for (int m = 1; m <= (k - 1) - (maxn - 1) % (k - 1); ++m)
				HeapInsert(heap, -1, 0);
		}
		while(heap.n > 1)
		{
			SelectMin(heap, k, p);
			for (a = 0; a < k; ++a)
			{
				HTree[p[a]].parent = i;
				HTree[i].freq += HTree[p[a]].freq;
			}
			HeapInsert(heap, i, HTree[i].freq);
			for (pcur = HTree[i].head.next, a = 0; pcur != NULL; pcur = pcur->next)
				pcur->child = p[a++];
			++i;
		}
	}
	else
	{
		///
	}
}

//sort所需比较函数1
bool cmp_1(CodeTab_C a, CodeTab_C b)
{
	return HTree[a.num].freq > HTree[b.num].freq;
}

//sort所需比较函数2
bool cmp_2(CodeTab_S a, CodeTab_S b)
{
	return HTree[a.num].freq > HTree[b.num].freq;
}

//根据哈夫曼树生成并保存编码表
void ProcessCodeTable(char ch)
{
	int c, p, i;
	int max, count = -1;
	int start;
	char code[100];
	HTC* pcur;
	string temp;
	if (ch == '1')
	{
		max = resultc.size();
		map<char, int>::iterator it;
		sort(CT_C, CT_C + max, cmp_1);
		for (i = 0; i < max; ++i)
		{
			c = CT_C[i].num;
			start = 99;
			temp = "";
			while ((p = HTree[c].parent) >= 0)
			{
				pcur = HTree[p].head.next;
				while (pcur != NULL)
				{
					++count;
					if (pcur->child == c)
						break;
					pcur = pcur->next;
				}
				if (count <= 9)
					code[--start] = '0' + count;
				else
					code[--start] = 'A' + count - 10;
				count = -1;
				c = p;  //继续上溯;
			}
			for (int j = start; j < 99; ++j)
			{
				temp += code[j];
			}
			CT_C[i].code = temp;
		}
	}
	else
	{
		///
	}
}

//显示统计信息
void ShowInfo(char ch)
{
	if (ch == '1')
	{
		map<char, int>::iterator it;
		cout << "Character\t" << "Frequency\t" << "Huffman Code" << endl;
		for (unsigned int i = 0; i < resultc.size(); ++i)
		{
			cout << CT_C[i].ch << "\t\t";
			it = resultc.find(CT_C[i].ch);
			cout << it->second << "\t\t";
			cout << CT_C[i].code << endl;
		}
		cout << "Number of type:" << resultc.size() << endl;
	}
	else
	{
		///
	}
}

//根据编码生成压缩文件
void EncodeFile(void)
{
	ifstream ReadFile;
	ofstream WriteFile;
	char temp;
	int q;
	int count = 0;
	ReadFile.open(InputDir);
	WriteFile.open(CompDir);
	string::iterator it;
	map<char, double>::iterator mt;
	if (!ReadFile || !WriteFile)
	{
		cout << "File open failed!" << endl;
		return;
	}
	while (ReadFile.get(temp))
	{
		if (temp == '\n')
			temp = '#';   //转换换行符
		for (q = 0; q < (int)resultc.size(); ++q)
		{
			if (CT_C[q].ch == temp)
				break;
		}
		WriteFile << CT_C[q].code;
	}
	ReadFile.close();
	WriteFile.close();
	cout << "File encode completed." << endl;
}

//计算压缩率
void CalculateRate(int countt, int k)
{
	double rate;
	int count = 0;
	int encode = ceil(log2(1.0 * k));
	map<char, int>::iterator it;
	string::iterator its;
	for (unsigned int i = 0; i < resultc.size(); ++i)
	{
		it = resultc.find(CT_C[i].ch);
		count += encode * it->second * CT_C[i].code.size();
	}
	rate = (double)count / 8 / (double)countt * 100;
	cout << "Encode rate:" << rate << "%" << endl;
}

int main(void)
{
	int countt, k;
	char ch = '1';
	Heap heap;
	InitHeap(heap);
	/*cout << "[1] Character\t[2] Word" << endl;
	cout << "Input choice:";
	cin >> ch;*/
	countt = ReadFromFile(ch);
	if (!countt)
	{
		cout << "Illegal operation or empty file!" << endl;
		return 0;
	}
	CreateHT(heap, ch, k);
	ProcessCodeTable(ch);
	ShowInfo(ch);
	cout << "Total characters:" << countt << endl;
	system("pause");
	EncodeFile();
	CalculateRate(countt, k);
	system("pause");
	return 0;
}