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
#include "stack_type.h"

using namespace std;

class stack_manager {
    vector<array<uint, 2>> main_stack;
    unordered_map<string, uint> addr_map;

public:
    uint stack_index = 0;

    void move_addr(const string &name, bool overwrite);
    uint access_addr(const string &name);

    uint assert_last_stack(stack_type expect_type);

    array<uint, 2> dereference(array<uint, 2> &stack_element);

    void push_int(int n);
    void push(stack_type type, uint n);

    int pop_int();
    array<uint, 2> pop();
};


#endif //KITA_STACK_MANAGER_H
