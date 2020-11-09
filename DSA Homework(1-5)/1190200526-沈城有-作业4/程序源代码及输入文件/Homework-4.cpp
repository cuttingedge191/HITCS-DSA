/*
* 作业4 图型结构及其应用
* 题目：图的存储结构的建立与搜索（有向图）
* 学号：1190200526
* 姓名：沈城有
*/

#include <iostream>
#include <fstream>
#include <stack>
#include <queue>
using namespace std;

#define Max_N 50
#define VertexData char         //顶点数据类型
#define EdgeData int            //边数据类型
#define InputDir "Input.txt"    //文件输入路径

//邻接矩阵数据结构
typedef struct
{
	VertexData vertex[Max_N];     //顶点表
	EdgeData edge[Max_N][Max_N];  //边表
	int n, e;                     //图的顶点数和边数
}AdjMatrix;

//邻接表数据结构
typedef struct node
{
	int adjvex;                   //邻接点域（下标）
	EdgeData cost;                //边上的权值
	struct node* next;            //指针域
}EdgeNode;

typedef struct
{
	VertexData vertex;            //顶点数据域
	EdgeNode* firstedge;          //指向边表首结点的指针域
}VertexNode;

typedef struct
{
	VertexNode VexList[Max_N];    //顶点表
	int n, e;                     //图的顶点数和边数
}AdjGraph;

bool Visited[Max_N];              //标记顶点是否访问过
int	fn[Max_N];                    //顶点编号

AdjMatrix* CreateAdjM(void);                           //创建用邻接矩阵表示的有向图
AdjGraph* CreateAdjG(void);                            //创建用（正）邻接表表示的有向图
AdjGraph* ConvertAMtoAG(AdjMatrix* adjm);              //有向图邻接矩阵转换为邻接表
AdjMatrix* ConvertAGtoAM(AdjGraph* adjg);              //有向图邻接表转换为邻接矩阵
void PrintAdj(AdjMatrix* adjm, AdjGraph* adjg);        //屏幕显示邻接矩阵、邻接表（若存在）
void DFS_R_AM_Main(AdjMatrix* adjm);                   //在有向图邻接矩阵上深搜（递归）主算法
void DFS_R_AM(AdjMatrix* adjm, int i, int& count);     //在有向图邻接矩阵上一次深搜（递归）算法
void DFS_R_AG_Main(AdjGraph* adjg);                    //在有向图邻接表上深搜（递归）主算法
void DFS_R_AG(AdjGraph* adjg, int i, int& count);      //在有向图邻接表上一次深搜（递归）算法
void DFS_NR_AM_Main(AdjMatrix* adjm);                  //在有向图邻接矩阵上深搜（非递归）主算法
void DFS_NR_AM(AdjMatrix* adjm, int i, int& count);    //在有向图邻接矩阵上一次深搜（非递归）算法
void DFS_NR_AG_Main(AdjGraph* adjg);                   //在有向图邻接表上深搜（非递归）主算法
void DFS_NR_AG(AdjGraph* adjg, int i, int& count);     //在有向图邻接表上一次深搜（非递归）算法
void BFS_AM_Main(AdjMatrix* adjm);                     //在有向图邻接矩阵上广搜主算法
void BFS_AM(AdjMatrix* adjm, int i, int& count);       //在有向图邻接矩阵上一次广搜算法
void BFS_AG_Main(AdjGraph* adjg);                      //在有向图邻接表上广搜主算法
void BFS_AG(AdjGraph* adjg, int i, int& count);        //在有向图邻接表上一次广搜算法

//创建用邻接矩阵表示的有向图
AdjMatrix* CreateAdjM(void)
{
	AdjMatrix* adj = new AdjMatrix();
	cout << "Input the number of vertex:";
	cin >> adj->n;
	cout << "Input the number of edge:";
	cin >> adj->e;
	cout << "Input vertex data:";
	for (int i = 0; i < adj->n; ++i)
		cin >> adj->vertex[i];
	for (int i = 0; i < adj->n; ++i)
	{
		for (int j = 0; j < adj->n; ++j)
		{
			adj->edge[i][j] = 0;  //邻接矩阵初始化
		}
	}
	cout << "Input edge and weight(vertex i, vertex j, weight k)(i->j):" << endl;
	for (int i = 0; i < adj->e; ++i)
	{
		int a, b, c;
		cin >> a >> b >> c;
		adj->edge[a][b] = c;
	}
	cout << "Input complete!" << endl;
	return adj;
}

