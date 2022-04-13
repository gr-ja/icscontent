// SkipListSet.hpp
//
// ICS 46 Winter 2019
// Project #3: Set the Controls for the Heart of the Sun
//
// A SkipListSet is an implementation of a Set that is a skip list, implemented
// as we discussed in lecture.  A skip list is a sequence of levels
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the keys and their
// values.  Instead, you'll need to implement your own dynamically-allocated
// nodes, with pointers connecting them.  You can, however, use other parts of
// the C++ Standard Library -- including <random>, notably.
//
// Each node should contain only two pointers: one to the node that follows it
// on the same level and another to the equivalent node on the level below it.
// Additional pointers use more memory but don't enable any techniques not
// enabled by the other two.
//
// A couple of utilities are included here: SkipListKind and SkipListKey.
// You can feel free to use these as-is and probably will not need to
// modify them, though you can make changes to them, if you'd like.

#ifndef SKIPLISTSET_HPP
#define SKIPLISTSET_HPP

#include <memory>
#include <random>
#include "Set.hpp"




// SkipListKind indicates a kind of key: a normal one, the special key
// -INF, or the special key +INF.  It's necessary for us to implement
// the notion of -INF and +INF separately, since we're building a class
// template and not all types of keys would have a reasonable notion of
// -INF and +INF.

enum class SkipListKind
{
    Normal,
    NegInf,
    PosInf
};




// A SkipListKey represents a single key in a skip list.  It is possible
// to compare these keys using < or == operators (which are overloaded here)
// and those comparisons respect the notion of whether each key is normal,
// -INF, or +INF.

template <typename ElementType>
class SkipListKey
{
public:
    SkipListKey(SkipListKind kind, const ElementType& element);

    bool operator==(const SkipListKey& other) const;
    bool operator<(const SkipListKey& other) const;

private:
    SkipListKind kind;
    ElementType element;
};


template <typename ElementType>
SkipListKey<ElementType>::SkipListKey(SkipListKind kind, const ElementType& element)
    : kind{kind}, element{element}
{
}


template <typename ElementType>
bool SkipListKey<ElementType>::operator==(const SkipListKey& other) const
{
    return kind == other.kind
        && (kind != SkipListKind::Normal || element == other.element);
}


template <typename ElementType>
bool SkipListKey<ElementType>::operator<(const SkipListKey& other) const
{
    switch (kind)
    {
    case SkipListKind::NegInf:
        return other.kind != SkipListKind::NegInf;

    case SkipListKind::PosInf:
        return false;

    default: // SkipListKind::Normal
        return other.kind == SkipListKind::PosInf
            || (other.kind == SkipListKind::Normal && element < other.element);
    }
}



// The SkipListLevelTester class represents the ability to decide whether
// a key placed on one level of the skip list should also occupy the next
// level.  This is the "coin flip," so to speak.  Note that this is an
// abstract base class with one implementation, RandomSkipListLevelTester,
// just below it.  RandomSkipListLevelTester is what it sounds like: It
// makes the decision at random (with a 50/50 chance of deciding whether
// a key should occupy the next level).  However, by setting things up
// this way, we have a way to control things more carefully in our
// testing (as you can, as well).
//
// DO NOT MAKE CHANGES TO THE SIGNATURES OF THE MEMBER FUNCTIONS OF
// THE "level tester" CLASSES.  You can add new member functions or even
// whole new level tester classes, but the ones declared below are part
// of how we test your implementation, so they need to maintain the
// parts of the interface that are declared already.

template <typename ElementType>
class SkipListLevelTester
{
public:
    virtual ~SkipListLevelTester() = default;

    virtual bool shouldOccupyNextLevel(const ElementType& element) = 0;
    virtual std::unique_ptr<SkipListLevelTester<ElementType>> clone() = 0;
};


template <typename ElementType>
class RandomSkipListLevelTester : public SkipListLevelTester<ElementType>
{
public:
    RandomSkipListLevelTester();
    virtual ~RandomSkipListLevelTester() = default;

    virtual bool shouldOccupyNextLevel(const ElementType& element) override;
    virtual std::unique_ptr<SkipListLevelTester<ElementType>> clone() override;

private:
    std::default_random_engine engine;
    std::bernoulli_distribution distribution;
};


template <typename ElementType>
RandomSkipListLevelTester<ElementType>::RandomSkipListLevelTester()
    : engine{std::random_device{}()}, distribution{0.5}
{
}


