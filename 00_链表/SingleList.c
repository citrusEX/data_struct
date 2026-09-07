#define ENABLE_SELF_TEST 1

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
LinkList CreateByHead(int a[], int n){
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

    while(p != NULL){
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
    LNode *nxt;

    while(cur != NULL){
        nxt = cur->next;    /* ① 先记住下一个 */
        cur->next = pre;    /* ② 当前结点指向前驱 */
        pre = cur;          /* ③ pre 前移 */
        cur = nxt;          /* ④ cur 前移 */
    }
    L->next = pre;          /* 头结点指向新的第一个结点 */
}

/* ---------- 9. 清空 / 销毁 ---------- */

/* 清空：释放所有数据结点，保留头结点（链表还能继续用） */
void ClearList(LinkList L){
    LNode *p = L->next;
    LNode *q;

    while(p != NULL){
        q = p->next;
        free(p);
        p = q;
    }
    L->next = NULL;
}

/* 销毁：连头结点一起释放，并把外面的头指针置 NULL */
void DestroyList(LinkList *L){
    ClearList(*L);
    free(*L);
    *L = NULL;
}

/* ---------- 自测工具 ---------- */
static int tests = 0, passed = 0;

void check(int cond , const char *msg){
    tests++;
    if(cond){
        passed++;
        printf("[PASS] %s\n",msg);
    }else{
        printf("[FAIL] %s\n",msg);
    }
}

/* ---------- 主函数：逐项自测 ---------- */
#if ENABLE_SELF_TEST

int main(void){
    /* L 尾插表 / M 头插表 / E 空表演练 / S 单结点 / F 空表 */
    LinkList L, M, E, S, F;
    int a[] = {1,2,3,4,5};
    int e;

    /* ===== 1. 空表测试 ===== */
    printf("--- 1. 空表测试 ---\n");
    InitList(&E);
    check(Length(E) == 0, "空表长度为 0");
    PrintList(E);
    check(GetElem(E,1) == NULL, "空表按位查找返回 NULL");
    check(LocateElem(E, 5) == NULL, "空表按值查找返回 NULL");
    check(ListDelete(E, 1, &e) == 0, "空表删除返回失败");
    check(ListInsert(E, 1, 7) == 1, "空表在第 1 位插入成功");
    PrintList(E);   /* 期望: 7 -> NULL */
    check(ListInsert(E, 1, 8) == 1, "空表插入后再在表头插入成功");
    PrintList(E);   /* 期望: 8 -> 7 -> NULL */

    /* ===== 2. 尾插法建表（保持顺序） ===== */
    printf("\n--- 2. 尾插法建表 ---\n");
    L = CreateByTail(a, 5);
    PrintList(L);                                  /* 期望: 1 -> 2 -> 3 -> 4 -> 5 -> NULL */
    check(Length(L) == 5, "尾插法建表长度为 5");

    /* ===== 3. 头插法建表（结果逆序） ===== */
    printf("\n--- 3. 头插法建表 ---\n");
    M = CreateByHead(a, 5);
    PrintList(M);                                  /* 期望: 5 -> 4 -> 3 -> 2 -> 1 -> NULL */
    check(Length(M) == 5, "头插法建表长度为 5");
    check(GetElem(M, 1)->data == 5, "头插法第 1 个元素是 5（逆序验证）");

    /* ===== 4. 查找测试（L = 1 2 3 4 5） ===== */
    printf("\n--- 4. 查找测试 ---\n");
    check(GetElem(L, 1)->data == 1, "按位查找表头第 1 个元素为 1");
    check(GetElem(L, 3)->data == 3, "按位查找中间第 3 个元素为 3");
    check(GetElem(L, 5)->data == 5, "按位查找表尾第 5 个元素为 5");
    check(GetElem(L, 0) == NULL, "按位查找 i=0 返回 NULL");
    check(GetElem(L, 6) == NULL, "按位查找越界(i=6)返回 NULL");
    check(LocateElem(L, 4) != NULL, "按值查找 4 成功");
    check(LocateElem(L, 99) == NULL, "按值查找不存在的 99 返回 NULL");

    /* ===== 5. 插入测试（表头 / 中间 / 表尾 / 越界） ===== */
    printf("\n--- 5. 插入测试 ---\n");
    check(ListInsert(L, 3, 99) == 1, "中间第 3 位插入 99 成功");
    PrintList(L);                                  /* 期望: 1 -> 2 -> 99 -> 3 -> 4 -> 5 */
    check(ListInsert(L, 1, 100) == 1, "表头第 1 位插入 100 成功");
    PrintList(L);                                  /* 期望: 100 -> 1 -> 2 -> 99 -> 3 -> 4 -> 5 */
    check(ListInsert(L, Length(L) + 1, 200) == 1, "表尾(第 len+1 位)插入 200 成功");
    PrintList(L);                                  /* 期望: 100 -> 1 -> 2 -> 99 -> 3 -> 4 -> 5 -> 200 */
    check(ListInsert(L, 0, 0) == 0, "i=0 插入失败");
    check(ListInsert(L, Length(L) + 2, 0) == 0, "i 超过 len+1 插入失败");

    /* ===== 6. 删除测试（表头 / 中间 / 表尾 / 越界） ===== */
    printf("\n--- 6. 删除测试 ---\n");
    check(ListDelete(L, 1, &e) == 1 && e == 100, "删除表头第 1 位，带回值 100");
    PrintList(L);                                  /* 期望: 1 -> 2 -> 99 -> 3 -> 4 -> 5 -> 200 */
    check(ListDelete(L, 3, &e) == 1 && e == 99, "删除中间第 3 位，带回值 99");
    PrintList(L);                                  /* 期望: 1 -> 2 -> 3 -> 4 -> 5 -> 200 */
    check(ListDelete(L, Length(L), &e) == 1 && e == 200, "删除表尾最后一位，带回值 200");
    PrintList(L);                                  /* 期望: 1 -> 2 -> 3 -> 4 -> 5 -> NULL */
    check(ListDelete(L, 0, &e) == 0, "i=0 删除失败");
    check(ListDelete(L, Length(L) + 1, &e) == 0, "i 越界删除失败");

    /* ===== 7. 反转测试 ===== */
    printf("\n--- 7. 反转测试 ---\n");
    Reverse(L);                                    /* L = 1 2 3 4 5 */
    PrintList(L);                                  /* 期望: 5 -> 4 -> 3 -> 2 -> 1 */
    check(GetElem(L, 1)->data == 5 && GetElem(L, 5)->data == 1, "反转后首尾元素正确");
    Reverse(L);
    PrintList(L);                                  /* 期望: 1 -> 2 -> 3 -> 4 -> 5 */
    check(GetElem(L, 1)->data == 1 && GetElem(L, 5)->data == 5, "再次反转恢复原序");

    /* ===== 8. 单结点 / 空链表边界 ===== */
    printf("\n--- 8. 单结点 / 空链表边界 ---\n");
    S = CreateByTail(a, 1);                        /* S = 1 */
    check(ListInsert(S, 2, 9) == 1, "单结点链表在表尾(第 2 位)插入成功");
    PrintList(S);                                  /* 期望: 1 -> 9 -> NULL */
    check(ListDelete(S, 1, &e) == 1 && e == 1, "删除单结点链表的表头，带回值 1");
    PrintList(S);                                  /* 期望: 9 -> NULL */
    Reverse(S);
    check(GetElem(S, 1)->data == 9, "单结点链表反转后仍为 9");

    InitList(&F);                                  /* 空链表 */
    Reverse(F);                                    /* 空链表反转不应崩溃 */
    check(Length(F) == 0, "空链表反转后长度仍为 0");

    /* ===== 9. 清空 / 销毁测试 ===== */
    printf("\n--- 9. 清空 / 销毁测试 ---\n");
    ClearList(E);                                  /* E 之前是 8 7 */
    check(Length(E) == 0, "清空后长度为 0");
    check(E != NULL, "清空后头结点仍保留，链表可复用");
    DestroyList(&E);
    check(E == NULL, "销毁后头指针被置 NULL");

    DestroyList(&L);
    DestroyList(&M);
    DestroyList(&S);
    DestroyList(&F);

    /* ===== 汇总 ===== */
    printf("\n===== 测试结果：%d / %d 通过 =====\n", passed, tests);
    return 0;
}

#endif