//
// Created by kumaraswamy on 4/24/24.
//

#include <stdexcept>
#include <iostream>
#include "stack_frame.h"

void stack_frame::move_address() {
    addresses.emplace_back(stack_length - 1);
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

pair<pair<stack_type, uint64_t>, pair<stack_type, uint64_t>> stack_frame::smart_lookup() {
    return { stack[stack_length - 1], stack[stack_length - 2] };
}