//创建用邻接表表示的有向图
AdjGraph* CreateAdjG(void)
{
	AdjGraph* adj = new AdjGraph();
	EdgeNode* temp;
	int a, b, c;
	cout << "Input the number of vertex:";
	cin >> adj->n;
	cout << "Input the number of edge:";
	cin >> adj->e;
	cout << "Input vertex data:";
	for (int i = 0; i < adj->n; ++i)
	{
		cin >> adj->VexList[i].vertex;
		adj->VexList[i].firstedge = NULL;  //边表初始化
	}
	cout << "Input edge and weight(vertex i, vertex j, weight k)(i->j):" << endl;
	for (int i = 0; i < adj->e; ++i)
	{
		cin >> a >> b >> c;
		temp = adj->VexList[a].firstedge;
		adj->VexList[a].firstedge = new EdgeNode();
		adj->VexList[a].firstedge->adjvex = b;
		adj->VexList[a].firstedge->cost = c;
		adj->VexList[a].firstedge->next = temp;
	}
	cout << "Input complete!" << endl;
	return adj;
}

//有向图邻接矩阵转换为邻接表
AdjGraph* ConvertAMtoAG(AdjMatrix* adjm)
{
	AdjGraph* adjg = new AdjGraph();
	EdgeNode* temp;
	adjg->e = adjm->e;
	adjg->n = adjm->n;
	for (int i = 0; i < adjg->n; ++i)
	{
		adjg->VexList[i].vertex = adjm->vertex[i];
		adjg->VexList[i].firstedge = NULL;
	}
	for (int i = 0; i < adjg->n; ++i)
	{
		for (int j = 0; j < adjg->n; ++j)
		{
			if (adjm->edge[i][j] != 0)
			{
				temp = adjg->VexList[i].firstedge;
				adjg->VexList[i].firstedge = new EdgeNode();
				adjg->VexList[i].firstedge->adjvex = j;
				adjg->VexList[i].firstedge->cost = adjm->edge[i][j];
				adjg->VexList[i].firstedge->next = temp;
			}
		}
	}
	cout << "Convert complete!" << endl;
	return adjg;
}

//有向图邻接表转换为邻接矩阵
AdjMatrix* ConvertAGtoAM(AdjGraph* adjg)
{
	AdjMatrix* adjm = new AdjMatrix();
	EdgeNode* temp;
	adjm->e = adjg->e;
	adjm->n = adjg->n;
	for (int i = 0; i < adjm->n; ++i)
	{
		adjm->vertex[i] = adjg->VexList[i].vertex;
		temp = adjg->VexList[i].firstedge;
		while (temp != NULL)
		{
			adjm->edge[i][temp->adjvex] = temp->cost;
			temp = temp->next;
		}
	}
	cout << "Convert complete!" << endl;
	return adjm;
}

//屏幕显示邻接矩阵、邻接表（若存在）
void PrintAdj(AdjMatrix* adjm, AdjGraph* adjg)
{
	EdgeNode* pcur;  //存储当前遍历位置
	int count = 0;
	cout << "--------------------Adjacency Matrix--------------------" << endl;
	if (adjm == NULL)
		cout << "No adjacency matrix!" << endl;
	else
	{
		cout << "Vertex:";
		for (int i = 0; i < adjm->n; ++i)
			cout << "V" << i << ":" << adjm->vertex[i] << " ";
		cout << endl;
		for (int i = 0; i < adjm->n; ++i)
		{
			for (int j = 0; j < adjm->n; ++j)
				cout << adjm->edge[i][j] << " ";
			cout << endl;
		}
	}
	cout << "--------------------Adjacency Graph--------------------" << endl;
	if (adjg == NULL)
		cout << "No adjacency graph!" << endl;
	else
	{
		cout << "Vertex:";
		for (int i = 0; i < adjg->n; ++i)
			cout << "V" << i << ":" << adjg->VexList[i].vertex << " ";
		cout << endl;
		for (int i = 0; i < adjg->n; ++i)
		{
			pcur = adjg->VexList[i].firstedge;
			while (pcur)
			{
				++count;
				cout << "Edge" << count << ":" << i << "->" << pcur->adjvex << " weight:" << pcur->cost << "\t";
				if (count % 3 == 0)
					cout << endl;
				pcur = pcur->next;
			}
		}
		cout << endl;
	}
}

