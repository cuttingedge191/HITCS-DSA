/*
* 实验3 图型结构及其应用
* 实验题目：图型结构的应用算法
* 实验选题1：最短路径算法的实现
* 学号：1190200526
* 姓名：沈城有
*/
#include <iostream>
#include <fstream>
#include <cstdio>
using namespace std;

#define Max_N 50
#define VertexData char         //顶点数据类型
#define EdgeData int            //边数据类型
#define Inf 1e8                 //无穷大定义
#define InputDir "Input.txt"    //文件输入路径

//邻接矩阵数据结构
typedef struct
{
	VertexData vertex[Max_N];     //顶点表
	EdgeData edge[Max_N][Max_N];  //边表
	int n, e;                     //图的顶点数和边数
}AdjMatrix;

//最小堆数据结构实现
typedef struct nodedata
{
	int num;
	int weight;
}NodeData;

typedef struct minheap
{
	NodeData elem[Max_N + 1];   //编号+权值
	int n;
}MinHeap;

//最小堆结构（优先级队列）相关基本操作声明
void InitHeap(MinHeap& heap);
bool HeapEmpty(MinHeap heap);
bool HeapFull(MinHeap heap);
void HeapInsert(MinHeap& heap, NodeData elem);
void HeapDeleteMin(MinHeap& heap, int& num);
//功能函数声明
AdjMatrix* CreateAdjM(void);                                                                            //创建用邻接矩阵表示的有向图
void Dijkstra(AdjMatrix* adjm, MinHeap& heap, int start, bool S[], int pre[], int dis[]);               //Dijkstra算法（堆优化）
void Floyd_Warshall(AdjMatrix* adjm, EdgeData e[][Max_N], int path[][Max_N]);                           //Floyd-Warshall算法
void Single_Target(AdjMatrix* adjm, MinHeap& heap, bool S[], int pre[], int dis[], int target);         //单目标最短路径（Dijkstra算法修改）
void Single_Vertex_Pair(AdjMatrix* adjm, MinHeap& heap, bool S[], int pre[], int dis[], int s, int t);  //单顶点对最短路径（Dijkstra算法修改）
void Reachable_Matrix(AdjMatrix* adjm, EdgeData m[][Max_N]);                                            //计算并显示可达矩阵（Floyd-Warshall算法修改）
void PrintResult_D(AdjMatrix* adjm, int start, bool S[], int pre[], int dis[]);                         //屏幕显示Dijkstra算法结果函数
void PrintPath(AdjMatrix* adjm, int path[][Max_N], int i, int j);                                       //处理并屏幕显示最短路径中各点（递归）
void PrintResult_F(AdjMatrix* adjm, EdgeData e[][Max_N], int path[][Max_N]);                            //屏幕显示Floyd-Warshall算法结果函数
void PrintResult_ST(AdjMatrix* adjm, bool S[], int pre[], int dis[], int target);                       //屏幕显示单目标最短路径结果函数
void PrintResult_SVP(AdjMatrix* adjm, bool S[], int pre[], int dis[], int s, int t);                    //屏幕显示单顶点对最短路径结果函数

void InitHeap(MinHeap& heap)
{
	heap.n = 0;
}

bool HeapEmpty(MinHeap heap)
{
	return (!heap.n);
}

bool HeapFull(MinHeap heap)
{
	return (heap.n == Max_N);
}

void HeapInsert(MinHeap& heap, NodeData e)
{
	int i;
	if (HeapFull(heap))
	{
		cout << "Heap is full!" << endl;
		return;
	}
	i = ++heap.n;
	while ((i != 1) && (e.weight < heap.elem[i / 2].weight))
	{
		heap.elem[i] = heap.elem[i / 2];
		i /= 2;
	}
	heap.elem[i] = e;
}

void HeapDeleteMin(MinHeap& heap, int& num)
{
	int parent = 1, child = 2;
	NodeData temp;
	if (!HeapEmpty(heap))
	{
		num = heap.elem[1].num;
		temp = heap.elem[heap.n--];
		while (child <= heap.n)
		{
			if ((child < heap.n) && (heap.elem[child].weight > heap.elem[child + 1].weight))
				++child;  //选更小的
			if (temp.weight <= heap.elem[child].weight)
				break;
			heap.elem[parent] = heap.elem[child];
			parent = child;
			child *= 2;
		}
	}
	else
	{
		cout << "Heap is empty!" << endl;
		return;
	}
	heap.elem[parent] = temp;
}

