/*
* ʵ��1 ���Խṹ��Ӧ��
* ʵ����Ŀ�����Ա����ʽ�洢�ṹ��Ӧ��
* ʵ����Ŀ��һԪ����ʽ������
* ѧ�ţ�1190200526
* �����������
*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#define FileDir1 "D:\\poly.txt"                         //�����ļ�·��
#define FileDir2 "D:\\output.txt"                       //����ļ�·��

struct LinkNode
{
	int numer;  //����
	int deno;   //��ĸ
	int expo;   //ָ��
	LinkNode* next;
};
typedef LinkNode* Poly;

Poly CreateNewPoly(void);                                //�����¶���ʽ��������ͷ��
void PolyReverse(Poly& L);                               //����ʽ�������ú���
void AddNode(Poly& L, int numer, int deno, int expo);    //����ʽ�����
void DeleteNode(Poly& L, Poly& p);                       //����ʽɾ����
int PolyFRead(Poly& L1, Poly& L2);                       //����ʽ�ļ����뺯��
int PolyFWrite(Poly OUT, int n);                         //����ʽ�ļ��������
void Swap(int& a, int& b);                               //Ԫ�ؽ���
void PolySort(Poly& L);                                  //����ʽָ���������д�����
void PolyPrint(Poly& L);                                 //����ʽ��ʾ����
void PolyAdd(Poly L1, Poly L2, Poly& OUT);               //����ʽ�ӷ�����
void PolySub(Poly L1, Poly L2, Poly& OUT);               //����ʽ��������
void PolyMuti(Poly L1, Poly L2, Poly& OUT);              //����ʽ�˷�����
void PolyDiv(Poly L1, Poly L2, Poly& OUT, Poly& REM);    //����ʽ��������
void PolyDiff(Poly& L);                                  //����ʽ�󵼺���
float PolyValue(Poly L, float x);                        //����ʽ��ֵ����
void FracSimp(int& numer, int& deno);                    //����ϵ��������

//�����¶���ʽ��������ͷ��
Poly CreateNewPoly(void)
{
	Poly L;
	L = (LinkNode*)malloc(sizeof(LinkNode));
	if (L == NULL)
	{
		printf("Memory allocation failed!\n");
		return NULL;
	}
	L->next = NULL;
	return L;
}

//����ʽ�������ú���
void PolyReverse(Poly& L)
{
	Poly p, q;
	p = L->next;
	if (p)
	{
		q = p->next;
		p->next = NULL;
		while (q != NULL)
		{
			p = q;
			q = q->next;
			p->next = L->next;
			L->next = p;
		}
	}
}

//����ʽ�����
void AddNode(Poly& L, int numer, int deno, int expo)
{
	Poly temp;
	temp = L->next;
	L->next = (LinkNode*)malloc(sizeof(LinkNode));
	if (L->next == NULL)
	{
		printf("Memory allocation failed!\n");
		return;
	}
	L->next->numer = numer;
	L->next->deno = deno;
	L->next->expo = expo;
	L->next->next = temp;
}

//����ʽɾ����
void DeleteNode(Poly& L, Poly& p)
{
	Poly q;
	if (p->next != NULL)
	{
		q = p->next;
		p->next = q->next;
		free(q);
	}
}

//����ʽ�ļ����뺯��
int PolyFRead(Poly& L1, Poly& L2)
{
	FILE* fp;
	char ch;
	int value[3] = { 0, 1, 0 };
	fp = fopen(FileDir1, "r");
	if (fp == NULL)
	{
		printf("File open failed!\n");
		return 0;
	}
	do {
		fscanf(fp, "%d %d %d", &value[0], &value[1], &value[2]);
		AddNode(L1, value[0], value[1], value[2]);
		ch = fgetc(fp);
	}while(ch != '#');
	do {
		fscanf(fp, "%d %d %d", &value[0], &value[1], &value[2]);
		AddNode(L2, value[0], value[1], value[2]);
		ch = fgetc(fp);
	} while (ch != '#');
	fclose(fp);
	return 1;
}

//����ʽ�ļ����������
int PolyFWrite(Poly OUT, int n) 
{
	FILE* fp;
	fp = fopen(FileDir2, "a");
	Poly p = OUT;
	int test = 1;
	if (fp == NULL)
	{
		printf("File write failure!\n");
		return 0;
	}
	switch (n)
	{
	case 1:
		fprintf(fp, "Polynomial1:");
		break;
	case 2:
		fprintf(fp, "Polynomial2:");
		break;
	case 3:
		fprintf(fp, "Result of [1st polynomial] + [2nd polynomial]:");
		break;
	case 4:
		fprintf(fp, "Result of [1st polynomial] - [2nd polynomial]:");
		break;
	case 5:
		fprintf(fp, "Result of [1st polynomial] * [2nd polynomial]:");
		break;
	case 6:
		fprintf(fp, "Quotient polynomial of [1st polynomial] / [2nd polynomial]:");
		break;
	case 7:
		fprintf(fp, "Remain polynomial of [1st polynomial] / [2nd polynomial]:");
		break;
	default:
		return 0;
	}
	while (p->next != NULL)
	{
		p = p->next;
		if (test)
			test = 0;
		else if (p->numer > 0)
			fprintf(fp, "+");
		if (p->numer != 1 || p->deno != 1)
			fprintf(fp, "%d", p->numer);
		if (p->deno != 1)
			fprintf(fp, "/%d", p->deno);
		if (p->expo != 0)
		{
			fprintf(fp, "x");
			if (p->expo != 1)
				fprintf(fp, "^%d", p->expo);
		}
	}
	fprintf(fp, "\n");
	fclose(fp);
	return 1;
}

//Ԫ�ؽ���
void Swap(int& a, int& b)
{
	int temp;
	temp = a;
	a = b;
	b = temp;
}

//����ʽָ���������д�����
void PolySort(Poly& L)
{
	Poly p = NULL;
	Poly q = NULL;
	Poly t = NULL;
	if (L == NULL || L->next == NULL)
		return;
	for (p = L->next; p != NULL; p = p->next)
	{
		t = p;
		for (q = p->next; q != NULL; q = q->next)
		{
			if (q->expo > t->expo)
				t = q;
		}
		if (t != p)
		{
			Swap(t->expo, p->expo);
			Swap(t->numer, p->numer);
			Swap(t->deno, p->deno);
		}
	}
	for (p = L->next; p != NULL; p = p->next)
	{
		FracSimp(p->numer, p->deno);
	}
	t = L;
	for (p = L->next; p != NULL; )
	{
		if (p->next != NULL)
		{
			if (p->expo == p->next->expo)
			{
				p->numer = p->numer * p->next->deno + p->next->numer * p->deno;
				p->deno = p->deno * p->next->deno;
				FracSimp(p->numer, p->deno);
				DeleteNode(L, p);
			}
		}
		if (p->numer == 0)
			DeleteNode(L, t);
		else if (p->deno < 0)
		{
			p->deno = 0 - p->deno;
			p->numer = 0 - p->numer;
		}
		t = t->next;
		p = t->next;
	}
}

//����ϵ��������
void FracSimp(int& numer, int& deno)
{
	int temp, r;
	int n = numer;
	int m = deno;
	if (n < m)
	{
		temp = n;
		n = m;
		m = temp;
	}
	while (m != 0)
	{
		r = n % m;
		n = m;
		m = r;
	}
	numer /= n;
	deno /= n;
}

//����ʽ��ʾ����
void PolyPrint(Poly& L)
{
	Poly p = L;
	int test = 1;
	while (p->next != NULL)
	{
		p = p->next;
		if (test)
			test = 0;
		else if(p->numer > 0)
			printf("+");
		if (p->numer != 1 || p->deno != 1)
			printf("%d", p->numer);
		if (p->deno != 1)
			printf("/%d", p->deno);
		if (p->expo != 0)
		{
			printf("x");
			if (p->expo != 1)
				printf("^%d", p->expo);
		}
	}
}

//����ʽ�ӷ�����
void PolyAdd(Poly L1, Poly L2, Poly& OUT)
{
	Poly p = L1->next;
	Poly q = L2->next;
	Poly temp;
	temp = CreateNewPoly();
	int numer, deno;
	while (p != NULL && q != NULL)
	{
		if (p->expo == q->expo)
		{
			numer = p->numer * q->deno + q->numer * p->deno;
			deno = p->deno * q->deno;
			FracSimp(numer, deno);
			AddNode(temp, numer, deno, p->expo);
			p = p->next;
			q = q->next;
		}
		else if (p->expo > q->expo)
		{
			AddNode(temp, p->numer, p->deno, p->expo);
			p = p->next;
		}
		else if (p->expo < q->expo)
		{
			AddNode(temp, q->numer, q->deno, q->expo);
			q = q->next;
		}
	}
	while (p != NULL)
	{
		AddNode(temp, p->numer, p->deno, p->expo);
		p = p->next;
	}
	while (q != NULL)
	{
		AddNode(temp, q->numer, q->deno, q->expo);
		q = q->next;
	}
	OUT = temp;
	PolySort(OUT);
}

//����ʽ��������
void PolySub(Poly L1, Poly L2, Poly& OUT)
{
	Poly p = L1->next;
	Poly q = L2->next;
	Poly temp;
	temp = CreateNewPoly();
	int numer, deno;
	while (p != NULL && q != NULL)
	{
		if (p->expo == q->expo)
		{
			numer = p->numer * q->deno - q->numer * p->deno;
			deno = p->deno * q->deno;
			FracSimp(numer, deno);
			AddNode(temp, numer, deno, p->expo);
			p = p->next;
			q = q->next;
		}
		else if (p->expo > q->expo)
		{
			AddNode(temp, p->numer, p->deno, p->expo);
			p = p->next;
		}
		else if (p->expo < q->expo)
		{
			AddNode(temp, 0 - q->numer, q->deno, q->expo);
			q = q->next;
		}
	}
	while (p != NULL)
	{
		AddNode(temp, p->numer, p->deno, p->expo);
		p = p->next;
	}
	while (q != NULL)
	{
		AddNode(temp, 0 - q->numer, q->deno, q->expo);
		q = q->next;
	}
	OUT = temp;
	PolySort(OUT);
}

//����ʽ�˷�����
void PolyMuti(Poly L1, Poly L2, Poly& OUT)
{
	Poly p, q;
	Poly C1, C2;   //C1�Ǳ�ͷ��C2������ʱ�����������������
	Poly C3 = (Poly)malloc(sizeof(LinkNode));   //������ʱ��������
	Poly temp;  //������������
	C3->next = NULL;
	int expo, numer, deno;
	int count = 0;
	OUT = (Poly)malloc(sizeof(LinkNode));
	OUT->next = NULL;
	p = L1->next;
	while (p != NULL)
	{
		OUT = (Poly)malloc(sizeof(LinkNode));
		OUT->next = NULL;
		C1 = (Poly)malloc(sizeof(LinkNode));
		C1->next = NULL;
		temp = C1;
		q = L2->next;
		while (q != NULL)
		{
			C2 = (Poly)malloc(sizeof(LinkNode));
			expo = p->expo + q->expo;
			numer = p->numer * q->numer;
			deno = p->deno * q->deno;
			FracSimp(numer, deno);
			C2->expo = expo;
			C2->numer = numer;
			C2->deno = deno;
			temp->next = C2;
			temp = temp->next;
			q = q->next;
		}
		p = p->next;
		++count;
		temp->next = NULL;
		PolyAdd(C1, C3, OUT);
		C3 = OUT;
	}
	PolySort(OUT);
}

//����ʽ��������
void PolyDiv(Poly L1, Poly L2, Poly& OUT, Poly& REM)
{
	Poly p = L1->next;
	Poly q = L2->next;
	Poly temp1, temp2;
	OUT = CreateNewPoly();
	REM = CreateNewPoly();
	temp1 = CreateNewPoly();
	temp2 = CreateNewPoly();
	PolyAdd(L1, temp1, temp1);
	while (p->expo >= q->expo)
	{
		temp2->next = (Poly)malloc(sizeof(LinkNode));
		temp2->next->numer = p->numer * q->deno;
		temp2->next->deno = p->deno * q->numer;
		FracSimp(temp2->next->numer, temp2->next->deno);
		temp2->next->expo = p->expo - q->expo;
		temp2->next->next = NULL;
		AddNode(OUT, temp2->next->numer, temp2->next->deno, temp2->next->expo);
		PolyMuti(L2, temp2, temp2);
		PolySub(L1, temp2, L1);
		p = L1->next;
		temp2->next = NULL;
	}
	PolyMuti(L2, OUT, temp2);    //���ó˷�
	PolySub(temp1, temp2, REM);  //���ü���������
	PolySort(OUT);
	PolySort(REM);
}

//����ʽ�󵼺���
void PolyDiff(Poly& L)
{
	Poly p, q, pt;
	p = L;
	q = L->next;
	while(q != NULL)
	{
		if (q->expo == 0)
		{
			pt = q;
			q = q->next;
			p->next = q;
			free(pt);
		}
		else
		{
			q->numer *= q->expo;
			FracSimp(q->numer, q->deno);
			--q->expo;
			p = q;
			q = q->next;
		}
	}
	PolySort(L);
}

//����ʽ��ֵ����
float PolyValue(Poly L, float x)
{
	float result = 0;
	Poly p = L;
	while (p->next != NULL)
	{
		p = p->next;
		result += ((float)p->numer / (float)p->deno) * (float)(pow(x, p->expo));
	}
	return result;
}

int main(void)
{
	Poly L1, L2, OUT, REM;
	char ch;
	int n;
	float value;
	L1 = CreateNewPoly();
	L2 = CreateNewPoly();
	if (PolyFRead(L1, L2))
	{
		PolySort(L1);
		PolySort(L2);
		printf("File read and sort completed.\n");
		PolyFWrite(L1, 1);
		PolyFWrite(L2, 2);
		do {
			OUT = CreateNewPoly();
			printf("1st polynomial:");
			PolyPrint(L1);
			printf("\n2nd polynomial:");
			PolyPrint(L2);
			printf("\n");
			printf("1.Poly1 + Poly2          2.Poly1 - Poly2\n");
			printf("3.Poly1 * Poly2          4.Poly1 / Poly2\n");
			printf("5.n-order diff of Poly1  6.Poly1(x0)\n");
			printf("Input your choice(other char to exit):");
			scanf("%c", &ch);
			switch (ch)
			{
			case '1': {
				PolyAdd(L1, L2, OUT);
				printf("Result of [1st polynomial] + [2nd polynomial]:");
				PolyPrint(OUT);
				printf("\n");
				PolyFWrite(OUT, 3);
			}
				  break;
			case '2': {
				PolySub(L1, L2, OUT);
				printf("Result of [1st polynomial] - [2nd polynomial]:");
				PolyPrint(OUT);
				printf("\n");
				PolyFWrite(OUT, 4);
			}
					break;
			case '3': {
				PolyMuti(L1, L2, OUT);
				printf("Result of [1st polynomial] * [2nd polynomial]:\n");
				PolyPrint(OUT);
				printf("\n");
				PolyFWrite(OUT, 5);
			}
					break;
			case '4': {
				PolyDiv(L1, L2, OUT, REM);
				printf("Quotient polynomial of [1st polynomial] / [2nd polynomial]:");
				PolyPrint(OUT);
				printf("\n");
				printf("Remain polynomial of [1st polynomial] / [2nd polynomial]:");
				PolyPrint(REM);
				printf("\n");
				PolyFWrite(OUT, 6);
				PolyFWrite(REM, 7);
			}
					break;
			case '5': {
				printf("Input n:");
				scanf("%d", &n);
				if (n <= 0)   //���
					printf("Illegal input(n > 0)!\n");
				else
				{
					for (int i = 1; i <= n; ++i)
						PolyDiff(L1);
					printf("Result polynomial:");
					PolyPrint(L1);
					printf("\n");
				}
			}
					break;
			case '6': {
				printf("Input x0:");
				scanf("%f", &value);
				value = PolyValue(L1, value);
				printf("Result:%.3f\n", value);
			}
					break;
			default:return 0;
			}
			free(OUT);
			getchar();
			system("pause");
			system("cls");
		} while (1);
	}
	else
		printf("File read failed or illegal input!\n");
	return 0;
}