#include <bits/stdc++.h>

using namespace std;

struct node {
    int key = 0;
    int priority = rand();
    int size = 1;
    node *left = nullptr;
    node *right = nullptr;

    node(int key_, int priority_, node *left_ = nullptr, node *right_ = nullptr) {
        key = key_;
        priority = priority_;
        left = left_;
        right = right_;
    }
} *ROOT = nullptr, *LEFT = nullptr, *RIGHT = nullptr;

int getNodeSize(node* &obj){
    if(obj == nullptr)
        return 0;
    return obj->size;
}

node *merge(node *&Left, node *&Right) {
    if (Left == nullptr)return Right;
    if (Right == nullptr)return Left;
    if (Left->priority < Right->priority) {
        Right->left = merge(Left, Right->left);
        Right->size = getNodeSize(Right->left) + getNodeSize(Right->right) + 1;
        return Right;
    } else {
        Left->right = merge(Left->right, Right);
        Left->size = getNodeSize(Left->left) + getNodeSize(Left->right) + 1;
        return Left;
    }
}

void split(int key, node *&current, node *&Left, node *&Right) {
    if (current == nullptr) return;
    if (current->key < key) {
        Left = current;
        node *next = current->right;
        Left->right = nullptr;
        split(key, next, Left->right, Right);
        Left->size = getNodeSize(Left->left) + getNodeSize(Left->right) + 1;
        next = nullptr;
        delete next;
    } else {
        Right = current;
        node *next = current->left;
        Right->left = nullptr;
        split(key, next, Left, Right->left);
        Right->size = getNodeSize(Right->left) + getNodeSize(Right->right) + 1;
        next = nullptr;
        delete next;
    }
}

void splitEq(int key, node *&current, node *&Left, node *&Right) {
    if (current == nullptr) return;
    if (current->key <= key) {
        Left = current;
        node *next = current->right;
        Left->right = nullptr;
        split(key, next, Left->right, Right);
        Left->size = getNodeSize(Left->left) + getNodeSize(Left->right) + 1;
        next = nullptr;
        delete next;
    } else {
        Right = current;
        node *next = current->left;
        Right->left = nullptr;
        split(key, next, Left, Right->left);
        Right->size = getNodeSize(Right->left) + getNodeSize(Right->right) + 1;
        next = nullptr;
        delete next;
    }
}

void insert(node *&root, int key, int priority = rand()) {
    node *L= nullptr,*R= nullptr;
    split(key, root, L, R);
    node *newNode = new node(key, priority);
    node *newLeft = merge(L, newNode );
    root = merge(newLeft, R);
    newNode = nullptr;
    newLeft = nullptr;
    delete newNode;
    delete newLeft;
}

void erase(node *&root, int key) {
    node *LL= nullptr,*LR= nullptr,*RL= nullptr,*RR= nullptr;
    split(key,root,LL,LR);
    splitEq(key,LR,RL,RR);
    root = merge(LL,RR);
}

int get(node* &root, int k){
    if(root == nullptr) return 0;
    if(getNodeSize(root->left) + 1 == k){
        return root->key;
    }
    if(getNodeSize(root->left) + 1 < k){
        return get(root->right,k - getNodeSize(root->left) - 1);
    }
    return get(root->left,k);
}

int main() {
    insert(ROOT,13,12);
    insert(ROOT,5,14);
    insert(ROOT,15,7);
    insert(ROOT,3,8);
    insert(ROOT,14,2);
    insert(ROOT,4,1);
    insert(ROOT,7,9);
    insert(ROOT,0,6);
    insert(ROOT,18,3);
    insert(ROOT,2,11);
    insert(ROOT,19,10);
    insert(ROOT,17,0);
    insert(ROOT,20,5);
    for(int i = 1; i <= 13;i++){
        cout<<get(ROOT,i)<<' ';
    }
//    cout<<get(ROOT,4)<<endl;

}
