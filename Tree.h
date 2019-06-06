#include <stdio.h>
#include <algorithm>
#include <iostream>
#ifndef TREE_H
#define TREE_H
using namespace std;

template <typename Comparable>

class AVLTree {
    
public:
    AVLTree(): root{nullptr}{}
    
    AVLTree(const AVLTree & rhs): root{nullptr}{
        root = clone(rhs.root);
    }
    
    AVLTree(AVLTree && rhs): root{rhs.root}{
        rhs.root = nullptr;
    }
    
    ~AVLTree(){
        makeEmpty();
    }
    
    AVLTree & operator=(const AVLTree & rhs); // Deep copy
    AVLTree & operator=(AVLTree && rhs); // Move
    const Comparable & findMin() const; //Find the smallest item in the tree
    const Comparable & findMax() const; //Find the largest item in the tree
    bool contains(const Comparable & x);
    bool isEmpty() const;
    void printTree() const; // Print the tree in sorted order
    void makeEmpty(); // Make the tree logically empty
    void insert(const Comparable & x);
    void insert(Comparable && x);
    void remove(const Comparable & x);
    void left_left();
    void right_right();
    void left_right();
    void right_left();
    int rotation;
    int visited;
    int inserted;
    int countLL, countRR, countLR, countRL;

private:
    Comparable nodeNum = 0;
    //Node declaration for AVL trees
    struct AVLNode{
        int element;
        AVLNode *left;
        AVLNode *right;
        int height;
        int up;
        int low;
        //string rotationType;
        
        AVLNode(const Comparable & ele, AVLNode *lt, AVLNode *rt, int h = 0):
        element{ ele }, left{ lt }, right{ rt }, height{ h } { }
        //AVLNode(const Comparable & ele, AVLNode *lt, AVLNode *rt, int h = 0, int d = 0):
        //element{ ele }, left{ lt }, right{ rt }, height{ h }, depth{ d } { }
        
        AVLNode( Comparable && ele, AVLNode *lt, AVLNode *rt, int h = 0):
        element { std::move(ele)}, left{ lt }, right{ rt }, height{ h } { }
        //AVLNode( Comparable && ele, AVLNode *lt, AVLNode *rt, int h = 0, int d = 0):
        //element { std::move(ele)}, left{ lt }, right{ rt }, height{ h }, depth{ d } { }
    };
    
    AVLNode *root;
    
    /*
     * Internal method to insert into a subtree.
     * x is the item to insert.
     * t is the node that roots the subtree.
     * Set the new root of the subtree.
     */
    void insert(const Comparable & x, AVLNode * & t){
//        inserted = 0;
//        visited = 0;
        if(t == nullptr){
            t = new AVLNode{x, nullptr, nullptr};
            inserted++;
        }else if (x < t->element){
            visited++;
            insert(x, t->left);
        }else if (x > t->element){
            visited++;
            insert(x, t->right);
        }else{
            visited++;
        }
        
        balance(t);
    }
    
    /*
     * Internal method to insert into a subtree.
     * x is the item to insert.
     * t is the node that roots the subtree.
     * Set the new root of the subtree.
     */
    void insert(Comparable && x, AVLNode * & t){
//        inserted = 0;
//        visited = 0;
        if(t == nullptr){
            t = new AVLNode{std::move(x), nullptr, nullptr};
            inserted++;
        }else if (x < t->element){
            visited++;
            insert(std::move(x), t->left);
        }else if (x > t->element){
            visited++;
            insert(std::move(x), t->right);
        }else{
            visited++;
        }
        
        balance(t);
    }
    
    // Deletion in an AVL tree
    void remove(const Comparable & x, AVLNode * & t){
        if (t == nullptr)
            return; // Item not found
        
        if (x < t->element)
            remove(x, t->left);
        else if (x > t->element)
            remove(x, t->right);
        else if (t->left != nullptr && t->right != nullptr){ // 2 children
            t->element = findMin(t->right)->element;
            remove(t->element, t->right);
        }else{
            AVLNode *oldNode = t;
            if (t->left != nullptr)
                t = t->left;
            else
                t = t->right;
            
            delete oldNode;
        }
        balance(t);
    }
    
    static const int ALLOWED_IMBALANCE = 1;
    
    // To keep the tree balanced
    void balance(AVLNode * & t){
        //rotation = 0;
        if (t == nullptr) {
            return;
        }
        if (height(t->left) - height(t->right) > ALLOWED_IMBALANCE) {
            if (height(t->left->left) >= height(t->left->right)) {
                rightRotation(t);
                rotation++;
            }else{
                leftRightRotation(t);
                rotation += 2;
            }
        }else if (height(t->right) - height(t->left) > ALLOWED_IMBALANCE){
            if (height(t->right->right) >= height(t->right->left)) {
                leftRotation(t);
                rotation++;
            }else{
                rightLeftRotation(t);
                rotation += 2;
            }
        }
        
        // Update height and depth
        t->height = max(height(t->left), height(t->right)) + 1;
    }
    
