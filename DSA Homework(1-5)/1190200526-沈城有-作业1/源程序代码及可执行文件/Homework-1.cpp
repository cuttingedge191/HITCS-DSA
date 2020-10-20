//作业 1 线性结构的存储结构与应用
//学号：1190200526
//姓名：沈城有
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#define maxlength 100
#define Eletype int

//顺序结构线性表实现及算法(第54-221行)
struct SeqList
{
    Eletype elements[maxlength];
    int last = 0;
};
typedef int position;

//链式结构线性表实现及算法(第223-442行)
struct LinkNode
{
    Eletype element;
    LinkNode* next;
};
typedef LinkNode* LList;
typedef LinkNode* Pos;

//顺序结构线性表相关函数声明
position SeqEnd(SeqList L);
void SeqMakeNull(SeqList& L);
void SeqInsert(Eletype x, position p, SeqList& L);
position SeqLocate(Eletype x, SeqList L);
void SeqDelete(position p, SeqList& L);           //1.删除顺序结构线性表中给定元素
void SeqRemoveCopy(SeqList& L);                   //2.删除已排序顺序结构线性表中所有重复元素
void SeqReverse(SeqList& L, int start, int end);  //3.顺序结构线性表逆置
void SeqMove(SeqList& L, int k);                  //4.顺序结构线性表循环左移（右移）k位(左移正k，右移负k)
void SeqMerge(SeqList L, SeqList M, SeqList& Out);//5.合并两个排好序的顺序结构线性表（假设两表递增排列，且不需删除重复元素）
void SeqPrint(SeqList L);                         //打印顺序结构线性表
void SeqRead(SeqList& L);                         //读取顺序结构线性表
//链接结构线性表相关函数声明
Pos LinkListEnd(LList L);
int LinkListCount(LList L);
void LinkListMakeNull(LList& L);
void LinkListInsert(Eletype x, Pos p);
Pos LinkListLocate(Eletype x, LList L);
void LinkListDel(LList& L, Pos p);                //1.删除链接结构线性表中给定元素
int LinkListRemoveCopy(LList& L);                 //2.删除链接表所有重复元素
void LinkListReverse(LList& L);                   //3.链接表逆置
void LinkListMove(LList& L, int k);               //4.链接表循环左/右移k位
void LinkListMerge(LList L, LList M, LList Out);  //5.合并链接表
void LinkListRead(LList& L);                      //读取链接结构线性表
void LinkListPrint(LList L);                      //打印链接结构线性表

position SeqEnd(SeqList L)
{
    return (L.last + 1);
}

void SeqMakeNull(SeqList& L)
{
    L.last = 0;
}

void SeqInsert(Eletype x, position p, SeqList& L)
{
    position q;
    if (L.last >= maxlength - 1)
        printf("线性表已满！\n");
    else if ((p > L.last + 1) || (p < 1))
        printf("指定的位置不合法！\n");
    else
    {
        for (q = L.last; q >= p; --q)
            L.elements[q + 1] = L.elements[q];
        L.last = L.last + 1;
        L.elements[p] = x;
    }
}

position SeqLocate(Eletype x, SeqList L)
{
    position q;
    for (q = 1; q <= L.last; ++q)
    {
        if (L.elements[q] == x)
        {
            return q;
        }
    }
    return (L.last + 1);
}

//1.删除顺序结构线性表中给定元素
void SeqDelete(position p, SeqList& L)
{
    position q;
    if ((p > L.last) || (p < 1))
        printf("指定的位置不合法或未找到此元素！\n");
    else
    {
        L.last = L.last - 1;
        for (q = p; q <= L.last; ++q)
            L.elements[q] = L.elements[q + 1];
        printf("删除成功！\n");
    }
}

//2.删除已排序顺序结构线性表中所有重复元素
void SeqRemoveCopy(SeqList& L)
{
    for (int i = 1; i < L.last; )
    {
        if (L.elements[i + 1] == L.elements[i])
        {
            SeqDelete(i + 1, L);
        }
        else ++i;
    }
}

//3.顺序结构线性表逆置
void SeqReverse(SeqList& L, int start, int end)
{
    Eletype temp;
    int i;
    int j;
    if (start < 1 || end > L.last || start > end)
    {
        printf("逆置非法操作！\n");
    }
    i = start;
    j = end;
    while (i < j)
    {
        temp = L.elements[i];
        L.elements[i] = L.elements[j];
        L.elements[j] = temp;
        ++i;
        --j;
    }
}

