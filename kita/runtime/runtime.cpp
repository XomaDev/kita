//
// Created by kumaraswamy on 4/20/24.
//

#include <stdexcept>
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
            stack.push_int(value);
            break;
        }
        case bytecode::NAME_TYPE: {
            string name = read_name();
            stack.push(stack_type::PTR, stack.access_addr(name));
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
            int r = stack.pop_int(), l = stack.pop_int();
            stack.push_int(l + r);
            break;
        }
        case bytecode::NEG: {
            int r = stack.pop_int(), l = stack.pop_int();
            stack.push_int(l - r);
            break;
        }
        case bytecode::MUL: {
            int r = stack.pop_int(), l = stack.pop_int();
            stack.push_int(l * r);
            break;
        }
        case bytecode::DIV: {
            int r = stack.pop_int(), l = stack.pop_int();
            stack.push_int(l / r);
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
                cout << to_string(stack.pop_int()) << endl;
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
            stack.assert_last_stack(stack_type::INT);
            stack.move_addr(name, false);
            break;
        }
        default: {
            throw runtime_error("Unknown class type " + to_string(class_type));
        }
    }
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
