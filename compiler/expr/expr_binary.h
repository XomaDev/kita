//
// Created by kumaraswamy on 4/14/24.
//

#ifndef KITA_EXPR_BINARY_H
#define KITA_EXPR_BINARY_H

#include <memory>
#include <utility>
#include "expr_base.h"
#include "../token.h"

class expr_binary: public expr_base {
    const unique_ptr<token>& _operator;
    const unique_ptr<expr_base> left, right;
public:
    expr_binary(unique_ptr<token>& _operator, unique_ptr<expr_base> left, unique_ptr<expr_base> right)
            : expr_base(
                    "expr_binary{op=" + _operator->value + " left=" + left->to_string() + ", right=" + right->to_string() + "}"
                    ), _operator(std::move(_operator)), left(std::move(left)), right(std::move(right))  {
        // constructor initialized
    }
    void write() override {
        cout << "write: expr_binary" << endl;
    }
};

#endif //KITA_EXPR_BINARY_H
