/*
* ʵ��2 ���ͽṹ����Ӧ�ã�˼�����֣�
* ʵ����Ŀ�����νṹ����Ӧ��
* ʵ����Ŀ�����������������뷽��
* ѧ�ţ�1190200526
* �����������
*/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <string>
#include <cmath>
using namespace std;
#define InputDir "Input.txt"      //Ӣ���ı�·��
#define CompDir "Comp.ht"         //�����ı�����·��
#define N 256                     //���Ҷ�������ַ���������
#define Max_K 32                  //���kֵ
#define M Max_K * N - 1           //���������

//��С�����ݽṹʵ��
typedef struct heap
{
	int data[M + 1][2];  //���+Ƶ��
	int n;
}Heap;

//K������������
typedef struct childnode
{
	int child;
	childnode* next;
}HTC;

//K����������ṹ����
typedef struct K_htnode
{
	int parent;
	int freq;
	HTC head;  //���������ͷ
}HTNode;

//�ַ������
typedef struct codingtablec
{
	int num;
	char ch;
	string code;
}CodeTab_C;

//���ʱ����
typedef struct codingtables
{
	int num;
	string word;
	string ch;
}CodeTab_S;

//���ݽṹ��ػ�����������
void InitHeap(Heap& heap);
bool HeapEmpty(Heap heap);
bool HeapFull(Heap heap);
void HeapInsert(Heap& heap, int num, int freq);
void HeapDeleteMin(Heap& heap, int& num);
//����ʵ����غ�������
int ReadFromFile(char ch);                             //���ļ��ж�ȡ��ͳ����Ϣ
void CreateHeap(char ch, Heap& heap);                  //������ʼ��
HTC* HTCNodeInit(int k);                               //��ʼ��K������������          
int HTNodeInit(char ch);                               //��ͳ�����ݳ�ʼ�����ɽ��
void SelectMin(Heap& heap, int k, int num[]);          //ѡȡk����СȨֵ�����öѣ�
void CreateHT(Heap heap, char ch, int& k);             //�����������
void ProcessCodeTable(char ch);                        //���ݹ����������ɲ���������
void ShowInfo(char ch);                                //��ʾͳ����Ϣ�������
void EncodeFile(void);                                 //���ݱ�������ѹ���ļ�
void CalculateRate(int countt, int k);                 //����ѹ����

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

HTNode HTree[M];           //K���������
map<char, int> resultc;    //ͳ���ַ���
map<string, int> results;  //ͳ�Ƶ�����
CodeTab_C CT_C[N];         //�洢�ַ������
CodeTab_S CT_S[N];         //�洢���ʱ����

//���ļ��ж�ȡ��ͳ����Ϣ
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
				temp = '#';  //ת�����з�������ʾ
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
	}*/  //���ʲ���
}

//������ʼ��
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

//��ʼ��K������������
HTC* HTCNodeInit(int k)
{
	HTC* pcur;
	HTC* q;
	q = (HTC*)malloc(sizeof(HTC));  //��¼��һ�����
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

//��ͳ�����ݳ�ʼ�����ɽ��
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

//ѡȡk����СȨֵ�����öѣ�
void SelectMin(Heap& heap, int k, int num[])
{
	for (int i = 0; i < k; ++i)
		HeapDeleteMin(heap, num[i]);
}

//�����������
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
		if ((maxn - 1) % (k - 1))  //���伸��������ĵ��Ա�֤ÿ�ζ���ȡk��
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

//sort����ȽϺ���1
bool cmp_1(CodeTab_C a, CodeTab_C b)
{
	return HTree[a.num].freq > HTree[b.num].freq;
}

//sort����ȽϺ���2
bool cmp_2(CodeTab_S a, CodeTab_S b)
{
	return HTree[a.num].freq > HTree[b.num].freq;
}

//���ݹ����������ɲ���������
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
				c = p;  //��������;
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

//��ʾͳ����Ϣ
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

//���ݱ�������ѹ���ļ�
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
			temp = '#';   //ת�����з�
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

//����ѹ����
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