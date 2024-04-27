//
// Created by kumaraswamy on 4/14/24.
//

#ifndef KITA_EXPR_BASE_H
#define KITA_EXPR_BASE_H

#include <string>
#include <utility>
#include "../dump.h"

using namespace std;

class dump;

class expr_base {
    string display;
public:
    expr_base() = default;
    explicit expr_base(string display) : display(std::move(display)) {
        // constructor initialized
    }

    void set_display(string new_display) {
        this->display = std::move(new_display);
    }

    virtual ~expr_base() = default;

    string to_string() {
        return display;
    }

    virtual void dump(dump *pDump) {
        throw runtime_error("not implemented");
    }
};


#endif //KITA_EXPR_BASE_H
