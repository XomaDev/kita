//
// Created by kumaraswamy on 4/24/24.
//

#include <stdexcept>
#include <iostream>
#include "stack_frame.h"

void stack_frame::move_address() {
    addresses.emplace_back(stack_length - 1);
}

uint64_t stack_frame::access_address(ulong index) {
    return addresses[index];
}

void stack_frame::push(stack_type type, uint64_t value) {
    stack.emplace_back(type, value);
    stack_length++;
}

pair<stack_type, uint64_t> stack_frame::pop() {
    auto element = stack.back();
    stack.pop_back();
    stack_length--;
    return element;
}

pair<stack_type, uint64_t> stack_frame::peek_back() {
    return stack.back();
}

stack_frame::~stack_frame() {
    while (stack_length) {
        auto popped = stack[--stack_length];
        if (popped.first == stack_type::STRING) {
            auto chars = reinterpret_cast<const char*>(popped.second);
            delete[] chars;
        } else if (popped.first == stack_type::FUNC_PTR) {
            auto func_object = reinterpret_cast<class func_obj*>(popped.second);
            delete func_object;
        }
    }
}
