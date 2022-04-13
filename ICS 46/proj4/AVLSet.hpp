// AVLSet.hpp
//
// ICS 46 Winter 2019
// Project #3: Set the Controls for the Heart of the Sun
//
// An AVLSet is an implementation of a Set that is an AVL tree, which uses
// the algorithms we discussed in lecture to maintain balance every time a
// new element is added to the set.  The balancing is actually optional,
// with a bool parameter able to be passed to the constructor to explicitly
// turn the balancing on or off (on is default).  If the balancing is off,
// the AVL tree acts like a binary search tree (e.g., it will become
// degenerate if elements are added in ascending order).
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to implement your AVL tree
// using your own dynamically-allocated nodes, with pointers connecting them,
// and with your own balancing algorithms used.

#ifndef AVLSET_HPP
#define AVLSET_HPP

#include <functional>
#include "Set.hpp"

template <typename ElementType>
class AVLSet : public Set<ElementType>
{
public:
    // A VisitFunction is a function that takes a reference to a const
    // ElementType and returns no value.
    using VisitFunction = std::function<void(const ElementType&)>;

public:
    // Initializes an AVLSet to be empty, with or without balancing.
    explicit AVLSet(bool shouldBalance = true);

    // Cleans up the AVLSet so that it leaks no memory.
    virtual ~AVLSet() noexcept;

    // Initializes a new AVLSet to be a copy of an existing one.
    AVLSet(const AVLSet& s);

    // Initializes a new AVLSet whose contents are moved from an
    // expiring one.
    AVLSet(AVLSet&& s) noexcept;

    // Assigns an existing AVLSet into another.
    AVLSet& operator=(const AVLSet& s);

