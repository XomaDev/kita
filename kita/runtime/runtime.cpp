//
// Created by kumaraswamy on 4/20/24.
//

#include <stdexcept>
#include <iostream>
#include "runtime.h"
#include "../compiler/bytecode.h"

void runtime::run() {
    while (!isEOF()) {
        exec_next();
    }
}

void runtime::exec_next() {
    auto byte_code = next();
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
        default: {
            throw runtime_error("Unknown bytecode " + to_string(byte_code));
        }
    }
}

void runtime::load() {
    auto type = next();
    switch (static_cast<bytecode>(type)) {
        case bytecode::INT_TYPE: {
            int value = next() & 255 |
                        (next() & 255) << 8 |
                        (next() & 255) << 16 |
                        (next() & 255) << 24;
            push(value);
            break;
        }
        case bytecode::INVOKE: {
            invoke();
        }
        default: {
            throw runtime_error("Unknown value type " + to_string(type));
        }
    }
}

void runtime::binary_operation() {
    auto binary_op = next();
    switch (static_cast<bytecode>(binary_op)) {
        case bytecode::ADD: {
            int r = pop(), l = pop();
            push(l + r);
            break;
        }
        case bytecode::NEG: {
            int r = pop(), l = pop();
            push(l - r);
            break;
        }
        case bytecode::MUL: {
            int r = pop(), l = pop();
            push(l * r);
            break;
        }
        case bytecode::DIV: {
            int r = pop(), l = pop();
            push(l / r);
            break;
        }
        default: {
            throw runtime_error("Unknown binary operator " + to_string(binary_op));
        }
    }
}

void runtime::invoke() {
    int num_args = next();
    int method = next();
    switch (static_cast<bytecode>(method)) {
        case bytecode::DISP: {
            for (int i = 0; i < num_args; i++) {
                cout << to_string(pop()) << endl;
            }
            break;
        }
        default: {
            throw runtime_error("Unknown method code " + to_string(method));
        }
    }
}

int runtime::back_stack(int n) {
    return main_stack[stack_index - 1 - n];
}

void runtime::push(int n) {
    cout << "Push: " << to_string(n) << endl;
    main_stack.emplace_back(n);
    stack_index++;
}

int runtime::pop() {
    int value = main_stack.back();
    main_stack.pop_back();
    return value;
}

bool runtime::isEOF() {
    return index == length;
}

uchar runtime::next() {
    return bytes[index++];
}
uchar runtime::peek() {
    return bytes[index++];
}
