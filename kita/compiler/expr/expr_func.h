//
// Created by kumaraswamy on 4/14/24.
//

#ifndef KITA_EXPR_FUNC_H
#define KITA_EXPR_FUNC_H


#include <string>
#include <utility>
#include <vector>
#include <memory>
#include "expr_type.h"
#include "expr_group.h"

using namespace std;

class expr_func : public expr_base {
    string func_name;
    bool inline_expr;
    unique_ptr<expr_group> func_body;
    unique_ptr<expr_group> type_args;

public:
    explicit expr_func(string func_name, bool inline_expr, unique_ptr<expr_group> func_body, unique_ptr<expr_group> type_args)
            : func_name(std::move(func_name)), inline_expr(inline_expr), func_body(std::move(func_body)), type_args(std::move(type_args)) {
        set_display(
                "func{name=" + this->func_name + ", inline=" + (inline_expr ? "true" : "false")
                 + ", args=" + this->type_args->to_string() + ", body=" + this->func_body->to_string() + "}"
        );
    }

    void dump(class dump *pDump) override {
        pDump->write(bytecode::FUNC);
        // dump func name
        pDump->write_name(func_name);

        pDump->write_uint8(type_args->args_size);
        // dump parameter types
        type_args->dump(pDump);

        // first dump the bytes into memory
        class dump mem_dump;
        func_body->dump(&mem_dump);

        // write the size of the scope
        cout << "Scope Size: " << std::to_string(mem_dump.size()) << endl;
        pDump->write_int(mem_dump.size());
        pDump->write(bytecode::SCOPE_START);

        // transfer content of mem_dump -> pdump
        mem_dump.transfer(pDump);

        pDump->write(bytecode::SCOPE_END);
    }

    bool is_leaf() override {
        return false;
    }
};


#endif //KITA_EXPR_FUNC_H
