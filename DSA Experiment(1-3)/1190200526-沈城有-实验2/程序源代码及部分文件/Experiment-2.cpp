/*
* ʵ��2 ���νṹ����Ӧ��
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
using namespace std;
#define InputDir "D:\\Input.txt"      //Ӣ���ı�·��
#define CompDir "D:\\Comp.ht"         //ѹ���ı�����·����д��+��ȡ��
#define DeCompDir "D:\\Decomp.txt"    //��ѹ�ı�����·����д�룩
#define N 256                         //���Ҷ�������ַ���������
#define M 2 * N - 1                   //���������

typedef struct htnode
{
	double weight;
	int lchild;
	int rchild;
	int parent;
}HTNode;

typedef struct codingtable
{
	int num;
	char ch;
	string code;
}CodeTab;

HTNode HTree[M];           //����������̬��������
map<char, double> result;  //ʹ��STL�е�mapͳ����Ϣ
CodeTab CT[N];             //�洢�����

//����ʵ����غ�������
int ReadFromFile(void);                                        //���ļ��ж�ȡ��ͳ����Ϣ
void ShowStat(void);                                           //��ʾͳ����Ϣ
void HTNodeInit(void);                                         //��ͳ�����ݳ�ʼ�����ɽ��
void SelectMin(int n, int& p1, int& p2);                       //ѡȡ������СȨֵ
void CreateHT(void);                                           //�����������
bool cmp(CodeTab a, CodeTab b);                                //sort����ȽϺ���
void PrintCodingTable(void);                                   //��Ļ��ʾ�����
bool CompressFile(void);                                       //���ݱ�������ѹ���ļ�
void CalculateRate(void);                                      //����ѹ����
bool DecompressFile(void);                                     //ѹ���ļ�����

//���ļ��ж�ȡ��ͳ����Ϣ
int ReadFromFile(void)
{
	ifstream OpenFile(InputDir);
	map<char, int> data;
	map<char, int>::iterator it;
	char temp;
	int countt = 0;
	double rate;
	if (!OpenFile)
	{
		cout << "File open failed!" << endl;
		return 0;
	}
	while (OpenFile.get(temp))
	{
		if (temp == '\n')
			temp = '#';  //ת�����з�������ʾ
		it = data.find(temp);
		if (it != data.end())
			++it->second;
		else
			data.insert(pair<char, int>(temp, 1));
	}
	OpenFile.close();
	for (it = data.begin(); it != data.end(); ++it)
		countt += it->second;
	for (it = data.begin(); it != data.end(); ++it)
	{
		rate = (double)it->second / (double)countt;
		result.insert(pair<char, double>(it->first, rate));
	}
	cout << "File read and analysis completed." << endl;
	return countt;
}

//��ʾͳ����Ϣ
void ShowStat(void)
{
	int countn = 0;    //���ַ�������
	map<char, double>::iterator it;
	cout << "-----------------------------------Statistics-----------------------------------" << endl;
	for (it = result.begin(); it != result.end(); ++it)
	{
		cout << "|" << it->first << "|" << ":" << it->second;
		cout << "\t";
		++countn;
		if (countn % 5 == 0)
			cout << endl;
	}
	cout << "\n[# stands for newline.]" << endl;
	cout << "Number of type:" << countn << endl;
}

//��ͳ�����ݳ�ʼ�����ɽ��
void HTNodeInit(void)
{
	map<char, double>::iterator it;
	int i = 0;
	for (it = result.begin(); it != result.end(); ++it)
	{
		HTree[i].weight = it->second;
		HTree[i].parent = -1;
		HTree[i].lchild = -1;
		HTree[i].rchild = -1;
		++i;
	}
	for (; i < M; ++i)
	{
		HTree[i].parent = -1;
		HTree[i].lchild = -1;
		HTree[i].rchild = -1;
	}
}

//ѡȡ������СȨֵ
void SelectMin(int n, int& p1, int& p2)
{
	int i, j;
	for (i = 0; i < n; ++i)
	{
		if (HTree[i].parent == -1)
		{
			p1 = i;
			break;
		}
	}
	for (j = i + 1; j < n; ++j)
	{
		if (HTree[j].parent == -1)
		{
			p2 = j;
			break;
		}
	}
	for (i = 0; i < n; ++i)
	{
		if ((HTree[p1].weight > HTree[i].weight) && (HTree[i].parent == -1) && (p2 != i))
			p1 = i;
	}
	for (j = 0; j < n; ++j)
	{
		if ((HTree[p2].weight > HTree[j].weight) && (HTree[j].parent == -1) && (p1 != j))
			p2 = j;
	}
}

//�����������
void CreateHT(void)
{
	int i, p1, p2;
	int maxn = result.size();
	HTNodeInit();
	for (i = maxn; i < 2 * maxn - 1; ++i)
	{
		SelectMin(i, p1, p2);
		HTree[p1].parent = HTree[p2].parent = i;
		HTree[i].lchild = p1;
		HTree[i].rchild = p2;
		HTree[i].weight = HTree[p1].weight + HTree[p2].weight;
	}
}

//sort����ȽϺ���
bool cmp(CodeTab a, CodeTab b)
{
	return HTree[a.num].weight > HTree[b.num].weight;
}

//��Ļ��ʾ�����
void PrintCodingTable(void)
{
	int c, p, i;
	int max = result.size();
	char code[100];
	map<char, double>::iterator it;
	int start;
	string temp;
	for (i = 0, it = result.begin(); i < max, it != result.end(); ++i, ++it)
	{
		CT[i].ch = it->first;
		CT[i].num = i;
	}
	sort(CT, CT + max, cmp);
	for (i = 0; i < max; ++i)
	{
		c = CT[i].num;
		start = 99;
		temp = "";
		while ((p = HTree[c].parent) >= 0)
		{
			code[--start] = (HTree[p].lchild == c) ? '0' : '1';
			c = p;  //��������;
		}
		for (int j = start; j < 99; ++j)
		{
			temp += code[j];
		}
		CT[i].code = temp;
	}
	for (int k = 1; k <= max; ++k)
	{
		cout << "|" << CT[k - 1].ch << "|:" << CT[k - 1].code;
		cout << " \t";
		if (k % 5 == 0 && k)
			cout << endl;
	}
}

//���ݱ�������ѹ���ļ�
bool CompressFile(void)
{
	ifstream ReadFile;
	ofstream WriteFile;
	char temp;
	char bin = 0;       //�������浱ǰ��huffman����
	int i = 0;         //��¼����λ������8��дһ�Σ�
	int q;
	ReadFile.open(InputDir);
	WriteFile.open(CompDir, ios::binary);
	string::iterator it;
	map<char, double>::iterator mt;
	if (!ReadFile || !WriteFile)
	{
		cout << "File open failed!" << endl;
		return false;
	}
	//����д������Ƚ�ѹ��������Ϣ
	WriteFile << result.size();  //�ַ�������
	for (mt = result.begin(); mt != result.end(); ++mt)
		WriteFile << mt->first;  //д�����������ַ�
	for (unsigned int k = 0; k < 2 * result.size() - 1; ++k)  //д�����������lchild��rchild������������ݹ���ȥparent)
		WriteFile << " " << HTree[k].lchild << " " << HTree[k].rchild;
    while(ReadFile.get(temp))
	{
		for (q = 0; q < (int)result.size(); ++q)
		{
			if (temp == '\n')
				temp = '#';   //ת�����з�
			if (CT[q].ch == temp)
				break;
		}
		for(it = CT[q].code.begin(); it != CT[q].code.end(); ++it)
		{//01��ת�����ƴ���
			if (*it == '0')
				bin = bin << 1;
			else
				bin = (bin << 1) + 1;
			++i;
			if(i == 8)
			{
				WriteFile << bin;
				i = 0;
				bin = 0;
            }
        }
    }
	q = (8 - i) % 8;  //����ĩβ������
	bin = bin << q;
	WriteFile << bin << q;
	ReadFile.close();
	WriteFile.close();
	result.clear();
	cout << "File compression completed." << endl;
	return true;
}

//����ѹ����
void CalculateRate(void)
{
	double TextSize;
	double ZipSize;
	double rate;
	struct _stat textinfo, zipinfo;
	_stat(InputDir, &textinfo);
	_stat(CompDir, &zipinfo);
	TextSize = textinfo.st_size;
	ZipSize = zipinfo.st_size;
	rate = ZipSize / TextSize * 100;
	cout << "The size of original file:" << TextSize << endl;
	cout << "The size of compressed file:" << ZipSize << endl;
	cout << "compression rate:" << rate << "%" << endl;
}

//ѹ���ļ�����
bool DecompressFile(void)
{
	ifstream ReadFile;
	ofstream WriteFile;
	int count;       //�����ַ�������
	int flag = -1;
	int n;           //������ĩβ����0
	char b1, b2, b3; //������ļ��ж�ȡ����,����Ƿ����һ����Ҫ����0
	char test[5];    //����ļ�ĩβ
	int i = 0;       //λ������
	int end;         //������������������,����Ϊ�м�������봦��
	ReadFile.open(CompDir, ios::binary);
	WriteFile.open(DeCompDir);
	if (!ReadFile || !WriteFile)
	{
		cout << "File open failed!" << endl;
		return false;
	}
	//���ȶ�������Ƚ�ѹ��������Ϣ
	ReadFile >> count;
	ReadFile >> noskipws;  //������Կո�
	for (int k = 0; k < count; ++k)
		ReadFile >> CT[k].ch;
	end = 2 * count - 2;
	ReadFile >> skipws;
	for (int k = 0; k <= end; ++k)
		ReadFile >> HTree[k].lchild >> HTree[k].rchild;
	ReadFile >> b1 >> b2 >> b3;
	do{
		if (!((b1 >> (7 - i)) & 1))
			end = HTree[end].lchild;
		else
			end = HTree[end].rchild;
		++i;
		if (end < 0)
			return false;
		if (end < count)
		{
			if (CT[end].ch != '#')
				WriteFile << CT[end].ch;
			else
				WriteFile << "\n";
			end = 2 * count - 2;
		}
		if (i == 8)
		{
			i = 0;
			b1 = b2;
			b2 = b3;
			if (flag >= 0)
			{
				b3 = test[flag];
				flag = -1;
			}
			else if (!ReadFile.get(b3))
			{
				for (int k = 0; k < 5; ++k)
				{
					if (ReadFile.get(test[k]))
						flag = k;
				}
				if (flag == -1)
					break;
			}
		}
	}while(1);
	n = b2 - '0';
	if (end < 0)
		return false;
	while (i <= 7 - n)
	{
		if (!((b1 >> (7 - i)) & 1))
			end = HTree[end].lchild;
		if ((b1 >> (7 - i)) & 1)
			end = HTree[end].rchild;
		++i;
		if (end < count)
		{
			if (CT[end].ch != '#')
				WriteFile << CT[end].ch;
			else
				WriteFile << endl;
			end = 2 * count - 2;
		}
	}
	ReadFile.close();
	WriteFile.close();
	cout << "File decompress completed." << endl;
	return true;
}

int main(void)
{
	int countt;
	char ch;
	cout << "------------------------------Huffman File Process------------------------------" << endl;
	cout << "[1] File Compress\n" << "[2] File Decompress" << endl;
	cout << "Input your choice(other keys to exit):";
	ch = getchar();
	if (ch == '1')
	{
		countt = ReadFromFile();
		if (!countt)
		{
			cout << "Illegal operation or empty file!" << endl;
			return 0;
		}
		ShowStat();
		cout << "Total characters:" << countt << endl;
		system("pause");
		CreateHT();
		cout << "-----------------------------------Code Table-----------------------------------" << endl;
		PrintCodingTable();
		cout << endl;
		system("pause");
		if (!CompressFile())
		{
			cout << "Error:File compress failed!" << endl;
			return 0;
		}
		CalculateRate();
		system("pause");
	}
	else if (ch == '2')
	{
		if (!DecompressFile())
			cout << "Error:File decompress failed!" << endl;
		system("pause");
	}
	return 0;
}