//创建用邻接矩阵表示的有向图
AdjMatrix* CreateAdjM(void)
{
	AdjMatrix* adj = new AdjMatrix();
	cin >> adj->n;
	cin >> adj->e;
	for (int i = 0; i < adj->n; ++i)
		cin >> adj->vertex[i];
	for (int i = 0; i < adj->n; ++i)
	{
		for (int j = 0; j < adj->n; ++j)
		{
			adj->edge[i][j] = (int)Inf;  //邻接矩阵初始化
		}
	}
	for (int i = 0; i < adj->e; ++i)
	{
		int a, b, c;
		cin >> a >> b >> c;
		adj->edge[a][b] = c;
	}
	cout << "Input complete!" << endl;
	return adj;
}

//Dijkstra算法（堆优化）
void Dijkstra(AdjMatrix* adjm, MinHeap& heap, int start, bool S[], int pre[], int dis[])
{
	NodeData temp;
	int min = start;
	InitHeap(heap);
	for (int i = 0; i < adjm->n; ++i)
		S[i] = false;  //集合S初始化
	S[start] = true;
	for (int i = 0; i < adjm->n; ++i)
	{
		pre[i] = start;
		dis[i] = adjm->edge[start][i];
		if (dis[i] < Inf)
		{
			temp.weight = dis[i];
			temp.num = i;
			HeapInsert(heap, temp);
		}
	}
	while(1)
	{
		if (HeapEmpty(heap))
			break;
		HeapDeleteMin(heap, min);
		if (S[min])
		{
			if (HeapEmpty(heap))
				break;
			else
				HeapDeleteMin(heap, min);
		}
		S[min] = true;
		for (int i = 0; i < adjm->n; ++i)
		{
			if (!S[i])
			{
				if ((dis[min] + adjm->edge[min][i]) < dis[i])
				{
					pre[i] = min;
					dis[i] = dis[min] + adjm->edge[min][i];  //更新距离
					temp.num = i;
					temp.weight = dis[i];
					HeapInsert(heap, temp);
				}
			}
		}
	}
}

//处理并屏幕显示最短路径中各点（递归）
void PrintPath(AdjMatrix* adjm, int path[][Max_N], int i, int j)
{
	int k;
	k = path[i][j];
	if (k != -1)
	{
		PrintPath(adjm, path, i, k);
		cout << adjm->vertex[k] << " ";
		PrintPath(adjm, path, k, j);
	}
}

//Floyd-Warshall算法
void Floyd_Warshall(AdjMatrix* adjm, EdgeData e[][Max_N], int path[][Max_N])
{
	//设定最短路径矩阵初值
	for (int i = 0; i < adjm->n; ++i)
	{
		for (int j = 0; j < adjm->n; ++j)
		{
			if (i == j)
				e[i][j] = 0;
			else
				e[i][j] = adjm->edge[i][j];
			path[i][j] = -1;
		}
	}
	for (int k = 0; k < adjm->n; ++k)
	{
		for (int i = 0; i < adjm->n; ++i)
		{
			for (int j = 0; j < adjm->n; ++j)
			{
				if (e[i][k] + e[k][j] < e[i][j])
				{
					e[i][j] = e[i][k] + e[k][j];
					path[i][j] = k;
				}
			}
		}
	}
}

//单目标最短路径（Dijkstra算法修改）
void Single_Target(AdjMatrix* adjm, MinHeap& heap, bool S[], int pre[], int dis[], int target)
{//主要是逆向找路径
	NodeData temp;
	int min = target;
	InitHeap(heap);
	for (int i = 0; i < adjm->n; ++i)
		S[i] = false;  //集合S初始化
	S[target] = true;
	for (int i = 0; i < adjm->n; ++i)
	{
		pre[i] = target;
		dis[i] = adjm->edge[i][target];
		if (dis[i] < Inf)
		{
			temp.weight = dis[i];
			temp.num = i;
			HeapInsert(heap, temp);
		}
	}
	while (1)
	{
		if (HeapEmpty(heap))
			break;
		HeapDeleteMin(heap, min);
		if (S[min])
		{
			if (HeapEmpty(heap))
				break;
			else
				HeapDeleteMin(heap, min);
		}
		S[min] = true;
		for (int i = 0; i < adjm->n; ++i)
		{
			if (!S[i])
			{
				if ((dis[min] + adjm->edge[i][min]) < dis[i])
				{
					pre[i] = min;
					dis[i] = dis[min] + adjm->edge[i][min];  //更新距离
					temp.num = i;
					temp.weight = dis[i];
					HeapInsert(heap, temp);
				}
			}
		}
	}
}

