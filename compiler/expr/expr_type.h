//
// Created by kumaraswamy on 4/14/24.
//

#ifndef KITA_EXPR_TYPE_H
#define KITA_EXPR_TYPE_H

#include <memory>
#include "expr_base.h"

class expr_type : public expr_base {
    const string& class_name;
    const string& decl_name;

    unique_ptr<expr_base> expr;
public:
    expr_type(const string& class_name, const string& decl_name, unique_ptr<expr_base> expr)
            : expr_base("expr_type{class=" + class_name + ", name=" + decl_name + ", expr=" + expr->to_string() + "}"),
            class_name(class_name), decl_name(decl_name), expr(std::move(expr)) {
        // constructor initialized
    }
};

#endif //KITA_EXPR_TYPE_H
