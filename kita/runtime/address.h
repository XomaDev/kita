//
// Created by kumaraswamy on 4/27/24.
//

#ifndef KITA_ADDRESS_H
#define KITA_ADDRESS_H

#include <cstdlib>
#include <ostream>

struct address {
    bool static_definition;
    ulong scope;
    ulong index;

    [[nodiscard]] std::string print() const {
        return "addr{static=" + std::to_string(static_definition) + ", scope="
        + std::to_string(scope) + ", index=" + std::to_string(index) + "}";
    }
};

#endif //KITA_ADDRESS_H
