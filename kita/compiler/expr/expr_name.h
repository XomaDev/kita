//
// Created by kumaraswamy on 4/14/24.
//

#ifndef KITA_EXPR_NAME_H
#define KITA_EXPR_NAME_H

#include <memory>
#include "../token.h"
#include "expr_base.h"


using namespace std;

class expr_name : public expr_base {
    unique_ptr<token> _token;
public:
    explicit expr_name(unique_ptr<token> token) : expr_base(token->value), _token(std::move(token)) {
        // constructor initialized
    }

    void dump(class dump *pDump) override {
        pDump->write_name(_token->value);
    }
};


#endif //KITA_EXPR_NAME_H