//在有向图邻接矩阵上深搜（递归）主算法
void DFS_R_AM_Main(AdjMatrix* adjm)
{
	if (adjm == NULL)
	{
		cout << "Error:No adjacency matrix!" << endl;
		return;
	}
	int count = 1;
	int tree = 1;
	for (int i = 0; i < adjm->n; ++i)
		Visited[i] = false;  //初始化访问数组
	for (int i = 0; i < adjm->n; ++i)
	{
		if (!Visited[i])
		{
			cout << tree << ":";
			++tree;
			DFS_R_AM(adjm, i, count);
			cout << endl;
		}
	}
	cout << "Serial:";
	for (int i = 0; i < adjm->n; ++i)
		cout << fn[i] << " ";
	cout << endl;
}

//在有向图邻接矩阵上一次深搜（递归）算法
void DFS_R_AM(AdjMatrix* adjm, int i, int& count)
{
	cout << adjm->vertex[i] << " ";
	Visited[i] = true;
	fn[i] = count++;
	for (int j = 0; j < adjm->n; ++j)
	{
		if ((adjm->edge[i][j]) && !Visited[j])
			DFS_R_AM(adjm, j, count);
	}
}

//在有向图邻接表上深搜（递归）主算法
void DFS_R_AG_Main(AdjGraph* adjg)
{
	if (adjg == NULL)
	{
		cout << "Error:No adjacency graph!" << endl;
		return;
	}
	int count = 1;
	int tree = 1;
	for (int i = 0; i < adjg->n; ++i)
		Visited[i] = false;  //初始化访问数组
	for (int i = 0; i < adjg->n; ++i)
	{
		if (!Visited[i])
		{
			cout << tree << ":";
			++tree;
			DFS_R_AG(adjg, i, count);
			cout << endl;
		}
	}
	cout << "Serial:";
	for (int i = 0; i < adjg->n; ++i)
		cout << fn[i] << " ";
	cout << endl;
}

//在有向图邻接表上一次深搜（递归）算法
void DFS_R_AG(AdjGraph* adjg, int i, int& count)
{
	EdgeNode* p;
	cout << adjg->VexList[i].vertex << " ";
	Visited[i] = true;
	fn[i] = count++;
	p = adjg->VexList[i].firstedge;
	while (p)
	{
		if (!Visited[p->adjvex])
			DFS_R_AG(adjg, p->adjvex, count);
		p = p->next;
	}
}

//在有向图邻接矩阵上深搜（非递归）主算法
void DFS_NR_AM_Main(AdjMatrix* adjm)
{
	if (adjm == NULL)
	{
		cout << "Error:No adjacency matrix!" << endl;
		return;
	}
	int count = 1;
	int tree = 1;
	for (int i = 0; i < adjm->n; ++i)
		Visited[i] = false;  //初始化访问数组
	for (int i = 0; i < adjm->n; ++i)
	{
		if (!Visited[i])
		{
			cout << tree << ":";
			++tree;
			DFS_NR_AM(adjm, i, count);
			cout << endl;
		}
	}
	cout << "Serial:";
	for (int i = 0; i < adjm->n; ++i)
		cout << fn[i] << " ";
	cout << endl;
}

//在有向图邻接矩阵上一次深搜（非递归）算法
void DFS_NR_AM(AdjMatrix* adjm, int i, int& count)
{
	stack<int> s;
	s.push(i);
	cout << adjm->vertex[s.top()] << " ";
	Visited[i] = true;
	fn[i] = count++;
	for (int j = 0; ; )
	{
		if (j == adjm->n)
		{
			if (s.empty())
				break;
			i = s.top();
			s.pop();
			j = 0;
		}
		if ((adjm->edge[i][j]) && !Visited[j])
		{
			Visited[j] = true;
			fn[j] = count++;
			s.push(j);
			cout << adjm->vertex[j] << " ";
			i = j;
			j = -1;  //这里需要是-1否则会丢失与第一个顶点的边
		}
		++j;
	}
}

