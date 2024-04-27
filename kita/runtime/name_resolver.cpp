//
// Created by kumaraswamy on 4/27/24.
//

#include <algorithm>
#include <iostream>
#include "name_resolver.h"

void name_resolver::enter_frame() {
    names = new vector<string>();
    frames.emplace_back(names);
    depth++;
}

void name_resolver::exit_fame() {
    delete frames.back();
    frames.pop_back();
    names = frames.back();
    depth--;
}

void name_resolver::declare(const string& name) {
    names->emplace_back(name);
}

address name_resolver::resolve(const string &name, bool static_resolution) {
    auto iterate_depth = depth;
    while (iterate_depth) {
        auto frame = frames[--iterate_depth];
        auto resolved = std::find(frame->begin(), frame->end(), name);
        if (resolved != frame->end()) {
            auto stack_index = std::distance(frame->begin(), resolved);
            return address {
                static_resolution,
                static_resolution ? iterate_depth : depth - iterate_depth - 1,
                static_cast<ulong>(stack_index)
            };
        }
    }
    throw runtime_error("Couldn't resolve name '" + name + "'");
}
