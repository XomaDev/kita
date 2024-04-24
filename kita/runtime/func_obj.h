//
// Created by kumaraswamy on 4/24/24.
//

#ifndef KITA_FUNC_OBJ_H
#define KITA_FUNC_OBJ_H

#include <string>
#include <utility>
#include <vector>
#include "../bytecode.h"
#include "stack_manager.h"

using namespace std;

class stack_manager;

class func_obj {
    string name;
    u_int8_t args_size;
    vector<pair<bytecode, string>> parameters;
public:
    bytecode return_type;
    long index;

    func_obj(string name, bytecode return_type, u_int8_t args_size, vector<pair<bytecode, string>> parameters, long index)
                    : name(std::move(name)), return_type(return_type), args_size(args_size), parameters(std::move(parameters)), index(index) {
        // constructor initialized
    }

    void prepare(uint8_t args, stack_manager *stack);
};


#endif //KITA_FUNC_OBJ_H
