//
// Created by kumaraswamy on 4/24/24.
//

#ifndef KITA_STACK_FRAME_H
#define KITA_STACK_FRAME_H

#include <utility>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <string>
#include "stack_type.h"
#include "func_obj.h"

using namespace std;

class stack_frame {
    ulong stack_length = 0;
    vector<uint64_t> addresses;
public:
    vector<pair<stack_type, uint64_t>> stack;
    long depth;

    explicit stack_frame(long depth) : depth(depth) {
        // constructor initialized
    }
    ~stack_frame();

    void move_address();
    uint64_t access_address(ulong index);

    void push(stack_type type, uint64_t value);
    pair<stack_type, uint64_t> pop();
    pair<stack_type, uint64_t> peek_back();
};


#endif //KITA_STACK_FRAME_H
