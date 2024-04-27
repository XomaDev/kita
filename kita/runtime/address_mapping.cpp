//
// Created by kumaraswamy on 4/27/24.
//

#include <algorithm>
#include <stdexcept>
#include <iostream>
#include "address_mapping.h"

address_mapping::~address_mapping() {
    for (auto frame: frames) {
        delete frame;
    }
}

void address_mapping::enter_frame() {
    scope_names = new vector<string>();
    frames.emplace_back(scope_names);
    depth++;
}

void address_mapping::exit_frame() {
    scope_names = frames.back();
    frames.pop_back();
    depth--;
}

void address_mapping::declare(const string& name) {
    scope_names->emplace_back(name);
}

pair<ulong, ulong> address_mapping::dereference(const string &name, bool static_address) {
    auto current_depth = depth;
    for (;;) {
        auto names = frames[--current_depth];
        auto it = find(names->begin(), names->end(), name);
        if (it != names->end()) {
            auto index = std::distance(names->begin(), it);
            return {static_address ? current_depth : depth - current_depth - 1, index };
        }
        if (current_depth == 0) {
            throw runtime_error("Unable to resolve name '" + name + "'");
        }
    }
}
