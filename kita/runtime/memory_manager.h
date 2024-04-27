//
// Created by kumaraswamy on 4/24/24.
//

#ifndef KITA_MEMORY_MANAGER_H
#define KITA_MEMORY_MANAGER_H

#include <cstdint>
#include "stack_frame.h"

using namespace std;

class stack_frame;
class func_obj;

class memory_manager {
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

    pair<stack_type, uint64_t> pop_value();

    void assert_last(stack_type type);

    func_obj *lookup_func(pair<ulong, ulong>);

    void move_address();

    uint64_t access_address(pair<ulong, ulong> address);

    void free_all();

    pair<stack_type, uint64_t> dereference(pair<stack_type, uint64_t> element);
    void relocate_last();

    stack_frame *&get_frame(const pair<ulong, ulong> &address);
};


#endif //KITA_MEMORY_MANAGER_H
