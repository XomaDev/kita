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

void memory_manager::move_address(const string &name) {
    return current_frame->move_address(name);
}

pair<long, unsigned long>* memory_manager::access_address(const string &name) {
    long search_depth = current_depth;
    while (search_depth) {
        auto frame = frames[--search_depth];
        auto result = frame->access_address(name);
        if (result.first) {
            return new pair(search_depth, result.second);
        }
    }
    throw runtime_error("access_address, looked in all depths, cannot find address " + name);
}

func_obj* memory_manager::lookup_func(const string &name) {
    for (long i = 0; i < current_depth; i++) {
        auto frame = frames[i];
        auto result = frame->access_address(name);
        if (result.first) {
            auto pointer = frame->stack[result.second];
            if (pointer.first == stack_type::FUNC_PTR) {
                return reinterpret_cast<func_obj *>(pointer.second);
            }
        }
    }
    throw runtime_error("lookup_func, looked in all depths, cannot find function " + name);
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
            auto pair = reinterpret_cast<::pair<long, unsigned long>*>(element.second);
            auto depth = pair->first;
            auto stack_index = pair->second;
            return frames[depth]->stack[stack_index];
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