//4.顺序结构线性表循环左移（右移）k位(左移正k，右移负k)
void SeqMove(SeqList& L, int k)
{
    if (k > 0)
    {
        k = k % L.last;
        SeqReverse(L, 1, k);
        SeqReverse(L, k + 1, L.last);
        SeqReverse(L, 1, L.last);
    }
    else if (k < 0)
    {
        k = (0 - k) % L.last;
        SeqReverse(L, 1, L.last - k);
        SeqReverse(L, L.last - k + 1, L.last);
        SeqReverse(L, 1, L.last);
    }
}

//5.合并两个排好序的顺序结构线性表（假设两表递增排列，且不需删除重复元素）
void SeqMerge(SeqList L, SeqList M, SeqList& Out)
{
    position l = 1, m = 1, n = 1;
    while ((l != L.last + 1) && (m != M.last + 1))
    {
        if (L.elements[l] < M.elements[m])
        {
            SeqInsert(L.elements[l], n, Out);
            ++n;
            ++l;
        }
        else if (L.elements[l] > M.elements[m])
        {
            SeqInsert(M.elements[m], n, Out);
            ++n;
            ++m;
        }
        else if (L.elements[l] == M.elements[m])
        {
            SeqInsert(L.elements[l], n, Out);
            SeqInsert(M.elements[m], n + 1, Out);
            ++l;
            ++m;
            n += 2;
        }
    }
    while (l != L.last + 1)
    {
        SeqInsert(L.elements[l], n, Out);
        ++n;
        ++l;
    }
    while (m != M.last + 1)
    {
        SeqInsert(M.elements[m], n, Out);
        ++n;
        ++m;
    }
}

//打印顺序结构线性表
void SeqPrint(SeqList L)
{
    for (int i = 1; i <= L.last; ++i)
        printf("%d ", L.elements[i]);
}

void SeqRead(SeqList& L)
{
    int n;
    printf("输入元素个数(<=100)：");
    scanf("%d", &n);
    printf("输入元素：");
    for (int i = 1; i <= n; ++i)
    {
        scanf("%d", &L.elements[i]);
    }
    L.last = n;
}

Pos LinkListEnd(LList L)
{
    Pos q;
    q = L;
    while (q->next != NULL)
    {
        q = q->next;
    }
    return q;
}

int LinkListCount(LList L)
{
    int count = 0;
    Pos p = L;
    while (p->next != NULL)
    {
        p = p->next;
        ++count;
    }
    return count;
}

void LinkListInsert(Eletype x, Pos p)
{
    Pos temp;
    temp = p->next;
    p->next = (LList)malloc(sizeof(LinkNode));
    p->next->element = x;
    p->next->next = temp;
}

//1.删除给定元素
void LinkListDel(LList& L, Pos p)
{
    Pos q;
    if (p->next != NULL)
    {
        q = p->next;
        p->next = q->next;
        free(q);
        printf("删除成功！\n");
    }
    else printf("指定元素不存在！\n");
}

Pos LinkListLocate(Eletype x, LList L)
{
    Pos p;
    p = L;
    while (p->next != NULL)
    {
        if (p->next->element == x)
            return p;
        else
            p = p->next;
    }
    return p;
}

void LinkListMakeNull(LList& L)
{
    L = (LList)malloc(sizeof(LinkNode));
    if (L == NULL)
    {
        printf("内存分配失败！\n");
        return;
    }
    L->next = NULL;
}

//2.删除链接表所有重复元素
int LinkListRemoveCopy(LList& L)
{
    Pos p;
    p = L;
    int flag = 0;
    while (p->next != NULL)
    {
        if (p->element == p->next->element)
        {
            LinkListDel(L, p);
            flag = 1;
        }
        if (p->next == NULL)
            return 1;
        p = p->next;
    }
    return flag;
}

