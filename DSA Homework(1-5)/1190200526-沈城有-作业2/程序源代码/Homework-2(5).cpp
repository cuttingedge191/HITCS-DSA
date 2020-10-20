/*
* 作业2 线性结构及其应用
* 一、算术表达式求值（要求5）
* 学号：1190200526
* 姓名：沈城有
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define maxlength1 1000
#define maxlength2 100
#define FileDir "D:\\test.txt"

//顺序结构栈的实现
struct char_stack
{
	int top;
	char elements[maxlength1];
};
typedef struct char_stack CStack;

struct int_stack
{
	int top;
	int elements[maxlength2];
};
typedef struct int_stack IStack;

//栈相关操作函数声明
void CStackMakeNull(CStack& S);
int CStackEmpty(CStack S);
char CStackTop(CStack S);
void CStackPop(CStack& S);
void CStackPush(char x, CStack& S);
void CStackPrint(CStack S);
void IStackMakeNull(IStack& S);
int IStackEmpty(IStack S);
int IStackTop(IStack S);
void IStackPop(IStack& S);
void IStackPush(int x, IStack& S);
void IStackPrint(IStack S);
//功能相关函数声明
int FReadExp(char exp[]);                                  //从文件中读取中缀表达式
void PrintExp(char exp[]);                                 //在屏幕上打印表达式
int OpCompare(char a, char b);                             //运算符优先级比较函数
int Calculator(char op, int a, int b);                     //数值计算器
void CombineProcess(char Expin[], CStack& CS, IStack& Is); //两部分结合的算符优先法

void CStackMakeNull(CStack& S)
{
	S.top = -1;
}

int CStackEmpty(CStack S)
{
	if (S.top < 0)
		return 1;
	else
		return 0;
}

char CStackTop(CStack S)
{
	if (CStackEmpty(S))
		return 'E';
	else
		return (S.elements[S.top]);
}

void CStackPop(CStack& S)
{
	if (CStackEmpty(S))
	{
		printf("Stack is empty!\n");
	}
	else S.top = S.top - 1;
}

void CStackPush(char x, CStack& S)
{
	if (S.top == maxlength1 - 1)
	{
		printf("Stack is full!\n");
	}
	else
	{
		S.top = S.top + 1;
		S.elements[S.top] = x;
	}
}

void CStackPrint(CStack S)
{
	for (int i = 0; i <= S.top; ++i)
	{
		printf("%c", S.elements[i]);
	}
}

void IStackMakeNull(IStack& S)
{
	S.top = -1;
}

int IStackEmpty(IStack S)
{
	if (S.top < 0)
		return 1;
	else
		return 0;
}

int IStackTop(IStack S)
{
	if (IStackEmpty(S))
		return 'E';
	else
		return (S.elements[S.top]);
}

void IStackPop(IStack& S)
{
	if (IStackEmpty(S))
	{
		printf("Stack is empty!\n");
	}
	else S.top = S.top - 1;
}

void IStackPush(int x, IStack& S)
{
	if (S.top == maxlength2 - 1)
	{
		printf("Stack is full!\n");
	}
	else
	{
		S.top = S.top + 1;
		S.elements[S.top] = x;
	}
}

void IStackPrint(IStack S)
{
	for (int i = 0; i <= S.top; ++i)
	{
		printf("%d ", S.elements[i]);
	}
}

//从文件中读取中缀表达式
int FReadExp(char exp[])
{
	FILE* fp;
	char ch;
	int count = 0;
	fp = fopen(FileDir, "r");
	if (fp == NULL)
	{
		printf("File read failure！\n");
		return 0;
	}
	do {
		ch = fgetc(fp);
	} while (ch != '#');
	exp[count] = ch;
	++count;
	ch = fgetc(fp);
	while (ch != '#')
	{
		exp[count] = ch;
		++count;
		ch = fgetc(fp);
	}
	exp[count] = ch;
	fclose(fp);
	return 1;
}

//在屏幕上打印表达式
void PrintExp(char exp[])
{
	int count = 1;
	do {
		if (exp[count] == '#')
			printf("NULL");
		else
			printf("%c", exp[count]);
		++count;
	} while (exp[count] != '#');
	printf("\n");
}

//运算符优先级比较函数
int OpCompare(char a, char b)
{
	if (a == '+' || a == '-')
	{
		if (b != '#' && b != 'E' && b != '(')
			return 0;
	}
	else if (a == '*' || a == '/')
	{
		if (b == '*' || b == '/')
			return 0;
	}
	return 1;
}

//数值计算器
int Calculator(char op, int a, int b)
{
	switch (op)
	{
	case '+':return a + b;
		break;
	case '-':return a - b;
		break;
	case '*':return a * b;
		break;
	case '/':return a / b;
		break;
	default:printf("Unknown operator!\n");
		return 0;
	}
}

//两部分结合的算符优先法
void CombineProcess(char Expin[], CStack& CS, IStack& IS)
{
	int i = 1;
	int temp = 0;
	int count = 0;
	int op1, op2;
	char ch;
	int test = 0;
	CStackPush('#', CS);
	while (Expin[i] != '#')
	{
		if (Expin[i] >= '0' && Expin[i] <= '9')
		{
			temp = temp * 10 + Expin[i] - '0';
			test = 1;
		}
		else if (Expin[i] == ' ')
		{
			if (test)
			{
				test = 0;
				IStackPush(temp, IS);
				temp = 0;
			}
		}
		else if (Expin[i] == '(')
		{
			if (test)
			{
				test = 0;
				IStackPush(temp, IS);
				temp = 0;
			}
			CStackPush(Expin[i], CS);
		}
		else if (Expin[i] == ')')
		{
			if (test)
			{
				test = 0;
				IStackPush(temp, IS);
				temp = 0;
			}
			if (CStackEmpty(CS))
			{
				printf("Illegal infix expression!\n");
				return;
			}
			ch = CStackTop(CS);
			CStackPop(CS);
			while (ch != '(')
			{
				op1 = IStackTop(IS);
				IStackPop(IS);
				op2 = IStackTop(IS);
				IStackPop(IS);
				temp = Calculator(ch, op2, op1);
				IStackPush(temp, IS);
				temp = 0;
				if (CStackEmpty(CS))
				{
					printf("Illegal infix expression!\n");
					return;
				}
				else
				{
					ch = CStackTop(CS);
					CStackPop(CS);
				}
			}
		}
		else
		{
			if (test)
			{
				test = 0;
				IStackPush(temp, IS);
				temp = 0;
			}
			if (OpCompare(Expin[i], CStackTop(CS)))
				CStackPush(Expin[i], CS);
			else
			{
				while (!OpCompare(Expin[i], CStackTop(CS)))
				{
					op1 = IStackTop(IS);
					IStackPop(IS);
					op2 = IStackTop(IS);
					IStackPop(IS);
					ch = CStackTop(CS);
					CStackPop(CS);
					temp = Calculator(ch, op2, op1);
					IStackPush(temp, IS);
					temp = 0;
					if (CStackEmpty(CS))
					{
						printf("Illegal infix expression!\n");
						return;
					}
				}
				CStackPush(Expin[i], CS);
			}
		}
		++count;
		++i;
		printf("____________Step %d:____________\n", count);
		printf("Operator Stack Status:");
		CStackPrint(CS);
		printf("\nOperand Stack Status:");
		IStackPrint(IS);
		printf("\n");
	}
	if (temp)
	{
		IStackPush(temp, IS);
		temp = 0;
	}
	if (!CStackEmpty(CS))
	{
		ch = CStackTop(CS);
		CStackPop(CS);
		while (ch != '#')
		{
			op1 = IStackTop(IS);
			IStackPop(IS);
			op2 = IStackTop(IS);
			IStackPop(IS);
			temp = Calculator(ch, op2, op1);
			IStackPush(temp, IS);
			ch = CStackTop(CS);
			CStackPop(CS);
			++count;
			printf("____________Step %d:____________\n", count);
			printf("Operator Stack Status:");
			CStackPrint(CS);
			printf("\nOperand Stack Status:");
			IStackPrint(IS);
			printf("\n");
		}
	}
	printf("Result:");
	IStackPrint(IS);
	return;
}

int main(void)
{
	char Expin[500];
	CStack CS;
	IStack IS;
	CStackMakeNull(CS);
	IStackMakeNull(IS);
	if (FReadExp(Expin))
	{
		printf("Infix expression:");
		PrintExp(Expin);
		system("pause");
		CombineProcess(Expin, CS, IS);
		system("pause");
	}
	return 0;
}