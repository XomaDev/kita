//
// Created by kumaraswamy on 4/20/24.
//

#include <stdexcept>
#include <iostream>
#include <cstring>
#include <optional>
#include "runtime.h"
#include "stack_type.h"
#include "func_obj.h"
#include "visitable.h"

void runtime::prepare() {
    while (!isEOF()) {
        auto next = encapsule_next();
        if (next) {
            instructions.emplace_back(next.value());
        }
    }
}

void runtime::run() {
    auto instructions_size = instructions.size();
    for (; instruction_index < instructions_size; instruction_index++) {
        auto code = instructions[instruction_index].execute();
        if (code != 0) {
            if (code == 2) {
                // pop last value and display it here
                cout << "Program exited with value " << last_call_result.second << endl;
                break;
            }
        }
    }
    free_memory();
}

optional<visitable> runtime::encapsule_next() {
    auto byte_code = advance();
    switch (static_cast<bytecode>(byte_code)) {
        case bytecode::LOAD:
            return load();
        case bytecode::BINARY_OP:
            return binary_operation();
        case bytecode::INVOKE:
            return invoke();
        case bytecode::DECLARE:
            return declare();
        case bytecode::IF:
            return if_decl();
        case bytecode::RETURN:
            return return_decl();
        case bytecode::FUNC:
            return func_decl();
        default: {
            throw runtime_error("Unknown bytecode " + to_string(byte_code));
        }
    }
}

visitable runtime::return_decl() {
    auto result_code = advance() == 1 ? 1 : 2;
    return visitable {
        [result_code, this]() {
            if (result_code == 2) {
                // something has been returned
                last_call_result = memory.pop();
            }
            return result_code;
        }
    };
}

visitable runtime::func_decl() {
    auto func_name = read_string();
    auto return_type = advance_bytecode();
    auto args_size = static_cast<uint8_t>(advance());

    resolver.declare("func@" + func_name);
    resolver.enter_frame();
    for (uint8_t i = 0; i < args_size; i++) {
        auto name = read_string();
        resolver.declare("var@" + name);
    }
    auto func_body = encapsule_scope(false);
    ondemand_visitables.emplace_back(func_body);
    auto func = new func_obj(func_name, return_type, args_size, ondemand_index++);

    return visitable {
        [func, this]() {
            memory.push(stack_type::FUNC_PTR, reinterpret_cast<uint64_t>(func));
            memory.move_address();
            return 0;
        }
    };
}

visitable runtime::load() {
    auto type = advance();
    switch (static_cast<bytecode>(type)) {
        case bytecode::BOOL_TYPE: {
            bool value = advance() == 1;
            return visitable {
                [value, this]() {
                    memory.push(stack_type::BOOL, value);
                    return 0;
                }
            };
        }
        case bytecode::INT_TYPE: {
            int value = read_int32();
            return visitable {
                [value, this]() {
                    memory.push_int(value);
                    return 0;
                },
            };
        }
        case bytecode::NAME_TYPE: {
            string name = read_string();
            auto address = resolver.resolve("var@" + name, false);
            return visitable {
                [address, this]() {
                    memory.access_address(address);
                    return 0;
                }
            };
        }
        case bytecode::STRING_TYPE: {
            string content = read_string();
            char* chars = new char[content.size() + 1];
            strcpy(chars, content.c_str());

            return visitable {
                [chars, this]() {
                    memory.push(stack_type::STRING, reinterpret_cast<uint64_t>(chars));
                    return 0;
                }
            };
        }
        default: {
            throw runtime_error("Unknown value type " + to_string(type));
        }
    }
}

visitable runtime::binary_operation() {
    auto binary_op = advance();

    switch (static_cast<bytecode>(binary_op)) {
        case bytecode::ADD: {
            return binary_addition();
        }
        case bytecode::NEG: {
            return visitable {
                [this]() {
                    auto r = memory.pop_int(), l = memory.pop_int();
                    memory.push_int(l - r);
                    return 0;
                }
            };
        }
        case bytecode::MUL: {
            return visitable {
                [this]() {
                    auto r = memory.pop_int(), l = memory.pop_int();
                    memory.push_int(l * r);
                    return 0;
                }
            };
        }
        case bytecode::DIV: {
            return visitable {
                [this]() {
                    auto r = memory.pop_int(), l = memory.pop_int();
                    memory.push_int(l / r);
                    return 0;
                }
            };
        }
        case bytecode::BITWISE_AND: {
            return visitable {
                [this]() {
                    auto r = memory.pop_int(), l = memory.pop_int();
                    memory.push_int(l & r);
                    return 0;
                }
            };
        }
        case bytecode::BITWISE_OR: {
            return visitable {
                [this]() {
                    auto r = memory.pop_int(), l = memory.pop_int();
                    memory.push_int(l | r);
                    return 0;
                }
            };
        }
        case bytecode::LOGICAL_AND: {
            return visitable {
                [this]() {
                    auto r = memory.pop_int(), l = memory.pop_int();
                    memory.push(stack_type::BOOL, l && r);
                    return 0;
                }
            };
        }
        case bytecode::LOGICAL_OR: {
            return visitable {
                [this]() {
                    auto r = memory.pop_int(), l = memory.pop_int();
                    memory.push(stack_type::BOOL, l || r);
                    return 0;
                }
            };
        }
        case bytecode::EQUALS: {
            return visitable {
                [this]() {
                    memory.push(stack_type::BOOL, binary_equals());
                    return 0;
                }
            };
        }
        case bytecode::NOT_EQUALS: {
            return visitable {
                [this]() {
                    memory.push(stack_type::BOOL, !binary_equals());
                    return 0;
                }
            };
        }
        case bytecode::GREATER_THAN: {
            return visitable {
                [this]() {
                    auto r = memory.pop_int(), l = memory.pop_int();
                    memory.push(stack_type::BOOL, l > r);
                    return 0;
                }
            };
        }
        case bytecode::LESSER_THAN: {
            return visitable {
                [this]() {
                    auto r = memory.pop_int(), l = memory.pop_int();
                    memory.push(stack_type::BOOL, l < r);
                    return 0;
                }
            };
        }
        case bytecode::GREATER_EQUALS: {
            return visitable {
                [this]() {
                    auto r = memory.pop_int(), l = memory.pop_int();
                    memory.push(stack_type::BOOL, l >= r);
                    return 0;
                }
            };
        }
        case bytecode::LESSER_EQUALS: {
            return visitable {
                [this]() {
                    auto r = memory.pop_int(), l = memory.pop_int();
                    memory.push(stack_type::BOOL, l <= r);
                    return 0;
                }
            };
        }
        default: {
            throw runtime_error("Unknown binary operator " + to_string(binary_op));
        }
    }
}

