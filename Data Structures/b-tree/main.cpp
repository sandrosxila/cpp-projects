#include <bits/stdc++.h>

using namespace std;

template<typename T>
class bTree {
private:
    int size = 0;
    int base = 0;
    int min_elements = 0;

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
        bool is_leaf = true;
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

        void addElement(element<bNode *> *new_element) {
            if (elements.empty() || elements.back()->value <= new_element->value) {
                elements.push_back(new_element);
            } else {
                for (int idx = 0; idx < elements.size(); idx++) {
                    if (elements[idx]->value > new_element->value) {
                        elements.insert(elements.begin() + idx, new_element);
                        if (idx >= 1) {
                            elements[idx - 1]->right_child = new_element->left_child;
                        }
                        if (idx + 1 < elements.size()) {
                            elements[idx + 1]->left_child = new_element->right_child;
                        }
                        break;
                    }
                }
            }
        }

    } *ROOT = nullptr;

    tuple<bNode *, int, int, bool> find(T elem, bNode *&current, int parent_index = -1, bool side = false) {
        if (elem > current->elements.back()->value) {
            if (current->elements.back()->right_child != nullptr)
                return find(elem, current->elements.back()->right_child, int(current->elements.size()) - 1, true);
            else return {current, int(current->elements.size()), parent_index, side};
        } else {
            for (int idx = 0; idx < current->elements.size(); idx++) {
                if (current->elements[idx]->value == elem) {
                    return {current, idx, parent_index, side};
                }
                if (current->elements[idx]->value > elem) {
                    if (current->elements[idx]->left_child != nullptr)
                        return find(elem, current->elements[idx]->left_child, idx, false);
                    else {
                        return {current, idx, parent_index, side};
                    }
                }
            }
        }
        return {nullptr, 0, parent_index, side};
    }

    void lift_up(bNode *node) {
        if (node->elements.size() == base) {
            if (node->parent == nullptr) {
                node->parent = new bNode(&size);
            }
            element<bNode *> *new_element = node->elements[node->elements.size() / 2];
            bNode *new_node = new bNode(&size);

            for (int i = node->elements.size() / 2 + 1; i < node->elements.size(); i++) {
                if (node->elements[i]->left_child != nullptr)
                    node->elements[i]->left_child->parent = new_node;
                if (node->elements[i]->right_child != nullptr)
                    node->elements[i]->right_child->parent = new_node;
                new_node->addElement(node->elements[i]);
            }
            for (int i = node->elements.size() / 2; i != node->elements.size(); node->elements.pop_back());

            new_node->is_leaf = node->is_leaf;
            new_node->parent = node->parent;
            new_element->right_child = new_node;
            new_element->left_child = node;
            node->parent->addElement(new_element);
            node->parent->is_leaf = false;

            if (node == ROOT) {
                ROOT = node->parent;
            }

            if (node->parent->elements.size() == base) {
                lift_up(node->parent);
            }

//            new_element = nullptr;
//            delete new_element;
//
//            new_node = nullptr;
//            delete new_node;
        }
    }

    void insert(T elem, bNode *&root) {
        if (root == nullptr) {
            root = new bNode(&size);
            root->addElement(elem);
        } else {
            tuple<bNode *, int, int, bool> place = find(elem, root);
            bNode *node = get<0>(place);
            int pos = get<1>(place);
            node->addElement(elem, pos);
            if (node->elements.size() == base) {
                lift_up(node);
            }
        }
    }

    void erase(T elem, bNode *&root) {
        if (root != nullptr) {
            tuple<bNode *, int, int, bool> place = find(elem, root);
            bNode *node = get<0>(place);
            int pos = get<1>(place);
            int parent_pos = get<2>(place);
            int side = get<3>(place);
            if (node != nullptr) {
                if (node->is_leaf) {
                    if (node->elements.size() > min_elements) {
                        node->elements.erase(node->elements.begin() + pos);
                    } else if (node->elements.size() == min_elements) {
                        bNode *right_sibling = nullptr;
                        bNode *left_sibling = nullptr;
                        int left_parent_pos, right_parent_pos;
                        if (side == false) {
                            right_parent_pos = parent_pos;
                            right_sibling = node->parent->elements[right_parent_pos]->right_child;
                            if (parent_pos != 0) {
                                left_parent_pos = parent_pos - 1;
                                left_sibling = node->parent->elements[left_parent_pos]->left_child;
                            }
                        }
                        if (side == true) {
                            left_parent_pos = parent_pos;
                            left_sibling = node->parent->elements[left_parent_pos]->left_child;
                            if (parent_pos != node->parent->elements.size() - 1) {
                                right_parent_pos = parent_pos + 1;
                                right_sibling = node->parent->elements[right_parent_pos]->left_child;
                            }
                        }
                        if (left_sibling != nullptr && left_sibling->elements.size() > min_elements) {
                            node->elements.erase(node->elements.begin() + pos);
                            node->addElement(node->parent->elements[parent_pos]->value);
                            node->parent->elements[parent_pos]->value = left_sibling->elements.back()->value;
                            left_sibling->elements.pop_back();
                        }
                        else if (right_sibling != nullptr && right_sibling->elements.size() > min_elements){
                            node->elements.erase(node->elements.begin() + pos);
                            node->addElement(node->parent->elements[parent_pos]->value);
                            node->parent->elements[parent_pos]->value = right_sibling->elements.front()->value;
                            right_sibling->elements.erase(right_sibling->elements.begin());
                        }
                    }
                }
            }
        }
    }

    void build_test(bNode *&root) {
        // root node
        root = new bNode(&size);
        root->is_leaf = false;
        root->addElement(new element<bNode *>(50));
        root->addElement(new element<bNode *>(80));

        //middle nodes

        //middle node 1
        auto middle_node1 = new bNode(&size);
        middle_node1->is_leaf = false;
        middle_node1->addElement(new element<bNode *>(10));
        middle_node1->addElement(new element<bNode *>(20));
        //middle node 2
        auto middle_node2 = new bNode(&size);
        middle_node2->is_leaf = false;
        middle_node2->addElement(new element<bNode *>(60));
        middle_node2->addElement(new element<bNode *>(70));
        middle_node2->addElement(new element<bNode *>(75));
        //middle node 3
        auto middle_node3 = new bNode(&size);
        middle_node3->is_leaf = false;
        middle_node3->addElement(new element<bNode *>(90));
        middle_node3->addElement(new element<bNode *>(95));

        //low nodes

        //low node 1
        auto low_node1 = new bNode(&size);
        low_node1->addElement(new element<bNode *>(4));
        low_node1->addElement(new element<bNode *>(5));
        low_node1->addElement(new element<bNode *>(6));

        //low node 2
        auto low_node2 = new bNode(&size);
        low_node2->addElement(new element<bNode *>(14));
        low_node2->addElement(new element<bNode *>(15));
        low_node2->addElement(new element<bNode *>(16));

        //low node 3
        auto low_node3 = new bNode(&size);
        low_node3->addElement(new element<bNode *>(23));
        low_node3->addElement(new element<bNode *>(27));

        //low node 4
        auto low_node4 = new bNode(&size);
        low_node4->addElement(new element<bNode *>(51));
        low_node4->addElement(new element<bNode *>(52));

        //low node 5
        auto low_node5 = new bNode(&size);
        low_node5->addElement(new element<bNode *>(64));
        low_node5->addElement(new element<bNode *>(65));
        low_node5->addElement(new element<bNode *>(68));

        //low node 6
        auto low_node6 = new bNode(&size);
        low_node6->addElement(new element<bNode *>(72));
        low_node6->addElement(new element<bNode *>(73));

        //low node 7
        auto low_node7 = new bNode(&size);
        low_node7->addElement(new element<bNode *>(77));
        low_node7->addElement(new element<bNode *>(78));
        low_node7->addElement(new element<bNode *>(79));

        //low node 8
        auto low_node8 = new bNode(&size);
        low_node8->addElement(new element<bNode *>(81));
        low_node8->addElement(new element<bNode *>(82));
        low_node8->addElement(new element<bNode *>(89));

        //low node 9
        auto low_node9 = new bNode(&size);
        low_node9->addElement(new element<bNode *>(92));
        low_node9->addElement(new element<bNode *>(93));

        //low node 10
        auto low_node10 = new bNode(&size);
        low_node10->addElement(new element<bNode *>(108));
        low_node10->addElement(new element<bNode *>(110));
        low_node10->addElement(new element<bNode *>(118));

        //building low-level

        low_node1->parent = middle_node1;
        low_node2->parent = middle_node1;
        low_node3->parent = middle_node1;

        middle_node1->elements[0]->left_child = low_node1;
        middle_node1->elements[0]->right_child = low_node2;
        middle_node1->elements[1]->left_child = low_node2;
        middle_node1->elements[1]->right_child = low_node3;

        low_node4->parent = middle_node2;
        low_node5->parent = middle_node2;
        low_node6->parent = middle_node2;
        low_node7->parent = middle_node2;

        middle_node2->elements[0]->left_child = low_node4;
        middle_node2->elements[0]->right_child = low_node5;
        middle_node2->elements[1]->left_child = low_node5;
        middle_node2->elements[1]->right_child = low_node6;
        middle_node2->elements[2]->left_child = low_node6;
        middle_node2->elements[2]->right_child = low_node7;

        low_node8->parent = middle_node3;
        low_node9->parent = middle_node3;
        low_node10->parent = middle_node3;

        middle_node3->elements[0]->left_child = low_node8;
        middle_node3->elements[0]->right_child = low_node9;
        middle_node3->elements[1]->left_child = low_node9;
        middle_node3->elements[1]->right_child = low_node10;

        //building high-level

        middle_node1->parent = root;
        middle_node2->parent = root;
        middle_node3->parent = root;

        root->elements[0]->left_child = middle_node1;
        root->elements[0]->right_child = middle_node2;
        root->elements[1]->left_child = middle_node2;
        root->elements[1]->right_child = middle_node3;

        size = 35;

    }

public:
    explicit bTree(int Base) {
        base = Base;
        min_elements = (Base + 1) / 2 - 1;
    }

    void insert(T elem) {
        insert(elem, ROOT);
    };

    void erase(T elem) {
        erase(elem, ROOT);
    }

    void test() {
        build_test(ROOT);
    }

};

int main() {

    bTree<int> b(5);
    b.test();
    b.erase(64);
    b.erase(23);
    b.erase(72);
    cout << "end";

    return 0;
}
