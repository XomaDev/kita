//
// Created by kumaraswamy on 4/18/24.
//

#ifndef KITA_EXPR_IF_H
#define KITA_EXPR_IF_H

#include <memory>
#include "expr_base.h"
#include "expr_group.h"

using namespace std;

class expr_if : public expr_base {
    unique_ptr<expr_group> logical_exprs;

    unique_ptr<expr_group> if_body;
    unique_ptr<expr_group> else_body;

public:
    expr_if(unique_ptr<expr_group> logical_exprs, unique_ptr<expr_group> if_body, unique_ptr<expr_group> else_body)
                            : logical_exprs(std::move(logical_exprs)), if_body(std::move(if_body)), else_body(std::move(else_body)) {
        set_display(
                "if{conditions=" + this->logical_exprs->to_string() +
                ", if=" + this->if_body->to_string() + ", else=" + this->else_body->to_string()
                );
    }
};


#endif