//单顶点对最短路径（Dijkstra算法修改）
void Single_Vertex_Pair(AdjMatrix* adjm, MinHeap& heap, bool S[], int pre[], int dis[], int s, int t)
{
	NodeData temp;
	int min;
	min = s;
	InitHeap(heap);
	for (int i = 0; i < adjm->n; ++i)
		S[i] = false;  //集合S初始化
	S[s] = true;
	for (int i = 0; i < adjm->n; ++i)
	{
		pre[i] = s;
		dis[i] = adjm->edge[s][i];
		if (dis[i] < Inf)
		{
			temp.weight = dis[i];
			temp.num = i;
			HeapInsert(heap, temp);
		}
	}
	while (1)
	{
		if (HeapEmpty(heap))
			break;
		HeapDeleteMin(heap, min);
		if (S[min])
		{
			if (HeapEmpty(heap) || min == t)
				break;
			else
				HeapDeleteMin(heap, min);
		}
		S[min] = true;
		for (int i = 0; i < adjm->n; ++i)
		{
			if (!S[i])
			{
				if ((dis[min] + adjm->edge[min][i]) < dis[i])
				{
					pre[i] = min;
					dis[i] = dis[min] + adjm->edge[min][i];  //更新距离
					temp.num = i;
					temp.weight = dis[i];
					HeapInsert(heap, temp);
				}
			}
		}
	}
}

//计算并显示可达矩阵（Floyd-Warshall算法修改）
void Reachable_Matrix(AdjMatrix* adjm, EdgeData m[][Max_N])
{
	//设定可达矩阵初值
	for (int i = 0; i < adjm->n; ++i)
	{
		for (int j = 0; j < adjm->n; ++j)
		{
			if (i == j)
				m[i][j] = 1;
			else
				m[i][j] = (adjm->edge[i][j] < Inf) ? 1 : 0;
		}
	}
	for (int k = 0; k < adjm->n; ++k)
	{
		for (int i = 0; i < adjm->n; ++i)
		{
			for (int j = 0; j < adjm->n; ++j)
				m[i][j] = (m[i][j] || (m[i][k] && m[k][j]));
		}
	}
	//屏幕打印可达矩阵
	cout << "--------------------Reachable Matrix--------------------" << endl;
	for (int i = 0; i < adjm->n; ++i)
	{
		for (int j = 0; j < adjm->n; ++j)
			cout << m[i][j] << " ";
		cout << endl;
	}
}

//屏幕显示Dijkstra算法结果函数
void PrintResult_D(AdjMatrix* adjm, int start, bool S[], int pre[], int dis[])
{
	char path[Max_N];
	int count;
	cout << "--------------------Result--------------------" << endl;
	for (int i = 0; i < adjm->n; ++i)
	{
		if (i != start)
		{
			printf("The shortest path from V%d(%c) to V%d(%c):\n", start, adjm->vertex[start], i, adjm->vertex[i]);
			if (dis[i] == (int)Inf)
				printf("No Path\n");
			else
			{
				count = 0;
				path[count] = adjm->vertex[i];
				for (int j = i; j != start; )
				{
					path[++count] = adjm->vertex[pre[j]];
					j = pre[j];
				}
				for (int j = count; j >= 0; --j)
					cout << path[j] << " ";
				printf("\nDistance:%d\n", dis[i]);
			}
		}
	}
}

//屏幕显示Floyd-Warshall算法结果函数
void PrintResult_F(AdjMatrix* adjm, EdgeData e[][Max_N], int path[][Max_N])
{
	cout << "--------------------Result--------------------" << endl;
	cout << "Begin\tEnd\tDistance\tPath" << endl;
	for (int i = 0; i < adjm->n; ++i)
	{
		for (int j = 0; j < adjm->n; ++j)
		{
			cout << "V" << i << ":" << adjm->vertex[i] << "\t";
			cout << "V" << j << ":" << adjm->vertex[j] << "\t";
			if (e[i][j] < (int)Inf)
			{
				cout << e[i][j] << "\t\t";
				cout << adjm->vertex[i] << " ";
				PrintPath(adjm, path, i, j);
				cout << adjm->vertex[j] << endl;
			}
			else
			{
				cout << "inf" << "\t\t";
				cout << "NULL" << endl;
			}
		}
	}
}

