//
// Created by kumaraswamy on 4/20/24.
//

#include <stdexcept>
#include <iostream>
#include <cstring>
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
        case bytecode::BOOL_TYPE: {
            bool value = advance() == 1;
            stack.push(stack_type::BOOL, value);
            break;
        }
        case bytecode::INT_TYPE: {
            int value = advance() & 255 |
                        (advance() & 255) << 8 |
                        (advance() & 255) << 16 |
                        (advance() & 255) << 24;
            stack.push_int(value);
            break;
        }
        case bytecode::NAME_TYPE: {
            string name = read_string();
            stack.push(stack_type::PTR, stack.access_addr(name));
            break;
        }
        case bytecode::STRING_TYPE: {
            string content = read_string();
            char* chars = new char[content.size() + 1];
            strcpy(chars, content.c_str());

            stack.push(stack_type::STRING, reinterpret_cast<uint64_t>(chars));
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
            binary_addition();
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

void runtime::binary_addition() {
    // not just limited to Int + Int, includes String too
    auto right = stack.pop_value(), left = stack.pop_value();
    auto right_type = static_cast<stack_type>(left[0]), left_type = static_cast<stack_type>(right[0]);
    if (left_type == stack_type::INT && right_type == stack_type::INT) {
        stack.push_int(static_cast<int>(left[1]) + static_cast<int>(right[1]));
        return;
    }
    const char* left_string = element_to_string(left);
    const char* right_string = element_to_string(right);

    auto left_len = strlen(left_string), right_len = strlen(right_string);
    char* concatenated = new char[left_len + right_len + 1]; // +1 for nullptr

    strcpy(concatenated, left_string);
    strcat(concatenated, right_string);

    stack.push(stack_type::STRING, reinterpret_cast<uint64_t>(concatenated));
}

const char* runtime::element_to_string(array<uint64_t, 2> element) {
    auto type = static_cast<stack_type>(element[0]);
    if (type == stack_type::STRING) {
        return reinterpret_cast<const char*>(element[1]);
    } else if (type == stack_type::BOOL || type == stack_type::INT) {
        return to_string(static_cast<int>(element[1])).c_str();
    }
    throw runtime_error("Unknown left operand type " + to_string(element[0]));
}

void runtime::invoke() {
    int num_args = advance();
    int method = advance();
    switch (static_cast<bytecode>(method)) {
        case bytecode::DISP: {
            for (int i = 0; i < num_args; i++) {
                auto value = stack.pop_value();
                cout << element_to_string(value) << endl;
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
    string name = read_string();

    switch (static_cast<bytecode>(class_type)) {
        case bytecode::BOOL_CLASS: {
            stack.assert_last_stack(stack_type::BOOL);
            break;
        }
        case bytecode::INT_CLASS: {
            stack.assert_last_stack(stack_type::INT);
            break;
        }
        case bytecode::STRING_CLASS: {
            stack.assert_last_stack(stack_type::STRING);
            break;
        }
        default: {
            throw runtime_error("Unknown class type " + to_string(class_type));
        }
    }
    stack.move_addr(name, false);
}

bool runtime::isEOF() const {
    return index == length;
}

string runtime::read_string() {
    uchar name_length = advance();
    string name(reinterpret_cast<const char *>(&bytes[index]), name_length);
    index += name_length;
    return name;
}

uchar runtime::advance() {
    return bytes[index++];
}
