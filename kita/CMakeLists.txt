cmake_minimum_required(VERSION 3.27)
project(kita)

set(CMAKE_CXX_STANDARD 17)

add_executable(kita main.cpp
        compiler/tokenizer.cpp
        compiler/tokenizer.h
        compiler/token.cpp
        compiler/token.h
        compiler/kita_type.h
        compiler/parser.cpp
        compiler/parser.h
        expr/expr_base.h
        expr/expr_token.h
        expr/expr_binary.h
        expr/expr_type.h
        expr/expr_invoke.h
        expr/expr_func.h
        expr/expr_group.h
        expr/expr_inlineif.h
        compiler/bytecode.h
        compiler/dump.cpp
        compiler/dump.h
        compiler/dump.cpp
)