template <typename ElementType>
bool RandomSkipListLevelTester<ElementType>::shouldOccupyNextLevel(const ElementType& element)
{
    return distribution(engine);
}


template <typename ElementType>
std::unique_ptr<SkipListLevelTester<ElementType>> RandomSkipListLevelTester<ElementType>::clone()
{
    return std::unique_ptr<SkipListLevelTester<ElementType>>{
        new RandomSkipListLevelTester<ElementType>};
}




template <typename ElementType>
class SkipListSet : public Set<ElementType>
{
public:
    // Initializes an SkipListSet to be empty, with or without a
    // "level tester" object that will decide, whenever a "coin flip"
    // is needed, whether a key should occupy the next level above.
    SkipListSet();
    explicit SkipListSet(std::unique_ptr<SkipListLevelTester<ElementType>> levelTester);

    // Cleans up the SkipListSet so that it leaks no memory.
    virtual ~SkipListSet() noexcept;

    // Initializes a new SkipListSet to be a copy of an existing one.
    SkipListSet(const SkipListSet& s);

    // Initializes a new SkipListSet whose contents are moved from an
    // expiring one.
    SkipListSet(SkipListSet&& s) noexcept;

    // Assigns an existing SkipListSet into another.
    SkipListSet& operator=(const SkipListSet& s);

