//
// Created by kumaraswamy on 4/14/24.
//

#ifndef KITA_PARSER_H
#define KITA_PARSER_H

#include <memory>
#include <utility>
#include <vector>
#include "token.h"
#include "expr/expr_base.h"
#include "expr/expr_binary.h"
#include "expr/expr_invoke.h"
#include "expr/expr_func.h"

class parser {
    int index = 0;
    const int size;
    vector<unique_ptr<token>> tokens;

    vector<unique_ptr<expr_base>> expressions;

    unique_ptr<expr_base> parse_next();
    unique_ptr<expr_base> type_decl(unique_ptr<token>& ptr);

    unique_ptr<expr_invoke> invoke_decl(unique_ptr<token>& ptr);

    vector<unique_ptr<expr_base>> multi_expr_read();

    unique_ptr<expr_base> expr_decl();
    unique_ptr<expr_base> read_expr();

    unique_ptr<expr_binary> expr_binary_decl(unique_ptr<expr_base> ptr);

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

    unique_ptr<expr_func> function_decl();
};


#endif //KITA_PARSER_H
