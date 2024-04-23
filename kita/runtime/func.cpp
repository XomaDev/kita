//
// Created by kumaraswamy on 4/23/24.
//

#include <stdexcept>
#include "func.h"
#include "stack_manager.h"

void func::verify_signature(int invoke_args, stack_manager& manager) {
    if (invoke_args != args_size) {
        throw runtime_error("Expected " + to_string(parameters.size()) + ", but got " + to_string(invoke_args));
    }
    auto future_scope = manager.stack_depth + 1;
    for (auto i = 0; i < args_size; i++) {
        auto value = manager.pop_value();
        verify_parameter_signature(value.first, parameter_classes[i]);

        manager.push(value.first, value.second);
        manager.move_addr_depth(future_scope, "var@" + parameters[i], false);
    }
}

void func::verify_parameter_signature(stack_type type, bytecode match) {
    bool success;
    switch (type) {
        case stack_type::INT:
            success = match == bytecode::INT_CLASS;
            break;
        case stack_type::STRING:
            success = match == bytecode::STRING_CLASS;
            break;
        case stack_type::BOOL:
            success = match == bytecode::BOOL_CLASS;
            break;
        default: {
            throw runtime_error("Unknown stack type " + to_string(static_cast<int>(type)));
        }
    }
    if (!success) {
        throw runtime_error("Parameter mismatch, expected bytecode " + to_string(static_cast<int>(match))
                                     + "but got stack_type " + to_string(static_cast<int>(type)));
    }
}

string func::print() {
    return "func{index=" + to_string(index) + ", name=" +
           func_name + ", parameter=" + to_string(parameters.size()) + "}";
}
