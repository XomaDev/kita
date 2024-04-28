//
// Created by kumaraswamy on 4/24/24.
//

#ifndef KITA_STACK_FRAME_H
#define KITA_STACK_FRAME_H

#include <utility>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include "stack_type.h"

using namespace std;

class stack_frame {
public:
    vector<uint64_t> addresses;
    vector<pair<stack_type, uint64_t>> stack;


    void move_address();
    uint64_t access_address(ulong index);

    void push(stack_type type, uint64_t value);
    pair<stack_type, uint64_t> pop();
    pair<stack_type, uint64_t> peek_back();

    ulong stack_length = 0;
};


#endif //KITA_STACK_FRAME_H
