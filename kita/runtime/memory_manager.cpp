//
// Created by kumaraswamy on 4/24/24.
//

#include <stdexcept>
#include <iostream>
#include "memory_manager.h"

void memory_manager::push_frame() {
    current_frame = new stack_frame(current_depth++);
    frames.emplace_back(current_frame);
}

void memory_manager::release_frame() {
    auto frame = frames.back();
    delete frame;
    frames.pop_back();
    current_depth--;
    current_frame = frames.back();
}

void memory_manager::move_address() {
    return current_frame->move_address();
}

uint64_t memory_manager::access_address(pair<ulong, ulong> address) {
    return get_frame(address)->access_address(address.second);
}

stack_frame *&memory_manager::get_frame(const pair<ulong, ulong> &address) {
    return frames[current_depth - address.first - 1];
}

func_obj* memory_manager::lookup_func(pair<ulong, ulong> address) {
    auto frame = frames[address.first];
    auto pointer = frame->stack[access_address(address)];
    if (pointer.first == stack_type::FUNC_PTR) {
        return reinterpret_cast<func_obj *>(pointer.second);
    }
    throw runtime_error("lookup_func, expected FUNC_PTR, got " + to_string(static_cast<int>(pointer.first)));
}

void memory_manager::push(stack_type type, uint64_t value) {
    current_frame->push(type, value);
}

void memory_manager::push_int(int64_t value) {
    current_frame->push(stack_type::INT, static_cast<uint64_t>(value));
}

int64_t memory_manager::pop_int() {
    auto element = pop_value();
    if (element.first == stack_type::BOOL || element.first == stack_type::INT) {
        return static_cast<int64_t>(element.second);
    }
    throw runtime_error("pop_int() got " + to_string(static_cast<int>(element.first)));
}

pair<stack_type, uint64_t> memory_manager::pop_value() {
    return dereference(current_frame->pop());
}

void memory_manager::relocate_last() {
    auto value = dereference(frames[current_depth - 2]->pop());
    push(value.first, value.second);
}

void memory_manager::assert_last(stack_type type) {
    auto element = dereference(current_frame->peek_back());
    if (element.first != type) {
        throw runtime_error("assert_last expected " + to_string(static_cast<int>(type)) + " but got" +
                            to_string(static_cast<int>(element.first)));
    }
}

pair<stack_type, uint64_t> memory_manager::dereference(pair<stack_type, uint64_t> element) {
    for (;;) {
        if (element.first == stack_type::PTR) {
            auto pair = reinterpret_cast<::pair<ulong, ulong>*>(element.second);
            auto value = get_frame(*pair)->stack[pair->second];
            delete pair;
            return value;
        } else {
            return element;
        }
    }
}

void memory_manager::free_all() {
    while (current_depth) {
        auto frame = frames[--current_depth];
        delete frame;
    }
}
