//
//  LockFreeLinkedList.h
//  Lock-Free Ordered Linked List
//
//  Created by Станислав Райцин on 03.02.15.
//  Copyright (c) 2015 Станислав Райцин. All rights reserved.
//

#ifndef __Lock_Free_Ordered_Linked_List__LockFreeLinkedList__
#define __Lock_Free_Ordered_Linked_List__LockFreeLinkedList__

#include <atomic>
#include "NodeTypes.h"
#include "LFLLIterator.h"

template <typename T>
class LockFreeLinkedList {
public:
    
private:
    void            update(LFLLIterator<T>* it);            // done
    Node<T>*        safeRead(std::atomic<Node<T>*>* p);
    void            release(Node<T>* p);
    
    std::atomic<CellNode<T>*> FIRST;
    std::atomic<CellNode<T>*> LAST;

};

template <typename T>
void LockFreeLinkedList<T>::update(LFLLIterator<T>* it) {
    if (it->pre_aux.load()->next.load() == it->target.load()) {
        return;
    }
    Node<T>*        p = it->pre_aux.load();
    Node<T>*        n = safeRead(&p->next);
    release(it->target.load());
    while (n != LAST.load() && !dynamic_cast<CellNode<T>*>(n)) {
        it->pre_cell.load()->next.compare_exchange_weak(p, n);
        release(p);
        p = n;
        n = safeRead(&p->next);
    }
    it->pre_aux.store(p);
    it->target.store(dynamic_cast<CellNode<T>*>(n));
    }
}

#endif /* defined(__Lock_Free_Ordered_Linked_List__LockFreeLinkedList__) */
