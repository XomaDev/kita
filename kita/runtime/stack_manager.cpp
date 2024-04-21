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
    if (element.first != expect_type) {
        throw runtime_error("Expected " + to_string(static_cast<int>(expect_type)) +
                            ", but got " + to_string(static_cast<int>(element.first)));
    }
    return element.second;
}

pair<stack_type, uint64_t> stack_manager::dereference(pair<stack_type, uint64_t> stack_element) {
    for (;;) {
        if (stack_element.first == stack_type::PTR) {
            // points to another value on the stack_manager
            stack_element = main_stack[stack_element.second];
        } else {
            return stack_element;
        }
    }
}

void stack_manager::push_int(int64_t n) {
    push(stack_type::INT, static_cast<uint64_t>(n));
}

void stack_manager::push(stack_type type, uint64_t n) {
    main_stack.emplace_back(type, n);
    stack_index++;
}

int64_t stack_manager::pop_int() {
    auto element = pop_value();
    if (element.first == stack_type::INT) {
        return static_cast<int64_t>(element.second);
    }
    throw runtime_error("Expected stack_type:INT, got " + to_string(static_cast<int>(element.first)));
}

pair<stack_type, uint64_t> stack_manager::pop_value() {
    return dereference(pop());
}

pair<stack_type, uint64_t> stack_manager::pop() {
    auto element = main_stack.back();
    main_stack.pop_back();
    stack_index--;
    return element;
}