    // Internal method to find the smallest item in a subtree t
    AVLNode * findMin(AVLNode *t) const{
        if (t == nullptr) {
            return nullptr;
        }
        if (t->left == nullptr) {
            return t;
        }
        return findMin(t->left);
    }
    
    // Internal method to find the largest item in a subtree t
    AVLNode * findMax(AVLNode *t) const{
        if (t != nullptr) {
            while (t->right != nullptr) {
                t = t->right;
            }
        }
        return t;
    }
    
    // To find if a number has been in the tree
    bool contains(const Comparable & x, AVLNode *t) {
        if(t == nullptr)
            return false;
        else if (x < t->element){
            visited++;
            return contains(x, t->left);
        }else if (x > t->element){
            visited++;
            return contains(x, t->right);
        }else{
            visited++;
            return true;
        }
    }
    
    // Internal method to make subtree empty
    void makeEmpty(AVLNode * & t){
        if (t != nullptr) {
            makeEmpty(t->left);
            makeEmpty(t->right);
            delete t;
        }
        t = nullptr;
    }
    
    // Internal method to print a subtree rooted at t in sorted order
    void printTree(AVLNode *t, int blankNum) const{
        int cnt = blankNum; // The level of the node.
        while (cnt--) {
            cout << " ";
        }
        if (t->left == nullptr && t->right == nullptr) {
            cout << "Leaf(" << t->element << ")" << endl;
        }
        else{
            cout << "Node(" << t->element << ", h=" << t->height << "):" << endl;
            if (t->left != nullptr) {
                printTree(t->left, blankNum + 2);
            }else{
                cnt = blankNum + 2;
                while (cnt--) {
                    cout << " ";
                }
                cout << "Null" << endl;
            }
            
            if (t->right != nullptr) {
                printTree(t->right, blankNum + 2);
            }else{
                cnt = blankNum + 2;
                while (cnt--) {
                    cout << " ";
                }
                cout << "Null" << endl;
            }
        }
    }
    
    void left_left(AVLNode * & t, const Comparable & parent){
        if (t == nullptr) {
            return;
        }else if (t != nullptr) {
            // Find a node
            if ((height(t->left) - height(t->right) == ALLOWED_IMBALANCE) && (height(t->left->left) >= height(t->left->right))) {
                countLL++;
                // To set the value of low and up
                if (t->element > parent) {
                    t->low = (int)parent + 1;
                    t->up = (int)(t->left->element) - 1;
                }else{
                    t->low = -2147483648;
                    t->up = (int)(t->left->element) - 1;
                }
                // Output the value
                if (countLL == 1) {
                    cout << "The following inserts would cause a left-left rotation:" << endl;
                    if (t->low == t->up) {
                        cout << t->low;
                    }else{
                        cout << t->low << " to " << t->up;
                    }
                }else if(countLL > 1){
                    cout << ", ";
                    if (t->low == t->up) {
                        cout << t->low;
                    }else{
                        cout << t->low << " to " << t->up;
                    }
                }
            }
        }
        // Continue to find the node
        left_left(t->left, t->element);
        left_left(t->right, t->element);
    }
    
    void right_right(AVLNode * & t, const Comparable & parent){
        if (t == nullptr) {
            return;
        }else if (t != nullptr) {
            // Find a node
            if ((height( t->right ) - height( t->left ) == ALLOWED_IMBALANCE) && (height( t->right->right ) >= height( t->right->left ))) {
                countRR++;
                // To set the value of low and up
                if (t->element > parent) {
                    t->low =(int)(t->right->element) + 1;
                    t->up = 2147483647;
                }else{
                    t->low = (int)(t->right->element) - 1;
                    t->up = (int)parent - 1;
                }
                // Output the value
                if (countRR == 1) {
                    cout << "The following inserts would cause a right-right rotation:" << endl;
                    if (t->low == t->up) {
                        cout << t->low;
                    }else{
                        cout << t->low << " to " << t->up;
                    }
                }else if(countRR > 1){
                    cout << ", ";
                    if (t->low == t->up) {
                        cout << t->low;
                    }else{
                        cout << t->low << " to " << t->up;
                    }
                }
            }
        }
        // Continue to find the node
        right_right(t->left, t->element);
        right_right(t->right, t->element);
    }
    
