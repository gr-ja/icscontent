// HashSet.hpp
//
// ICS 46 Winter 2019
// Project #3: Set the Controls for the Heart of the Sun
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
// in your data structure.  Instead, you'll need to use a dynamically-
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

    double loadFactor() const;
    unsigned int cap; // capacity

private:

    HashFunction hashFunction; 

    // unsigned int cap;// capacity
    unsigned int total; // current size

    struct HashNode
    {
        ElementType value{};
        HashNode* next = nullptr;     
    };

    // HashNode(const ElementType& element): value(element), next(nullptr) {};

    HashNode** object;

    void emptyHashTable(HashNode** h);
    void deleteAllHashNodes(HashNode** h, unsigned int size);
    void copyHash(HashNode** h, HashNode** s, unsigned int size);
    void addNode(HashNode** table, const ElementType& element, unsigned int index);
    void rehash(HashNode** newTable, HashNode* node);
    // double loadFactor() const;
};



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
    : hashFunction{hashFunction}, total{0}, object{new HashNode* [DEFAULT_CAPACITY]}
{
    cap = DEFAULT_CAPACITY;
    // std::cout << "constructor" << std::endl;
    emptyHashTable(object);
}


template <typename ElementType>
HashSet<ElementType>::~HashSet() noexcept
{
    // std::cout << "DESTRUCTOR" << std::endl;
    total = 0;

    for(int i=0; i<cap; i++)
    {
         HashNode*& node = object[i];

         while(node != nullptr)
         {
            HashNode* dNode = node;
            node = node->next;
            delete dNode;


         }

    }

    delete[] object;
}


template <typename ElementType>
HashSet<ElementType>::HashSet(const HashSet& s)
    : hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>}
{
    // std::cout << "HashSet &s copy" << std::endl;
    cap = s.cap;
    object = new HashNode* [cap];
    copyHash(object, s.object, cap);
    total = s.total;
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashSet&& s) noexcept
    : hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>}
{
    // std::cout << "HashSet &&s copy" << std::endl;
    cap = s.cap;
    object = new HashNode* [cap];
    copyHash(object, s.object, cap);
    total = s.total;

    // std::cout << "HASH" << s.hash << std::endl;

    for(int i=0; i<s.cap; i++)
    {
         HashNode*& node = s.object[i];

         // std::cout << node << std::endl;
         delete node;

    }
    // delete[] s.hash; // ?????
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(const HashSet& s)
{
    // std::cout << "HashSet &s assign" << std::endl;
    if(this != &s)
    {
        HashNode** newHashTable = new HashNode* [s.cap];
        emptyHashTable(newHashTable);
        copyHash(newHashTable, s.object, s.cap);
        total = s.total;

        deleteAllHashNodes(object, cap);
        delete[] object;
        cap = s.cap;
        object = newHashTable;

        newHashTable = nullptr;
        delete[] newHashTable;

    }

    return *this;
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(HashSet&& s) noexcept
{
    // std::cout << "HashSet &&s assign" << std::endl;
    if(this != &s)
    {
        HashNode** newHashTable = new HashNode* [s.cap];
        emptyHashTable(newHashTable);
        copyHash(newHashTable, s.object, s.cap);
        total = s.total;

        deleteAllHashNodes(object, cap);
        delete[] object;
        cap = s.cap;
        object = newHashTable;

        newHashTable = nullptr;
        delete[] newHashTable;

        for(int i=0; i<s.cap; i++)
        {
             HashNode*& node = s.object[i];

             // std::cout << node << std::endl;
             delete node;

        }
    }

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
    // std::cout << "ADD" << std::endl;
    
    if(contains(element))
        return;

    unsigned int index = static_cast<unsigned int>(hashFunction(element)) % cap;

    addNode(object, element, index);
    total++;
    // std::cout << hash[index]->value << std::endl;

}


template <typename ElementType>
bool HashSet<ElementType>::contains(const ElementType& element) const
{
    // bool check = false;
    // std::cout << "CONTAINS" << std::endl;
    unsigned int index = static_cast<unsigned int>(hashFunction(element)) % cap;
    HashNode* node = object[index];

    while( node != nullptr )
    {
        if(node->value == element)
            return true;
        else
            node = node->next;
    }

    return false;
    
}


template <typename ElementType>
unsigned int HashSet<ElementType>::size() const noexcept
{
    // std::cout << "SIZE" << std::endl;
    return total;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::elementsAtIndex(unsigned int index) const
{
    // std::cout << "elementsAtIndex" << std::endl;
    int sizeIndex = 0;
    HashNode* temp = object[index];
    while(temp != nullptr)
    {
        sizeIndex++;
        temp = temp->next;
    }
    return sizeIndex;
}


template <typename ElementType>
bool HashSet<ElementType>::isElementAtIndex(const ElementType& element, unsigned int index) const
{
    // std::cout << "isElementAtIndex" << std::endl;
    HashNode* temp = object[index];
    while(temp != nullptr)
    {
        if(temp->value == element)
            return true;
        temp = temp->next;
    }
    return false;
}


// ===========================
// ADDITIONAL MEMBER FUNCTIONS
// ===========================


template <typename ElementType>
void HashSet<ElementType>::emptyHashTable(HashNode** h)
{
    for(int i=0; i<this->cap; i++)
        h[i] = nullptr;
}

template <typename ElementType>
void HashSet<ElementType>::deleteAllHashNodes(HashNode** h, unsigned int size)
{
    for(int i=0; i<size; i++)
    {
        HashNode*& node = h[i];
        while(node != nullptr)
        {
            HashNode* dNode = node;
            node = node->next;   
            dNode = nullptr;
            delete dNode;
            
        }
        
    }
}


template <typename ElementType>
void HashSet<ElementType>::copyHash(HashNode** h, HashNode** s, unsigned int size)
{
    for(int i=0; i<size; i++)
    {
        HashNode* n = s[i];
        HashNode** p = &h[i];
        *p = nullptr;
        while(n)
        {
            HashNode* c = new HashNode (*n);
            *p = c;
            p = &c -> next;
            n = n->next;
            delete c;
        }
    }
}


template <typename ElementType>
double HashSet<ElementType>::loadFactor() const
{
    return (double)this->size()/cap;
}


template<typename ElementType>
void HashSet<ElementType>::addNode(HashNode** table, const ElementType& element, unsigned int index)
{
    HashNode* prev = nullptr;
    HashNode* current = table[index];

    while(current != nullptr)
    {
        prev = current;
        current = current->next;
    }

    if(current == nullptr)
    {
        current = new HashNode{element};
        if(prev == nullptr)
        {
            table[index] = current;
        }
        else
            prev->next = current;
    }

    else
    {
        current->value = element;
    }
}

template <typename ElementType>
void HashSet<ElementType>::rehash(HashNode** newTable, HashNode* node)
{
    while(node!=nullptr)
    {
        unsigned int index = static_cast<unsigned int>(hashFunction(node->value)) % cap;
        addNode(newTable, node->value, index);
        HashNode* dNode = node;
        node = node->next;
        delete dNode;
    }
}


#endif // HASHSET_HPP

