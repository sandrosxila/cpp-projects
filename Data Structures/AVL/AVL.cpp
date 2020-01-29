#include <iostream>
//made by Sandro Skhirltadze
using namespace std;

template<typename T>
class AVL {
private:
//define node structure for AVL tree
    struct node {
//    define variables
        T key;
        node *left = nullptr, *right = nullptr;
        int height = 0;
    } *ROOT = nullptr;

//declare variable in order to calculate size of the AVL tree
    int sz = 0;

//define function to balance AVL tree
    void balance(node *&current, node *&root, node *&parent) {
//    calculate height of left and right child to determine direction
        int leftHeight = current->left == nullptr ? 0 : current->left->height;
        int rightHeight = current->right == nullptr ? 0 : current->right->height;
//    direct to the left child
        if (leftHeight > rightHeight) {
//        calculate height of left and right grand-child to determine direction
            int leftLeftHeight = current->left->left == nullptr ? 0 : current->left->left->height;
            int leftRightHeight = current->left->right == nullptr ? 0 : current->left->right->height;
//        direct to the left grand-child
            if (leftLeftHeight > leftRightHeight) {
//            Left Left case
//            declare variable for current node
                node *z = current;
//            declare variable for the child of current node
                node *y = current->left;
//            declare variable for the grand-child of current node
                node *x = current->left->left;
//            rotate
                z->left = y->right;
                y->right = z;
//            if parent was the root, then the highest node becomes root
                if (z == root)root = y;
//            if parent was not the root, then highest node becomes child of parent
                else {
                    if (y->key < parent->key) parent->left = y;
                    else parent->right = y;
                }
//            calculate heights of nodes
                z->height =
                        max((z->left == nullptr ? 0 : z->left->height), (z->right == nullptr ? 0 : z->left->height)) +
                        1;
                y->height =
                        max((y->left == nullptr ? 0 : y->left->height), (y->right == nullptr ? 0 : y->left->height)) +
                        1;

            }
//        direct to the right grand-child
            else {
//            Left Right case
//            declare variable for current node
                node *z = current;
//            declare variable for the child of current node
                node *y = current->left;
//            declare variable for the grand-child of current node
                node *x = current->left->right;
//            rotate
                y->right = x->left;
                x->left = y;
                z->left = x->right;
                x->right = z;
//            if parent was the root, then the highest node becomes root
                if (z == root) root = x;
//            if parent was not the root, then highest node becomes child of parent
                else {
                    if (x->key < parent->key) parent->left = x;
                    else parent->right = x;
                }
//            calculate heights of nodes
                z->height =
                        max((z->left == nullptr ? 0 : z->left->height), (z->right == nullptr ? 0 : z->left->height)) +
                        1;
                y->height =
                        max((y->left == nullptr ? 0 : y->left->height), (y->right == nullptr ? 0 : y->left->height)) +
                        1;
                x->height =
                        max((x->left == nullptr ? 0 : x->left->height), (x->right == nullptr ? 0 : x->left->height)) +
                        1;
            }
        }
//    direct to the right child
        else {
            int rightLeftHeight = current->right->left == nullptr ? 0 : current->right->left->height;
            int rightRightHeight = current->right->right == nullptr ? 0 : current->right->right->height;
            if (rightLeftHeight > rightRightHeight) {
//            Right Left case
//            declare variable for current node
                node *z = current;
//            declare variable for the child of current node
                node *y = current->right;
//            declare variable for the grand-child of current node
                node *x = current->right->left;
//            rotate
                y->left = x->right;
                x->right = y;
                z->right = x->left;
                x->left = z;
//            if parent was the root, then the highest node becomes root
                if (z == root)root = x;
//            if parent was not the root, then highest node becomes child of parent
                else {
                    if (x->key < parent->key) parent->left = x;
                    else parent->right = x;
                }
//            calculate heights of nodes
                z->height =
                        max((z->left == nullptr ? 0 : z->left->height), (z->right == nullptr ? 0 : z->left->height)) +
                        1;
                y->height =
                        max((y->left == nullptr ? 0 : y->left->height), (y->right == nullptr ? 0 : y->left->height)) +
                        1;
                x->height =
                        max((x->left == nullptr ? 0 : x->left->height), (x->right == nullptr ? 0 : x->left->height)) +
                        1;
            } else {
//            Right Right case
//            declare variable for current node
                node *z = current;
//            declare variable for the child of current node
                node *y = current->right;
//            declare variable for the grand-child of current node
                node *x = current->right->right;
//            rotate
                z->right = y->left;
                y->left = z;
//            if parent was the root, then the highest node becomes root
                if (z == root)root = y;
//            if parent was not the root, then highest node becomes child of parent
                else {
                    if (y->key < parent->key) parent->left = y;
                    else parent->right = y;
                }
//            calculate heights of nodes
                z->height =
                        max((z->left == nullptr ? 0 : z->left->height), (z->right == nullptr ? 0 : z->left->height)) +
                        1;
                y->height =
                        max((y->left == nullptr ? 0 : y->left->height), (y->right == nullptr ? 0 : y->left->height)) +
                        1;
            }
        }
    }

//define AVL tree element insertion function
    void insert(node *&current, T value, node *&root, node *p = nullptr) {
//    insert the element in the AVL tree
        if (current == nullptr) {
            current = new node();
            current->key = value;
            current->height = 1;
//        increase size of the AVL tree
            sz++;
            return;
        }
//    redirect to the left child
        if (value < current->key) insert(current->left, value, root, current);
//    redirect to the right child
        else if (value > current->key) insert(current->right, value, root, current);
//    do not insert duplicate
        else return;
//    calculate height for the current node
        int leftHeight = current->left == nullptr ? 0 : current->left->height;
        int rightHeight = current->right == nullptr ? 0 : current->right->height;
        current->height = max(leftHeight, rightHeight) + 1;
//    check whether we need to balance the AVL tree
        if (abs(rightHeight - leftHeight) == 2) balance(current, root, p);
    }

// declare the search function to find parent of the element which will be deleted
    void search(node *&current, T value, node *&parent, bool &F) {
//    check if passed element exists in the AVL tree
        if (current == nullptr) {
            F = false;
            return;
        }
//    check if we have already found the element
        if (value == current->key) return;
//    use current node as parent for the successor node
        parent = current;
//    direct to the left if the element is less than the key of the current node
        if (value < current->key) search(current->left, value, parent, F);
//    direct to the left if the element is more than the key of the current node
        if (value > current->key) search(current->right, value, parent, F);
    }

//create function to implement deletion process in the AVL tree
    void deletion(node *&current, node *&parent, node *&root) {
//    case 1: element has no child
        if (current->left == nullptr && current->right == nullptr) {
//        declare variable for parent node
            node *z = parent;
//        declare variable for child node
            node *y = current;
//        check if parent node is the root of the AVL tree
            if (z != nullptr) {
                if (y->key < z->key) z->left = nullptr;
                else z->right = nullptr;
            }
//        if we have to delete the root, which has no child, then the value of the AVL tree root becomes NULL
            else root = nullptr;
//        delete child node
            delete y;
        }
//    case 2: if the element has right child
        else if (current->left == nullptr && current->right != nullptr) {
//        declare variable for parent node
            node *z = parent;
//        declare variable for child node
            node *y = current;
//        declare variable for grand-child node
            node *x = current->right;
//        check if parent node is the root of the AVL tree
            if (z != nullptr) {
                if (y->key < z->key) z->left = x;
                else z->right = x;
            }
//        if we have to delete the root, which has only one child, then the value of the AVL tree root becomes the grand-child node
            else root = x;
            delete y;
        }
//    case 3: if the element has left child
        else if (current->left != nullptr && current->right == nullptr) {
//        declare variable for parent node
            node *z = parent;
//        declare variable for child node
            node *y = current;
//        declare variable for grand-child node
            node *x = current->left;
//        check if parent node is the root of the AVL tree
            if (z != nullptr) {
                if (y->key < z->key) z->left = x;
                else z->right = x;
            }
//        if we have to delete the root, which has only one child, then the value of the AVL tree root becomes the grand-child node
            else root = x;
            delete y;
        }
//    case 4: if the element has left and right child
        else {
//        declare the variable for the element and describe the element as parent node,
//        because we have to find and erase the leftmost element in the right subtree of the element
            node *P = current;
//        declare variable for right child of the element and describe it as the child of the parent node
            node *C = current->right;
//        finding the leftmost element in the right subtree of the element
            while (C->left != nullptr) {
                P = C;
                C = C->left;
            }
//        right child of the found element will be linked to it's parent
            if (C->key < P->key) {
                P->left = C->right;
            } else {
                P->right = C->right;
            }
//        the key of the element which we passed at the beginning will become the key of the found element
            current->key = C->key;
//        delete found element
            delete C;
        }
    }

// declare the function to update height of the deleted node's predecessors
    void update(node *&current, node *&final, node *&root, node *p = nullptr) {
//    direct to the left if the element which we are looking for is less than the current node
        if (final->key < current->key) update(current->left, final, root, current);
        //    direct to the right if the element which we are looking for is more than the current node
        if (final->key > current->key) update(current->right, final, root, current);
//    calculate height for the current element
        int leftHeight = current->left == nullptr ? 0 : current->left->height;
        int rightHeight = current->right == nullptr ? 0 : current->right->height;
        current->height = max(leftHeight, rightHeight) + 1;
//    check if we need to balance the AVL tree
        if (abs(rightHeight - leftHeight) == 2)balance(current, root, p);
//    if the deleted node's parent if found, the we return back
        if (final == current)return;
    }

//define erase function
    void erase(node *&root, T value) {
//    declare variables for the parent node and check flag for the node, which will be found or not
        node *P = nullptr;
        bool found = true;
//    search passed value in the AVL tree
        search(root, value, P, found);
//    if the element will be found then delete the element
        if (found) {
//        check if we are deleting the root of AVL tree
            if (P == nullptr) {
                deletion(root, P, root);
                P = root;
            }
//        take parent into perspective if we are not deleting the root of AVL tree
            else {
//            if our element is less than it's parent, then delete left child of parent node
                if (value < P->key) deletion(P->left, P, root);
//            if our element is more than it's parent, then delete right child of parent node
                else deletion(P->right, P, root);
            }
//        update heights of the predecessors
            update(root, P, root);
//        decrease size of the AVL tree
            sz--;
        }
    }
//define function to print out the elements
    void preOrder(node *&current) {
        if (current == nullptr) return;
        cout << current->key << ' ';
        preOrder(current->left);
        preOrder(current->right);
    }
//define function to print out the sorted elements
    void inOrder(node *&current) {
        if (current == nullptr) return;
        inOrder(current->left);
        cout << current->key << ' ';
        inOrder(current->right);
    }

public:
//    define public function for insertion
    void insert(T value) {
        insert(ROOT, value, ROOT);
    }
//    define public function to erase the element
    void erase(T value) {
        erase(ROOT, value);
    }
//    define the function to get the root element of the AVL tree
    T top() {
        if (ROOT == nullptr) return 0;
        return ROOT->key;
    }
//    define the function to print out the elements of the AVL tree
    void print(bool sorted = false) {
        if (sorted) inOrder(ROOT);
        else preOrder(ROOT);
        cout << endl;
    }
//    define the function to get size of the AVL tree
    int size() {
        return sz;
    }
};

int main() {
//    create instance of AVL class
    AVL<int> x;
//    insert numbers in the AVL tree
    x.insert(9);
    x.insert(5);
    x.insert(10);
    x.insert(0);
    x.insert(6);
    x.insert(11);
    x.insert(-1);
    x.insert(1);
    x.insert(2);
//    duplicate values won't be inserted
    x.insert(11);
    x.insert(6);
//    print size of the AVL tree
    cout << "Size of the AVL tree is: " << x.size() << endl;
//    print AVL tree sorted elements
    cout << "Sorted elements of the AVL tree: ";
    x.print(true);
//    erase elements in AVL tree
    x.erase(10);
    x.erase(5);
    x.erase(2);
//    print AVL tree
    cout << "AVL tree elements, that are not sorted: ";
    x.print();
//    erase some element
    x.erase(6);
    x.erase(9);
//    elements that does not exist in the AVL tree won't be erased
    x.erase(9999);
    x.erase(1234567);
//    print the root of the AVL tree
    cout << "The root element of the AVL tree: " << x.top() << endl;
//    print size of the AVL tree
    cout << "Size of the AVL tree is: " << x.size() << endl;
}