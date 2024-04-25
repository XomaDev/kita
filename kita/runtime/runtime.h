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
#include "memory_manager.h"

using namespace std;

class runtime {
    unique_ptr<uchar[]> bytes;
    long length;

    long index = 0;

    // long last_call_markup = -1;
    pair<stack_type, uint64_t> last_call_result;

    memory_manager memory;

    string read_string();
    int read_int32();

    void expect(bytecode code);

    bytecode advance_bytecode();
    uchar peek();
    uchar advance();

    [[nodiscard]] bool isEOF() const;

    int exec_next();
    void load();

    int return_decl();
    void func_decl();

    void binary_operation();

    bool binary_equals();
    void binary_addition();

    static string element_to_string(pair<stack_type, uint64_t> element);

    void invoke();
    void func_invoke(int i);

    void declare();

    int if_decl();

    int evaluate_scope(bool new_frame);
    void pass_scope();

    void free_memory();
public:
    runtime(unique_ptr<uchar[]> bytes, long length) : bytes(std::move(bytes)), length(length) {
        // constructor initialized
    }

    void run();

};


#endif //KITA_RUNTIME_H