    void left_right(AVLNode * & t, const Comparable & parent){
        if (t == nullptr) {
            return;
        }else if (t != nullptr) {
            // Find a node
            if ((height(t->left) - height(t->right) == ALLOWED_IMBALANCE) && (height(t->left->left) < height(t->left->right))) {
                //countLR++;
                // To set the value of low and up
                if (t->element != parent) {
                    if (height(t->left->right->left) > height(t->left->right->right)) {
                        countLR++;
                        t->low = (int)(t->left->element) + 1;
                        t->up = (int)(t->left->right->element) - 1;
                    }else if(height(t->left->right->left) < height(t->left->right->right)){
                        countLR++;
                        t->low = (int)(t->left->right->element) + 1;
                        t->up = (int)(t->element) - 1;
                    }
//                    if (t->left->right->left != nullptr && t->left->right->right != nullptr) {
//                        if (height(t->left->right->left) > height(t->left->right->right)) {
//                            countLR++;
//                            t->low = (int)(t->left->element) + 1;
//                            t->up = (int)(t->left->right->element) - 1;
//                        }else if(height(t->left->right->left) < height(t->left->right->right)){
//                            countLR++;
//                            t->low = (int)(t->left->right->element) + 1;
//                            t->up = (int)(t->element) - 1;
//                        }
//                    }
                }else{
                    countLR++;
                    t->low = (int)(t->left->element) + 1;
                    t->up = (int)(t->element) - 1;
                }
                // Output the value
                if (countLR == 1) {
                    cout << "The following inserts would cause a left-right rotation:" << endl;
                    if (t->low == t->up) {
                        cout << t->low;
                    }else{
                        cout << t->low << " to " << t->up;
                    }
                }else if(countLR > 1){
                    cout << ", ";
                    if (t->low == t->up) {
                        cout << t->low;
                    }else{
                        cout << t->low << " to " << t->up;
                    }
                }
            }
        }
        // Continue to find the node
        left_right(t->left, t->element);
        left_right(t->right, t->element);
    }
    
    void right_left(AVLNode * & t, const Comparable & parent){
        if (t == nullptr) {
            return;
        }else if (t != nullptr) {
            // Find a node
            if ((height( t->right ) - height( t->left ) == ALLOWED_IMBALANCE) && (height( t->right->right ) < height( t->right->left ))) {
                //countRL++;
                // To set the value of low and up
                if (t->element != parent) {
                    if (height(t->right->left->left) > height(t->right->left->right)) {
                        countRL++;
                        t->low = (int)(t->element) + 1;
                        t->up = (int)(t->right->left->element) - 1;
                    }else if(height(t->right->left->left) < height(t->right->left->right)){
                        countRL++;
                        t->low = (int)(t->right->left->element) + 1;
                        t->up = (int)(t->right->element) - 1;
                    }
//                    if (t->right->left->left != nullptr && t->right->left->right != nullptr) {
//                        if (height(t->right->left->left) > height(t->right->left->right)) {
//                            countRL++;
//                            t->low = (int)(t->element) + 1;
//                            t->up = (int)(t->right->left->element) - 1;
//                        }else if(height(t->right->left->left) < height(t->right->left->right)){
//                            countRL++;
//                            t->low = (int)(t->right->left->element) + 1;
//                            t->up = (int)(t->right->element) - 1;
//                        }
//                    }
                }else{
                    countRL++;
                    t->low = (int)(t->element) + 1;
                    t->up = (int)(t->right->element) - 1;
                }
                // Output the value
                if (countRL == 1) {
                    cout << "The following inserts would cause a right-left rotation:" << endl;
                    if (t->low == t->up) {
                        cout << t->low;
                    }else{
                        cout << t->low << " to " << t->up;
                    }
                }else if(countRL > 1){
                    cout << ", ";
                    if (t->low == t->up) {
                        cout << t->low;
                    }else{
                        cout << t->low << " to " << t->up;
                    }
                }
            }
        }
        // Continue to find the node
        right_left(t->left, t->element);
        right_left(t->right, t->element);
    }
    
    // Internal method to clone subtree
    AVLNode * clone(AVLNode *t) const{
        if (t == nullptr)
            return nullptr;
        else
            return new AVLNode{t->element, clone(t->left), clone(t->right), t->height};
    }
    
    // To compute height of an AVL tree
    int height(AVLNode *t) const{
        if(t == nullptr)
            return -1;
        else
            return t -> height;
    }
    
    
    int max(int lhs, int rhs) const{
        if (lhs > rhs)
            return lhs;
        else
            return rhs;
    }

    // Single right rotation with left child
    void rightRotation(AVLNode * & p){
        AVLNode *leftChild = p->left;
        p->left = leftChild->right;
        leftChild->right = p;
        p->height = max(height(p->left), height(p->right)) + 1;
        leftChild->height = max(height(leftChild->left), height(leftChild->right)) + 1;
        p = leftChild;
        if (root == p) {
            root = leftChild;
        }
    }
    
    // Single left rotation with right child
    void leftRotation(AVLNode * & p){
        AVLNode *rightChild = p->right;
        p->right = rightChild->left;
        rightChild->left = p;
        p->height = max(height(p->left), height(p->right)) + 1;
        rightChild->height = max(height(rightChild->left), height(rightChild->right)) + 1;
        p = rightChild;
        if (root == p) {
            root = rightChild;
        }
    }
    
    // Double rotation with left child
    void leftRightRotation(AVLNode * & p){
        leftRotation(p->left);
        rightRotation(p);
    }
    
    // Double rotation with right child
    void rightLeftRotation(AVLNode * & p){
        rightRotation(p->right);
        leftRotation(p);
    }

};
#endif
