//
// Created by kumaraswamy on 4/14/24.
//

#ifndef KITA_EXPR_INVOKE_H
#define KITA_EXPR_INVOKE_H


#include <vector>
#include <memory>
#include "expr_base.h"
#include "expr_group.h"
#include "../dump.h"

class expr_invoke : public expr_base {
    const string method_name;
    unique_ptr<expr_group> args;

public:
    expr_invoke(const string& method_name, unique_ptr<expr_group> args)
            : method_name(method_name), args(std::move(args)) {
        set_display("invoke{method=" + method_name + ", args=" + this->args->to_string() + "}");
    }

    void dump(class dump *pDump) override {
        args->dump(pDump);
        pDump->write(bytecode::INVOKE);
        if (method_name == "disp") {
            pDump->write(bytecode::DISP);
        }
    }

    bool is_leaf() override {
        return false;
    }
};


#endif //KITA_EXPR_INVOKE_H
