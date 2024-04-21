//
// Created by kumaraswamy on 4/20/24.
//

#ifndef KITA_BYTECODE_H
#define KITA_BYTECODE_H

enum class bytecode {
    // value types
    BOOL_TYPE = 0,
    INT_TYPE = 1,
    NAME_TYPE = 2,
    STRING_TYPE = 3,

    // instruction types
    LOAD = 11,
    BINARY_OP = 12,
    INVOKE = 13,
    DECLARE = 14,

    // declaration class types
    BOOL_CLASS = 21,
    INT_CLASS = 22,
    STRING_CLASS = 23,

    // binary instructions
    ADD = 31,
    NEG = 32,
    MUL = 33,
    DIV = 34,

    LOGICAL_AND = 35,
    LOGICAL_OR = 36,
    BITWISE_AND = 37,
    BITWISE_OR = 38,
    EQUALS = 39,
    NOT_EQUALS = 40,
    GREATER_THAN = 41,
    LESSER_THAN = 42,
    GREATER_EQUALS = 43,
    LESSER_EQUALS = 44,


    DISP = 50,
};

#endif //KITA_BYTECODE_H