    // Assigns an expiring AVLSet into another.
    AVLSet& operator=(AVLSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement an AVLSet, false otherwise.
    virtual bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function always runs in O(log n) time
    // when there are n elements in the AVL tree.
    virtual void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function always runs in O(log n) time when
    // there are n elements in the AVL tree.
    virtual bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    virtual unsigned int size() const noexcept override;


    // height() returns the height of the AVL tree.  Note that, by definition,
    // the height of an empty tree is -1.
    int height() const;


    // preorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a preorder traversal of the AVL
    // tree.
    void preorder(VisitFunction visit) const;


    // inorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by an inorder traversal of the AVL
    // tree.
    void inorder(VisitFunction visit) const;


    // postorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a postorder traversal of the AVL
    // tree.
    void postorder(VisitFunction visit) const;


private:
    // You'll no doubt want to add member variables and "helper" member
    // functions here.
    struct Node
    {
        ElementType content;
        Node* right;
        Node* left;
    };

    int treeHeight; // the height of the tree
    int treeSize; // the size of the tree

    Node* root; // root of the tree
    Node* current;  // current node
    bool balance;

    void leftRotate(Node*& n);
    void rightRotate(Node*& n);
    void deleteAllNodes(Node* n); // delete all nodes existing in the tree
    void copyAllNodes(const Node* source, Node*& current);
    bool exists(const ElementType& element, Node* node) const;
    void insertBalenced(const ElementType& element, Node*& node);
    void insertUnBalenced(const ElementType& element, Node*& node);
    int getHeight(Node* node) const;
    void LL(Node*& current);
    void RL(Node*& current);
    void RR(Node*& current);
    void LR(Node*& current);

    void preorderCall(VisitFunction visit, Node* node) const;
    void inorderCall(VisitFunction visit, Node* node) const;
    void postorderCall(VisitFunction visit, Node* node) const;

};



template <typename ElementType>
AVLSet<ElementType>::AVLSet(bool shouldBalance)
: treeSize{0}, root{nullptr}, balance{shouldBalance}
{
}


template <typename ElementType>
AVLSet<ElementType>::~AVLSet() noexcept
{
    // delete all elements on the tree
    deleteAllNodes(root);
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(const AVLSet& s)
{
    treeHeight = s.treeHeight;
    treeSize = s.treeSize;
    copyAllNodes(s.root, root);
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(AVLSet&& s) noexcept
{
    treeHeight = s.treeHeight;
    treeSize = s.treeSize;
    copyAllNodes(s.root, root);
    deleteAllNodes(s.root);
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(const AVLSet& s)
{
    if(this != &s)
    {
        deleteAllNodes(this->root);
        treeHeight = s.treeHeight;
        treeSize = s.treeSize;
        copyAllNodes(s.root, root);    
    }

    return *this;
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(AVLSet&& s) noexcept
{
    if(this != &s)
    {
        deleteAllNodes(this->root);
        treeHeight = s.treeHeight;
        treeSize = s.treeSize;
        copyAllNodes(s.root, root);  
        deleteAllNodes(s.root);  
    }

    return *this;
}


template <typename ElementType>
bool AVLSet<ElementType>::isImplemented() const noexcept
{
    return true;
}

template <typename ElementType>
void AVLSet<ElementType>::rightRotate(Node*& n)
{
    Node* temp;

    temp = n->right;
    n->right = temp->left;
    temp->left = n;
    n = temp;
}

template <typename ElementType>
void AVLSet<ElementType>::leftRotate(Node*& n)
{
    Node* temp;

    temp = n->left;
    n->left = temp->right;
    temp->right = n;
    n = temp;
}


template <typename ElementType>
void AVLSet<ElementType>::add(const ElementType& element)
{
    // check if the element is in the tree by "contain"
    if(contains(element))
        return;
    
    // insert the element in the tree
    if(balance == false)
        insertUnBalenced(element, root);  
    else
        insertBalenced(element, root);
    treeSize++;  
    
    // if the tree is balance
    // rotate the tree to make the tree to accept new Node balencly
   
}


template <typename ElementType>
bool AVLSet<ElementType>::contains(const ElementType& element) const
{   
    return exists(element, root);
}


template <typename ElementType>
unsigned int AVLSet<ElementType>::size() const noexcept
{
    return treeSize;
}


template <typename ElementType>
int AVLSet<ElementType>::height() const
{
    return getHeight(root);
}


template <typename ElementType>
void AVLSet<ElementType>::preorder(VisitFunction visit) const
{
    preorderCall(visit, root);

}


template <typename ElementType>
void AVLSet<ElementType>::inorder(VisitFunction visit) const
{
    inorderCall(visit, root);
}


template <typename ElementType>
void AVLSet<ElementType>::postorder(VisitFunction visit) const
{
    postorderCall(visit, root);
}


// ===========================
// ADDITIONAL MEMBER FUNCTIONS
// ===========================


template <typename ElementType>
void AVLSet<ElementType>::deleteAllNodes(Node* n){
    if(n == nullptr)
        return;
    deleteAllNodes(n->left);
    deleteAllNodes(n->right);
    delete n;
}


template<typename ElementType>
void AVLSet<ElementType>::copyAllNodes(const Node* source, Node*& current)
{
    if(source == nullptr)
        current = nullptr;
    else
    {
        current = new Node{source->content};
        if(source->left == nullptr && source->right == nullptr)
            return;
        copyAllNodes(source->left, current->left);
        copyAllNodes(source->right, current->right);
    }
}


template <typename ElementType>
bool AVLSet<ElementType>::exists(const ElementType& element, Node* node) const
{
    if (node == nullptr)
        return false;
    else if(node->content == element)
        return true;
    else if (node->content > element)
        return exists(element, node->left);
    else
        return exists(element, node->right);

    // return true;

}


template <typename ElementType>
void AVLSet<ElementType>::insertBalenced(const ElementType& element, Node*& current)
{
    if(current == nullptr)
        current = new Node{element};
    else if(element > current->content)
    {
        insertBalenced(element, current->right);
        if(getHeight(current->right)-getHeight(current->left) == 2)
        {
            if(element > current->right->content)
                RR(current);
            else
                LR(current);
        }
    }
    else if(element < current->content)
    {
        insertBalenced(element, current->left);
        if(getHeight(current->left)-getHeight(current->right) == 2)
        {
            if(element < current->left->content)
                LL(current);
            else
                RL(current);
        }
    }
}


template <typename ElementType>
void AVLSet<ElementType>::insertUnBalenced(const ElementType& element, Node*& node)
{
    if(node == nullptr)
    {
        node = new Node{element};
    }

    else if(element < node->content)
    {
        insertUnBalenced(element, node->left);
    }

    else
    {
        insertUnBalenced(element, node->right);
    }

    // treeSize++;
}


template <typename ElementType>
int AVLSet<ElementType>::getHeight(Node* node) const
{
    // if the tree is balance
    // or not

    if(node == nullptr)
        return -1;

    int l = getHeight(node->left);
    int r = getHeight(node->right);

    if(l>r)
        return l+1;
    else
        return r+1;

}



template <typename ElementType>
void AVLSet<ElementType>::LL(Node*& current){
     Node* temp;

     temp = current->left;
     current->left = temp->right;
     temp->right = current;
     current = temp;
}


template <typename ElementType>
void AVLSet<ElementType>::RL(Node*& current){
     RR(current->left);
     LL(current);
}


template <typename ElementType>
void AVLSet<ElementType>::RR(Node*& current){
     Node* temp;

     temp = current->right;
     current->right = temp->left;
     temp->left = current;
     current = temp;
}


template <typename ElementType>
void AVLSet<ElementType>::LR(Node*& current){
     LL(current->right);
     RR(current);
}


template <typename ElementType>
void AVLSet<ElementType>::preorderCall(VisitFunction visit, Node* node) const
{
    if(node == nullptr)
        return;
    visit(node->content);
    preorderCall(visit, node->left);
    preorderCall(visit, node->right);

}


template <typename ElementType>
void AVLSet<ElementType>::inorderCall(VisitFunction visit, Node* node) const
{
    if(node == nullptr)
        return;
    inorderCall(visit, node->left);
    visit(node->content);
    inorderCall(visit, node->right);

}


template <typename ElementType>
void AVLSet<ElementType>::postorderCall(VisitFunction visit, Node* node) const
{
    if(node == nullptr)
        return;
    postorderCall(visit, node->left);
    postorderCall(visit, node->right);
    visit(node->content);

}

#endif // AVLSET_HPP

