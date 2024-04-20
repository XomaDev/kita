//
// Created by kumaraswamy on 4/14/24.
//

#ifndef KITA_EXPR_TOKEN_H
#define KITA_EXPR_TOKEN_H

#include <memory>
#include "../token.h"
#include "expr_base.h"


using namespace std;

class expr_token : public expr_base {
    unique_ptr<token> _token;
public:
    explicit expr_token(unique_ptr<token> token) : expr_base(token->value), _token(std::move(token)) {
        // constructor initialized
    }

    void dump(class dump *pDump) override {
        if (_token->first_type == "Number") {
            pDump->write(bytecode::LOAD);
            pDump->write_int(stoi(_token->value));
        }
    }

    bool is_leaf() override {
        return true;
    }
};


#endif //KITA_EXPR_TOKEN_H
