/* ============================================================
 * 文件名 : CircularList.c
 * 功能   : 带头结点的循环单链表 + 约瑟夫环 —— 任务 00-3
 * 编译   : gcc CircularList.c -o CircularList && ./CircularList
 * ============================================================ */

/* 任务清单对照（对应 README 任务 00-3）
 * [x] 循环链表建表（尾结点回指首结点）
 * [x] 循环遍历（终止条件：回到头结点）
 * [x] Josephus(n, m) 出列顺序实现
 * [x] 验证 Josephus(5,3) = 3 1 5 2 4
 */

#include <stdio.h>
#include <stdlib.h>

/* ---------- 结构体定义（沿用单链表的 LNode） ---------- */
typedef struct LNode{
    int data;
    struct LNode *next;
} LNode, *LinkList;

/* 分配新结点 */
LNode *NewNode(void){
    LNode *s = (LNode*)malloc(sizeof(LNode));
    if(s == NULL){
        printf("内存分配失败\n");
        exit(1);
    }
    return s;
}

/* ---------- 1. 带头结点的循环链表 ---------- */

/* 初始化：空循环链表 = 头结点自己指向自己 L->next == L */
void InitCList(LinkList *L){
    *L = NewNode();
    (*L)->next = *L;
}

/* 头插：和普通单链表代码完全一样（空表时 L->next 就是 L，天然成立） */
void CHeadInsert(LinkList L,int e){
    LNode *s = NewNode();

    s->data = e;
    s->next = L->next;
    L->next = s;
}

/* 尾插：先找到表尾（next 指向头结点的那个结点），再接新结点 */
void CTailInsert(LinkList L,int e){
    LNode *p = L;
    LNode *s = NewNode();

    while(p->next != L){
        p = p->next;
    }
    s->data = e;
    s->next = L;
    p->next = s;
}

/* 工具：尾插法把数组建成带头结点的循环链表 */
LinkList CCreateByTail(int a[],int n){
    LinkList L;
    int i;

    InitCList(&L);
    for(i=0; i<n; i++){
        CTailInsert(L,a[i]);
    }
    return L;
}

/*头插法建链*/
LinkList CCreateByHead(int a[],int n){
    LinkList L;
    int i;

    InitCList(&L);
    for(i=0; i<n; i++){
        CHeadInsert(L,a[i]);
    }
    return L;
}

/* ---------- 2. 循环遍历 / 长度 ---------- */

/* 遍历：从头结点的下一个开始，绕一圈回到头结点就停 */
void CPrint(LinkList L){
    LNode *p = L->next;

    if(p == L){
        printf("空循环链表\n");
        return;
    }
    while(p != L){
        printf("%d -> ",p->data);
        p = p->next;
    }
    printf("(到头)\n");
}

int CLength(LinkList L){
    int n = 0;
    LNode *p = L->next;

    while(p != L){
        n++;
        p = p->next;
    }
    return n;
}

/* 判断 p 是不是表尾结点（表尾的 next 指回头结点） */
int CIsTail(LinkList L, LNode *p){
    return p->next == L;
}

/* 判断 p 是不是第一个数据结点 */
int CIsHead(LinkList L, LNode *p){
    return p == L->next;
}

/* ---------- 3. 约瑟夫环 ---------- */

/* n 个人（编号 1~n）围成一圈，从 1 开始报数，报到 m 的人出列，
 * 然后从下一个人重新从 1 报数。返回出列顺序数组（含最后幸存者），
 * 调用方负责 free；同时把顺序打印出来。
 * 说明：约瑟夫环用"不带头结点"的环更直观，p 直接在数据结点上走。 */
int *Josephus(int n,int m){
    LNode *head = NULL, *tail = NULL;
    LNode *p, *pre, *s;
    int *order;
    int i, cnt, idx = 0;

    order = (int *)malloc(n * sizeof(int));
    if(order == NULL){
        printf("内存分配失败\n");
        exit(1);
    }

    /* (1) 建环：1 -> 2 -> ... -> n -> 1 */
    for(i = 1; i<=n; i++){
        s = NewNode();
        s->data = i;
        if(head == NULL){
            head = s;
        }else{
            tail->next = s;
        }
        tail = s;
    }
    tail->next = head;  /* 首尾相连成环 */

    /* (2) 报数出列：pre 始终是 p 的前驱（删 p 要用） */
    p = head;
    pre = tail;
    while(p->next != p){    /* 环里只剩 p 自己时结束 */
        for(cnt = 1; cnt < m; cnt++){   /* p 走 m-1 步，指向出列者 */
            pre = p;
            p = p->next;
        }
        order[idx++] = p->data; /* 记录出列者 */
        printf("%d ",p->data);
        pre->next = p->next;    /* 把 p 从环中摘掉 */
        free(p);
        p = pre->next;          /* 下一个人重新从 1 报数 */
    }
    order[idx] = p->data;       /* 最后幸存者 */
    printf("%d ",p->data);
    free(p);
    return order;
}

