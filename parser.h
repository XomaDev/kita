//
// Created by kumaraswamy on 4/14/24.
//

#ifndef KITA_PARSER_H
#define KITA_PARSER_H

#include <memory>
#include <utility>
#include <vector>
#include "token.h"

class parser {
    int index = 0;
    const int size;
    vector<unique_ptr<token>> tokens;

    void parse_next();
    void type_decl(unique_ptr<token>& ptr);

    void expr_decl();

    unique_ptr<token>& strict_match(const string& type);
    bool next_match(const string& type);

    void skip();

    unique_ptr<token>& next();
    unique_ptr<token>& peek();
    bool isEOF() const;
public:
    explicit parser(vector<unique_ptr<token>> tokens) : size(tokens.size()), tokens(std::move(tokens)) {
        // constructor initialized
    }
    void parse();
};


#endif //KITA_PARSER_H
