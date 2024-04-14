//
// Created by kumaraswamy on 4/13/24.
//

#ifndef KITA_TOKEN_H
#define KITA_TOKEN_H


#include <string>
#include <utility>
#include <vector>

using namespace std;

class token {
public:
    string first_type;

    vector<string> types;
    const string value;

    token(const string& type, string value) :value(std::move(value)) {
        // constructor initialized
        types.emplace_back(type);
        first_type = this->types[0];
    }

    token(vector<string> types, string value) : types(std::move(types)), value(std::move(value)) {
        // constructor initialized
        first_type = this->types[0];
    }

    bool has_type(const string& type);
    string str_repr();

    string types_str_repr();
};


#endif //KITA_TOKEN_H
