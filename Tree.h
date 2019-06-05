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
    int rotation;
    int visited;
    int inserted;

private:
    Comparable nodeNum = 0;
    //Node declaration for AVL trees
    struct AVLNode{
        int element;
        AVLNode *left;
        AVLNode *right;
        int height; // From leave to root
        //int depth; // From root to leaf
        
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
        inserted = 0;
        visited = 0;
        if(t == nullptr){
            t = new AVLNode{x, nullptr, nullptr};
            inserted++;
        }
        else if (x < t->element){
            visited++;
            insert(x, t->left);
        }
        else if (x > t->element){
            visited++;
            insert(x, t->right);
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
        inserted = 0;
        visited = 0;
        if(t == nullptr)
            t = new AVLNode{std::move(x), nullptr, nullptr};
        else if (x < t->element){
            visited++;
            insert(std::move(x), t->left);
        }
        else if (x > t->element){
            visited++;
            insert(std::move(x), t->right);
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
        rotation = 0;
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
        //t->depth = root->height - t->height;
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
        visited = 0;
        if(t == nullptr)
            return false;
        else if (x < t->element){
            visited++;
            return contains(x, t->left);
        }
        else if (x > t->element){
            visited++;
            return contains(x, t->right);
        }
        else{
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
        int cnt = blankNum;
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
    
//    // Internal method to print a subtree rooted at t in sorted order
//    void printTree(AVLNode *t) const{
//        int depth = 0;
//        if (t != nullptr) {
//            printBlank(depth);
//            if(t->height == 0){
//                cout << "Leaf(" << t->element << ")" << endl;
//            }else{
//                cout << "Node(" << t->element << ", h=" << t->height << "):" << endl;
//                depth++;
//            }
//            printTree(t->left);
//            printTree(t->right);
//        }
//    }
    
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
    
    // To compute depth of an AVL tree
//    int depth(AVLNode *t) const{
//        if(t == nullptr)
//            return -1;
//        else
//            return t -> depth;
//    }
    
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
        p->height = max(height(p->left), height(p->right));
        leftChild->height = max(height(leftChild->left), height(leftChild->right)) + 1;
        p = leftChild;
    }
    
    // Single left rotation with right child
    void leftRotation(AVLNode * & p){
        AVLNode *rightChild = p->right;
        p->right = rightChild->left;
        rightChild->left = p;
        p->height = max(height(p->left), height(p->right));
        rightChild->height = max(height(rightChild->left), height(rightChild->right)) + 1;
        p = rightChild;
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
