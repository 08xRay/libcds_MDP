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
    void            first(LFLLIterator<T>* it);             // done
    bool            next(LFLLIterator<T>* it);              // done
    bool            tryInsert(LFLLIterator<T>* it, CellNode<T>& q, Node<T>& a); // check & or *
    bool            tryDelete(LFLLIterator<T>* it);         // done
    Node<T>*        safeRead(std::atomic<Node<T>*>* p);
    void            release(Node<T>* p);
    
    std::atomic<CellNode<T>*> FIRST_NODE;
    std::atomic<CellNode<T>*> LAST_NODE;

};

template <typename T>
void LockFreeLinkedList<T>::update(LFLLIterator<T>* it) {
    if (it->pre_aux.load()->next.load() == it->target.load()) {
        return;
    }
    Node<T>*        p = it->pre_aux.load();
    Node<T>*        n = safeRead(&p->next);
    release(it->target.load());
    while (n != LAST_NODE.load() && !dynamic_cast<CellNode<T>*>(n)) {
        it->pre_cell.load()->next.compare_exchange_weak(p, n);
        release(p);
        p = n;
        n = safeRead(&p->next);
    }
    it->pre_aux.store(p);
    it->target.store(dynamic_cast<CellNode<T>*>(n));
}

template <typename T>
void LockFreeLinkedList<T>::first(LFLLIterator<T>* it) {
    it->pre_cell.store(safeRead(&FIRST_NODE));       // check dynamic_cast
    it->pre_aux.store(safeRead(&FIRST_NODE->next));
    it->target.store(nullptr);
    update(it);
}

template <typename T>
bool LockFreeLinkedList<T>::next(LFLLIterator<T>* it) {
    if (it->target.load() == LAST_NODE.load()) {
        return false;
    } else {
        release(it->pre_cell.load());
        it->pre_cell.store(safeRead(&it->target));// check dynamic_cast
        release(it->pre_aux.load());
        it->pre_aux.store(safeRead(&it->target.load()->next));
        update(it);
        return true;
    }
}

template <typename T>
bool LockFreeLinkedList<T>::tryInsert(LFLLIterator<T>* it, CellNode<T>& q, Node<T>& a) {
    q.next.store(a);
    a.next.store(it->target.load());
    bool r = it->pre_aux.load()->next.compare_exchange_weak(it->target.load(), q);
    return r;
}

template <typename T>
bool LockFreeLinkedList<T>::tryDelete(LFLLIterator<T>* it) {
    CellNode<T>*     d = it->target.load();
    Node<T>*         n = it->target.load()->next.load();
    bool r = it->pre_aux.load()->next.compare_exchange_weak(d, n);
    if (!r) {
        return false;
    } else {
        d->back_link.store(it->pre_cell.load());
        CellNode<T>* p = it->pre_cell.load();
        while (p->back_link.load() != nullptr) {
            CellNode<T>* q = dynamic_cast<CellNode<T>*>(safeRead(&p->back_link));
            release(p);
            p = q;
        }
        Node<T>*     s = safeRead(&p->next);
        while (!dynamic_cast<CellNode<T>*>(n->next.load())) {
            Node<T>* q = safeRead(&n->next);
            release(n);
            n = q;
        }
        do {
            r = p->next.compare_exchange_weak(s, n);
            if (!r) {
                release(s);
                s = safeRead(&p->next);
            }
        } while (!r && p->back_link.load() == nullptr
                 && dynamic_cast<CellNode<T>*>(n->next.load()));
        release(p);
        release(s);
        release(n);
        return true;
    }
}


#endif /* defined(__Lock_Free_Ordered_Linked_List__LockFreeLinkedList__) */
