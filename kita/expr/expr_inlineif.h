//
// Created by kumaraswamy on 4/18/24.
//

#ifndef KITA_EXPR_INLINEIF_H
#define KITA_EXPR_INLINEIF_H

#include <memory>
#include "expr_base.h"
#include "expr_group.h"

using namespace std;

class expr_inlineif : public expr_base {
    unique_ptr<expr_group> logical_exprs;

    unique_ptr<expr_base> if_expr;
    unique_ptr<expr_base> else_expr;

public:
    expr_inlineif(unique_ptr<expr_group> logical_exprs, unique_ptr<expr_base> if_expr, unique_ptr<expr_base> else_expr)
                            : logical_exprs(std::move(logical_exprs)), if_expr(std::move(if_expr)), else_expr(std::move(else_expr)) {
        set_display(
                "inline_if{conditions=" + this->logical_exprs->to_string() +
                ", if=" + this->if_expr->to_string() + ", else=" + this->else_expr->to_string()
                );
    }
};


#endif //KITA_EXPR_INLINEIF_H
