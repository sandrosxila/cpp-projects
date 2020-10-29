#include <bits/stdc++.h>

using namespace std;

template<typename T>
class bTree {
private:
    int size = 0;
    int base = 0;

    template<typename U>
    struct element {
        T value;
        U left_child = nullptr;
        U right_child = nullptr;

        element(T val) {
            value = val;
        }
    };

    struct bNode {
        int *size;
        bNode *parent = nullptr;
        vector<element<bNode *> *> elements;

        bNode(int *_size) {
            size = _size;
        }

        void addElement(T elem, int pos = -1) {
            if (elements.empty() || elements.back()->value <= elem || pos == elements.size()) {
                elements.push_back(new element<bNode *>(elem));
            } else if (pos != -1) {
                elements.insert(elements.begin() + pos, new element<bNode *>(elem));
            } else {
                for (int idx = 0; idx < elements.size(); idx++) {
                    if (elements[idx]->value > elem) {
                        elements.insert(elements.begin() + idx, new element<bNode *>(elem));
                        break;
                    }
                }
            }
            *size = *size + 1;
        }

        void addElement(element<bNode *> *new_element){
            if(elements.empty() || elements.back()->value <= new_element->value){
                elements.push_back(new_element);
            }
            else {
                for (int idx = 0; idx < elements.size(); idx++){
                    if(elements[idx]->value > new_element->value){
                        elements.insert(elements.begin() + idx, new_element);
                        if(idx >= 1){
                            elements[idx - 1]->right_child = new_element->left_child;
                        }
                        if(idx + 1 < elements.size()){
                            elements[idx + 1]->left_child = new_element->right_child;
                        }
                        break;
                    }
                }
            }
        }

    } *ROOT = nullptr;

    pair<bNode *, int> find(T elem, bNode *&current) {
        if (elem > current->elements.back()->value) {
            if (current->elements.back()->right_child != nullptr)
                return find(elem, current->elements.back()->right_child);
            else return {current, int(current->elements.size())};
        } else {
            for (int idx = 0; idx < current->elements.size(); idx++) {
                if (current->elements[idx]->value == elem) {
                    return {current, idx};
                }
                if (current->elements[idx]->value > elem) {
                    if (current->elements[idx]->left_child != nullptr)
                        return find(elem, current->elements[idx]->left_child);
                    else {
                        return {current, idx};
                    }
                }
            }
        }
        return {nullptr, 0};
    }

    void lift_up(bNode *&node) {
        if (node->elements.size() == base) {
            if (node->parent == nullptr) {
                node->parent = new bNode(&size);
            }
            element<bNode *> *new_element = new element<bNode *>(node->elements[node->elements.size() / 2]->value);
            bNode *new_node = new bNode(&size);

            for (int i = node->elements.size() / 2 + 1; i < node->elements.size(); i++) {
                new_node->addElement(node->elements[i]);
            }
            for (int i = node->elements.size() / 2; i != node->elements.size(); node->elements.pop_back());

            new_element->right_child = new_node;
            new_element->left_child = node;
            new_element->right_child->parent = node->parent;
            new_element->left_child->parent = node->parent;
            node->parent->addElement(new_element);

            if(node == ROOT){
                ROOT = node->parent;
            }

            if(node->parent->elements.size() == base){
                lift_up(node->parent);
            }

            new_element = nullptr;
            delete new_element;

            new_node = nullptr;
            delete new_node;
        }
    }

    void insert(T elem, bNode *&root) {
        if (root == nullptr) {
            root = new bNode(&size);
            root->addElement(elem);
        } else {
            pair<bNode *, int> place = find(elem, root);
            bNode *node = place.first;
            int pos = place.second;
            node->addElement(elem, pos);
            if(node->elements.size() == base){
                lift_up(node);
            }
        }
    }



public:
    explicit bTree(int Base) {
        base = Base;
    }

    void insert(T elem) {
        insert(elem, ROOT);
    };

};

int main() {

    bTree<int> b(5);
    b.insert(20);
    b.insert(10);
    b.insert(40);
    b.insert(30);
    b.insert(50);
    b.insert(70);
    b.insert(60);
    b.insert(80);
    b.insert(11);
    b.insert(12);
    b.insert(13);
    b.insert(21);
    b.insert(22);
    b.insert(23);
    b.insert(42);
    b.insert(43);
    b.insert(45);
//    b.insert(95);
    cout << "end";

    return 0;
}