/* ---------- 4. 销毁 ---------- */
void CDestroyList(LinkList *L){
    LNode *p = (*L)->next;
    LNode *q;

    while(p != *L){
        q = p->next;
        free(p);
        p = q;
    }
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

/* 比较两个长度都为 n 的数组是否相等 */
int arraysEqual(int a[],int b[],int n){
    int i;

    for(i=0; i<n; i++){
        if(a[i] != b[i]) return 0;
    }
    return 1;
}

/* ---------- 主函数：逐项自测 ---------- */
int main(void) {
    LinkList C, E;
    LNode *p;
    int *r;
    int a[] = {1, 2, 3, 4, 5};
    int exp1[] = {3, 1, 5, 2, 4};       /* Josephus(5,3) 期望 */
    int exp2[] = {3, 6, 2, 7, 5, 1, 4}; /* Josephus(7,3) 期望 */

    /* ===== 1. 空循环链表 ===== */
    printf("--- 1. 空循环链表 ---\n");
    InitCList(&E);
    check(CLength(E) == 0, "空循环链表长度为 0");
    check(E->next == E, "空表：头结点自指 L->next == L");
    CPrint(E);                           /* (空循环链表) */

    CHeadInsert(E, 7);                   /* E: 7 */
    CTailInsert(E, 9);                   /* E: 7 9 */
    check(CLength(E) == 2, "头插 7 + 尾插 9 后长度为 2");
    printf("E: "); CPrint(E);            /* 7 -> 9 -> (回到头) */
    p = E->next;                         /* 结点 7 */
    check(CIsTail(E, p->next), "结点 9 是表尾");
    check(p->next->next == E, "表尾 9 的 next 确实回到头结点");

    /* ===== 2. 尾插法建循环链表 ===== */
    printf("\n--- 2. 尾插法建循环链表 ---\n");
    C = CCreateByTail(a, 5);             /* C: 1 2 3 4 5 成环 */
    check(CLength(C) == 5, "建表后长度为 5");
    printf("C: "); CPrint(C);            /* 1 -> 2 -> 3 -> 4 -> 5 -> (回到头) */

    p = C->next;
    while (p->next != C)                 /* 找到表尾 */
        p = p->next;
    check(p->data == 5, "表尾结点数据是 5");
    check(CIsTail(C, p), "CIsTail 判断表尾正确");

    CTailInsert(C, 6);                   /* 表尾后继续尾插 6 */
    check(CLength(C) == 6, "表尾之后继续尾插 6 成功");
    printf("C: "); CPrint(C);            /* 1 -> ... -> 6 -> (回到头) */

    CHeadInsert(C, 0);                   /* 表头头插 0 */
    check(CLength(C) == 7, "表头头插 0 后长度为 7");
    check(CIsHead(C, C->next) && C->next->data == 0, "0 现在是第一个数据结点");
    printf("C: "); CPrint(C);            /* 0 -> 1 -> ... -> 6 -> (回到头) */

    /* ===== 3. 约瑟夫环 ===== */
    printf("\n--- 3. 约瑟夫环 ---\n");
    printf("Josephus(5,3) = "); r = Josephus(5, 3);
    check(arraysEqual(r, exp1, 5), "Josephus(5,3) 出列顺序 = 3 1 5 2 4");
    free(r);

    printf("Josephus(7,3) = "); r = Josephus(7, 3);
    check(arraysEqual(r, exp2, 7), "Josephus(7,3) 出列顺序 = 3 6 2 7 5 1 4");
    free(r);

    printf("Josephus(1,5) = "); r = Josephus(1, 5);
    check(r[0] == 1, "Josephus(1,5) 只剩 1 自己");
    free(r);

    CDestroyList(&C);
    CDestroyList(&E);

    printf("\n===== 测试结果：%d / %d 通过 =====\n", passed, tests);
    return 0;
}