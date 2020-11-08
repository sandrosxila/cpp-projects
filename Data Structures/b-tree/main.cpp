#include <bits/stdc++.h>

using namespace std;

//declaration of b-Tree class

template<typename T>
class bTree {
private:
//    b-Tree private properties:
    int size = 0;
    int base = 0;
    int min_elements = 0;

//    declaration of the element structure which will be stored in b-Tree node (bNode)

    template<typename U>
    struct element {
        T value;
        U left_child;
        U right_child;

        explicit element(T value, U leftChild = nullptr, U rightChild = nullptr) : value(value), left_child(leftChild), right_child(rightChild) {}

//        check if element has a right child
        bool has_right_child(){
            return right_child != nullptr;
        }
//        check if element has a left child
        bool has_left_child(){
            return left_child != nullptr;
        }
    };

//    b-Tree will be made up with bNode structures

    struct bNode {
//        bNode properties with default values
        bool is_leaf = true;
        bNode *parent = nullptr;
        deque<element<bNode *> *> elements;

//        check if the elements are empty
        bool empty(){
            return elements.empty();
        }

//        get quantity of bNode elements
        int size(){
            return elements.size();
        }

//        get element at index
        element<bNode *> * at(int index){
            if(empty())
                return nullptr;
            return elements[index];
        }

//        get the first element
        element<bNode *> * first(){
            return at(0);
        }

//        get the last element
        element<bNode *> * last(){
            return at(size() - 1);
        }

//        check if bNode has a parent
        bool has_parent(){
            return parent != nullptr;
        }

//        check if the first element of bNode has a left child
        bool has_leftmost_child(){
            if(first() == nullptr)
                return false;
            return first()->has_left_child();
        }

//        check if the last element of bNode has a right child
        bool has_rightmost_child(){
            if(last() == nullptr)
                return false;
            return last()->has_right_child();
        }

//        get value of the first element
        T first_value(){
            return first()->value;
        }

//        get left child of the first element
        T first_left_child(){
            return first()->left_child;
        }

//        get right child of the first element
        T first_right_child(){
            return first()->right_child;
        }

//        get value of the last element
        T last_value(){
            return last()->value;
        }

//        get value of the element at index
        T value_at(int index){
            return at(index)->value;
        }

//        get a left child of the element at index
        bNode * left_child_at(int index){
            return at(index)->left_child;
        }

//        get a right child of the element at index
        bNode * right_child_at(int index){
            return at(index)->right_child;
        }

//        check if element at index has a left child
        bool has_left_child_at(int index){
            if(index > size() || index < 0){
                return false;
            }
            if(at(index)->has_left_child()){
                return true;
            }
            return false;
        }

//        check if element at index has a right child
        bool has_right_child_at(int index){
            if(index >= size() || index < 0){
                return false;
            }
            if(at(index)->has_right_child()){
                return true;
            }
            return false;
        }

//        get the right child of the last element of b-Node
        bNode * get_rightmost_child(){
            return last()->right_child;
        }

//        get the left child of the first element of b-Node
        bNode * get_leftmost_child(){
            return first()->left_child;
        }

//        add an element at the first position
        void add_first(element<bNode *> * elem){
            elements.push_front(elem);
        }

//        add an element at the last position
        void add_last(element<bNode *> * elem){
            elements.push_back(elem);
        }

//        add the element at index
        void add_at(int index, element<bNode *> * elem){
            elements.insert(elements.begin() + index, elem);
        }

//        pop an element at the first position
        element<bNode *> * pop_first(){
            element<bNode *> * elem = first();
            elements.pop_front();
            return elem;
        }

//        pop an element at the last position
        element<bNode *> * pop_last(){
            element<bNode *> * elem = last();
            elements.pop_back();
            return elem;
        }

//        pop an element at index
        element<bNode *> * pop_at(int index){
            element<bNode *> * elem = at(index);
            elements.erase(elements.begin() + index);
            return elem;
        }

//        add new element automatically
        void add(element<bNode *> * elem){
            if(empty() || (!empty() && elem->value > last_value()) ){
                add_last(elem);
            }
            for(int idx = 0; idx < size(); idx++){
                if(elem->value < value_at(idx)){
                    if(has_left_child_at(idx))
                        at(idx)->left_child = elem->right_child;
                    if(has_right_child_at(idx - 1))
                        at(idx - 1)->right_child = elem->left_child;
                    add_at(idx,elem);
                }
            }
        }
    } *ROOT = nullptr;

//    split the b-Node and return new node
    bNode * split(bNode * node){
        bNode * new_node = new bNode();
        while(node->size() > base / 2){
            new_node->add_first(node->pop_last());
            if(new_node->first()->has_left_child())
                new_node->first()->left_child->parent = new_node;
            if(new_node->first()->has_right_child())
                new_node->first()->right_child->parent = new_node;
        }
        return new_node;
    }

//    split up node and increase height of b-tree
    void lift_up(bNode * node){
        element<bNode *>* parent_element = node->pop_at(base / 2);
        bNode * new_node = split(node);

        if(!node->has_parent())
            node->parent = new bNode();

        if(ROOT == node)
            ROOT = node->parent;

        new_node->parent = node->parent;
        parent_element->left_child = node;
        parent_element->right_child = new_node;

        node->parent->add(parent_element);
        node->parent->is_leaf = false;
    }