//在有向图邻接表上深搜（非递归）主算法
void DFS_NR_AG_Main(AdjGraph* adjg)
{
	if (adjg == NULL)
	{
		cout << "Error:No adjacency graph!" << endl;
		return;
	}
	int count = 1;
	int tree = 1;
	for (int i = 0; i < adjg->n; ++i)
		Visited[i] = false;  //初始化访问数组
	for (int i = 0; i < adjg->n; ++i)
	{
		if (!Visited[i])
		{
			cout << tree << ":";
			++tree;
			DFS_NR_AG(adjg, i, count);
			cout << endl;
		}
	}
	cout << "Serial:";
	for (int i = 0; i < adjg->n; ++i)
		cout << fn[i] << " ";
	cout << endl;
}

//在有向图邻接表上一次深搜（非递归）算法
void DFS_NR_AG(AdjGraph* adjg, int i, int& count)
{
	stack<int> s;
	EdgeNode* p;
	int temp = i;
	s.push(i);
	cout << adjg->VexList[i].vertex << " ";
	Visited[i] = true;
	fn[i] = count++;
	while (1)
	{
		p = adjg->VexList[i].firstedge;
		while (p)
		{
			if (!Visited[p->adjvex])
			{
				s.push(p->adjvex);
				cout << adjg->VexList[p->adjvex].vertex << " ";
				Visited[p->adjvex] = true;
				fn[p->adjvex] = count++;
				if (adjg->VexList[p->adjvex].firstedge)
					p = adjg->VexList[p->adjvex].firstedge;
				else
					p = p->next;
			}
			else
				p = p->next;
		}
		if (s.empty())
			break;
		i = s.top();
		s.pop();
	}
}

//在有向图邻接矩阵上广搜主算法
void BFS_AM_Main(AdjMatrix* adjm)
{
	if (adjm == NULL)
	{
		cout << "Error:No adjacency matrix!" << endl;
		return;
	}
	int count = 1;
	int tree = 1;
	for (int i = 0; i < adjm->n; ++i)
		Visited[i] = false;  //初始化访问数组
	for (int i = 0; i < adjm->n; ++i)
	{
		if (!Visited[i])
		{
			cout << tree << ":";
			++tree;
			BFS_AM(adjm, i, count);
			cout << endl;
		}
	}
	cout << "Serial:";
	for (int i = 0; i < adjm->n; ++i)
		cout << fn[i] << " ";
	cout << endl;
}

//在有向图邻接矩阵上一次广搜算法
void BFS_AM(AdjMatrix* adjm, int i, int& count)
{
	int temp;
	queue<int> q;
	cout << adjm->vertex[i] << " ";
	Visited[i] = true;
	fn[i] = count++;
	q.push(i);
	while (!q.empty())
	{
		temp = q.front();
		q.pop();
		for (int j = 0; j < adjm->n; ++j)
		{
			if (adjm->edge[temp][j] && !Visited[j])
			{
				cout << adjm->vertex[j] << " ";
				Visited[j] = true;
				fn[j] = count++;
				q.push(j);
			}
		}
	}
}

//在有向图邻接表上广搜主算法
void BFS_AG_Main(AdjGraph* adjg)
{
	if (adjg == NULL)
	{
		cout << "Error:No adjacency graph!" << endl;
		return;
	}
	int count = 1;
	int tree = 1;
	for (int i = 0; i < adjg->n; ++i)
		Visited[i] = false;  //初始化访问数组
	for (int i = 0; i < adjg->n; ++i)
	{
		if (!Visited[i])
		{
			cout << tree << ":";
			++tree;
			BFS_AG(adjg, i, count);
			cout << endl;
		}
	}
	cout << "Serial:";
	for (int i = 0; i < adjg->n; ++i)
		cout << fn[i] << " ";
	cout << endl;
}

