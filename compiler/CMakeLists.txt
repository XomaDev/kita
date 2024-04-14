cmake_minimum_required(VERSION 3.27)
project(kita)

set(CMAKE_CXX_STANDARD 17)

add_executable(kita main.cpp
        tokenizer.cpp
        tokenizer.h
        token.cpp
        token.h
        kita_type.h
        parser.cpp
        parser.h
        expr/expr_base.h
        expr/expr_token.h
        expr/expr_binary.h
        expr/expr_type.h
        expr/expr_invoke.h
)
