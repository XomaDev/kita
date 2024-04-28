//
// Created by kumaraswamy on 4/24/24.
//

#ifndef KITA_MEMORY_MANAGER_H
#define KITA_MEMORY_MANAGER_H

#include <cstdint>
#include <stack>
#include "stack_frame.h"
#include "address.h"

using namespace std;

class stack_frame;
class func_obj;

class memory_manager {
    stack<stack_frame *> frames_pool;

    vector<stack_frame *> frames;
    stack_frame* current_frame;

    long current_depth = 0;
public:
    memory_manager() { // NOLINT(*-pro-type-member-init)
        push_frame();
    }
    void push_frame();
    void release_frame();

    void push(stack_type type, uint64_t value);
    void push_int(int64_t value);

    int64_t pop_int();
    pair<stack_type, uint64_t> pop();

    ulong lookup_func(address address);

    void move_address();

    void access_address(address address);

    void free_all();
    void relocate_last();

    static void free_frame_content(stack_frame *pFrame);
};


#endif //KITA_MEMORY_MANAGER_H
