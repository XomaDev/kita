//
// Created by kumaraswamy on 4/24/24.
//

#include <stdexcept>
#include <iostream>
#include "func_obj.h"

void func_obj::prepare(uint8_t args, memory_manager *memory) {
    if (this->args_size != args) {
        throw runtime_error("Expected " + to_string(this->args_size) + " for function '"
                            + name + ", but got" + to_string(args));
    }
    memory->push_frame();
    for (uint8_t i = 0; i < args; i++) {
        auto value = memory->peek_pop_value();
        memory->push(value.first, value.second);
        memory->move_address("var@" + parameter_names[i]);
    }
}
