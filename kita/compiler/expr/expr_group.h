//
// Created by kumaraswamy on 4/17/24.
//

#ifndef KITA_EXPR_GROUP_H
#define KITA_EXPR_GROUP_H

#include <vector>
#include <memory>
#include <limits>
#include "expr_base.h"

using namespace std;

class expr_group: public expr_base {
    vector<unique_ptr<expr_base>> exprs;
public:
    uint8_t args_size;

    explicit expr_group(vector<unique_ptr<expr_base>> exprs) : exprs(std::move(exprs)) {
        int size = this->exprs.size();
        if (size > std::numeric_limits<uint8_t>::max()) {
            throw std::out_of_range("too many expressions, uint_8 limit");
        }
        args_size = static_cast<uint8_t>(size);
        string body_expr = "[";
        for (const unique_ptr<expr_base>& expr : this->exprs) {
            body_expr += expr->to_string() + " ";
        }
        body_expr += "]";
        set_display(body_expr);
    }

    static unique_ptr<expr_group> singleton(unique_ptr<expr_base> expr) {
        vector<unique_ptr<expr_base>> single_expr;
        single_expr.emplace_back(std::move(expr));
        return make_unique<expr_group>(std::move(single_expr));
    }

    void dump(class dump *pDump) override {
        for (auto &expr: exprs) {
            expr->dump(pDump);
        }
    }
};

#endif