    void insert(T elem, bNode *current){
        if(current->empty()){
            current->add_first(new element<bNode *>(elem));
        }
        else if(elem > current->last()->value){
            if(current->has_rightmost_child()){
                insert(elem, current->get_rightmost_child());
            }
            else{
                current->add_last(new element<bNode *>(elem));
            }
        }
        else{
            for(int idx = 0; idx < current->size(); idx++){
                if(elem < current->value_at(idx)){
                    if(current->has_left_child_at(idx)){
                        insert(elem,current->left_child_at(idx));
                        break;
                    }
                    else{
                        current->add_at(idx, new element<bNode *>(elem));
                        break;
                    }
                }
            }
        }
        if(current->size() == base){
            lift_up(current);
        }
    }

    bNode* get_right_sibling(bNode* node, bool side, int parent_pos){
        if(node->has_parent()){
            if(node->parent->has_right_child_at(side ? parent_pos + 1 : parent_pos))
                return node->parent->at(side ? parent_pos + 1 : parent_pos)->right_child;
        }
        return nullptr;
    }

    bNode* get_left_sibling(bNode* node, bool side, int parent_pos){
        if(node->has_parent()){
            if(node->parent->has_left_child_at(side ? parent_pos : parent_pos - 1))
                return node->parent->at(side ? parent_pos : parent_pos - 1)->left_child;
        }
        return nullptr;
    }

    void merge_to_left(bNode *&left_node, bNode *right_node){
        left_node->last()->right_child = right_node->get_leftmost_child();
        while(!right_node->empty()){
            left_node->add_last(right_node->pop_first());
        }
        delete right_node;
    }

    void merge_to_right(bNode *left_node, bNode *&right_node){
        right_node->first()->left_child = left_node->get_rightmost_child();
        while(!left_node->empty()){
            right_node->add_first(left_node->pop_last());
        }
        delete left_node;
    }

    void propagate_from_left_sibling(bNode* node,int parent_pos, bNode* left_sibling){
        element<bNode *> *node_element = node->parent->pop_at(parent_pos);
        node_element->left_child = left_sibling->get_rightmost_child();
        node_element->right_child = node->get_leftmost_child();
        node->add_first(node_element);
        element<bNode *> *parent_element = left_sibling->pop_last();
        parent_element->left_child = left_sibling;
        parent_element->right_child = node;
        node->parent->add_at(parent_pos,parent_element);
    }

    void propagate_from_right_sibling(bNode* node,int parent_pos, bNode* right_sibling){
        element<bNode *> *node_element = node->parent->pop_at(parent_pos);
        node_element->left_child = node->get_rightmost_child();
        node_element->right_child = right_sibling->get_leftmost_child();
        node->add_last(node_element);
        element<bNode *> *parent_element = right_sibling->pop_first();
        parent_element->left_child = node;
        parent_element->right_child = right_sibling;
        node->parent->add_at(parent_pos,parent_element);
    }

