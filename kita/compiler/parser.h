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
#include "expr/expr_let.h"

class parser {
    int index = 0;
    const int size;
    vector<unique_ptr<token>> tokens;

    unique_ptr<expr_base> parse_next();

    unique_ptr<expr_let> let_decl();

    unique_ptr<expr_group> read_body();

    unique_ptr<expr_base> if_decl(unique_ptr<token>& if_token);

    unique_ptr<expr_func> func_decl();
    void func_arg_names(vector<unique_ptr<expr_base>> &type_args);
    unique_ptr<expr_base> identifier();

    unique_ptr<expr_invoke> invoke_decl(unique_ptr<token>& ptr);

    unique_ptr<expr_group> multi_expr_read(const string& type_delimiter);

    static int operator_precedence(unique_ptr<token> &token_operator) ;

    unique_ptr<expr_group> read_invoke_args(const string &type_delimiter);

    unique_ptr<expr_base> parse_expr(int precedence);

    unique_ptr<expr_base> unary_expression(unique_ptr<token> &uniquePtr);
    unique_ptr<expr_base> read_expr();

    unique_ptr<token>& strict_match(const string& type);
    bool next_match(const string& type);

    void back();
    void skip();

    unique_ptr<token>& next();
    unique_ptr<token>& peek();
    bool isEOF() const;
public:
    vector<unique_ptr<expr_base>> expressions;

    explicit parser(vector<unique_ptr<token>> tokens) : size(tokens.size()), tokens(std::move(tokens)) {
        // constructor initialized
    }
    void parse();

};


#endif //KITA_PARSER_H
