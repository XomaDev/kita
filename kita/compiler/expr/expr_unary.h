//
// Created by kumaraswamy on 4/27/24.
//

#ifndef KITA_EXPR_UNARY_H
#define KITA_EXPR_UNARY_H

#include "expr_base.h"
#include "../token.h"

using namespace std;

class expr_unary: public expr_base {
    const unique_ptr<token>& _operator;
    const unique_ptr<expr_base> expr;
public:
    expr_unary(unique_ptr<token>& _operator, unique_ptr<expr_base> expr) : _operator(_operator), expr(std::move(expr)) {
        // constructor initialized
    }
    void dump(class dump *pDump) override {
        expr->dump(pDump);
        pDump->write(bytecode::UNARY_OP);
        auto unary_operator = _operator->first_type;
        if (unary_operator == "Negate") {
            pDump->write(bytecode::NEG);
        } else if (unary_operator == "Not") {
            pDump->write(bytecode::NOT);
        } else {
            throw runtime_error("Unknown unary operator " + _operator->to_string());
        }
    }

    bool is_leaf() override {
        return false;
    }
};

#endif //KITA_EXPR_UNARY_H
