#include <bits/stdc++.h>

using namespace std;

struct node {
    int key = 0;
    int priority = rand();
    node *left = nullptr;
    node *right = nullptr;

    node(int key_, int priority_, node *left_ = nullptr, node *right_ = nullptr) {
        key = key_;
        priority = priority_;
        left = left_;
        right = right_;
    }
} *ROOT = nullptr, *LEFT = nullptr, *RIGHT = nullptr;

node *merge(node *&Left, node *&Right) {
    if (Left == nullptr)return Right;
    if (Right == nullptr)return Left;
    if (Left->priority < Right->priority) {
        Right->left = merge(Left, Right->left);
        return Right;
    } else {
        Left->right = merge(Left->right, Right);
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
        next = nullptr;
        delete next;
    } else {
        Right = current;
        node *next = current->left;
        Right->left = nullptr;
        split(key, next, Left, Right->left);
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
        next = nullptr;
        delete next;
    } else {
        Right = current;
        node *next = current->left;
        Right->left = nullptr;
        split(key, next, Left, Right->left);
        next = nullptr;
        delete next;
    }
}

void insert(int key, node *&root) {
    node *L= nullptr,*R= nullptr;
    split(key, root, L, R);
    node *newNode = new node(key, rand());
    node *newLeft = merge(L, newNode );
    root = merge(newLeft, R);
    newNode = nullptr;
    newLeft = nullptr;
    delete newNode;
    delete newLeft;
}

void erase(int key, node *&root) {
    node *LL= nullptr,*LR= nullptr,*RL= nullptr,*RR= nullptr;
    split(key,root,LL,LR);
    splitEq(key,LR,RL,RR);
    root = merge(LL,RR);
}

int main() {

}
