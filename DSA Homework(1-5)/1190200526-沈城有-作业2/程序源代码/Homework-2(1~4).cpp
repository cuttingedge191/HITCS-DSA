/*
* 作业2 线性结构及其应用
* 一、算术表达式求值（要求1至4）
* 学号：1190200526
* 姓名：沈城有
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define FileDir "D:\\test.txt"
#define ElemType char
#define maxlength 1000

//顺序结构栈的实现
struct stack
{
	int top;
	ElemType elements[maxlength];
};
typedef struct stack Stack;

//栈相关操作函数声明
void StackMakeNull(Stack& S);
int StackEmpty(Stack S);
ElemType StackTop(Stack S);
void StackPop(Stack& S);
void StackPush(ElemType x, Stack& S);
void StackPrint(Stack S);                                //屏幕打印栈状态
//功能相关函数声明
int FReadExp(char exp[]);                                //从文件中读取中缀表达式
void PrintExp(char exp[]);                               //在屏幕上打印表达式
int OpCompare(char a, char b);                           //运算符优先级比较函数
char* strrpc(char str[], char oldstr[], char newstr[]);  //字符串部分替换函数
void MinusProcess(char Expin[]);                         //处理表达式中负号
int ParameterProcess(char Expin[]);                      //识别并处理表达式中的变量
int ExpConvert(char Expin[], char Expout[], Stack& S);   //处理中缀表达式转换为后缀表达式
double Calculator(char op, double a, double b);          //数值计算器
double Factorial(double a);                              //阶乘计算函数
void DataPushToStack(double d, Stack& S);                //double转字符串函数（保留三位小数），并压入栈
void ExpCalculate(char Expout[], Stack& S);              //后缀表达式求值
void PrintResult(Stack& S);                              //打印计算结果

void StackMakeNull(Stack& S)
{
	S.top = -1;
}

int StackEmpty(Stack S)
{
	if (S.top < 0)
		return 1;
	else
		return 0;
}

ElemType StackTop(Stack S)
{
	if (StackEmpty(S))
		return 'E';
	else
		return (S.elements[S.top]);
}

void StackPop(Stack& S)
{
	if (StackEmpty(S))
	{
		printf("Stack is empty!\n");
	}
	else S.top = S.top - 1;
}

void StackPush(ElemType x, Stack& S)
{
	if (S.top == maxlength - 1)
	{
		printf("Stack is full!\n");
	}
	else
	{
		S.top = S.top + 1;
		S.elements[S.top] = x;
	}
}

//屏幕打印栈状态
void StackPrint(Stack S)
{
	for (int i = 0; i <= S.top; ++i)
	{
		printf("%c", S.elements[i]);
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
		if (b == '^' || b == '!' || b == '*' || b == '/')
			return 0;
	}
	else if (a == '^' || a == '!')
	{
		if (b == '^' || b == '!')
			return 0;
	}
	return 1;

}

//字符串部分替换函数
char* strrpc(char str[], char oldstr[], char newstr[])
{
	char bstr[200];
	memset(bstr, '\0', 200);
	for (int i = 0; i < (int)strlen(str); ++i)
	{
		if (!strncmp(str + i, oldstr, strlen(oldstr)))
		{//查找目标字符串
			strcat(bstr, newstr);
			i += strlen(oldstr) - 1;
		}
		else
		{
			strncat(bstr, str + i, 1);
		}
	}
	strcpy(str, bstr);
	return str;
}

//处理表达式中负号
void MinusProcess(char Expin[])
{
	int i = 1;
	int count = 1;
	int ini = 1;
	char ch = Expin[i];
	while (Expin[count] != '#')
		++count;
	do {
		if (Expin[i] == '-')
		{
			if (!((Expin[i - 1] >= '0' && Expin[i - 1] <= '9') || (Expin[i-1] == ')')))
			{
				for (int k = count + 1; k >= i; --k)
				{
					Expin[k + 1] = Expin[k];
				}
				Expin[i] = '0';
				++count;
			}
		}
		++i;
	} while (i < count);
}

//识别并处理表达式中的变量
int ParameterProcess(char Expin[])
{
	char temp[20];
	double Value;    //临时保存变量的值
	char Para[10][25];//记录变量信息
	int countn = -1;  //记录变量个数
	int countl = 0;  //记录变量名长度
	int i = 1;
	int test = 0;
	for (int n = 0; n < 10; ++n)
	{
		memset(Para[n], '\0', 5);
	}
	while (Expin[i] != '#')
	{
		if ((Expin[i] >= 'a' && Expin[i] <= 'z') || (Expin[i] >= 'A' && Expin[i] <= 'Z'))
		{
			if (!test)
			{
				++countn;
				countl = 0;
			}
			test = 1;
			if (countn < 10 && countl < 25)
			{
				Para[countn][countl] = Expin[i];
				++countl;
			}
			else
			{
				printf("Variable name is too long or too many variables!\n");
				return 0;
			}
		}
		else if (test && ((Expin[i] >= 'a' && Expin[i] <= 'z') || (Expin[i] >= 'A' && Expin[i] <= 'Z') || (Expin[i] >= '0' && Expin[i] <= '9')))
		{
			Para[countn][countl] = Expin[i];
			++countl;
			if (countl > 25)
			{
				printf("Variable name is too long(>25char)!\n");
				return 0;
			}
		}
		else
			test = 0;
		++i;
	}
	for (int k = 0; k <= countn; ++k)
	{
		printf("Please input the value of %s:", Para[k]);
		scanf("%lf", &Value);
		if (Value < 0)
			sprintf(temp, "(0%.3lf)", Value);
		else
			sprintf(temp, "(%.3lf)", Value);//不做此处理发现替换为负数时会有栈操作错误
		Expin = strrpc(Expin, Para[k], temp);
	}
	return countn;
}

//处理中缀表达式转换为后缀表达式
int ExpConvert(char Expin[], char Expout[], Stack& S)
{
	char ch;
	int i = 0;
	int j = 1;
	int flag = 0;   //检测括号匹配
	int count = 1;  //记录处理步骤
	int check = 0;  //检测上一位是否为数字
	Expout[0] = '#';
	do {
		if ((Expin[i] >= '0' && Expin[i] <= '9') || Expin[i] == '.')
		{
			if (!check && j != 1)
			{
				Expout[j] = ' ';
				++j;
			}
			Expout[j] = Expin[i];
			++i;
			++j;
			check = 1;
		}
		else if (Expin[i] == '(')
		{
			check = 0;
			++flag;
			StackPush(Expin[i], S);
			++i;
		}
		else if (Expin[i] == ')')
		{
			check = 0;
			if (!flag)
			{
				printf("Bracket mismatch!\n");
				return 0;
			}
			--flag;
			do {
				ch = StackTop(S);
				StackPop(S);
				Expout[j] = ' ';
				Expout[j + 1] = ch;
				j += 2;
			} while (ch != '(');
			j -= 2;
			Expout[j + 1] = '#';
			++i;
		}
		else if (Expin[i] == ' ')
		{
			++i;
		}
		else if (!OpCompare(Expin[i], StackTop(S)))
		{
			check = 0;
			do {
				ch = StackTop(S);
				StackPop(S);
				Expout[j] = ' ';
				Expout[j + 1] = ch;
				j = j + 2;
			} while (!OpCompare(Expin[i], StackTop(S)));
			StackPush(Expin[i], S);
			++i;
		}
		else
		{
			check = 0;
			StackPush(Expin[i], S);
			++i;
		}
		printf("_____________________Step:%d_____________________\nStack status(Bottom -> Top):", count);
		StackPrint(S);
		printf("\n");
		++count;
		printf("Processed expression:");
		PrintExp(Expout);
	} while (Expin[i] != '#');
	while (!StackEmpty(S))
	{
		ch = StackTop(S);
		StackPop(S);
		Expout[j] = ' ';
		Expout[j + 1] = ch;
		j += 2;
	}
	printf("_____________________Step:%d_____________________\nStack status(Bottom -> Top):", count);
	StackPrint(S);
	printf("\n");
	++count;
	printf("Processed expression:");
	PrintExp(Expout);
	return 1;
}

//数值计算器
double Calculator(char op, double a, double b)
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
	case '^':return pow(a, b);
		break;
	default:printf("Unknown operator!\n");
		return 0;
	}
}

double Factorial(double a)
{
	if (a == 0 || a == 1) return 1;
	if (a < 2 && a - 1 != 0)
	{
		printf("Illegal factorial operation!\n");
		return 0;
	}
	else return a * Factorial(a - 1);
}

//double转字符串函数（保留三位小数），并压入栈
void DataPushToStack(double d, Stack& S)
{
	char s[20];
	sprintf(s, "%.3lf", d);
	for (int i = 0; i < (int)strlen(s); ++i)
	{
		StackPush(s[i], S);
	}
}

//后缀表达式求值
void ExpCalculate(char Expout[], Stack& S)
{
	int flag = 0;
	int test = 0; //检测是否已读完一个数
	int i = 1;
	int dot = 0, digit = 0;  //记录小数点位置
	char ch;
	double temp1 = 0, temp2 = 0;
	int count = 1;
	int a, b;
	do {
		a = 1;
		b = 1;
		if ((Expout[i] >= '0' && Expout[i] <= '9') || Expout[i] == '.')
		{
			if (!test)
			{
				StackPush('#', S);
				test = 1;
			}
			StackPush(Expout[i], S);
		}
		else if (Expout[i] == ' ')
			test = 0;
		else if (Expout[i] != ' ' && Expout[i] != '!')
		{
			flag = 0;
			test = 0;
			temp1 = temp2 = 0;
			dot = digit = 0;
			do {
				ch = StackTop(S);
				StackPop(S);
				if (ch == '.')
				{
					dot = digit;
				}
				else if (ch == '-')
					a = -1;
				else
				{
					temp1 = temp1 + ((double)ch - '0') * (pow(10, digit));
					++digit;
				}
			} while (S.elements[S.top] != '#');
			if (dot)
				temp1 = temp1 / (pow(10, dot));
			flag = 0;
			dot = digit = 0;
			StackPop(S);
			do {
				ch = StackTop(S);
				StackPop(S);
				if (ch == '.')
				{
					dot = digit;
				}
				else if (ch == '-')
					b = -1;
				else
				{
					temp2 = temp2 + ((double)ch - '0') * (pow(10, digit));
					++digit;
				}
			} while (S.elements[S.top] != '#' && !StackEmpty(S));
			if (dot)
				temp2 = temp2 / (pow(10, dot));
			temp1 = Calculator(Expout[i], b * temp2, a * temp1);
			DataPushToStack(temp1, S);
		}
		else if (Expout[i] == '!')
		{
			flag = 0;
			test = 0;
			temp1 = 0;
			a = 1;
			dot = digit = 0;
			do {
				ch = StackTop(S);
				StackPop(S);
				if (ch == '.')
				{
					dot = digit;
				}
				else if (ch == '-')
					a = -1;
				else
				{
					temp1 = temp1 + ((double)ch - '0') * (pow(10, digit));
					++digit;
				}
			} while (S.elements[S.top] != '#');
			if (dot)
				temp1 = temp1 / (pow(10, dot));
			flag = 0;
			dot = digit = 0;
			temp1 = Factorial(temp1);
			DataPushToStack(temp1, S);
		}
		++i;
		printf("_____________________Step:%d_____________________\nStack status(Bottom -> Top):", count);
		StackPrint(S);
		printf("\n");
		++count;
	} while (Expout[i] != '#');
}

//打印计算结果
void PrintResult(Stack& S)
{
	for (int i = 1; i <= S.top; ++i)
		printf("%c", S.elements[i]);
}

int main(void)
{
	int test = 0;
	char Expin[500];
	char Expout[500];
	memset(Expin, '\0', 500);
	memset(Expout, '#', 500);
	Stack S;
	StackMakeNull(S);
	if (FReadExp(Expin))
	{
		printf("Infix expression:");
		PrintExp(Expin);
		if (ParameterProcess(Expin))
		{
			printf("Expression after replacing all the variables and sort:");
			MinusProcess(Expin);
			PrintExp(Expin);
			test = 1;
		}
	}
	system("pause");
	if (!test) return 0;
	printf("##### Infix Expression -> Postfix Expression #####\n");
	if (ExpConvert(Expin, Expout, S))
	{
		printf("Postfix expression:");
		PrintExp(Expout);
	}
	else
	{
		test = 0;
		printf("ERROR:Illegal infix expression!\n");
	}
	system("pause");
	if (test)
	{
		printf("##### Calculate the value of Postfix Expression #####\n");
		ExpCalculate(Expout, S);
		printf("Calculation Result:");
		PrintResult(S);
		printf("\n");
	}
	system("pause");
	return 0;
}