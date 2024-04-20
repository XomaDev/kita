//
// Created by kumaraswamy on 4/20/24.
//

#ifndef KITA_BYTECODE_H
#define KITA_BYTECODE_H

enum class bytecode {
    IntType = 0,

    LOAD = 11,
    INVOKE = 12,

    ADD = 21,
    NEG = 22,
    MUL = 23,
    DIV = 24,

    DISP = 40,
};

#endif //KITA_BYTECODE_H
