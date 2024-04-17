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
};


#endif //KITA_EXPR_TOKEN_H
