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
        compiler/expr/expr_base.h
        compiler/expr/expr_token.h
        compiler/expr/expr_binary.h
        compiler/expr/expr_invoke.h
        compiler/expr/expr_func.h
        compiler/expr/expr_group.h
        compiler/expr/expr_inlineif.h
        bytecode.h
        compiler/dump.cpp
        compiler/dump.h
        compiler/dump.cpp
        runtime/runtime.cpp
        runtime/runtime.h
        runtime/stack_type.h
        compiler/expr/expr_return.h
        compiler/expr/expr_class.h
        runtime/stack_frame.cpp
        runtime/stack_frame.h
        runtime/memory_manager.cpp
        runtime/memory_manager.h
        runtime/visitable.h
        compiler/expr/expr_name.h
        runtime/name_resolver.cpp
        runtime/name_resolver.h
        runtime/address.h
        compiler/expr/expr_let.h
        compiler/expr/expr_unary.h
)
