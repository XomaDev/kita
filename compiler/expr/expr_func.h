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
};


#endif //KITA_EXPR_FUNC_H
