//
// Created by kumaraswamy on 4/14/24.
//

#ifndef KITA_EXPR_TYPE_H
#define KITA_EXPR_TYPE_H

#include <memory>
#include "expr_base.h"

class expr_type : public expr_base {
    const string class_name;
    const string decl_name;

    unique_ptr<expr_base> expr;
public:
    expr_type(const string& class_name, const string& decl_name, unique_ptr<expr_base> expr):
            class_name(class_name), decl_name(decl_name), expr(std::move(expr)) {
        string display = "type{class=" + class_name + ", name=" + decl_name +
                (this->expr == nullptr ? "" : ", expr=" + this->expr->to_string())
                + "}";
        set_display(display);
    }

    void dump(class dump *pDump) override {
        if (expr != nullptr) {
            // sometimes, it could be uninit, or func parameter decl
            expr->dump(pDump);
            pDump->write(bytecode::DECLARE);
        }
        if (class_name == "Int") {
            pDump->write(bytecode::INT_CLASS);
        } else if (class_name == "Bool") {
            pDump->write(bytecode::BOOL_CLASS);
        } else if (class_name == "String") {
            pDump->write(bytecode::STRING_CLASS);
        } else {
            throw runtime_error("Unknown class name '" + class_name + "'");
        }
        pDump->write_name(decl_name);
    }

    bool is_leaf() override {
        return false;
    }
};

#endif //KITA_EXPR_TYPE_H
