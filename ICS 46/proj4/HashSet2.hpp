// HashSet.hpp
//
// ICS 46 Spring 2018
// Project #4: Set the Controls for the Heart of the Sun
//
// A HashSet is an implementation of a Set that is a separately-chained
// hash table, implemented as a dynamically-allocated array of linked
// lists.  At any given time, the HashSet has a "size" indicating
// how many elements are stored within it, along with a "capacity"
// indicating the size of the array.
//
// As elements are added to the HashSet and the proportion of the HashSet's
// size to its capacity exceeds 0.8 (i.e., there are more than 80% as many
// elements as there are array cells), the HashSet should be resized so
// that it is twice as large as it was before.
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your content structure.  Instead, you'll need to use a dynamically-
// allocated array and your own linked list implemenation; the linked list
// doesn't have to be its own class, though you can do that, if you'd like.

#ifndef HASHSET_HPP
#define HASHSET_HPP

#include <functional>
#include "Set.hpp"
#include <iostream>


template <typename ElementType>
class HashSet : public Set<ElementType>
{
public:
    // The default capacity of the HashSet before anything has been
    // added to it.
    static constexpr unsigned int DEFAULT_CAPACITY = 10;

    // A HashFunction is a function that takes a reference to a const
    // ElementType and returns an unsigned int.
    using HashFunction = std::function<unsigned int(const ElementType&)>;

public:
    // Initializes a HashSet to be empty, so that it will use the given
    // hash function whenever it needs to hash an element.
    explicit HashSet(HashFunction hashFunction);

    // Cleans up the HashSet so that it leaks no memory.
    virtual ~HashSet() noexcept;

    // Initializes a new HashSet to be a copy of an existing one.
    HashSet(const HashSet& s);

    // Initializes a new HashSet whose contents are moved from an
    // expiring one.
    HashSet(HashSet&& s) noexcept;

    // Assigns an existing HashSet into another.
    HashSet& operator=(const HashSet& s);

