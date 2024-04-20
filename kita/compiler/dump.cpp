//
// Created by kumaraswamy on 4/20/24.
//

#include <iostream>
#include "dump.h"

using namespace std;

dump::dump(const string& file_path, vector<unique_ptr<expr_base>> exprs) : exprs(std::move(exprs)) {
    file = ofstream(file_path, ios::binary);
    if (!file) {
        cerr << "Failed to open file '" << file_path << "'" << endl;
        exit(1);
    }
    for (auto &expr: this->exprs) {
        expr->dump(this);  // Now valid, expr_base is fully known
    }
    file.close();
}

void dump::write(bytecode b) {
    // cout << "Write " << to_string(static_cast<int>(b)) << endl;
    auto value = static_cast<uint8_t>(b);
    file.write(reinterpret_cast<const char *>(&value), sizeof(value));
    file.flush();
}

void dump::write_uint8(uint8_t b) {
    // cout << "Write " << to_string(b) << endl;
    file.write(reinterpret_cast<const char *>(&b), sizeof(b));
    file.flush();
}

void dump::write_int(int n) {
    // cout << "WriteInt: " << to_string(n) << endl;
    file.write(reinterpret_cast<const char*>(&n), sizeof(n));
    file.flush();
}

void dump::write_name(string name) {
    // cout << "WriteName: " << name << endl;
    auto name_length = name.size();
    if (name_length > 255) {
        throw runtime_error("Maximum name length is 255 characters");
    }
    auto length_byte = static_cast<uint8_t>(name_length);
    file.write(reinterpret_cast<const char *>(&length_byte), sizeof(length_byte));
    file.write(name.data(), name_length);
    file.flush();
}