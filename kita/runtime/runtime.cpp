//
// Created by kumaraswamy on 4/20/24.
//

#include <stdexcept>
#include <iostream>
#include <cstring>
#include "runtime.h"
#include "stack_type.h"
#include "func_obj.h"

void runtime::run() {
    while (!isEOF()) {
        auto code = exec_next();
        if (code != 0) {
            if (code == 2) {
                // pop last value and display it here
                cout << "Program exited with value " << last_call_result.second << endl;
                break;
            }
//            if (last_call_markup != -1) {
//                break;
//            } else {
//                index = last_call_markup;
//            }
        }
    }
    free_memory();
}

int runtime::exec_next() {
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
            return if_decl();
        case bytecode::RETURN:
            return return_decl();
        case bytecode::FUNC:
            func_decl();
            break;
        default: {
            throw runtime_error("Unknown bytecode " + to_string(byte_code));
        }
    }
    return 0;
}

int runtime::return_decl() {
    auto result_code = advance() == 1 ? 1 : 2;
    if (result_code == 2) {
        // something has been returned
        last_call_result = stack.pop_value();
    }
    return result_code;
}

void runtime::func_decl() {
    auto func_name = read_string();
    auto return_type = advance_bytecode();
    auto args_size = static_cast<uint8_t>(advance());
    vector<pair<bytecode, string>> parameters;
    for (uint8_t i = 0; i < args_size; i++) {
        auto arg_class = advance_bytecode();
        auto arg_name = read_string();
        parameters.emplace_back(arg_class, arg_name);
    }
    long func_index = index;
    pass_scope();
    auto func = new func_obj(func_name, return_type, args_size, parameters, func_index);
    stack.push(stack_type::FUNC_PTR, reinterpret_cast<uint64_t>(func));
    stack.move_addr("func@" + func_name, false);
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
            int value = read_int32();
            stack.push_int(value);
            break;
        }
        case bytecode::NAME_TYPE: {
            string name = read_string();
            stack.push(stack_type::PTR, stack.access_addr("var@" + name));
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
    auto concatenated = (new string(element_to_string(right) + element_to_string(left)))->c_str();
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
    auto num_args = static_cast<uint8_t>(advance());
    int method = advance();
    switch (static_cast<bytecode>(method)) {
        case bytecode::DISP: {
            for (int i = 0; i < num_args; i++) {
                auto value = stack.pop_value();
                cout << element_to_string(value) << endl;
            }
            break;
        }
        case bytecode::SCOPE: {
            stack.push_int(stack.stack_depth);
            break;
        }
        case bytecode::NAME_TYPE:
            func_invoke(num_args);
            break;
        default: {
            throw runtime_error("Unknown method code " + to_string(method));
        }
    }
}

void runtime::func_invoke(int num_args) {
    auto name = read_string();
    auto func_obj = stack.lookup_func("func@" + name);
    func_obj->prepare(num_args, &stack);

    auto last_call_markup = index;
    index = func_obj->index;
    auto result_code = evaluate_scope();

    stack.free_stack(num_args);
    if (result_code == 2) {
        stack.push(last_call_result.first, last_call_result.second);
    }
    index = last_call_markup;
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
    stack.move_addr("var@" + name, false);
}

int runtime::if_decl() {
    auto eq_value = stack.pop_value();
    if (eq_value.first != stack_type::BOOL) {
        throw runtime_error("If (*non-bool*expr) found");
    }
    bool has_else_branch = advance() == 1;

    if (eq_value.second == 1) {
        auto code = evaluate_scope(); // evaluate *body*
        if (code != 0) return code;
        if (has_else_branch) pass_scope(); // skip else *body*
    } else {
        pass_scope(); // skip *body*
        if (has_else_branch) {
            // evaluate else *body*;
            auto code = evaluate_scope();
            if (code != 0) return code;
        }
    }
    return 0;
}

int runtime::evaluate_scope() {
    int scope_size = read_int32();

    expect(bytecode::SCOPE_START);
    auto stack_len_before = stack.stack_length;
    stack.stack_depth++;

    auto result_code = 0;
    for (;;) {
        auto code = exec_next();
        if (code != 0) {
            result_code = code;
            break;
        }
        if (static_cast<bytecode>(peek()) == bytecode::SCOPE_END) {
            index++;
            break;
        }
    }
    stack.stack_depth--;
    auto stack_len_after = stack.stack_length;
    if (stack_len_before < stack_len_after) {
        stack.free_stack(stack_len_after - stack_len_before);
    }
    return result_code;
}

void runtime::pass_scope() {
    int scope_size = read_int32();
    expect(bytecode::SCOPE_START);
    index += scope_size;
    expect(bytecode::SCOPE_END);
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

int runtime::read_int32() {
    return advance() & 255 |
           (advance() & 255) << 8 |
           (advance() & 255) << 16 |
           (advance() & 255) << 24;;
}

void runtime::expect(bytecode code) {
    auto next_bytecode = advance();
    auto expected = static_cast<uchar>(code);
    if (next_bytecode != expected) {
        throw runtime_error("Expected " + to_string(expected) + ", but got " + to_string(next_bytecode));
    }
}

bytecode runtime::advance_bytecode() {
    return static_cast<bytecode>(advance());
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
