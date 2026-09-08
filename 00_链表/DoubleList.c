/* ============================================================
 * 文件名 : DoubleList.c
 * 功能   : 带头结点的双向链表 —— 任务 00-2 全部实现 + 自测
 * 编译   : gcc DoubleList.c -o DoubleList && ./DoubleList
 * ============================================================ */

/* 任务清单对照（对应 README 任务 00-2）
 * [x] DNode 定义 + InitDList 初始化
 * [x] DHeadInsert 头插 / DTailInsert 尾插（prior 别忘改）
 * [x] DInsertAfter 在 p 后插入（前插可转为后插）
 * [x] DDeleteNext 删除 p 的后继
 * [x] 正向遍历 + 反向遍历打印
 * [x] main 自测：边界 p 是最后一个结点时插入/删除
 * [x] （附加）DInsertBefore 前插 / DDeleteNode 删除指定结点
 */

#define ENABLE_SELF_TEST 1

#include <stdio.h>
#include <stdlib.h>

/* ---------- 结构体定义 ---------- */
typedef struct DNode{
    int data;               /* 数据域 */
    struct DNode *prior;    /* 前驱指针 */
    struct DNode *next;     /* 后继指针 */
} DNode, *DLinkList;

/* 分配新结点 */
DNode *DNewNode(void){
    DNode *s = (DNode*)malloc(sizeof(DNode));
    if(s == NULL){
        printf("内存分配失败!");
        exit(1);
    }
    return s;
}

/* ---------- 1. 初始化 ---------- */
void InitDList(DLinkList *L){
    *L = DNewNode();
    (*L)->prior = NULL;
    (*L)->next = NULL;
}

/* 求长度 */
int DLength(DLinkList L){
    int n = 0;
    DNode *p = L->next;

    while(p != NULL){
        n++;
        p = p->next;
    }
    return n;
}

/* ---------- 2. 头插法 / 尾插法 ---------- */

/* 头插：插到头结点之后。注意：原第一个结点的 prior 要改指向新结点 */
void DHeadInsert(DLinkList L, int e){
    DNode *s = DNewNode();

    s->data = e;
    s->prior = L;           /* 新结点的前驱是头结点 */
    s->next = L->next;      /* 新结点的后继是原来的第一个结点 */
    if(L->next != NULL){    /* 空表时 L->next==NULL，不需要改 */
        L->next->prior = s; /* 原第一个结点的前驱改回新结点 */
    }
    L->next = s;
}

/* 尾插：先走到表尾，再接上新结点 */
void DTailInsert(DLinkList L, int e){
    DNode *p = L;
    DNode *s;

    while(p->next != NULL){
        p = p->next;
    }

    s = DNewNode();
    s->data = e;
    s->prior = p;
    s->next = NULL;
    p->next = s;
}

/* 工具：尾插法把数组建成双向链表 */
DLinkList DCreateByTail(int a[],int n){
    DLinkList L;
    int i;

    InitDList(&L);
    for(i = 0; i < n; i++){
        DTailInsert(L,a[i]);
    }
    return L;
}

/* 工具：头插法把数组建成双向链表 */
DLinkList DCreateByHead(int a[],int n){
    DLinkList L;
    int i;

    InitDList(&L);
    for(i = 0; i < n; i++){
        DHeadInsert(L,a[i]);
    }
    return L;
}

/* ---------- 3. 遍历打印 ---------- */

/* 正向：第一个数据结点 -> 表尾 */
void DPrint(DLinkList L){
    DNode *p = L->next;

    while(p != NULL){
        printf("%d <-> ", p->data);
        p = p->next;
    }
    printf("NULL\n");
}

/* 反向：先走到表尾，再沿 prior 一路走回头结点 */
void DPrintReverse(DLinkList L){
    DNode *p = L;

    while(p->next != NULL){
        p = p->next;
    }

    while(p != L){
        printf("%d <-> ",p->data);
        p = p->prior;
    }
    printf("NULL\n");
}

/* 按值查找：返回第一个值为 e 的结点（测试时用来定位结点） */
DNode *DLocateElem(DLinkList L,int e){
    DNode *p = L->next;

    while(p != NULL && p->data != e){
        p = p->next;
    }
    return p;
}

/* ---------- 4. 插入 ---------- */

/* 在结点 p 之后插入 e。
 * 双链表口诀：先处理"远的一端"，再处理"近的一端" */
void DInsertAfter(DNode *p, int e){
    DNode *s = DNewNode();

    s->data = e;
    s->prior = p;
    s->next = p->next;
    if(p->next != NULL){    /* p 是最后一个结点时 p->next==NULL，跳过 */
        p->next->prior = s; /* 远端：原后继的前驱改回新结点 */
    }
    p->next = s;            
}

/* 在结点 p 之前插入 e = 在 p 的前驱之后后插（前插转后插） */
void DInsertBefore(DNode *p,int e){
    DInsertAfter(p->prior, e);
}

/* ---------- 5. 删除 ---------- */

/* 删除 p 的后继。p 是最后一个结点时没有后继，返回 0 */
int DDeleteNext(DNode *p){
    DNode *q;

    if(p == NULL || p->next == NULL) return 0;
    q = p->next;
    p->next = q->next;       /* p 的后继改成 q 的后继 */
    if(q->next != NULL){
        q->next->prior = p;
    }
    free(q);
    return 1;
}

