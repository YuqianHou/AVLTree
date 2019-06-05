#include "Tree.h"
#include <algorithm>
#include <iostream>
using namespace std;

// Deep copy
template <typename Comparable>
AVLTree<Comparable> & AVLTree<Comparable>::operator=(const AVLTree<Comparable> & rhs){
    AVLTree<Comparable> copy = rhs;
    std::swap(*this, copy);
    return *this;
}

// Move
template <typename Comparable>
AVLTree<Comparable> & AVLTree<Comparable>::operator=(AVLTree<Comparable> && rhs){
    std::swap(root, rhs.root);
    return *this;
}

//Find the smallest item in the tree
template <typename Comparable>
const Comparable & AVLTree<Comparable>::findMin() const{
    if (isEmpty()) {
        cout << "The node is empty";
    }
    return findMin(root)->element;
}

//Find the largest item in the tree
template <typename Comparable>
const Comparable & AVLTree<Comparable>::findMax() const{
    if (isEmpty()) {
        cout << "The node is empty" <<endl;
    }
    return findMax(root)->element;
}

template <typename Comparable>
bool AVLTree<Comparable>::contains(const Comparable & x) {
    return contains(x, root);
}

template <typename Comparable>
bool AVLTree<Comparable>::isEmpty() const{
    return root == nullptr;
}

// Print the tree in sorted order
template <typename Comparable>
void AVLTree<Comparable>::printTree() const{
    if (isEmpty()) {
        cout << "Null" << endl;
    }
    else
        printTree(root);
}

// Make the tree logically empty
template <typename Comparable>
void AVLTree<Comparable>::makeEmpty(){
    makeEmpty(root);
}

template <typename Comparable>
void AVLTree<Comparable>::insert(const Comparable & x){
    insert(x, root);
}

template <typename Comparable>
void AVLTree<Comparable>::insert(Comparable && x){
    insert(std::move(x), root);
}

template <typename Comparable>
void AVLTree<Comparable>::remove(const Comparable & x){
    remove(x, root);
}
