//
// Created by kumaraswamy on 4/24/24.
//

#ifndef KITA_EXPR_CLASS_H
#define KITA_EXPR_CLASS_H

#include "expr_base.h"
#include "../token.h"

class expr_class: public expr_base {
    unique_ptr<token> token_class;
public:
    explicit expr_class(unique_ptr<token> token_class) : token_class(std::move(token_class)) {
        set_display(
                    this->token_class == nullptr ? "Void" : this->token_class->value
                );
    }

    void dump(class dump *pDump) override {
        if (token_class == nullptr || token_class->has_type("Void")) {
          pDump->write(bytecode::VOID);
        } else if (token_class->has_type("Int")) {
            pDump->write(bytecode::INT_CLASS);
        } else if (token_class->has_type("Bool")) {
            pDump->write(bytecode::BOOL_CLASS);
        } else if (token_class->has_type("String")) {
            pDump->write(bytecode::STRING_CLASS);
        } else {
            throw runtime_error("Unknown token " + token_class->to_string());
        }
    }

    bool is_leaf() override {
        return false; // dosent matter
    }
};

#endif //KITA_EXPR_CLASS_H
