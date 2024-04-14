//
// Created by kumaraswamy on 4/14/24.
//

#ifndef KITA_EXPR_FUNC_H
#define KITA_EXPR_FUNC_H


#include <string>
#include <utility>
#include <vector>
#include <memory>
#include "expr_base.h"

using namespace std;

class expr_func : public expr_base {
    string func_name;
    vector<unique_ptr<expr_base>> func_body;

    string args_repr() {
        string body_expr = "[";
        for (const unique_ptr<expr_base>& expr : func_body) {
            body_expr += expr->to_string() + " ";
        }
        body_expr += "]";
        return body_expr;
    }
public:
    explicit expr_func(string func_name, vector<unique_ptr<expr_base>> func_body)
            : func_name(std::move(func_name)), func_body(std::move(func_body)) {
        set_display("func{name=" + this->func_name + ", body=" + args_repr() + "}");
    }
};


#endif //KITA_EXPR_FUNC_H
