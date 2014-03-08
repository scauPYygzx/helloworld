#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define LH 1
#define EH 0
#define RH -1
#define  EQ(x,y) ((x)==(y))
#define  LT(x,y) ((x) < (y))
#define  LQ(x,y) ((x) <= (y))

using namespace std;

typedef int ElemType;

typedef struct node {
    ElemType data;
    int bf;
    struct node *lchild, *rchild;
}node;

void R_Rotate(node* &p) {
    node* lc = p -> lchild;
    p -> lchild = lc -> rchild;
    lc -> rchild = p;
    p = lc;
}

void L_Rotate(node* &p) {
    node* rc = p -> rchild;
    p -> rchild = rc -> lchild;
    rc -> lchild = p;
    p = rc; 
}

void LeftBalance(node* &T) {
    node* lc = T -> lchild;
    node* rd;
    switch (lc -> bf){
        case LH:
            T -> bf = lc -> bf = EH;
            R_Rotate(T);
            break;
        case RH:
            rd = lc -> rchild;
            switch (rd -> bf) {
                case LH:T -> bf = RH; lc -> bf = EH; break;
                case EH:T -> bf = lc -> bf = EH; break;
                case RH:T -> bf = EH; lc -> bf = LH; break;
            }
            rd -> bf = EH;
            L_Rotate(T -> lchild);
            R_Rotate(T);
    }    
}

void RightBalance(node* &T) {
    node* rc = T -> rchild;
    node* rd;
    switch (rc -> bf){
        case LH:
            T -> bf = rc -> bf = EH;
            L_Rotate(T);
            break;
        case RH:
            rd = rc -> lchild;
            switch (rd -> bf) {
                case LH:T -> bf = RH; rc -> bf = EH; break;
                case EH:T -> bf = rc -> bf = EH; break;
                case RH:T -> bf = EH; rc -> bf = LH; break;
            }
            rd -> bf = EH;
            R_Rotate(T -> rchild);
            L_Rotate(T);
    }    
}

int insert(node* &T, ElemType e, int &taller) {
    if(!T) {
        T = (node*)malloc(sizeof(node));
        T -> lchild = T -> rchild = NULL;
        T -> bf = EH;
        taller = 1;
    }
    else {
        if (EQ(e, T -> data)) {
            taller = 0;
            return 0;
        }
        if(LT(e, T -> data)) { 
            if(!insert(T -> lchild, e, taller)) return 0;
            if(taller) {
                switch (T -> bf){
                    case LH:
                        LeftBalance(T);
                        taller = 0;
                        break;
                    case EH:
                        T -> bf = LH;
                        taller = 1;
                        break;
                    case RH:
                        T -> bf = EH;
                        taller = 0;
                        break;
                }
            }
        }
        else {
            if(!insert(T -> rchild, e, taller)) return 0;
            if(taller) {
                switch (T -> bf){
                    case LH:
                        T -> bf = EH;
                        taller = 0;
                        break;
                    case EH:
                        T -> bf = RH;
                        taller = 1;
                        break;
                    case RH:
                        RightBalance(T);
                        taller = 0;
                        break;
                }
            }
        }
    }
    return 1;
}

void LDR(node *T) {
    if(T) {
        LDR(T -> lchild);
        printf("%d ", T -> data);
        LDR(T -> rchild);
    }
}

int main() {
    int n, m, tmp = 0;
    node* root = NULL;
    scanf("%d", &n);
    while(n--) {
        scanf("%d", &m);
        insert(root, m, tmp);   
        if(!root) puts("null"); 
    }
    //LDR(root);
    //puts("s");
    //printf("%d\n", root->data);
    system("pause");
    return 0;
}
/*
5
1 2 3 4 5
*/