//屏幕显示单目标最短路径结果函数
void PrintResult_ST(AdjMatrix* adjm, bool S[], int pre[], int dis[], int target)
{
	cout << "--------------------Result--------------------" << endl;
	for (int i = 0; i < adjm->n; ++i)
	{
		if (i != target)
		{
			printf("The shortest path from V%d(%c) to V%d(%c):\n", i, adjm->vertex[i], target, adjm->vertex[target]);
			if (dis[i] == (int)Inf)
				printf("No Path\n");
			else
			{
				cout << adjm->vertex[i] << " ";
				for (int j = i; j != target; )
				{
					cout << adjm->vertex[pre[j]] << " ";
					j = pre[j];
				}
				printf("\nDistance:%d\n", dis[i]);
			}
		}
	}
}

//屏幕显示单顶点对最短路径结果函数
void PrintResult_SVP(AdjMatrix* adjm, bool S[], int pre[], int dis[], int s, int t)
{
	int count;
	char path[Max_N];
	printf("The shortest path from V%d(%c) to V%d(%c):\n", s, adjm->vertex[s], t, adjm->vertex[t]);
	if (dis[t] == (int)Inf)
		printf("No Path\n");
	else
	{
		count = 0;
		path[count] = adjm->vertex[t];
		for (int j = t; j != s; )
		{
			path[++count] = adjm->vertex[pre[j]];
			j = pre[j];
		}
		for (int j = count; j >= 0; --j)
			cout << path[j] << " ";
		printf("\nDistance:%d\n", dis[t]);
	}
}

int main(void)
{
	AdjMatrix* adjm = NULL;
	ifstream Input;
	//Floyd-Warshall及其相关算法
	EdgeData e[Max_N][Max_N];  //最短路径长度矩阵
	int path[Max_N][Max_N];    //最短路径矩阵
	//Dijkstra及其相关算法
	MinHeap heap;
	int start;                 //起始顶点
	int target;                //目标顶点
	bool S[Max_N];             //标记集合S
	int pre[Max_N];            //记录最短路径（保存直接前导结点）
	int dis[Max_N];            //记录路径长度
	char ch;
	cout << "--------------------Read File--------------------" << endl;
	Input.open(InputDir);
	if (!Input)
	{
		cout << "File open failed!" << endl;
		system("pause");
		return 0;
	}
	streambuf* stream_buffer_cin = cin.rdbuf();
	streambuf* stream_buffer_file = Input.rdbuf();
	cin.rdbuf(stream_buffer_file);
	adjm = CreateAdjM();
	Input.close();
	cin.rdbuf(stream_buffer_cin);
	system("pause");
	do {
		system("cls");
		cout << "--------------------Shortest Path--------------------" << endl;
		cout << "[1]Dijkstra(single source)" << endl;
		cout << "[2]Floyd-Warshall(all)" << endl;
		cout << "[3]Single Target" << endl;
		cout << "[4]Single Vertex Pair" << endl;
		cout << "[5]Reachable matrix" << endl;
		cout << "Input choice(other characters to quit):";
		cin >> ch;
		switch (ch)
		{
		case '1': {
			cout << "Enter the starting vertex:";
			cin >> start;
			if (start >= adjm->n || start < 0)
				cout << "Illegal input!" << endl;
			else
			{
				Dijkstra(adjm, heap, start, S, pre, dis);
				PrintResult_D(adjm, start, S, pre, dis);
			}
		}
				break;
		case '2': {
			Floyd_Warshall(adjm, e, path);
			PrintResult_F(adjm, e, path);
		}
				break;
		case '3': {
			cout << "Enter the target vertex:";
			cin >> target;
			if (target >= adjm->n || target < 0)
				cout << "Illegal input!" << endl;
			else
			{
				Single_Target(adjm, heap, S, pre, dis, target);
				PrintResult_ST(adjm, S, pre, dis, target);
			}
		}
				break;
		case '4': {
			cout << "Enter the vertex pair:";
			cin >> start >> target;
			if (start >= adjm->n || start < 0 || target >= adjm->n || target < 0)
				cout << "Illegal input!" << endl;
			else if (start == target)
				cout << "The two vertices should be different!" << endl;
			else
			{
				Single_Vertex_Pair(adjm, heap, S, pre, dis, start, target);
				PrintResult_SVP(adjm, S, pre, dis, start, target);
				Single_Vertex_Pair(adjm, heap, S, pre, dis, target, start);
				PrintResult_SVP(adjm, S, pre, dis, target, start);
			}
		}
				break;
		case '5': Reachable_Matrix(adjm, e);
			break;
		default:
			cout << "Program terminated." << endl;
		}
		system("pause");
	} while (ch >= '1' && ch <= '5');
	return 0;
}