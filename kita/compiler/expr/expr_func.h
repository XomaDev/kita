//
// Created by kumaraswamy on 4/14/24.
//

#ifndef KITA_EXPR_FUNC_H
#define KITA_EXPR_FUNC_H


#include <string>
#include <utility>
#include <vector>
#include <memory>
#include "expr_group.h"
#include "expr_class.h"

using namespace std;

class expr_func : public expr_base {
    string func_name;
    bool inline_declaration;
    unique_ptr<expr_group> func_body;
    unique_ptr<expr_group> type_args;

public:
    explicit expr_func(string func_name, bool inline_declaration, unique_ptr<expr_group> func_body, unique_ptr<expr_group> type_args)
            : func_name(std::move(func_name)), inline_declaration(inline_declaration), func_body(std::move(func_body)),
            type_args(std::move(type_args)) {
        set_display(
                "func{name=" + this->func_name + ", inline=" + std::to_string(inline_declaration)
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

        // write the size of the scope
        pDump->write(bytecode::SCOPE_START);
        func_body->dump(pDump);
        if (inline_declaration) {
            pDump->write(bytecode::RETURN);
            pDump->write_uint8(0);
        }
        pDump->write(bytecode::SCOPE_END);
    }

    bool is_leaf() override {
        return false;
    }
};


#endif //KITA_EXPR_FUNC_H
