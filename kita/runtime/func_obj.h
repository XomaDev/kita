//
// Created by kumaraswamy on 4/24/24.
//

#ifndef KITA_FUNC_OBJ_H
#define KITA_FUNC_OBJ_H

#include <string>
#include <utility>
#include <vector>
#include "../bytecode.h"
#include "memory_manager.h"

using namespace std;

class memory_manager;
class visitable;

class func_obj {
    string name;
    u_int8_t args_size;
    vector<pair<bytecode, string>> parameters;
public:
    bytecode return_type;
    ulong visitable_index;

    func_obj(string name, bytecode return_type, u_int8_t args_size, vector<pair<bytecode, string>> parameters, ulong visitable_index)
                    : name(std::move(name)), return_type(return_type), args_size(args_size),
                    parameters(std::move(parameters)), visitable_index(visitable_index) {
        // constructor initialized
    }

    void prepare(uint8_t args, memory_manager *memory);
};


#endif //KITA_FUNC_OBJ_H
