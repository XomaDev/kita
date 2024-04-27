//
// Created by kumaraswamy on 4/27/24.
//

#ifndef KITA_EXPR_LET_H
#define KITA_EXPR_LET_H

#include "expr_base.h"

using namespace std;

class expr_let: public expr_base {
public:
    expr_let(const string &name, unique_ptr<expr_base> expr) : name(name), expr(std::move(expr)) {
        // constructor initialized
        set_display("let{name=" + name + ", expr=" + this->expr->to_string() + "}");
    }

    void dump(class dump *pDump) override {
        expr->dump(pDump);
        pDump->write(bytecode::LET);
        pDump->write_name(name);
    }

    bool is_leaf() override {
        return false;
    }

private:
    const string name;
    unique_ptr<expr_base> expr;
};

#endif //KITA_EXPR_LET_H
