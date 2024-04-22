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
    free_memory();
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
        case bytecode::IF:
            if_decl();
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
            auto r = stack.pop_int(), l = stack.pop_int();
            stack.push_int(l - r);
            break;
        }
        case bytecode::MUL: {
            auto r = stack.pop_int(), l = stack.pop_int();
            stack.push_int(l * r);
            break;
        }
        case bytecode::DIV: {
            auto r = stack.pop_int(), l = stack.pop_int();
            stack.push_int(l / r);
            break;
        }
        case bytecode::BITWISE_AND: {
            auto r = stack.pop_int(), l = stack.pop_int();
            stack.push_int(l & r);
            break;
        }
        case bytecode::BITWISE_OR: {
            auto r = stack.pop_int(), l = stack.pop_int();
            stack.push_int(l | r);
            break;
        }
        case bytecode::LOGICAL_AND: {
            auto r = stack.pop_int(), l = stack.pop_int();
            stack.push(stack_type::BOOL, l && r);
            break;
        }
        case bytecode::LOGICAL_OR: {
            auto r = stack.pop_int(), l = stack.pop_int();
            stack.push(stack_type::BOOL, l || r);
            break;
        }
        case bytecode::EQUALS: {
            stack.push(stack_type::BOOL, binary_equals());
            break;
        }
        case bytecode::NOT_EQUALS: {
            stack.push(stack_type::BOOL, !binary_equals());
            break;
        }
        case bytecode::GREATER_THAN: {
            auto r = stack.pop_int(), l = stack.pop_int();
            stack.push(stack_type::BOOL, l > r);
            break;
        }
        case bytecode::LESSER_THAN: {
            auto r = stack.pop_int(), l = stack.pop_int();
            stack.push(stack_type::BOOL, l < r);
            break;
        }
        case bytecode::GREATER_EQUALS: {
            auto r = stack.pop_int(), l = stack.pop_int();
            stack.push(stack_type::BOOL, l >= r);
            break;
        }
        case bytecode::LESSER_EQUALS: {
            auto r = stack.pop_int(), l = stack.pop_int();
            stack.push(stack_type::BOOL, l <= r);
            break;
        }
        default: {
            throw runtime_error("Unknown binary operator " + to_string(binary_op));
        }
    }
}

bool runtime::binary_equals() {
    auto r = stack.pop_value(), l = stack.pop_value();
    if (r.first != l.first) {
        // false if not same type
        return false;
    }
    if (r.second == l.second) {
        // true if points to same value
        return true;
    }
    if (r.first == stack_type::STRING && l.first == stack_type::STRING) {
        auto left = reinterpret_cast<const char*>(l.second);
        auto right = reinterpret_cast<const char*>(r.second);

        return strcmp(left, right) == 0;
    }
    return false;
}

void runtime::binary_addition() {
    // not just limited to Int + Int, includes String too
    auto right = stack.pop_value(), left = stack.pop_value();
    if (left.first == stack_type::INT && right.first == stack_type::INT) {
        stack.push_int(static_cast<int64_t>(left.second) + static_cast<int>(right.second));
        return;
    }
    auto concatenated = (new string(element_to_string(left) + element_to_string(right)))->c_str();
    stack.push(stack_type::STRING, reinterpret_cast<uint64_t>(concatenated));
}

string runtime::element_to_string(pair<stack_type, uint64_t> element) {
    if (element.first == stack_type::STRING) {
        return { reinterpret_cast<const char*>(element.second) };
    } else if (element.first == stack_type::BOOL || element.first == stack_type::INT) {
        return to_string(element.second);
    }
    throw runtime_error("Unknown left operand type " + to_string(static_cast<int>(element.first)));
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

void runtime::if_decl() {
    auto eq_value = stack.pop_value();
    if (eq_value.first != stack_type::BOOL) {
        throw runtime_error("If (*non-bool*expr) found");
    }
    bool has_else_branch = advance() == 1;

    if (eq_value.second == 1) {
        evaluate_scope(); // evaluate *body*
        if (has_else_branch) skip_scope(); // skip else *body*
    } else {
        skip_scope(); // skip *body*
        if (has_else_branch) evaluate_scope(); // evaluate else *body*;
    }
}

void runtime::evaluate_scope() {
    expect(bytecode::SCOPE_START);
    auto stack_len_before = stack.stack_length;
    for (;;) {
        exec_next();
        if (static_cast<bytecode>(peek()) == bytecode::SCOPE_END) {
            index++;
            break;
        }
    }
    auto stack_len_after = stack.stack_length;
    if (stack_len_before != stack_len_after) {
        // stack_len_after > stack_len_before

        // time to free up this stack
        stack.free_stack(stack_len_after - stack_len_before);
    }
}

void runtime::skip_scope() {
    expect(bytecode::SCOPE_START);

    uint num_inner_scopes = 0;
    for (;;) {
        auto next_bytecode = static_cast<bytecode>(advance());
        if (next_bytecode == bytecode::SCOPE_END) {
            if (num_inner_scopes == 0) {
                break;
            } else {
                num_inner_scopes--;
            }
        } else if (next_bytecode == bytecode::SCOPE_START) {
            num_inner_scopes++;
        }
    }
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

void runtime::expect(bytecode code) {
    auto next_bytecode = advance();
    auto expected = static_cast<uchar>(code);
    if (next_bytecode != expected) {
        throw runtime_error("Expected " + to_string(expected) + ", but got " + to_string(next_bytecode));
    }
}

uchar runtime::advance() {
    return bytes[index++];
}

uchar runtime::peek() {
    return bytes[index];
}

void runtime::free_memory() {
    // free all memory in stack
    stack.free_stack(stack.stack_length);
}