//3.链接表逆置
void LinkListReverse(LList& L)
{
    Pos p, q;
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

//4.链接表循环左/右移k位
void LinkListMove(LList& L, int k)
{
    int count = LinkListCount(L);
    int cnt = 0;
    LList now = L;
    LList p = NULL;
    LList q;
    LList temp;
    if (k > 0)
    {
        k = k % count;
        while (now->next != NULL)
        {
            now = now->next;
            ++cnt;
            if (cnt == k)
            {
                p = now;
                break;
            }
        }
        temp = L->next;
        L->next = p->next;
        q = p->next;
        p->next = NULL;
        while (q->next != NULL)
        {
            q = q->next;
        }
        q->next = temp;
    }
    else if (k < 0)
    {
        k = (k % count) + count;
        LinkListMove(L, k);
    }
}

//5.合并链接表
void LinkListMerge(LList L, LList M, LList Out)
{
    Pos l, m, n;
    l = L;
    m = M;
    n = Out;
    while ((l->next != NULL) && (m->next != NULL))
    {
        if (l->next->element < m->next->element)
        {
            LinkListInsert(l->next->element, n);
            n = n->next;
            l = l->next;
        }
        else if (l->next->element > m->next->element)
        {
            LinkListInsert(m->next->element, n);
            n = n->next;
            m = m->next;
        }
        else
        {
            LinkListInsert(l->next->element, n);
            LinkListInsert(m->next->element, n->next);
            n = n->next->next;
            l = l->next;
            m = m->next;
        }
    }
    while (l->next != NULL)
    {
        LinkListInsert(l->next->element, n);
        n = n->next;
        l = l->next;
    }
    while (m->next != NULL)
    {
        LinkListInsert(m->next->element, n);
        n = n->next;
        m = m->next;
    }
}

void LinkListRead(LList& L)
{
    int n;
    int count = 0;
    int temp;
    Pos p = L;
    printf("输入元素个数：");
    scanf("%d", &n);
    printf("输入元素：");
    while (count != n)
    {
        scanf("%d", &temp);
        LinkListInsert(temp, p);
        p = p->next;
        ++count;
    }
}

void LinkListPrint(LList L)
{
    Pos p = L;
    while (p->next != NULL)
    {
        printf("%d ", p->next->element);
        p = p->next;
    }
}

int main(void)
{
    SeqList L, M, OUT;
    LList A, B, C;
    Pos p;
    char ch;
    Eletype temp;
    LinkListMakeNull(A);
    LinkListMakeNull(B);
    LinkListMakeNull(C);
    printf("1：顺序表  2：链接表  选择（其他字符退出）：");
    scanf("%c", &ch);
    if (ch == '1')
    {
        while (1)
        {
            printf("1：读取第一个顺序表  2：读取第二个顺序表\n");
            printf("3：删除给定元素      4：删除所有重复元素\n");
            printf("5：逆置              6：循环左/右移\n");
            printf("7：合并两个顺序表    8：打印顺序表\n");
            printf("9：清空所有顺序表\n");
            printf("选择(其他符号退出)：");
            while (getchar() != '\n');
            scanf("%c", &ch);
            switch (ch)
            {
            case '1':SeqRead(L);
                break;
            case '2':SeqRead(M);
                break;
            case '3': {
                printf("输入给定元素：");
                scanf("%d", &temp);
                temp = SeqLocate(temp, L);
                SeqDelete(temp, L);
            }
                    break;
            case '4':SeqRemoveCopy(L);
                break;
            case '5':SeqReverse(L, 1, L.last);
                break;
            case '6': {
                printf("输入k（左正右负）:");
                scanf("%d", &temp);
                SeqMove(L, temp);
            }
                    break;
            case '7':SeqMerge(L, M, OUT);
                break;
            case '8': {
                printf("第一个：");
                SeqPrint(L);
                printf("\n第二个：");
                SeqPrint(M);
                printf("\n第三个：");
                SeqPrint(OUT);
                printf("\n");
            }
                    break;
            case '9': {
                SeqMakeNull(L);
                SeqMakeNull(M);
                SeqMakeNull(OUT);
            }
                    break;
            default:
                return 0;
            }
            system("pause");
            system("CLS");
        }
    }
    else if (ch == '2')
    {
        while (1)
        {
            printf("1：读取第一个链接表  2：读取第二个链接表\n");
            printf("3：删除给定元素      4：删除所有重复元素\n");
            printf("5：逆置              6：循环左/右移\n");
            printf("7：合并两个链接表    8：打印链接表\n");
            printf("9：清空所有链接表\n");
            printf("选择(其他符号退出)：");
            while (getchar() != '\n');
            scanf("%c", &ch);
            switch (ch)
            {
            case '1':LinkListRead(A);
                break;
            case '2':LinkListRead(B);
                break;
            case '3': {
                printf("输入给定元素：");
                scanf("%d", &temp);
                p = LinkListLocate(temp, A);
                LinkListDel(A, p);
            }
                    break;
            case '4': while (LinkListRemoveCopy(A));//发现链接表有重复元素删除不完全情况
                break;
            case '5':LinkListReverse(A);
                break;
            case '6': {
                printf("输入k（左正右负）:");
                scanf("%d", &temp);
                LinkListMove(A, temp);
            }
                    break;
            case '7':LinkListMerge(A, B, C);
                break;
            case '8': {
                printf("第一个：");
                LinkListPrint(A);
                printf("\n第二个：");
                LinkListPrint(B);
                printf("\n第三个：");
                LinkListPrint(C);
                printf("\n");
            }
                    break;
            case '9': {
                LinkListMakeNull(A);
                LinkListMakeNull(B);
                LinkListMakeNull(C);
            }
                    break;
            default:
                return 0;
            }
            system("pause");
            system("CLS");
        }
    }
    return 0;
}