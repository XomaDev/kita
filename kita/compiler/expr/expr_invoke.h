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
        set_display("invoke{method=" + method_name + (this->args == nullptr ? "" : ", args=" + this->args->to_string())
        + "}");
    }

    void dump(class dump *pDump) override {
        // could be 0 length args
        if (args != nullptr) {
            args->dump(pDump);
        }
        pDump->write(bytecode::INVOKE);
        pDump->write_uint8(args == nullptr ? 0 : args->args_size);

        if (method_name == "disp") {
            pDump->write(bytecode::DISP);
        } else if (method_name == "scope") {
            pDump->write(bytecode::SCOPE);
        } else {
            // write the method name this time ahh
            pDump->write(bytecode::NAME_TYPE);
            pDump->write_name(method_name);
        }
    }

    bool is_leaf() override {
        return false;
    }
};


#endif //KITA_EXPR_INVOKE_H
