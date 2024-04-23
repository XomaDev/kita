//
// Created by kumaraswamy on 4/20/24.
//

#ifndef KITA_DUMP_H
#define KITA_DUMP_H

#include <string>
#include <memory>
#include <utility>
#include <vector>
#include <fstream>
#include "../bytecode.h"
#include "expr/expr_base.h"

using namespace std;

class expr_base;

class dump {
    vector<unique_ptr<expr_base>> exprs;

    ostream* file;
public:
    dump();
    dump(const string& file_path, vector<unique_ptr<expr_base>> exprs);

    void write(bytecode b);
    void write_int(int n);
    void write_name(string name);
    void write_uint8(uint8_t b);

    // used when this->file is ostringstream
    int size();
    void transfer(dump *another);
};


#endif //KITA_DUMP_H
