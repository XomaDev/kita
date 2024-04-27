//
// Created by kumaraswamy on 4/23/24.
//

#ifndef KITA_EXPR_RETURN_H
#define KITA_EXPR_RETURN_H

#include "expr_base.h"

using namespace std;

class expr_return : public expr_base {
    unique_ptr<expr_base> expr;
public:
    explicit expr_return(unique_ptr<expr_base> expr) : expr(std::move(expr)) {
        // constructor initialized
        set_display("return{" + this->expr->to_string() + "}");
    }

    void dump(class dump *pDump) override {
        bool is_void = expr->to_string() == "Void";
        if (!is_void) {
            expr->dump(pDump);
        }
        pDump->write(bytecode::RETURN);
        pDump->write_uint8(is_void);
    }

};

#endif //KITA_EXPR_RETURN_H
