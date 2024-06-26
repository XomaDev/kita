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
                ", if=" + this->if_body->to_string() + (this->else_body == nullptr ? "" : ", else=" + this->else_body->to_string())
                + "}"
                );
    }

    void dump(class dump *pDump) override {
        logical_exprs->dump(pDump);

        pDump->write(bytecode::IF);
        // if it has else branch
        pDump->write_uint8(else_body == nullptr ? 0 : 1);

        pDump->write(bytecode::SCOPE_START);
        if_body->dump(pDump);
        pDump->write(bytecode::SCOPE_END);

        if (else_body != nullptr) {
            // else *body* branch
            pDump->write(bytecode::SCOPE_START);
            else_body->dump(pDump);
            pDump->write(bytecode::SCOPE_END);
        }
    }
};


#endif
