//
// Created by kumaraswamy on 4/14/24.
//

#ifndef KITA_EXPR_INVOKE_H
#define KITA_EXPR_INVOKE_H


#include <vector>
#include <memory>
#include "expr_base.h"

class expr_invoke : public expr_base {
    const string method_name;
    vector<unique_ptr<expr_base>> args;

    string args_repr() {
        string args_expr = "[";
        for (const unique_ptr<expr_base>& arg : args) {
            args_expr += arg->to_string() + " ";
        }
        args_expr += "]";
        return args_expr;
    }
public:
    expr_invoke(const string& method_name, vector<unique_ptr<expr_base>> args)
            : method_name(method_name), args(std::move(args)) {
        set_display("expr_invoke{method=" + method_name + ", args=" + args_repr() + "}");
    }
};


#endif //KITA_EXPR_INVOKE_H
