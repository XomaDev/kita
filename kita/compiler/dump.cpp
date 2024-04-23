//
// Created by kumaraswamy on 4/20/24.
//

#include <iostream>
#include <sstream>
#include "dump.h"

using namespace std;

dump::dump() {
    file = new ostringstream();
}

dump::dump(const string& file_path, vector<unique_ptr<expr_base>> exprs) : exprs(std::move(exprs)) {
    auto stream = ofstream(file_path, ios::binary);
    if (!stream) {
        cerr << "Failed to open file '" << file_path << "'" << endl;
        exit(1);
    }
    file = &stream;
    for (auto &expr: this->exprs) {
        expr->dump(this);
    }
    stream.close();
}

void dump::write(bytecode b) {
    // cout << "Write " << to_string(static_cast<int>(b)) << endl;
    auto value = static_cast<uint8_t>(b);
    file->write(reinterpret_cast<const char *>(&value), sizeof(value));
    file->flush();
}

void dump::write_uint8(uint8_t b) {
    // c->ut << "Write " << to_string(b) << endl;
    file->write(reinterpret_cast<const char *>(&b), sizeof(b));
    file->flush();
}

void dump::write_int(int n) {
    // c->ut << "WriteInt: " << to_string(n) << endl;
    file->write(reinterpret_cast<const char*>(&n), sizeof(n));
    file->flush();
}

void dump::write_name(string name) {
    // cout << "WriteName: " << name << endl;
    auto name_length = name.size();
    if (name_length > 255) {
        throw runtime_error("Maximum name length is 255 characters");
    }
    auto length_byte = static_cast<uint8_t>(name_length);
    file->write(reinterpret_cast<const char *>(&length_byte), sizeof(length_byte));
    file->write(name.data(), name_length);
    file->flush();
}

int dump::size() {
    return dynamic_cast<std::ostringstream*>(this->file)->str().size();
}

void dump::transfer(dump *d) {
    auto* stream = dynamic_cast<std::ostringstream*>(this->file);
    auto content = stream->str();
    d->file->write(content.c_str(), content.size());
    delete file;
}
