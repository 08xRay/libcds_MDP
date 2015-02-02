//
//  LFLLIterator.h
//  Lock-Free Ordered Linked List
//
//  Created by Станислав Райцин on 01.01.15.
//  Copyright (c) 2015 Станислав Райцин. All rights reserved.
//

#ifndef __Lock_Free_Ordered_Linked_List__LFLLIterator__
#define __Lock_Free_Ordered_Linked_List__LFLLIterator__

#include <atomic>

#include "NodeTypes.h"

template <typename T>
class LFLLIterator {
public:
    LFLLIterator() : pre_aux(nullptr), target(nullptr), pre_cell(nullptr) {}
    inline bool operator==(const LFLLIterator<T>& it) const;
    inline bool operator!=(const LFLLIterator<T>& it) const;
    
    std::atomic<AuxNode<T>*>    pre_aux;
    std::atomic<CellNode<T>*>   target;
    std::atomic<CellNode<T>*>   pre_cell;
};

template <typename T>
bool LFLLIterator<T>::operator==(const LFLLIterator<T>& it) const {
    return (pre_aux.load() == it.pre_aux.load() && target.load() == it.target.load() && pre_cell.load() == it.pre_cell.load());
}

template <typename T>
bool LFLLIterator<T>::operator!=(const LFLLIterator<T>& it) const {
    return (pre_aux.load() != it.pre_aux.load() && target.load() != it.target.load() && pre_cell.load() != it.pre_cell.load());
}

#endif /* defined(__Lock_Free_Ordered_Linked_List__LFLLIterator__) */
