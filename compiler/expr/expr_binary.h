//
// Created by kumaraswamy on 4/14/24.
//

#ifndef KITA_EXPR_BINARY_H
#define KITA_EXPR_BINARY_H

#include <memory>
#include <utility>
#include "expr_base.h"
#include "../token.h"
#include "../dump.h"

class expr_binary: public expr_base {
    const unique_ptr<token>& _operator;
    const unique_ptr<expr_base> left, right;
public:
    expr_binary(unique_ptr<token>& _operator, unique_ptr<expr_base> left, unique_ptr<expr_base> right)
            : expr_base(
                    "binary{'" + _operator->value + "' left=" + left->to_string() + ", right=" + right->to_string() + "}"
                    ), _operator(std::move(_operator)), left(std::move(left)), right(std::move(right))  {
        // constructor initialized
    }

    void dump(class dump *pDump) override {
        if (left->is_leaf()) {
            right->dump(pDump);
            left->dump(pDump);
        } else {
            left->dump(pDump);
            right->dump(pDump);
        }
        auto code = this->_operator->value;
        if (code == "+") {
            pDump->write(bytecode::ADD);
        } else if (code == "-") {
            pDump->write(bytecode::NEG);
        } else if (code == "*") {
            pDump->write(bytecode::MUL);
        } else if (code == "/") {
            pDump->write(bytecode::DIV);
        } else {
            throw runtime_error("Unknown operator " + _operator->to_string());
        }
    }

    bool is_leaf() override {
        return false;
    }
};

#endif //KITA_EXPR_BINARY_H
