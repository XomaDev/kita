//
// Created by kumaraswamy on 4/20/24.
//

#include <stdexcept>
#include <array>
#include <iostream>
#include "runtime.h"
#include "../compiler/bytecode.h"
#include "stack_type.h"

void runtime::run() {
    while (!isEOF()) {
        exec_next();
    }
}

void runtime::exec_next() {
    auto byte_code = advance();
    switch (static_cast<bytecode>(byte_code)) {
        case bytecode::LOAD:
            load();
            break;
        case bytecode::BINARY_OP:
            binary_operation();
            break;
        case bytecode::INVOKE:
            invoke();
            break;
        case bytecode::DECLARE:
            declare();
            break;
        default: {
            throw runtime_error("Unknown bytecode " + to_string(byte_code));
        }
    }
}

void runtime::load() {
    auto type = advance();
    switch (static_cast<bytecode>(type)) {
        case bytecode::INT_TYPE: {
            int value = advance() & 255 |
                        (advance() & 255) << 8 |
                        (advance() & 255) << 16 |
                        (advance() & 255) << 24;
            push_int(value);
            break;
        }
        case bytecode::NAME_TYPE: {
            string name = read_name();
            push(stack_type::PTR, access_addr(name));
            break;
        }
        default: {
            throw runtime_error("Unknown value type " + to_string(type));
        }
    }
}

void runtime::binary_operation() {
    auto binary_op = advance();
    switch (static_cast<bytecode>(binary_op)) {
        case bytecode::ADD: {
            int r = pop_int(), l = pop_int();
            push_int(l + r);
            break;
        }
        case bytecode::NEG: {
            int r = pop_int(), l = pop_int();
            push_int(l - r);
            break;
        }
        case bytecode::MUL: {
            int r = pop_int(), l = pop_int();
            push_int(l * r);
            break;
        }
        case bytecode::DIV: {
            int r = pop_int(), l = pop_int();
            push_int(l / r);
            break;
        }
        default: {
            throw runtime_error("Unknown binary operator " + to_string(binary_op));
        }
    }
}

void runtime::invoke() {
    int num_args = advance();
    int method = advance();
    switch (static_cast<bytecode>(method)) {
        case bytecode::DISP: {
            for (int i = 0; i < num_args; i++) {
                cout << to_string(pop_int()) << endl;
            }
            break;
        }
        default: {
            throw runtime_error("Unknown method code " + to_string(method));
        }
    }
}

void runtime::declare() {
    int class_type = advance();
    string name = read_name();

    switch (static_cast<bytecode>(class_type)) {
        case bytecode::INT_CLASS: {
            assert_last_stack(stack_type::INT);
            move_addr(name, false);
            break;
        }
        default: {
            throw runtime_error("Unknown class type " + to_string(class_type));
        }
    }
}

void runtime::move_addr(const string& name, bool overwrite) {
    auto find = addr_map.find(name);
    if (overwrite || find == addr_map.end()) {
        addr_map[name] = stack_index - 1;
        cout << "MoveAddr: " << name << " | " << stack_index << endl;
    } else {
        throw runtime_error("Addr already exists '" + name + "'");
    }
}

uint runtime::access_addr(const string& name) {
    auto find = addr_map.find(name);
    if (find != addr_map.end()) {
        return find->second;
    }
    throw runtime_error("Cannot access address '" + name + "'");
}

uint runtime::assert_last_stack(stack_type expect_type) {
    auto element = dereference(main_stack.back());
    auto type = static_cast<stack_type>(element[0]);
    if (type != expect_type) {
        throw runtime_error("Expected " + to_string(static_cast<int>(expect_type)) +
                ", but got " + to_string(element[0]));
    }
    return element[1];
}

void runtime::push_int(int n) {
    push(stack_type::INT, static_cast<uint>(n));
}

void runtime::push(stack_type type, uint n) {
    main_stack.push_back({ static_cast<uint>(type), n });
    stack_index++;
}

int runtime::pop_int() {
    auto element = pop();
    auto deref = dereference(element);
    if (static_cast<stack_type>(deref[0]) == stack_type::INT) {
        return static_cast<int>(deref[1]);
    }
    throw runtime_error("Expected stacktype::INT, got " + to_string(deref[0]));
}

array<uint, 2> runtime::dereference(array<uint, 2> &stack_element) {
    for (;;) {
        auto type = static_cast<stack_type>(static_cast<int>(stack_element[0]));
        if (type == stack_type::PTR) {
            // points to another value on the stack
            stack_element = main_stack[stack_element[1]];
        } else {
            return stack_element;
        }
    }
}

array<uint, 2> runtime::pop() {
    auto element = main_stack.back();
    main_stack.pop_back();
    stack_index--;
    return element;
}

bool runtime::isEOF() const {
    return index == length;
}

string runtime::read_name() {
    uchar name_length = advance();
    string name(reinterpret_cast<const char *>(&bytes[index]), name_length);
    index += name_length;
    return name;
}

uchar runtime::advance() {
    return bytes[index++];
}
