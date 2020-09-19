#include <bits/stdc++.h>

using namespace std;

template <class T>
class Treap {
private:
    struct node {
        T key;
        int priority = rand();
        int size = 1;
        node *left = nullptr;
        node *right = nullptr;

        node(T key_, int priority_, node *left_ = nullptr, node *right_ = nullptr) {
            key = key_;
            priority = priority_;
            left = left_;
            right = right_;
        }
    } *ROOT = nullptr, *LEFT = nullptr, *RIGHT = nullptr;

    inline int getNodeSize(node* &obj){
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

    void split(T key, node *&current, node *&Left, node *&Right) {
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

    void splitEq(T key, node *&current, node *&Left, node *&Right) {
        if (current == nullptr) return;
        if (current->key <= key) {
            Left = current;
            node *next = current->right;
            Left->right = nullptr;
            splitEq(key, next, Left->right, Right);
            Left->size = getNodeSize(Left->left) + getNodeSize(Left->right) + 1;
            next = nullptr;
            delete next;
        } else {
            Right = current;
            node *next = current->left;
            Right->left = nullptr;
            splitEq(key, next, Left, Right->left);
            Right->size = getNodeSize(Right->left) + getNodeSize(Right->right) + 1;
            next = nullptr;
            delete next;
        }
    }

    void insert(node *&root, T key, int priority = rand()) {
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

    void destroy(node *&current){
        if(current == nullptr) return;
        destroy(current->left);
        destroy(current->right);
        delete current;
    }

    bool destroy_one(node *&current){
        if(current == nullptr) return false;
        if(current->left == nullptr && current->right == nullptr){
            current = nullptr;
            delete current;
            return true;
        }
        if(!destroy_one(current->left)){
            if(destroy_one(current->right)){
                current->size = getNodeSize(current->left) + getNodeSize(current->right) + 1;
                return true;
            }
        }
        current->size = getNodeSize(current->left) + getNodeSize(current->right) + 1;
        return true;
    }

    void erase(node *&root, T key) {
        node *LL= nullptr,*LR= nullptr,*RL= nullptr,*RR= nullptr;
        split(key,root,LL,LR);
        splitEq(key,LR,RL,RR);
//    destroy(LR);
        destroy(RL);
        root = merge(LL,RR);
    }

    void pop(node *&root, T key) {
        node *LL= nullptr,*LR= nullptr,*RL= nullptr,*RR= nullptr;
        split(key,root,LL,LR);
        splitEq(key,LR,RL,RR);
        destroy_one(RL);
        node *RH = merge(RL,RR);
        root = merge(LL,RH);
    }

    T get(node* &root, int k){
        if(root == nullptr) return -1;
        if(getNodeSize(root->left) + 1 == k){
            return root->key;
        }
        if(getNodeSize(root->left) + 1 < k){
            return get(root->right,k - getNodeSize(root->left) - 1);
        }
        return get(root->left,k);
    }

    int getIndex(node* &root, T key, int index = 0){
        if(root == nullptr) return -1;
        if(root->key > key){
            return getIndex(root->left, key, index );
        }
        else if(root->key < key){
            return getIndex(root->right, key, index + getNodeSize(root->left) + 1);
        }
        return index + getNodeSize(root->left) + 1;
    }
public:
    void insert(T key, int priority = rand()){
        insert(ROOT,key,priority);
    }
    void erase(T key){
        erase(ROOT,key);
    }
    void pop(T key){
        pop(ROOT,key);
    }
    T get(int index){
        return get(ROOT,index);
    }
    int getIndex(T key){
        return getIndex(ROOT, key);
    }
    inline int size(){
        return getNodeSize(ROOT);
    }
};

int t,n;
int main() {
    Treap <int> T;
    T.insert(15);
    T.insert(29);
    T.insert(6);
    T.insert(9);
    T.insert(9);
    T.insert(58);
    T.insert(58);
    T.insert(58);
    for(int i = 1; i<=T.size(); i++){
        cout<< T.get(i) << ' ';
    }
    cout<<endl;
    T.pop(58);
    T.pop(109);
    T.pop(15);
    T.insert(25);
    T.insert(25);
    T.insert(25);
    T.erase(25);
    T.insert(25);
    T.insert(25);
    T.pop(25);
//    cout<<T.get(7);
    for(int i = 1; i<=T.size(); i++){
        cout<< T.get(i) << ' ';
    }
    cout<<endl;
    cout<<T.getIndex(58)<<endl;
    cout<<T.get(4)<<endl;

}