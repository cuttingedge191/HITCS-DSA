/*
* 实验2 树形结构及其应用
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
using namespace std;
#define InputDir "D:\\Input.txt"      //英文文本路径
#define CompDir "D:\\Comp.ht"         //压缩文本保存路径（写入+读取）
#define DeCompDir "D:\\Decomp.txt"    //解压文本保存路径（写入）
#define N 256                         //最大叶子数（字符种类数）
#define M 2 * N - 1                   //最大结点总数

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

HTNode HTree[M];           //哈夫曼树静态三叉链表
map<char, double> result;  //使用STL中的map统计信息
CodeTab CT[N];             //存储编码表

//功能实现相关函数声明
int ReadFromFile(void);                                        //从文件中读取并统计信息
void ShowStat(void);                                           //显示统计信息
void HTNodeInit(void);                                         //用统计数据初始化生成结点
void SelectMin(int n, int& p1, int& p2);                       //选取两个最小权值
void CreateHT(void);                                           //构造哈夫曼树
bool cmp(CodeTab a, CodeTab b);                                //sort所需比较函数
void PrintCodingTable(void);                                   //屏幕显示编码表
bool CompressFile(void);                                       //根据编码生成压缩文件
void CalculateRate(void);                                      //计算压缩率
bool DecompressFile(void);                                     //压缩文件解码

//从文件中读取并统计信息
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
			temp = '#';  //转换换行符便于显示
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

//显示统计信息
void ShowStat(void)
{
	int countn = 0;    //总字符种类数
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

//用统计数据初始化生成结点
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

//选取两个最小权值
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

//构造哈夫曼树
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

//sort所需比较函数
bool cmp(CodeTab a, CodeTab b)
{
	return HTree[a.num].weight > HTree[b.num].weight;
}

//屏幕显示编码表
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
			c = p;  //继续上溯;
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

//根据编码生成压缩文件
bool CompressFile(void)
{
	ifstream ReadFile;
	ofstream WriteFile;
	char temp;
	char bin = 0;       //用来储存当前的huffman编码
	int i = 0;         //记录处理位数（到8就写一次）
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
	//首先写入编码表等解压缩所需信息
	WriteFile << result.size();  //字符种类数
	for (mt = result.begin(); mt != result.end(); ++mt)
		WriteFile << mt->first;  //写入所有种类字符
	for (unsigned int k = 0; k < 2 * result.size() - 1; ++k)  //写入哈夫曼树中lchild、rchild（解码无需回溯故略去parent)
		WriteFile << " " << HTree[k].lchild << " " << HTree[k].rchild;
    while(ReadFile.get(temp))
	{
		for (q = 0; q < (int)result.size(); ++q)
		{
			if (temp == '\n')
				temp = '#';   //转换换行符
			if (CT[q].ch == temp)
				break;
		}
		for(it = CT[q].code.begin(); it != CT[q].code.end(); ++it)
		{//01串转二进制处理
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
	q = (8 - i) % 8;  //计算末尾补零数
	bin = bin << q;
	WriteFile << bin << q;
	ReadFile.close();
	WriteFile.close();
	result.clear();
	cout << "File compression completed." << endl;
	return true;
}

//计算压缩率
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

//压缩文件解码
bool DecompressFile(void)
{
	ifstream ReadFile;
	ofstream WriteFile;
	int count;       //读入字符类型数
	int flag = -1;
	int n;           //保存在末尾补的0
	char b1, b2, b3; //储存从文件中读取的数,检测是否到最后一个需要处理补0
	char test[5];    //检测文件末尾
	int i = 0;       //位数计数
	int end;         //保存哈夫曼树结点总数,并作为中间变量参与处理
	ReadFile.open(CompDir, ios::binary);
	WriteFile.open(DeCompDir);
	if (!ReadFile || !WriteFile)
	{
		cout << "File open failed!" << endl;
		return false;
	}
	//首先读入编码表等解压缩所需信息
	ReadFile >> count;
	ReadFile >> noskipws;  //避免忽略空格
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
