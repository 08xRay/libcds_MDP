//
//  NodeTypes.h
//  Lock-Free Ordered Linked List
//
//  Created by Станислав Райцин on 01.01.15.
//  Copyright (c) 2015 Станислав Райцин. All rights reserved.
//

#ifndef __Lock_Free_Ordered_Linked_List__NodeTypes__
#define __Lock_Free_Ordered_Linked_List__NodeTypes__

#include <atomic>

template <typename T>
struct CellNode;

template <typename T>
struct Node;


template <typename T>
struct Node {
    std::atomic<Node<T>*>   next;
    
    Node() :
    next(nullptr) {}
    
    Node (const Node&) = delete;             // copy constructor
    Node (Node&&) = delete;                  // move constructor
    Node& operator= (const Node&) = delete;  // copy assignment operator
    
    virtual ~Node() = default;               // polymorphic declaration
};

template <typename T>
struct CellNode : public Node<T> {
    T                       data;
    std::atomic<Node<T>*>   back_link;
    std::atomic_ulong       refct_claim;
    
    CellNode() :
    data(T()),
    back_link(nullptr),
    refct_claim(0) {}
    
    CellNode (const CellNode&) = delete;             // copy constructor
    CellNode (CellNode&&) = delete;                  // move constructor
    CellNode& operator= (const CellNode&) = delete;  // copy assignment operator
};

#endif /* defined(__Lock_Free_Ordered_Linked_List__NodeTypes__) */
