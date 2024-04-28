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
#include <optional>
#include "stack_type.h"
#include "../bytecode.h"
#include "structs/visitable.h"
#include "memory_manager.h"
#include "name_resolver.h"

using namespace std;

class visitable;
class memory_manager;

class runtime {
    unique_ptr<uchar[]> bytes;
    long length;

    long index = 0;

    vector<visitable> instructions;
    ulong instruction_index = 0;

    vector<visitable> ondemand_visitables;
    ulong ondemand_index = 0;

    name_resolver resolver;

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

    optional<visitable> encapsule_next();
    visitable load();

    visitable return_decl();
    visitable func_decl();

    visitable binary_operation();
    visitable unary_operation();

    bool binary_equals();
    visitable binary_addition();

    static string element_to_string(pair<stack_type, uint64_t> element);

    visitable invoke();
    visitable func_invoke(int i);

    visitable let();

    visitable if_decl();
    visitable inline_if_decl();

    visitable encapsule_scope(bool push_frame);
    vector<visitable> encapsule_set();

    static int execute_set(const vector<visitable>& local_instructions) ;

    void free_memory();
public:
    runtime(unique_ptr<uchar[]> bytes, long length) : bytes(std::move(bytes)), length(length) {
        // constructor initialized
    }

    void prepare();

    void run();

};


#endif //KITA_RUNTIME_H
