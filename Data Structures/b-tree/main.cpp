#pragma GCC optimize("Ofast,unroll-loops")
#pragma GCC target("avx,avx2,sse,sse2")

#include <bits/stdc++.h>

using namespace std;

//declaration of b-Tree class

template<typename T>
class bTree {
private:
//    b-Tree private properties:
    int _size = 0;
    int _base = 0;
    int min_elements = 0;

//    pre-declaration of bNode struct
    struct bNode;

//    declaration of the element structure which will be stored in b-Tree node (bNode)

    struct element {
        T value;
        bNode *left_child;
        bNode *right_child;

        explicit element(T value, bNode *leftChild = nullptr, bNode *rightChild = nullptr) : value(value),
                                                                                             left_child(leftChild),
                                                                                             right_child(rightChild) {}

//        check if element has a right child
        bool has_right_child() {
            return right_child != nullptr;
        }

//        check if element has a left child
        bool has_left_child() {
            return left_child != nullptr;
        }

    }  *leftmost = nullptr;

//    b-Tree will be made up with bNode structures

    struct bNode {
//        bNode properties with default values
        bool is_leaf = true;
        bNode *parent = nullptr;
        deque<element *> elements;

//        check if the elements are empty
        bool empty() {
            return elements.empty();
        }

//        get quantity of bNode elements
        int size() {
            return elements.size();
        }

//        get element at index
        element *at(int index) {
            if (empty())
                return nullptr;
            return elements[index];
        }

//        get the first element
        element *first() {
            return at(0);
        }

//        get the last element
        element *last() {
            return at(size() - 1);
        }

//        check if bNode has a parent
        bool has_parent() {
            return parent != nullptr;
        }

//        check if the first element of bNode has a left child
        bool has_leftmost_child() {
            if (first() == nullptr)
                return false;
            return first()->has_left_child();
        }

//        check if the last element of bNode has a right child
        bool has_rightmost_child() {
            if (last() == nullptr)
                return false;
            return last()->has_right_child();
        }

//        get value of the first element
        T first_value() {
            return first()->value;
        }

//        get left child of the first element
        bNode *first_left_child() {
            return first()->left_child;
        }

//        get right child of the first element
        bNode *first_right_child() {
            return first()->right_child;
        }

//        get value of the last element
        T last_value() {
            return last()->value;
        }

//        get value of the element at index
        T value_at(int index) {
            return at(index)->value;
        }

//        get a left child of the element at index
        bNode *left_child_at(int index) {
            return at(index)->left_child;
        }

//        get a right child of the element at index
        bNode *right_child_at(int index) {
            return at(index)->right_child;
        }

//        check if element at index has a left child
        bool has_left_child_at(int index) {
            if (index > size() || index < 0 || empty()) {
                return false;
            }
            if (at(index)->has_left_child()) {
                return true;
            }
            return false;
        }

//        check if element at index has a right child
        bool has_right_child_at(int index) {
            if (index >= size() || index < 0 || empty()) {
                return false;
            }
            if (at(index)->has_right_child()) {
                return true;
            }
            return false;
        }

//        get the right child of the last element of b-Node
        bNode *get_rightmost_child() {
            if (empty())
                return nullptr;
            return last()->right_child;
        }

//        get the left child of the first element of b-Node
        bNode *get_leftmost_child() {
            if (empty())
                return nullptr;
            return first()->left_child;
        }

//        add an element at the first position
        void add_first(element *elem) {
            elements.push_front(elem);
        }

//        add an element at the last position
        void add_last(element *elem) {
            elements.push_back(elem);
        }

//        add the element at index
        void add_at(int index, element *elem) {
            if (index == elements.size()) {
                add_last(elem);
            }
            else elements.insert(elements.begin() + index, elem);
        }

//        pop an element at the first position
        element *pop_first() {
            element *elem = first();
            elements.pop_front();
            return elem;
        }

//        pop an element at the last position
        element *pop_last() {
            element *elem = last();
            elements.pop_back();
            return elem;
        }

//        pop an element at index
        element *pop_at(int index) {
            element *elem = at(index);
            elements.erase(elements.begin() + index);
            return elem;
        }

//        add new element automatically
        void add(element *elem, int idx, bool side) {
            if (empty() || (idx == size() - 1 && side == true)) {
                add_last(elem);
            }
            else {
                if (has_left_child_at(idx))
                    at(idx)->left_child = elem->right_child;
                if (has_right_child_at(idx - 1))
                    at(idx - 1)->right_child = elem->left_child;
                add_at(idx, elem);
            }
        }
    } *ROOT = nullptr, *recent_node = nullptr;

//    split the b-Node and return new node
    bNode *split(bNode *node) {
        bNode *new_node = new bNode();
        new_node->is_leaf = node->is_leaf;
        while (node->size() > _base / 2) {
            new_node->add_first(node->pop_last());
            if (new_node->first()->has_left_child())
                new_node->first()->left_child->parent = new_node;
            if (new_node->first()->has_right_child())
                new_node->first()->right_child->parent = new_node;
        }
        return new_node;
    }

//    split up node and increase height of b-tree
    void lift_up(bNode *node, int parent_index, bool side) {
        element *parent_element = node->pop_at(_base / 2);
        bNode *new_node = split(node);

        if (!node->has_parent()) {
            node->parent = new bNode();
            node->parent->is_leaf = false;
        }

        if (ROOT == node)
            ROOT = node->parent;

        new_node->parent = node->parent;
        parent_element->left_child = node;
        parent_element->right_child = new_node;

        node->parent->add(parent_element, parent_index, side);
        node->parent->is_leaf = false;
    }

//    insert an element in b-tree
    void insert(T elem, bNode *current, int parent_index = -1, bool side = false, bool is_leftmost = true) {
        if (current->empty()) {
            current->add_first(new element(elem));
            if (is_leftmost)
                leftmost = current->first();
            _size++;
        }
        else if (elem >= current->last()->value) {
            if (current->has_rightmost_child()) {
                insert(elem, current->get_rightmost_child(), current->size() - 1, true, false);
            }
            else {
                current->add_last(new element(elem));
                _size++;
            }
        }
        else {
            for (int idx = 0; idx < current->size(); idx++) {
                if (elem < current->value_at(idx)) {
                    if (current->has_left_child_at(idx)) {
                        insert(elem, current->left_child_at(idx), idx, false, (idx == 0) && is_leftmost);
                        break;
                    }
                    else {
                        current->add_at(idx, new element(elem));
                        if (idx == 0 && is_leftmost)
                            leftmost = current->first();
                        _size++;
                        break;
                    }
                }
            }
        }
        if (current->size() == _base) {
            lift_up(current, parent_index, side);
        }
    }

//    get right sibling node
    bNode *get_right_sibling(bNode *node, bool side, int parent_pos) {
        if (node->has_parent()) {
            if (node->parent->has_right_child_at(side ? parent_pos + 1 : parent_pos))
                return node->parent->at(side ? parent_pos + 1 : parent_pos)->right_child;
        }
        return nullptr;
    }

//    get left sibling node
    bNode *get_left_sibling(bNode *node, bool side, int parent_pos) {
        if (node->has_parent()) {
            if (node->parent->has_left_child_at(side ? parent_pos : parent_pos - 1))
                return node->parent->at(side ? parent_pos : parent_pos - 1)->left_child;
        }
        return nullptr;
    }

//    merge elements from right sibling to left sibling
    void merge_to_left(bNode *&left_node, bNode *right_node) {
        if (!right_node->empty()) {
            left_node->last()->right_child = right_node->get_leftmost_child();
            if (left_node->has_rightmost_child())
                left_node->last()->right_child->parent = left_node;
            while (!right_node->empty()) {
                left_node->add_last(right_node->pop_first());
                if (left_node->last()->left_child != nullptr)
                    left_node->last()->left_child->parent = left_node;
                if (left_node->last()->right_child != nullptr)
                    left_node->last()->right_child->parent = left_node;
            }
        }
        right_node->parent = nullptr;
        delete right_node;
        right_node = nullptr;
    }

//    merge elements from left sibling to right sibling
    void merge_to_right(bNode *left_node, bNode *&right_node) {
        if (!left_node->empty()) {
            right_node->first()->left_child = left_node->get_rightmost_child();
            if (right_node->has_leftmost_child())
                right_node->first()->left_child->parent = right_node;
            while (!left_node->empty()) {
                right_node->add_first(left_node->pop_last());
                if (right_node->first()->left_child != nullptr)
                    right_node->first()->left_child->parent = right_node;
                if (right_node->first()->right_child != nullptr)
                    right_node->first()->right_child->parent = right_node;
            }
        }
        left_node->parent = nullptr;
        delete left_node;
        left_node = nullptr;
    }

//    shift one element from left child to parent and from parent to right child
    void propagate_from_left_sibling(bNode *node, int parent_pos, bNode *left_sibling) {
        element *node_element = node->parent->pop_at(parent_pos);
        node_element->left_child = left_sibling->get_rightmost_child();
        node_element->right_child = node->has_leftmost_child() ? node->get_leftmost_child() : recent_node;
        if (node_element->has_left_child())
            node_element->left_child->parent = node;
        if (node_element->has_right_child())
            node_element->right_child->parent = node;
        node->add_first(node_element);
        element *parent_element = left_sibling->pop_last();
        parent_element->left_child = left_sibling;
        parent_element->right_child = node;
        node->parent->add_at(parent_pos, parent_element);
    }

//    shift one element from right child to parent and from parent to left child
    void propagate_from_right_sibling(bNode *node, int parent_pos, bNode *right_sibling) {
        element *node_element = node->parent->pop_at(parent_pos);
        node_element->left_child = node->has_rightmost_child() ? node->get_rightmost_child() : recent_node;
        node_element->right_child = right_sibling->get_leftmost_child();
        if (node_element->has_left_child())
            node_element->left_child->parent = node;
        if (node_element->has_right_child())
            node_element->right_child->parent = node;
        node->add_last(node_element);
        element *parent_element = right_sibling->pop_first();
        parent_element->left_child = node;
        parent_element->right_child = right_sibling;
        node->parent->add_at(parent_pos, parent_element);
    }

//    push down parent node element to it's right child and then merge the children
    void propagate_down_from_left(bNode *node, int parent_pos, bNode *left_sibling) {
        element *parent_element = node->parent->pop_at(parent_pos);
        if (node->parent->has_right_child_at(parent_pos - 1)) {
            node->parent->at(parent_pos - 1)->right_child = node;
        }
        parent_element->left_child = nullptr;
        parent_element->right_child = node->has_leftmost_child() ? node->get_leftmost_child() : recent_node;
        node->add_first(parent_element);
        node->is_leaf = node->is_leaf && left_sibling->is_leaf;
        merge_to_right(left_sibling, node);
        if (node->parent == ROOT && ROOT->size() == 0) {
            ROOT = node;
            delete node->parent;
            node->parent = nullptr;
        }
    }

//    push down parent node element to it's left child and then merge the children
    void propagate_down_from_right(bNode *node, int parent_pos, bNode *right_sibling) {
        element *parent_element = node->parent->pop_at(parent_pos);
        if (node->parent->has_left_child_at(parent_pos)) {
            node->parent->at(parent_pos)->left_child = node;
        }
        parent_element->left_child = node->has_rightmost_child() ? node->get_rightmost_child() : recent_node;
        parent_element->right_child = nullptr;
        node->add_last(parent_element);
        node->is_leaf = node->is_leaf && right_sibling->is_leaf;
        merge_to_left(node, right_sibling);
        if (node->parent == ROOT && ROOT->size() == 0) {
            ROOT = node;
            delete node->parent;
            node->parent = nullptr;
        }
    }

//    restore minimum number of elements in node
    void balance(bNode *node, int parent_position, bool side) {
        bNode *left_sibling = get_left_sibling(node, side, parent_position);
        bNode *right_sibling = get_right_sibling(node, side, parent_position);

//        case 3: when leaf node has minimum number of elements and left-sibling with more than minimum number of elements
        if (left_sibling != nullptr && left_sibling->size() > min_elements)
            propagate_from_left_sibling(node, side ? parent_position : parent_position - 1, left_sibling);
//        case 4: when leaf node has minimum number of elements and right-sibling with more than minimum number of elements
        else if (right_sibling != nullptr && right_sibling->size() > min_elements)
            propagate_from_right_sibling(node, side ? parent_position + 1 : parent_position, right_sibling);
//        case 5: when leaf node has minimum number of elements and left-sibling with minimum number of elements
        else if (left_sibling != nullptr && left_sibling->size() <= min_elements)
            propagate_down_from_left(node, side ? parent_position : parent_position - 1, left_sibling);
//        case 6: when leaf node has minimum number of elements and right-sibling with minimum number of elements
        else if (right_sibling != nullptr && right_sibling->size() <= min_elements)
            propagate_down_from_right(node, side ? parent_position + 1 : parent_position, right_sibling);
        recent_node = node;
    }

//    take inorder predecessor and balance affected nodes excluding the root node of sub-tree
    element *take_inorder_predecessor(bNode *current, int parent_position, bool side, bool top = true) {
        element *inorder_predecessor =
                current->has_rightmost_child() ?
                take_inorder_predecessor(current->get_rightmost_child(), current->size() - 1, true, false)
                                               : current->pop_last();
        if (!top && current->size() < min_elements)
            balance(current, parent_position, side);
        return inorder_predecessor;
    }

//    take inorder successor and balance affected nodes excluding the root node of sub-tree
    element *take_inorder_successor(bNode *current, int parent_position, bool side, bool top = true) {
        element *inorder_successor =
                current->has_leftmost_child() ?
                take_inorder_successor(current->get_leftmost_child(), 0, false, false) : current->pop_first();
        if (!top && current->size() < min_elements)
            balance(current, parent_position, side);
        return inorder_successor;
    }

//    different cases of deletion are implemented here
    void erase_dispatch(bNode *node, int position, int parent_position, bool side) {
        if (node->is_leaf) {
//            case 1: when leaf-node has more than minimum number of elements
            if (node->size() > min_elements) {
                delete node->pop_at(position);
            }
//            case 2: when leaf-node has not more than minimum number of elements
            else {
                delete node->pop_at(position);
                balance(node, parent_position, side);
            }
        }
        else {
//            case 7: when internal node has a left child
            if (node->has_left_child_at(position)) {
                element *elem = node->at(position);
                element *inorder_predecessor = take_inorder_predecessor(elem->left_child, position, false);
                inorder_predecessor->left_child = elem->left_child;
                inorder_predecessor->right_child = elem->right_child;
                delete node->pop_at(position);
                node->add_at(position, inorder_predecessor);
                if (node->left_child_at(position)->size() < min_elements)
                    balance(node->left_child_at(position), position, false);
            }
//            case 8: when internal node has a right child
            else if (node->has_right_child_at(position)) {
                element *elem = node->at(position);
                element *inorder_successor = take_inorder_successor(elem->right_child, position, true);
                inorder_successor->left_child = elem->left_child;
                inorder_successor->right_child = elem->right_child;
                delete node->pop_at(position);
                node->add_at(position, inorder_successor);
                if (node->right_child_at(position)->size() < min_elements)
                    balance(node->right_child_at(position), position, true);
            }
        }
    }

//    deletion of element
    void erase(T elem, bNode *current, int parent_index = -1, bool side = false, bool is_root = true,
               bool is_leftmost = true) {
        recent_node = nullptr;
        if (current->empty()) {
            return;
        }
        else if (elem > current->last()->value) {
            if (current->has_rightmost_child()) {
                erase(elem, current->get_rightmost_child(), current->size() - 1, true, false, false);
            }
            else {
                return;
            }
        }
        else {
            for (int idx = 0; idx < current->size(); idx++) {
                if (elem == current->value_at(idx)) {
                    erase_dispatch(current, idx, parent_index, side);
                    _size--;
                    break;
                }
                else if (elem < current->value_at(idx)) {
                    if (current->has_left_child_at(idx)) {
                        erase(elem, current->left_child_at(idx), idx, false, false, (idx == 0) && is_leftmost);
                        break;
                    }
                    else {
                        return;
                    }
                }
            }
        }
        if (!is_root && current->size() < min_elements) {
            balance(current, parent_index, side);
        }
        if(current->is_leaf && is_leftmost)
            leftmost = current->first();
        recent_node = current;
    }

    void print_all(bNode *&current) {
        for (element *i : current->elements) {
            if (i->has_left_child())
                print_all(i->left_child);
            cout << i->value << ' ';
        }
        if (current->has_rightmost_child())
            print_all(current->last()->right_child);
    }

public:

    explicit bTree(int Base) {
        ROOT = new bNode();
        _base = Base;
        min_elements = (Base + 1) / 2 - 1;
    }

    void insert(T elem) {
        insert(elem, ROOT);
    }

    void erase(T elem) {
        erase(elem, ROOT);
    }

    int size() {
        return _size;
    }

    element* begin(){
        return leftmost;
    }

    void print_all() {
        print_all(ROOT);
    }
};

int main() {

    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    bTree<long long> b(5);
    int n = 10000;
    for (int i = 0; i < n; i++) {
        long long x = rand();
        b.insert(x);
    }
    cout<<b.size()<<endl;
    for (int i = 0; i < n ; i++) {
        long long x = rand();
        b.erase(x);
    }
//    b.erase(b.leftmost->value);
    cout << b.begin()->value << endl;
    b.print_all();
    cout << endl;

    return 0;
}
