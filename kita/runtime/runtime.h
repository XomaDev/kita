//
// Created by kumaraswamy on 4/20/24.
//

#ifndef KITA_RUNTIME_H
#define KITA_RUNTIME_H

#include <memory>
#include <vector>
#include <id3/globals.h>

using namespace std;

class runtime {
    unique_ptr<uchar[]> bytes;
    long length;

    long index = 0;

    // philosophy: do not include intermediate EOF checks that may make
    // execution a bit slower, assuming bytecode is absolutely right
    uchar next();
    uchar peek();

    bool isEOF();

    vector<int> main_stack;
    long stack_index;

    void exec_next();
    void load();

    void binary_operation();
    void invoke();

    int back_stack(int n);

    void push(int n);
    int pop();
public:
    runtime(unique_ptr<uchar[]> bytes, long length) : bytes(std::move(bytes)), length(length) {
        // constructor initialized
    }

    void run();
};


#endif //KITA_RUNTIME_H
