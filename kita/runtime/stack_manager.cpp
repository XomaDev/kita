//
// Created by kumaraswamy on 4/21/24.
//

#include <iostream>
#include "stack_manager.h"
#include "stack_type.h"
#include "func.h"

using namespace std;

void stack_manager::move_addr(const string &name, bool overwrite) {
    move_addr_depth(stack_depth, name, overwrite);
}

void stack_manager::move_addr_depth(uint64_t depth, const string &name, bool overwrite) {
    auto store_name = to_string(depth) + "@" + name;
    auto find = addr_map.find(store_name);
    if (overwrite || find == addr_map.end()) {
        addr_map[store_name] = stack_length - 1;
        rev_addr_map[stack_length - 1] = store_name;
    } else {
        throw runtime_error("Address already exists: " + store_name);
    }
}

uint64_t stack_manager::access_addr(const string& name) {
    auto search_depth = stack_length;
    for (;;) {
        auto store_name = to_string(search_depth) + "@" + name;
        auto find = addr_map.find(store_name);
        if (find != addr_map.end()) {
            return find->second;
        } else if (search_depth == 0) {
            throw runtime_error("Cannot access address '" + name);
        }
        search_depth--;
    }
}

class func* stack_manager::access_func(const string& name) {
    auto fun_stack = main_stack[access_addr(name)];
    if (fun_stack.first == stack_type::FUNC_PTR) {
        return reinterpret_cast<class func*>(fun_stack.second);
    }
    throw runtime_error("Expected stack_type::FUNC_PTR, got " + to_string(static_cast<int>(fun_stack.first)));
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
    stack_length++;
}

int64_t stack_manager::pop_int() {
    auto element = pop_value();
    if (element.first == stack_type::BOOL || element.first == stack_type::INT) {
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
    stack_length--;
    return element;
}

void stack_manager::free_stack(uint64_t last_n) {
    while (last_n != 0) {
        auto popped = pop();

        // find references pointing to this stack
        auto find = rev_addr_map.find(stack_length);
        if (find != rev_addr_map.end()) {
            auto addr_name = find->second;
            addr_map.erase(addr_name);
            rev_addr_map.erase(popped.second);
        }

        if (popped.first == stack_type::STRING) {
            // free dynamically allocated strings
            auto chars = reinterpret_cast<const char*>(popped.second);
            delete[] chars;
        } else if (popped.first == stack_type::FUNC_PTR) {
            auto func = reinterpret_cast<class func*>(popped.second);
            delete func;
        }
        last_n--;
    }
}