//在有向图邻接表上一次广搜算法
void BFS_AG(AdjGraph* adjg, int i, int& count)
{
	int temp;
	EdgeNode* p;
	queue<int> q;
	cout << adjg->VexList[i].vertex << " ";
	Visited[i] = true;
	fn[i] = count++;
	q.push(i);
	while (!q.empty())
	{
		temp = q.front();
		q.pop();
		p = adjg->VexList[temp].firstedge;
		while (p)
		{
			if (!Visited[p->adjvex])
			{
				cout << adjg->VexList[p->adjvex].vertex << " ";
				Visited[p->adjvex] = true;
				fn[p->adjvex] = count++;
				q.push(p->adjvex); 
			}
			p = p->next;
		}
	}
}

int main(void)
{
	char ch;
	AdjMatrix* adjm = NULL;
	AdjGraph* adjg = NULL;
	do {
		system("cls");
		cout << "--------------------Directed Graph--------------------" << endl;
		cout << "[1]Input data to create adjacency matrix" << endl;
		cout << "[2]Input data to create adjacency graph" << endl;
		cout << "[3]Read data from file" << endl;
		cout << "[4]Convert adjacency matrix -> adjacency graph" << endl;
		cout << "[5]Convert adjacency graph -> adjacency matrix" << endl;
		cout << "[6]Print adjacency graph and adjacency matrix" << endl;
		cout << "[7]Recursive DFS" << endl;
		cout << "[8]Non-recursive DFS" << endl;
		cout << "[9]BFS" << endl;
		cout << "Input choice(other characters to quit):";
		cin >> ch;
		switch (ch)
		{
		case '1': {
			adjm = CreateAdjM();
			if (adjm == NULL)
				cout << "Error:Create adjacency matrix failed!" << endl;
		}
				break;
		case '2': {
			adjg = CreateAdjG();
			if (adjg == NULL)
				cout << "Error:Create adjacency graph failed!" << endl;
		}
				break;
		case '3': {//输入重定向
			ifstream Input;
			Input.open(InputDir);
			if (!Input)
				cout << "File open failed!" << endl;
			else
			{
				cout << "[1]Adjacency matrix\t[2]Adjacency graph:";
				cin >> ch;
				streambuf* stream_buffer_cin = cin.rdbuf();
				streambuf* stream_buffer_file = Input.rdbuf();
				cin.rdbuf(stream_buffer_file);
				if (ch == '1')
					adjm = CreateAdjM();
				else if (ch == '2')
					adjg = CreateAdjG();
				else ch = '1';
				Input.close();
				cin.rdbuf(stream_buffer_cin);
			}
		}
				break;
		case '4':{
			if (adjm == NULL)
				cout << "Error:No adjacency matrix!" << endl;
			else
				adjg = ConvertAMtoAG(adjm);
		}
				break;
		case '5': {
			if (adjg == NULL)
				cout << "Error:No adjacency graph!" << endl;
			else
				adjm = ConvertAGtoAM(adjg);
		}
				break;
		case '6': PrintAdj(adjm, adjg);
			break;
		case '7': {
			cout << "[1]Adjacency matrix\t[2]Adjacency graph:";
			cin >> ch;
			if (ch == '1')
				DFS_R_AM_Main(adjm);
			else if (ch == '2')
				DFS_R_AG_Main(adjg);
			else ch = '1';
		}
				break;
		case '8': {
			cout << "[1]Adjacency matrix\t[2]Adjacency graph:";
			cin >> ch;
			if (ch == '1')
				DFS_NR_AM_Main(adjm);
			else if (ch == '2')
				DFS_NR_AG_Main(adjg);
			else ch = '1';
		}
				break;
		case '9': {
			cout << "[1]Adjacency matrix\t[2]Adjacency graph:";
			cin >> ch;
			if (ch == '1')
				BFS_AM_Main(adjm);
			else if (ch == '2')
				BFS_AG_Main(adjg);
			else ch = '1';
		}
				break;
		default:
			cout << "Program terminated." << endl;
		}
		system("pause");
	} while (ch >= '1' && ch <= '9');
	return 0;
}