    // Assigns an expiring SkipListSet into another.
    SkipListSet& operator=(SkipListSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement a SkipListSet, false otherwise.
    virtual bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function runs in an expected time
    // of O(log n) (i.e., over the long run, we expect the average to be
    // O(log n)) with very high probability.
    virtual void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in an expected time of O(log n)
    // (i.e., over the long run, we expect the average to be O(log n))
    // with very high probability.
    virtual bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    virtual unsigned int size() const noexcept override;


    // levelCount() returns the number of levels in the skip list.
    unsigned int levelCount() const noexcept;


    // elementsOnLevel() returns the number of elements that are stored
    // on the given level of the skip list.  Level 0 is the bottom level;
    // level 1 is the one above level 0; and so on.  If the given level
    // doesn't exist, this function returns 0.  (Note that the -INF
    // and +INF shouldn't be counted.)
    unsigned int elementsOnLevel(unsigned int level) const noexcept;


    // isElementOnLevel() returns true if the given element is on the
    // given level, false otherwise.  Level 0 is the bottom level; level 1
    // is the one above level 0; and so on.  If the given level doesn't
    // exist, this function returns false.
    bool isElementOnLevel(const ElementType& element, unsigned int level) const;


private:
    std::unique_ptr<SkipListLevelTester<ElementType>> levelTester;
    struct Node
    {
        Node(const ElementType& element) : value{ element }, key{SkipListKind::Normal, element}, prev{ nullptr }, next{ nullptr }, up{ nullptr }, down{ nullptr } {}
        Node(SkipListKind kind) : value{}, key{kind, ElementType()}, prev{ nullptr }, next{ nullptr }, up{ nullptr }, down{ nullptr }{}
        
        ElementType value;
        SkipListKey<ElementType> key;
        Node* prev;
        Node* next;
        Node* up;
        Node* down;
    };

    unsigned int elementNum;

    Node* head;

    void copyNode(const SkipListSet& s, Node* targetHead);
    void deleteAllNodes(SkipListSet& s) const;

};



template <typename ElementType>
SkipListSet<ElementType>::SkipListSet()
    : SkipListSet{std::make_unique<RandomSkipListLevelTester<ElementType>>()}
{
    std::cout << "constructor 1" << std::endl;
    head = {};
    elementNum=0;
}


template <typename ElementType>
SkipListSet<ElementType>::SkipListSet(std::unique_ptr<SkipListLevelTester<ElementType>> levelTester)
    : levelTester{std::move(levelTester)}
{
    std::cout << "constructor 2" << std::endl;
    head = {};
    elementNum=0;
}


template <typename ElementType>
SkipListSet<ElementType>::~SkipListSet() noexcept
{
    std::cout << "destructor: " << std::endl;
    
    if(head==nullptr)
        delete head;

    else
    {
        Node* currentHead = *&head;
        while(currentHead->down)
        {
            Node* temp = currentHead;
            if(currentHead->down != nullptr)
            {
                currentHead = currentHead->down;
            }
            while(temp)
            {
                Node* dNode = temp;
                temp = temp->next;
                delete dNode;
            }
        }
        elementNum=0;
    }

    
}


template <typename ElementType>
SkipListSet<ElementType>::SkipListSet(const SkipListSet& s)
{
    std::cout << "SkipListSet &s copy" << std::endl;

    levelTester = s.levelTester->clone();

    if(s.head == nullptr)
    {
        head = nullptr;
        return;
    }

    copyNode(s, head);

}


template <typename ElementType>
SkipListSet<ElementType>::SkipListSet(SkipListSet&& s) noexcept
{
    std::cout << "SkipListSet &&s copy" << std::endl;

    levelTester = s.levelTester->clone();

    if(s.head == nullptr)
    {
        head = nullptr;
        return;
    }

    copyNode(s, head);
    deleteAllNodes(s);
}


template <typename ElementType>
SkipListSet<ElementType>& SkipListSet<ElementType>::operator=(const SkipListSet& s)
{
    std::cout << "SkipListSet &s Assigns" << std::endl;

    levelTester = s.levelTester->clone();

    if(s.head == nullptr)
    {
        head = nullptr;
        return *this;
    }

    copyNode(s, head);
    return *this;
}


template <typename ElementType>
SkipListSet<ElementType>& SkipListSet<ElementType>::operator=(SkipListSet&& s) noexcept
{
    std::cout << "SkipListSet &&s Assigns" << std::endl;

    levelTester = s.levelTester->clone();

    if(s.head == nullptr)
    {
        head = nullptr;
        return *this;
    }
    copyNode(s, head);
    deleteAllNodes(s);
    
    return *this;
}


template <typename ElementType>
bool SkipListSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void SkipListSet<ElementType>::add(const ElementType& element)
{
    if(contains(element))
        return;


    Node* newNode = new Node(element);
    Node* current = head;

    // IF STATEMENT for inserting the newNode on the first level
    if(!current)
    {
        Node* newHead = new Node(SkipListKind::NegInf);
        Node* newTail = new Node(SkipListKind::PosInf);

        newHead->next = newNode;
        newNode->prev = newHead;
        newNode->next = newTail;
        newTail->prev = newNode;

        head = newHead;
        current = newNode;

    }
    else
    {
        while(current->down)
            current = current->down;

        while(current->next->key < newNode->key)
        {
            current = current->next;
        }

        Node* tempNext = current->next;
        current->next = newNode;
        newNode->prev = current;
        newNode->next = tempNext;
        tempNext->prev = newNode;

        current = newNode;

    }
    elementNum++;

    unsigned int currentLevel = 0;
    unsigned int totalLevel = levelCount();


    // WHILE STATEMENT to insert the newNode on the random levels
    while(levelTester->shouldOccupyNextLevel(element))
    {
        // std::cout << "talLevel << std::endl;
        // add new node above 
        // check the prev of the current node
        Node* newNodeAbove = new Node(element);
        current->up = newNodeAbove;
        newNodeAbove->down = current;
        currentLevel++;


        // if it needs a new level
        if(totalLevel==currentLevel)
        {
            Node* currentHead = head;
            Node* currentTail = current;
            current = current->up;
            Node* newHead = new Node(SkipListKind::NegInf);
            Node* newTail = new Node(SkipListKind::PosInf);
            newHead->down = currentHead;
            currentHead->up = newHead;

            newHead->next = current;
            current->prev = newHead;
            current->next = newTail;
            newTail->prev = current;

            while(currentTail->next)
                currentTail=currentTail->next;

            currentTail->up = newTail;
            newTail->down = currentTail;

            head = newHead;
            totalLevel++;

            // std::cout << "N->next->next << " " << std::endl;
        }

        // if there's already the level existing
        else
        {
            Node* currentPrev = current->prev;
            Node* currentNext = current->next;
            current = current->up;

            // // connect with the previous node
            while(currentPrev->up == nullptr)
                currentPrev = currentPrev->prev;

            currentPrev = currentPrev->up;
            currentPrev->next = current;
            current->prev = currentPrev;

            // // connect with the next node
            while(currentNext->up == nullptr)
            {
                if(currentNext->key == SkipListKey{SkipListKind::PosInf, ElementType()})
                    return;
                currentNext = currentNext->next;
            }
            currentNext = currentNext->up;
            currentNext->prev = current;
            current->next = currentNext;
        }
    }
}


template <typename ElementType>
bool SkipListSet<ElementType>::contains(const ElementType& element) const
{
    const SkipListKey k = SkipListKey{SkipListKind::Normal, element};

    Node* current = head;
    // std::cout << head 

    while(current)
    {
        if(current->key == k)
            return true;

        if(current->down == nullptr)
            if(k < current->next->key)
                return false;
            else
                current = current->next;

        else
            if(k < current->next->key)
                current = current->down;
            else
                current=current->next;
                        
    }

    return false;
}


template <typename ElementType>
unsigned int SkipListSet<ElementType>::size() const noexcept
{
    return elementNum;
}


template <typename ElementType>
unsigned int SkipListSet<ElementType>::levelCount() const noexcept
{
    unsigned int levelNum=0;
    if(head!=nullptr)
    {
        Node* temp = head;
        while(temp)
        {
            levelNum++;
            temp = temp->down;
        }
    }

    return levelNum;
}


template <typename ElementType>
unsigned int SkipListSet<ElementType>::elementsOnLevel(unsigned int level) const noexcept
{
    unsigned int eleNum = 0;;
    if(level < levelCount())
    {
        Node* current = head;
        for(int i=this->levelCount()-1; i>level; i--)
            current = current->down;

        while(current->next->next != nullptr)
        {
            current = current->next;
            eleNum++;
        }
    
    }

    
    return eleNum;
}


template <typename ElementType>
bool SkipListSet<ElementType>::isElementOnLevel(const ElementType& element, unsigned int level) const
{
    if(level < levelCount())
    {
        
        Node* current = head;
        int lvl = levelCount()-level-1;
        while(lvl!=0)
        {
            current = current->down;
            lvl--;
        }

        while(current->next != nullptr)
        {
            if(current->key == SkipListKey{SkipListKind::Normal, element})
            {
                return true;
            }
            current = current->next;
        }
    }


    return false;
}



// =============================================
//         ADDITIONAL MEMBER FUNCTIONS
// =============================================


template <typename ElementType>
void SkipListSet<ElementType>::copyNode(const SkipListSet& s, Node* targetHead)
{
    targetHead = new Node(SkipListKind::NegInf);
    Node* sNode = s.head;
    Node* current = targetHead;

    Node** arr = new Node* [s.levelCount()];
    int lvl = s.levelCount()-1;
    arr[lvl] = targetHead;

    // // creating NegInf nodes
    while(sNode->down)
    {
        lvl--;
        Node* newNode = new Node(SkipListKind::NegInf);
        current->down = newNode;
        newNode->up = current;
        Node* temp = newNode;

        arr[lvl] = temp; // just for checking
        current = current->down;
        sNode = sNode->down;
    }

    // copy other keys
    while(sNode->next == nullptr || sNode->next->key < SkipListKey(SkipListKind::PosInf, ElementType()))
    {
        sNode = sNode->next;
        Node* n = sNode;

        Node* newRowNode = new Node(*n);
        // std::cout << &newRowNode << std::endl;
        current->next = newRowNode;
        newRowNode->prev = current;

        current = current->next;

        Node* temp = newRowNode;
        arr[0] = temp;
        
        Node* sColumnNode = sNode;
        Node* columnNode = current;

        int i=1;
        while(sColumnNode->up)
        {
            sColumnNode=sColumnNode->up;
            Node* newColumnNode = new Node(*sColumnNode);
            columnNode->up = newColumnNode;
            newColumnNode->down = columnNode;

            Node* temp2 = arr[i];
            temp2->next = newColumnNode;
            newColumnNode->prev = temp2;

            temp2 = temp2->next;
            arr[i] = temp2;
            columnNode = columnNode->up;
            i++;
        }
    }

    for(int i=0; i<s.levelCount(); i++)
    {
        arr[i] = nullptr;
        delete arr[i];
    }

    delete[] arr;
}


template <typename ElementType>
void SkipListSet<ElementType>::deleteAllNodes(SkipListSet& s) const
{
    Node* sNode = s.head;
    Node* sHead = s.head;

    while(sHead != nullptr)
    {
        sHead = sHead->down;
        while(sNode != nullptr)
        {
            Node* dNode = sNode;
            sNode = sNode->next;
            delete dNode;
        }
    }
}



#endif // SKIPLISTSET_HPP