    void propagate_down_from_left(bNode* node, int parent_pos, bNode* left_sibling){
        element<bNode *> *parent_element = node->parent->pop_at(parent_pos);
        if(node->parent->has_right_child_at(parent_pos - 1)){
            node->parent->at(parent_pos - 1)->right_child = node;
        }
        parent_element->left_child = nullptr;
        parent_element->right_child = node->get_leftmost_child();
        node->add_first(parent_element);
        merge_to_right(left_sibling,node);
        if (node->parent == ROOT && ROOT->size() == 0){
            ROOT = node;
            delete node->parent;
            node->parent = nullptr;
        }
    }

    void propagate_down_from_right(bNode* node, int parent_pos, bNode* right_sibling){
        element<bNode *> *parent_element = node->parent->pop_at(parent_pos);
        if(node->parent->has_left_child_at(parent_pos)){
            node->parent->at(parent_pos)->left_child = node;
        }
        parent_element->left_child = node->get_rightmost_child();
        parent_element->right_child = nullptr;
        node->add_last(parent_element);
        merge_to_left(node,right_sibling);
        if (node->parent == ROOT && ROOT->size() == 0){
            ROOT = node;
            delete node->parent;
            node->parent = nullptr;
        }
    }

//    restore minimum number of elements in node
    void balance (bNode* node, int parent_position, bool side){
        bNode* left_sibling = get_left_sibling(node,side,parent_position);
        bNode* right_sibling = get_right_sibling(node,side,parent_position);

        if(left_sibling != nullptr && left_sibling->size() > min_elements)
            propagate_from_left_sibling(node, side ? parent_position : parent_position - 1, left_sibling);
        else if(right_sibling != nullptr && right_sibling->size() > min_elements)
            propagate_from_right_sibling(node, side ? parent_position + 1 : parent_position, right_sibling);
        else if(left_sibling != nullptr && left_sibling->size() == min_elements)
            propagate_down_from_left(node, side ? parent_position : parent_position - 1, left_sibling);
        else if(right_sibling != nullptr && right_sibling->size() == min_elements)
            propagate_down_from_right(node,side ? parent_position + 1 : parent_position,right_sibling);
    }

//    different cases of deletion are implemented here
    void erase_dispatch(bNode* node, int position, int parent_position, bool side){
        if(node->is_leaf){
//            case 1: when leaf-node has more than minimum number of elements
            if(node->size() > min_elements){
                node->pop_at(position);
            }
            else if(node->size() == min_elements){
                bNode* left_sibling = get_left_sibling(node,side,parent_position);
                bNode* right_sibling = get_right_sibling(node,side,parent_position);

//                case 2: when leaf node has minimum number of elements and left-sibling with more than minimum number of elements
                if(left_sibling != nullptr && left_sibling->size() > min_elements){
                    node->pop_at(position);
                    propagate_from_left_sibling(node, side ? parent_position : parent_position - 1, left_sibling);
                }
//                case 3: when leaf node has minimum number of elements and right-sibling with more than minimum number of elements
                else if(right_sibling != nullptr && right_sibling->size() > min_elements){
                    node->pop_at(position);
                    propagate_from_right_sibling(node, side ? parent_position + 1 : parent_position, right_sibling);
                }
//                case 4: when leaf node has minimum number of elements and left-sibling with minimum number of elements
                else if(left_sibling != nullptr && left_sibling->size() == min_elements){
                    node->pop_at(position);
                    propagate_down_from_left(node, side ? parent_position : parent_position - 1, left_sibling);
                }
//                case 5: when leaf node has minimum number of elements and right-sibling with minimum number of elements
                else if(right_sibling != nullptr && right_sibling->size() == min_elements){
                    node->pop_at(position);
                    propagate_down_from_right(node,side ? parent_position + 1 : parent_position,right_sibling);
                }
            }
        }
        else{

        }
    }

//    deletion of element
    void erase(T elem, bNode *current, int parent_index = -1, bool side = false) {
        if (current->empty()) {
            return;
        }
        else if (elem > current->last()->value) {
            if (current->has_rightmost_child()) {
                erase(elem, current->get_rightmost_child(), current->size() - 1, true);
            }
            else {
                return;
            }
        }
        else {
            for (int idx = 0; idx < current->size(); idx++) {
                if (elem == current->value_at(idx)) {
                    erase_dispatch(current,idx,parent_index,side);
                    break;
                }
                else if (elem < current->value_at(idx)) {
                    if (current->has_left_child_at(idx)) {
                        erase(elem, current->left_child_at(idx), idx, false);
                        break;
                    }
                    else {
                        return;
                    }
                }
            }
        }
        if (current != ROOT && current->size() < min_elements) {
            balance(current,parent_index,side);
        }
    }

