//
// Created by kumaraswamy on 4/21/24.
//

#include <iostream>
#include "stack_manager.h"
#include "stack_type.h"

using namespace std;

void stack_manager::move_addr(const string& name, bool overwrite) {
    auto find = addr_map.find(name);
    if (overwrite || find == addr_map.end()) {
        addr_map[name] = stack_index - 1;
    } else {
        throw runtime_error("Addr already exists '" + name + "'");
    }
}

uint64_t stack_manager::access_addr(const string& name) {
    auto find = addr_map.find(name);
    if (find != addr_map.end()) {
        return find->second;
    }
    throw runtime_error("Cannot access address '" + name + "'");
}

uint64_t stack_manager::assert_last_stack(stack_type expect_type) {
    auto element = dereference(main_stack.back());
    auto type = static_cast<stack_type>(element[0]);
    if (type != expect_type) {
        throw runtime_error("Expected " + to_string(static_cast<int>(expect_type)) +
                            ", but got " + to_string(element[0]));
    }
    return element[1];
}

array<uint64_t, 2> stack_manager::dereference(array<uint64_t, 2> &stack_element) {
    for (;;) {
        auto type = static_cast<stack_type>(static_cast<int>(stack_element[0]));
        if (type == stack_type::PTR) {
            // points to another value on the stack_manager
            stack_element = main_stack[stack_element[1]];
        } else {
            return stack_element;
        }
    }
}

void stack_manager::push_int(int64_t n) {
    push(stack_type::INT, static_cast<uint64_t>(n));
}

void stack_manager::push(stack_type type, uint64_t n) {
    main_stack.push_back({ static_cast<uint64_t>(type), n });
    stack_index++;
}

int64_t stack_manager::pop_int() {
    auto element = pop_value();
    auto type = static_cast<stack_type>(element[0]);
    if (type == stack_type::INT) {
        return static_cast<int>(element[1]);
    }
    throw runtime_error("Expected stack_type:INT, got " + to_string(element[0]));
}

array<uint64_t, 2> stack_manager::pop_value() {
    auto element = pop();
    return dereference(element);
}

array<uint64_t, 2> stack_manager::pop() {
    auto element = main_stack.back();
    main_stack.pop_back();
    stack_index--;
    return element;
}
