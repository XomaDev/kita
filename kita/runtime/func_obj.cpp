//
// Created by kumaraswamy on 4/24/24.
//

#include <stdexcept>
#include "func_obj.h"

void func_obj::prepare(uint8_t args, stack_manager *stack) {
    if (this->args_size != args) {
        throw runtime_error("Expected " + to_string(this->args_size) + " for function '"
                    + name + ", but got" + to_string(args));
    }
    auto function_depth = stack->stack_depth + 1;
    for (uint8_t i = 0; i < args; i++) {
        auto value = stack->pop_value();
        bool success;
        switch (value.first) {
            case stack_type::INT:
                success = parameters[i].first == bytecode::INT_CLASS;
                break;
            case stack_type::BOOL:
                success = parameters[i].first == bytecode::BOOL_CLASS;
                break;
            case stack_type::STRING:
                success = parameters[i].first == bytecode::STRING_CLASS;
                break;
            default: {
                throw runtime_error("Unrecognized stack type " + to_string(static_cast<int>(value.first)));
            }
        }
        if (!success) {
            throw runtime_error("Expected parameter type " + to_string(static_cast<int>(parameters[i].first))
                                + ", but got " + to_string(static_cast<int>(value.first)));
        }
        stack->push(value.first, value.second);
        stack->move_addr(function_depth, "var@" + parameters[i].second, false);
    }
}