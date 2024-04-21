//
// Created by kumaraswamy on 4/20/24.
//

#ifndef KITA_RUNTIME_H
#define KITA_RUNTIME_H

#include <memory>
#include <vector>
#include <array>
#include <id3/globals.h>
#include <unordered_map>
#include "stack_type.h"

using namespace std;

class runtime {
    unique_ptr<uchar[]> bytes;
    long length;

    long index = 0;

    // philosophy: do not include intermediate EOF checks that may make
    // execution a bit slower, assuming bytecode is absolutely right

    string read_name();

    uchar advance();

    [[nodiscard]] bool isEOF() const;

    vector<array<uint, 2>> main_stack;
    uint stack_index = 0;

    unordered_map<string, uint> addr_map;

    void exec_next();
    void load();

    void binary_operation();
    void invoke();

    void declare();

    void move_addr(const string &name, bool overwrite);
    uint access_addr(const string &name);

    uint assert_last_stack(stack_type expect_type);

    array<uint, 2> dereference(array<uint, 2> &stack_element);

    void push_int(int n);
    void push(stack_type type, uint n);

    int pop_int();
    array<uint, 2> pop();
public:
    runtime(unique_ptr<uchar[]> bytes, long length) : bytes(std::move(bytes)), length(length) {
        // constructor initialized
    }

    void run();
};


#endif //KITA_RUNTIME_H