/* 删除指定结点 p 本身（p 不能是头结点）。
 * 这是双链表相对单链表的优势：删除结点不用找前驱，O(1) */
void DDeleteNode(DNode *p){
    p->prior->next = p->next;
    if(p->next != NULL)
        p->next->prior = p->prior;
    free(p);
}

/* ---------- 6. 清空 / 销毁 ---------- */
void DClearList(DLinkList L){
    DNode *p = L->next;
    DNode *q;

    while(p != NULL){
        q = p->next;
        free(p);
        p = q;
    }
    L->next = NULL;
}

void DDestroyList(DLinkList *L){
    DClearList(*L);
    free(*L);
    *L = NULL;
}

/* ---------- 自测工具 ---------- */
static int tests = 0, passed = 0;

void check(int cond, const char *msg){
    tests++;
    if(cond){
        passed++;
        printf("[PASS] %s\n",msg);
    }else{
        printf("[FAIL] %s\n",msg);
    }
}

/* ---------- 主函数：逐项自测 ---------- */
int main(void) {
    DLinkList L, E;
    DNode *p, *q;
    int ok;
    int a[] = {1, 2, 3, 4, 5};

    /* ===== 1. 空表 / 头插 / 尾插 ===== */
    printf("--- 1. 空表测试 ---\n");
    InitDList(&E);
    check(DLength(E) == 0, "空表长度为 0");
    printf("正向: "); DPrint(E);            /* 只输出 NULL */

    DHeadInsert(E, 7);                      /* E: 7 */
    DTailInsert(E, 9);                      /* E: 7 9 */
    check(DLength(E) == 2, "空表头插 7 + 尾插 9 后长度为 2");
    printf("正向: "); DPrint(E);            /* 7 <-> 9 <-> NULL */
    printf("反向: "); DPrintReverse(E);     /* 9 <-> 7 <-> NULL */

    /* ===== 2. 建表 + 双向链接完整性 ===== */
    printf("\n--- 2. 建表 + 双向链接检查 ---\n");
    L = DCreateByTail(a, 5);                /* L: 1 2 3 4 5 */
    check(DLength(L) == 5, "尾插法建表长度为 5");
    printf("正向: "); DPrint(L);
    printf("反向: "); DPrintReverse(L);

    ok = 1;
    for (q = L->next; q != NULL && q->next != NULL; q = q->next)
        if (q->next->prior != q) { ok = 0; break; }
    check(ok, "双链完整：每个结点的 next->prior 都指回自己");

    /* ===== 3. 后插 / 前插 ===== */
    printf("\n--- 3. 在指定结点后插 / 前插 ---\n");
    DInsertAfter(DLocateElem(L, 3), 99);    /* L: 1 2 3 99 4 5 */
    check(DLength(L) == 6, "在结点 3 后面插 99 成功");
    p = DLocateElem(L, 99);
    check(p->prior->data == 3 && p->next->data == 4, "99 的前驱是 3、后继是 4");

    DInsertAfter(DLocateElem(L, 5), 200);   /* L: ... 5 200（边界：最后结点后插） */
    check(DLength(L) == 7, "在最后一个结点 5 后面插 200 成功（边界）");
    p = DLocateElem(L, 200);
    check(p->next == NULL && p->prior->data == 5, "200 成为新表尾且前驱是 5");

    DInsertBefore(DLocateElem(L, 99), 88);  /* L: 1 2 3 88 99 4 5 200 */
    check(DLength(L) == 8, "在 99 前面插 88 成功（前插转后插）");
    p = DLocateElem(L, 88);
    check(p->prior->data == 3 && p->next->data == 99, "88 的前驱是 3、后继是 99");
    printf("正向: "); DPrint(L);
    printf("反向: "); DPrintReverse(L);

    /* ===== 4. 删除 ===== */
    printf("\n--- 4. 删除 ---\n");
    DDeleteNode(DLocateElem(L, 88));        /* 删中间结点 88 本身 */
    check(DLength(L) == 7, "DDeleteNode 删除 88 成功");
    check(DLocateElem(L, 88) == NULL, "88 已不存在");

    check(DDeleteNext(DLocateElem(L, 3)) == 1, "删除结点 3 的后继(99)成功");
    check(DLength(L) == 6, "删除 99 后长度为 6");
    p = DLocateElem(L, 3);
    check(p->next->data == 4, "3 的后继现在直接是 4");

    check(DDeleteNext(DLocateElem(L, 200)) == 0, "最后一个结点没有后继，删除失败（边界）");
    DDeleteNode(DLocateElem(L, 200));       /* 删表尾 200 */
    check(DLength(L) == 5, "DDeleteNode 删除表尾 200 成功");

    check(DDeleteNext(L) == 1, "用头结点删除第一个数据结点成功");
    check(L->next->data == 2, "删除后第一个数据结点是 2");
    check(DLength(L) == 4, "长度变为 4");
    printf("正向: "); DPrint(L);            /* 2 <-> 3 <-> 4 <-> 5 <-> NULL */

    DDestroyList(&L);
    DDestroyList(&E);

    printf("\n===== 测试结果：%d / %d 通过 =====\n", passed, tests);
    return 0;
}