bool runtime::binary_equals() {
    auto r = memory.pop(), l = memory.pop();
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

visitable runtime::binary_addition() {
    return visitable {
        [this]() {
            // not just limited to Int + Int, includes String too
            auto right = memory.pop(), left = memory.pop();
            if (left.first == stack_type::INT && right.first == stack_type::INT) {
                memory.push_int(static_cast<int64_t>(left.second) + static_cast<int>(right.second));
                return 0;
            }
            auto concatenated = (new string(element_to_string(left) + element_to_string(right)))->c_str();
            memory.push(stack_type::STRING, reinterpret_cast<uint64_t>(concatenated));
            return 0;
        }
    };
}

string runtime::element_to_string(pair<stack_type, uint64_t> element) {
    if (element.first == stack_type::STRING) {
        return { reinterpret_cast<const char*>(element.second) };
    } else if (element.first == stack_type::BOOL || element.first == stack_type::INT) {
        return to_string(element.second);
    }
    throw runtime_error("Unknown left operand type " + to_string(static_cast<int>(element.first)));
}

visitable runtime::invoke() {
    auto num_args = static_cast<uint8_t>(advance());
    int method = advance();
    switch (static_cast<bytecode>(method)) {
        case bytecode::NAME_TYPE:
            return func_invoke(num_args);
        case bytecode::DISP: {
            return visitable {
                [num_args, this]() {
                    for (int i = 0; i < num_args; i++) {
                        auto value = memory.pop();
                        cout << element_to_string(value) << endl;
                    }
                    return 0;
                }
            };
        }
        default: {
            throw runtime_error("Unknown method code " + to_string(method));
        }
    }
}

visitable runtime::func_invoke(int num_args) {
    auto name = read_string();
    auto address = resolver.resolve("func@" + name, true);
    return visitable {
        [num_args, address, this]() {
            auto func_obj = memory.lookup_func(address);
            func_obj->prepare(num_args, &memory);

            auto func_body = ondemand_visitables[func_obj->visitable_index];
            auto result_code = func_body.execute();
            if (result_code == 2) {
                memory.push(last_call_result.first, last_call_result.second);
            }
            return 0;
        }
    };
}

visitable runtime::declare() {
    int class_type = advance();
    string name = read_string();

    stack_type type;
    switch (static_cast<bytecode>(class_type)) {
        case bytecode::BOOL_CLASS: {
            type = stack_type::BOOL;
            break;
        }
        case bytecode::INT_CLASS: {
            type = stack_type::INT;
            break;
        }
        case bytecode::STRING_CLASS: {
            type = stack_type::STRING;
            break;
        }
        default: {
            throw runtime_error("Unknown class type " + to_string(class_type));
        }
    }
    resolver.declare("var@" + name);
    return visitable {
        [type, this]() {
            memory.assert_last(type);
            memory.move_address();
            return 0;
        }
    };
}

visitable runtime::if_decl() {
    bool has_else = advance() == 1;

    auto if_body = encapsule_scope(true);
    visitable else_body;

    if (has_else) {
        else_body = encapsule_scope(true);
    }
    return visitable {
        [has_else, if_body, else_body, this]() {
            auto eq_value = memory.pop();
            if (eq_value.first != stack_type::BOOL) {
                throw runtime_error("If (*non-bool*expr) found");
            }
            if (eq_value.second) {
                return if_body.execute();
            } else if (has_else) {
                return else_body.execute();
            }
            return 0;
        }
    };
}

visitable runtime::encapsule_scope(bool push_frame) {
    vector<visitable> local_instructions;
    expect(bytecode::SCOPE_START);
    if (push_frame) {
        resolver.enter_frame();
    }
    for (;;) {
        auto next = encapsule_next();
        if (next) {
            local_instructions.emplace_back(next.value());
        }
        if (static_cast<bytecode>(peek()) == bytecode::SCOPE_END) {
            index++;
            break;
        }
    }
    resolver.exit_fame();
    return visitable {
        [push_frame, local_instructions, this]() {
            if (push_frame) {
                memory.push_frame();
            }
            auto result_code = 0;
            for (const auto& instruction : local_instructions) {
                auto code = instruction.execute();
                if (code != 0) {
                    result_code = code;
                    break;
                }
            }
            memory.release_frame();
            return result_code;
        }
    };
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
    memory.free_all();
}
