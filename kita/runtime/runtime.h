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
#include "stack_manager.h"

using namespace std;

class runtime {
    unique_ptr<uchar[]> bytes;
    long length;

    long index = 0;

    // philosophy: do not include intermediate EOF checks that may make
    // execution a bit slower, assuming bytecode is absolutely right

    stack_manager stack;

    string read_name();

    uchar advance();

    [[nodiscard]] bool isEOF() const;

    void exec_next();
    void load();

    void binary_operation();
    void invoke();

    void declare();
public:
    runtime(unique_ptr<uchar[]> bytes, long length) : bytes(std::move(bytes)), length(length) {
        // constructor initialized
    }

    void run();
};


#endif //KITA_RUNTIME_H
