#include <stdio.h>
#include <stdlib.h>

/*-----------结构体定义-------------*/
typedef struct LNode{
    int data;               /*数据域*/
    struct LNode *next;     /*指针域*/
} LNode, *LinkList;

/*-----------工具函数--------------*/

/*分配一个新节点*/
LNode *NewNode(void){
    LNode *s = (LNode *)malloc(sizeof(LNode));
    if(s == NULL){
        printf("内存分配失败\n");
        exit(1);
    }
    return s;
}

/*========特别说明，此链表带头节点，头节点不存数据=======*/

/*-----------1.初始化------------*/

/* 创建带头节点的空链表(二级指针才能改到外面的头指针)*/
void InitList(LinkList *L){
    *L = NewNode();
    (*L)->next = NULL;
}

/*----------2.头插法-----------------*/

/* 把 e 插到头结点之后，后插的会跑到前面，结果与原序列相反 */
void HeadInsert(LinkList L, int e){
    LNode *s = NewNode();
    s->data = e;
    s->next = L->next;
    L->next = s;
}

/*------------3.尾插法--------------*/

/* 把 e 追加到链表末尾，保持原序列顺序 */
void TailInsert(LinkList L, int e){
    LNode *p = L;
    LNode *s;

    while(p->next != NULL) /*找到表尾节点*/
        p = p->next;

    s = NewNode();
    s->data = e;
    s->next = NULL;
    p->next = s; 
}

/* 工具：用尾插法把数组 a 的前 n 个元素建成链表 */
LinkList CreateByTail(int a[], int n){
    LinkList L;
    int i;

    InitList(&L);
    for(i = 0; i < n; i++){
        TailInsert(L,a[i]);
    }
    return L;
}

/* 工具：用头插法把数组 a 的前 n 个元素建成链表（结果是逆序） */
LinkList CreatebyHead(int a[], int n){
    LinkList L;
    int i;

    InitList(&L);
    for(i = 0; i<n; i++){
        HeadInsert(L, a[i]);
    }
    return L;
}

/* ---------- 4. 遍历打印 / 求表长 ---------- */
void PrintList(LinkList L){
    LNode *p = L->next;

    while(p->next != NULL){
        printf("%d -> ",p->data);
        p = p->next;
    }
    printf("NULL\n");
}

int Length(LinkList L){
    int len = 0;
    LNode *p = L->next;

    while(p != NULL){
        len++;
        p = p->next;
    }
    return len;
}

/* ---------- 5. 查找 ---------- */

/* 按位查找：返回第 i 个结点（i 从 1 开始），不存在返回 NULL */
LNode *GetElem(LinkList L, int i){
    LNode *p;
    int j;

    if(i < 1) return NULL;
    p = L->next;
    j = 1;
    while(p != NULL && j < i){
        p = p->next;
        j++;
    }
    return p;
}

/* 按值查找：返回第一个值为 e 的结点，找不到返回 NULL */
LNode *LocateElem(LinkList L, int e){
    LNode *p = L->next;

    while(p != NULL && p->data != e){
        p = p->next;
    }
    return p;
}

/* ---------- 6. 插入 ---------- */

/* 在第 i 个位置插入 e（i 从 1 开始，i = 表长+1 表示插到末尾）
* 成功返回 1，失败返回 0 
*/
int ListInsert(LinkList L, int i, int e){
    LNode *p = L;   /* p 从头结点开始走 */
    LNode *s;
    int j = 0;

    if(i < 1) return 0;
    while(p != NULL && j < i-1){    /* 找第 i-1 个结点 */
        p = p->next;
        j++;
    }
    if(p == NULL) return 0; /* i 超过表长+1，插入位置不合法 */

    s = NewNode();
    s->data = e;
    s->next = p->next;  /* 关键：先连后断 */
    p->next = s;
    return 1;
}

/* ---------- 7. 删除 ---------- */

/* 删除第 i 个结点，被删元素的值通过 e 带回。成功返回 1 */
int ListDelete(LinkList L, int i, int *e){
    LNode *p = L;
    LNode *q;
    int j = 0;

    if(i < 1) return 0;
    while(p != NULL && j < i-1){
        p = p->next;
        j++;
    }
    if(p == NULL || p->next == NULL) return 0;

    q = p->next;
    *e = q->data;
    p->next = q->next;  /* 跳过 q */
    free(q);    /* 释放被删结点 */
    return 1;
}

/* ---------- 8. 反转 ---------- */

/* 就地反转：三指针 pre / cur / nxt 依次后移改方向 */
void Reverse(LinkList L){
    LNode *pre = NULL;
    LNode *cur = L->next;
    LNode * nxt;

    
}