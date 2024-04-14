//
// Created by kumaraswamy on 4/14/24.
//

#include <iostream>
#include "parser.h"

using namespace std;

void parser::parse() {
    while (!isEOF()) {
        parse_next();
    }
}

void parser::parse_next() {
    auto &token = next();
    if (!isEOF()) {
        if (token->has_type("Class") && next_match("Kita")) {
            type_decl(token);
        }
    }
}

void parser::type_decl(unique_ptr<token>& class_token) {
    strict_match("Kita");

    string class_name = class_token->value;
    string decl_name = strict_match("Identifier")->value;
    cout << "type_decl " << class_name << " " << decl_name << endl;

    if (next_match("Colon")) {
        // variable declaration
        skip();
        expr_decl();
    }
}

void parser::expr_decl() {

}

unique_ptr<token>& parser::strict_match(const std::string &type) {
    auto &peek_token = next();
    if (!peek_token->has_type(type)) {
        throw runtime_error("Expected type '" + type + "' but got '" + peek_token->types_str_repr() + "'");
    }
    return peek_token;
}

bool parser::next_match(const string& type) {
    return tokens[index]->has_type(type);
}

void parser::skip() {
    index++;
}

unique_ptr<token>& parser::peek() {
    return tokens[index];
}

unique_ptr<token>& parser::next() {
    return tokens[index++];
}

bool parser::isEOF() const {
    return index == size;
}