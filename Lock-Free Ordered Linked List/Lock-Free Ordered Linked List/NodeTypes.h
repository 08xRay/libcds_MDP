//
//  NodeTypes.h
//  Lock-Free Ordered Linked List
//
//  Created by Станислав Райцин on 01.01.15.
//  Copyright (c) 2015 Станислав Райцин. All rights reserved.
//

#ifndef __Lock_Free_Ordered_Linked_List__NodeTypes__
#define __Lock_Free_Ordered_Linked_List__NodeTypes__

template <typename T>
struct CellNode;

template <typename T>
struct AuxNode;


template <typename T>
struct AuxNode {
    std::atomic<CellNode<T>*>   next;
    
    AuxNode() :
    next(nullptr) {}
};

template <typename T>
struct CellNode {
    T                           data;
    std::atomic<AuxNode<T>*>    next;
    std::atomic<CellNode<T>*>   back_link;
    std::atomic_ulong           refct_claim;
    
    CellNode() :
    data(T()),
    next(nullptr),
    back_link(nullptr),
    refct_claim(0) {}
};

#endif /* defined(__Lock_Free_Ordered_Linked_List__NodeTypes__) */
