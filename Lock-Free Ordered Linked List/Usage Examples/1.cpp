//
//  main.cpp
//  Lock-Free Ordered Linked List
//
//  Created by Станислав Райцин on 01.01.15.
//  Copyright (c) 2015 Станислав Райцин. All rights reserved.
//

#include <iostream>
#include <thread>
#include "LockFreeLinkedList.h"

std::chrono::seconds timeout(1);
LockFreeLinkedList<int>* list;

void writer_method() {
    int val = 0;
    while (true) {
        list->_insert(++val);
        std::this_thread::sleep_for(timeout);
    }
}

void reader_method() {
    int val = 1;
    while (true) {
        int target = std::rand() % val + 1;
        if (list->_find(target)) {
            std::cout << "List contains: " << target << std::endl;
            list->_delete(target);
            ++val;
        } else {
            std::cout << "Value: " << target << " not found in list" << std::endl;
        }
        std::this_thread::sleep_for(timeout);
    }
}

int main(int argc, const char * argv[]) {
    list = new LockFreeLinkedList<int>(10000);
    std::thread reader(reader_method);
    std::thread writer(writer_method);
    reader.join();
    writer.join();
    return 0;
}