    // Assigns an expiring HashSet into another.
    HashSet& operator=(HashSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement a HashSet, false otherwise.
    virtual bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function triggers a resizing of the
    // array when the ratio of size to capacity would exceed 0.8.  In the case
    // where the array is resized, this function runs in linear time (with
    // respect to the number of elements, assuming a good hash function);
    // otherwise, it runs in constant time (again, assuming a good hash
    // function).
    virtual void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in constant time (with respect
    // to the number of elements, assuming a good hash function).
    virtual bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    virtual unsigned int size() const noexcept override;


    // elementsAtIndex() returns the number of elements that hashed to a
    // particular index in the array.  If the index is out of the boundaries
    // of the array, this function returns 0.
    unsigned int elementsAtIndex(unsigned int index) const;


    // isElementAtIndex() returns true if the given element hashed to a
    // particular index in the array, false otherwise.  If the index is
    // out of the boundaries of the array, this functions returns 0.
    bool isElementAtIndex(const ElementType& element, unsigned int index) const;


private:
    HashFunction hashFunction;


    struct Node
    {
    ElementType content;
    Node* next;
    };

    Node** hash;
    int cap;
    int remain_size;
    int total;
    
    void copy_helper(Node** dest, Node** src);

    void delete_everything(int current_size);

    Node* copy_ll(Node* ll_head); 
//    bool contains_helper(Node** hash, ElementType content);

    
    
};

// =========================================================================================================================

namespace impl_
{
    template <typename ElementType>
    unsigned int HashSet__undefinedHashFunction(const ElementType& element)
    {
        return 0;
    }
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashFunction hashFunction)
    : hashFunction{hashFunction}
{

    hash = new Node*[DEFAULT_CAPACITY];
    cap = DEFAULT_CAPACITY;
    remain_size = cap;
    total = 0;
    for (int i = 0; i < cap; ++i)
    {
        hash[i] = nullptr;
    }
}

template <typename ElementType> 
void HashSet<ElementType>::delete_everything(int current_size)
{
for (int i = 0; i < current_size; ++i)
    {
        if (hash[i] != nullptr)
        {
            Node* head = hash[i];
            while (head != nullptr)
            {
                Node* temp = head;
                head = head -> next;
                delete temp;
            }
        }
    }
delete [] hash;
}

template <typename ElementType>
HashSet<ElementType>::~HashSet() noexcept
{
    delete_everything(cap);
}

template <typename ElementType>
typename HashSet<ElementType>::Node* HashSet<ElementType>::copy_ll(Node* ll_head)
{
    Node* current = ll_head;
    Node* newHead = nullptr;

    try
    {
        while (current != nullptr)
        {
        newHead = new Node{current -> content, newHead};
        current = current-> next;
        }
        return newHead;

    }
    catch(...)
    {
        throw;
    }
}


template <typename ElementType>
HashSet<ElementType>::HashSet(const HashSet& s)
    : hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>}
{
    
    cap = s.cap;
    remain_size = s.remain_size;
    total = s.total;
    hashFunction = s.hashFunction;

    hash = new Node*[s.cap];
    for (int i = 0; i < s.cap; ++i)
    {
    if (s.hash[i] == nullptr)
        hash[i] = nullptr;
    else
        {
        hash[i] = copy_ll(s.hash[i]);
        }
    }
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashSet&& s) noexcept
    : hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>}
{
    hash = new Node*[DEFAULT_CAPACITY];
    cap = 0;
    remain_size = 0;
    total = 0;
    for (int i = 0; i < cap; ++i)
    {
        hash[i] = nullptr;
    }

    std::swap(hash, s.hash);
    std::swap(cap, s.cap);
    std::swap(remain_size, s.remain_size);
    std::swap(total, s.total);
    std::swap(hashFunction, s.hashFunction);
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(const HashSet& s)
{
    delete_everything(cap);
    cap = 0;
    remain_size = 0;
    total = 0;


    hash = new Node*[s.cap];
    for (int i = 0; i < s.cap; ++i)
    {
    if (s.hash[i] == nullptr)
        hash[i] = nullptr;
    else
        {
        hash[i] = copy_ll(s.hash[i]);
        }    
    }
    cap = s.cap;
    remain_size = s.remain_size;
    total = s.total;
    hashFunction = s.hashFunction;
    return *this;
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(HashSet&& s) noexcept
{
    std::swap(hash, s.hash);
    std::swap(cap, s.cap);
    std::swap(remain_size, s.remain_size);
    std::swap(total, s.total);
    std::swap(hashFunction, s.hashFunction);
    return *this;
}


template <typename ElementType>
bool HashSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void HashSet<ElementType>::add(const ElementType& element)
{
    if (contains(element) == true)
        return;
//get index
    
    if (remain_size <= cap*0.2)
        {
        
        int double_capacity = 2*cap;
        Node** new_hash = new Node*[double_capacity];
        for (int i = 0; i < double_capacity; ++i)
            {
            new_hash[i] = nullptr;
            }
        int temp_cap = cap;
        cap = double_capacity;
        remain_size = cap;
        total = 0;
        //finished initializing new_hash;
        for (int i = 0; i < temp_cap; ++i)
            {
            if (hash[i] != nullptr)
                {
                Node* temp = hash[i];
                while (temp != nullptr)
                    {
                    unsigned int new_index = (hashFunction(temp -> content) % cap);
                    if (new_hash[new_index] == nullptr)
                        {
                            --remain_size;
                            Node* new_node = new Node{element, nullptr};
                            ++total;
                            new_hash[new_index] = new_node;
                        }
                    else
                        {
                            Node* current_node = new_hash[new_index];
                            Node* new_node = new Node{element, current_node};
                            ++total;
                            new_hash[new_index] = new_node;
                        }
                    temp = temp -> next;
                    }
                }            
            }
        Node** temp = new_hash;
        delete_everything(temp_cap);
        hash = temp;
        }
    else
    {
    unsigned int  hash_int = hashFunction(element);
    unsigned int index = (hash_int % cap);
        if (hash[index] == nullptr)
            {
            //Node* current_index = hash[index];
            remain_size--;
            Node* new_node = new Node;
            total++;
            new_node -> content = element;
            new_node -> next = nullptr;
            //current_index = new_node;
            hash[index] = new_node;
            }
        else if (hash[index] != nullptr)
            {
            Node* temp = hash[index];
            Node* new_node = new Node;
            total++;
            new_node -> content = element;
            new_node -> next = temp;
            hash[index] = new_node;
            }
        else
            return;
    }
// else if hash[index] has a linked list already.
}


template <typename ElementType>
bool HashSet<ElementType>::contains(const ElementType& element) const
{
    unsigned int hash_int  = hashFunction(element);
    unsigned int index = (hash_int % cap);
    bool existed = isElementAtIndex(element, index);
    // in the existed,if the index is out of boundary, then it will return 0. WIll this 0 become
    // a false, or it is still an int 0?
    return existed;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::size() const noexcept
{
    return total;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::elementsAtIndex(unsigned int index) const
{
    if (index > cap - 1)
        return 0;
    else
        {
        Node* temp = hash[index];
        if (temp == nullptr)
            return 0;
        else
            {
            int count_element = 0;
            while (temp != nullptr)
                {
                ++count_element;
                temp = temp -> next;
                }
            return count_element;
            }
        }
}


template <typename ElementType>
bool HashSet<ElementType>::isElementAtIndex(const ElementType& element, unsigned int index) const
{
    if (index > cap -1)
        return 0;
    else
    {
        if (hash[index] == nullptr)
            return false;
        else 
            {
            Node* temp = hash[index];
            while (temp != nullptr)
                {
                if (temp -> content == element)
                    return true;
                else
                    temp = temp -> next;
                }
            return false;
            }
    }
}



#endif // HASHSET_HPP