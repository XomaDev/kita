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
#include "../bytecode.h"
#include "stack_manager.h"

using namespace std;

class runtime {
    unique_ptr<uchar[]> bytes;
    long length;

    long index = 0;

    // philosophy: do not include intermediate EOF checks that may make
    // execution a bit slower, assuming bytecode is absolutely right

    stack_manager stack;

    string read_string();

    void expect(bytecode code);

    uchar peek();
    uchar advance();

    [[nodiscard]] bool isEOF() const;

    void exec_next();
    void load();

    void binary_operation();

    bool binary_equals();
    void binary_addition();

    static string element_to_string(pair<stack_type, uint64_t> element);

    void invoke();
    void func_invoke(int num_args);

    void declare();

    void if_decl();
    void fun_decl();

    void evaluate_scope();
    void skip_scope();

    void free_memory();

public:
    runtime(unique_ptr<uchar[]> bytes, long length) : bytes(std::move(bytes)), length(length) {
        // constructor initialized
    }

    void run();
};


#endif //KITA_RUNTIME_H
