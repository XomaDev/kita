//
// Created by kumaraswamy on 4/17/24.
//

#ifndef KITA_EXPR_GROUP_H
#define KITA_EXPR_GROUP_H

#include <vector>
#include <memory>
#include "expr_base.h"

using namespace std;

class expr_group: public expr_base {
    vector<unique_ptr<expr_base>> exprs;
public:
    explicit expr_group(vector<unique_ptr<expr_base>> exprs) : exprs(std::move(exprs)) {
        string body_expr = "[";
        for (const unique_ptr<expr_base>& expr : this->exprs) {
            body_expr += expr->to_string() + " ";
        }
        body_expr += "]";
        set_display(body_expr);
    }
};

#endif