    void build_test(bNode *&root) {
        // root node
        root = new bNode();
        root->is_leaf = false;
        root->add(new element<bNode *>(50));
        root->add(new element<bNode *>(80));

        //middle nodes

        //middle node 1
        auto middle_node1 = new bNode();
        middle_node1->is_leaf = false;
        middle_node1->add(new element<bNode *>(10));
        middle_node1->add(new element<bNode *>(20));
        //middle node 2
        auto middle_node2 = new bNode();
        middle_node2->is_leaf = false;
        middle_node2->add(new element<bNode *>(60));
        middle_node2->add(new element<bNode *>(70));
        middle_node2->add(new element<bNode *>(75));
        //middle node 3
        auto middle_node3 = new bNode();
        middle_node3->is_leaf = false;
        middle_node3->add(new element<bNode *>(90));
        middle_node3->add(new element<bNode *>(95));

        //low nodes

        //low node 1
        auto low_node1 = new bNode();
        low_node1->add(new element<bNode *>(4));
        low_node1->add(new element<bNode *>(5));
        low_node1->add(new element<bNode *>(6));

        //low node 2
        auto low_node2 = new bNode();
        low_node2->add(new element<bNode *>(14));
        low_node2->add(new element<bNode *>(15));
        low_node2->add(new element<bNode *>(16));

        //low node 3
        auto low_node3 = new bNode();
        low_node3->add(new element<bNode *>(23));
        low_node3->add(new element<bNode *>(27));

        //low node 4
        auto low_node4 = new bNode();
        low_node4->add(new element<bNode *>(51));
        low_node4->add(new element<bNode *>(52));

        //low node 5
        auto low_node5 = new bNode();
        low_node5->add(new element<bNode *>(64));
        low_node5->add(new element<bNode *>(65));
        low_node5->add(new element<bNode *>(68));

        //low node 6
        auto low_node6 = new bNode();
        low_node6->add(new element<bNode *>(72));
        low_node6->add(new element<bNode *>(73));

        //low node 7
        auto low_node7 = new bNode();
        low_node7->add(new element<bNode *>(77));
        low_node7->add(new element<bNode *>(78));
        low_node7->add(new element<bNode *>(79));

        //low node 8
        auto low_node8 = new bNode();
        low_node8->add(new element<bNode *>(81));
        low_node8->add(new element<bNode *>(82));
        low_node8->add(new element<bNode *>(89));

        //low node 9
        auto low_node9 = new bNode();
        low_node9->add(new element<bNode *>(92));
        low_node9->add(new element<bNode *>(93));

        //low node 10
        auto low_node10 = new bNode();
        low_node10->add(new element<bNode *>(108));
        low_node10->add(new element<bNode *>(110));
        low_node10->add(new element<bNode *>(118));

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
        ROOT = new bNode();
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
//
//    int arr[] = {4, 5, 6, 10, 14, 15, 16, 20, 23, 27, 50, 51, 52, 60, 64, 65, 68, 70, 72, 73, 75, 77, 78, 79, 81, 82,
//                 89, 90, 92, 93, 95, 108, 110, 111};
//
//    for (int i : arr){
//        b.insert(i);
//    }
    b.test();
    b.erase(64);
    b.erase(23);
    b.erase(72);
    b.erase(65);
    b.erase(20);
    return 0;
}
