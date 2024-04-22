//
// Created by kumaraswamy on 4/21/24.
//

#ifndef KITA_STACK_MANAGER_H
#define KITA_STACK_MANAGER_H

#include <vector>
#include <array>
#include <id3/globals.h>
#include <unordered_map>
#include <string>
#include <cstdint>
#include "stack_type.h"

using namespace std;

class stack_manager {
    vector<pair<stack_type, uint64_t>> main_stack;

    unordered_map<string, uint64_t> addr_map;
    unordered_map<uint64_t, string> rev_addr_map;

public:
    uint64_t stack_length = 0;

    void move_addr(const string &name, bool overwrite);
    uint64_t access_addr(const string &name);

    uint64_t assert_last_stack(stack_type expect_type);

    pair<stack_type, uint64_t> dereference(pair<stack_type, uint64_t> stack_element);

    void push_int(int64_t n);
    void push(stack_type type, uint64_t n);

    int64_t pop_int();
    pair<stack_type, uint64_t> pop_value();
    pair<stack_type, uint64_t> pop();

    void free_stack(uint64_t last_n);
};


#endif //KITA_STACK_MANAGER_H
