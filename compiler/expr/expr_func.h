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
#include "expr_type.h"

using namespace std;

class expr_func : public expr_base {
    string func_name;
    vector<unique_ptr<expr_base>> func_body;
    vector<unique_ptr<expr_type>> type_args;

    string args_repr() {

    }
public:
    explicit expr_func(string func_name, vector<unique_ptr<expr_base>> func_body, vector<unique_ptr<expr_type>> type_args)
            : func_name(std::move(func_name)), func_body(std::move(func_body)), type_args(std::move(type_args)) {
        string body_expr = "[";
        for (const unique_ptr<expr_base>& expr : this->func_body) {
            body_expr += expr->to_string() + " ";
        }
        body_expr += "]";

        string type_args_str = "[";
        for (const unique_ptr<expr_type>& type_arg : this->type_args) {
            type_args_str += type_arg->to_string() + " ";
        }
        type_args_str += "]";
        set_display("func{name=" + this->func_name + ", args=" + type_args_str + ", body=" + body_expr + "}");
    }
};


#endif //KITA_EXPR_FUNC_H
