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
                cout << to_string(pop()[1]) << endl;
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
        case bytecode::INT_CLASS:
            assert_last_stack(stack_type::INT);
            break;
        default: {
            throw runtime_error("Unknown class type " + to_string(class_type));
        }
    }

    cout << "Pop: " << to_string(pop()[1]) << endl;
}

void runtime::assert_last_stack(stack_type expect_type) {
    auto last_type = static_cast<stack_type>(main_stack.back()[0]);
    if (last_type != expect_type) {
        if (last_type == stack_type::PTR) {
            // this links to another value, access it, check again
        }
        throw runtime_error("Expected " + to_string(static_cast<int>(expect_type)) +
                ", but got " + to_string(static_cast<int>(last_type)));
    }
}

void runtime::push_int(int n) {
    push(stack_type::INT, n);
}

void runtime::push(stack_type type, int n) {
    main_stack.push_back({ static_cast<int>(type), n });
    stack_index++;
}

int runtime::pop_int() {
    auto element = pop();
    if (static_cast<stack_type>(element[0]) == stack_type::INT) {
        return element[1];
    }
    throw runtime_error("Expected IntStack, got " + to_string(element[0]));
}

array<int, 2> runtime::pop() {
    auto element = main_stack.back();
    main_stack.pop_back();
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
