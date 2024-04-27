//
// Created by kumaraswamy on 4/24/24.
//

#include <stdexcept>
#include <iostream>
#include "memory_manager.h"
#include "address.h"

void memory_manager::push_frame() {
    current_frame = new stack_frame(current_depth++);
    frames.emplace_back(current_frame);
}

void memory_manager::release_frame() {
    auto frame = frames.back();
    free_frame_content(frame);
    delete frame;
    frames.pop_back();
    current_depth--;
    current_frame = frames.back();
}

void memory_manager::move_address() {
    return current_frame->move_address();
}

void memory_manager::access_address(address addr) {
    auto frame_index = addr.static_definition ? addr.scope : current_depth - addr.scope - 1;
    auto result = frames[frame_index]->access_address(addr.index);
    auto dereferenced = frames[frame_index]->stack[result];
    push(dereferenced.first, dereferenced.second);
}

ulong memory_manager::lookup_func(address address) {
    auto frame = frames[address.scope];
    auto pointer = frame->stack[address.index];
    if (pointer.first == stack_type::FUNC_PTR) {
        return pointer.second;
    }
    throw runtime_error("lookup_func, looked in all depths, cannot find address " + address.print());
}

void memory_manager::push(stack_type type, uint64_t value) {
    current_frame->push(type, value);
}

void memory_manager::push_int(int64_t value) {
    current_frame->push(stack_type::INT, static_cast<uint64_t>(value));
}

int64_t memory_manager::pop_int() {
    auto element = current_frame->pop();
    if (element.first == stack_type::BOOL || element.first == stack_type::INT) {
        return static_cast<int64_t>(element.second);
    }
    throw runtime_error("pop_int() got " + to_string(static_cast<int>(element.first)));
}

pair<stack_type, uint64_t> memory_manager::pop() {
    return current_frame->pop();
}

void memory_manager::relocate_last() {
    auto value = frames[current_depth - 2]->pop();
    push(value.first, value.second);
}

void memory_manager::assert_last(stack_type type) {
    auto element = current_frame->peek_back();
    if (element.first != type) {
        throw runtime_error("assert_last expected " + to_string(static_cast<int>(type)) + " but got" +
                            to_string(static_cast<int>(element.first)));
    }
}

void memory_manager::free_frame_content(stack_frame *pFrame) {
    while (pFrame->stack_length) {
        auto popped = pFrame->stack[--pFrame->stack_length];
        if (popped.first == stack_type::STRING) {
            auto chars = reinterpret_cast<const char*>(popped.second);
            delete[] chars;
        } else if (popped.first == stack_type::FUNC_PTR) {
            auto func_object = reinterpret_cast<class func_obj*>(popped.second);
            delete func_object;
        }
    }
}

void memory_manager::free_all() {
    while (current_depth) {
        auto frame = frames[--current_depth];
        delete frame;
    }
}
