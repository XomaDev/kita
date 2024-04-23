//
// Created by kumaraswamy on 4/23/24.
//

#ifndef KITA_FUNC_H
#define KITA_FUNC_H

#include <string>
#include <utility>
#include <vector>
#include "stack_manager.h"
#include "../bytecode.h"

using namespace std;

class stack_manager;

class func {
    static void verify_parameter_signature(stack_type type, bytecode match);
public:
    const uint8_t args_size;
    const long index;

    const string func_name;

    const vector<string> parameters;
    const vector<bytecode> parameter_classes;

    func(long index, string func_name, vector<string> parameters, vector<bytecode> parameter_classes)
                : args_size(parameters.size()), index(index), func_name(std::move(func_name)),
                  parameters(std::move(parameters)), parameter_classes(std::move(parameter_classes)) {
        // constructor initialized
    }

    void verify_signature(int num_args, stack_manager& manager);
    string print();
};


#endif //KITA_FUNC_H
