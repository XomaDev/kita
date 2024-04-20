//
// Created by kumaraswamy on 4/14/24.
//

#include <iostream>
#include "parser.h"
#include "../expr/expr_base.h"
#include "../expr/expr_token.h"
#include "../expr/expr_binary.h"
#include "../expr/expr_type.h"
#include "../expr/expr_invoke.h"
#include "../expr/expr_func.h"
#include "../expr/expr_if.h"
#include "../expr/expr_inlineif.h"

using namespace std;

void parser::parse() {
    while (!isEOF()) {
        expressions.emplace_back(parse_next());
    }

}

unique_ptr<expr_base> parser::parse_next() {
    auto &token = next();
    if (!isEOF()) {
        if (token->has_type("Class") && next_match("Kita")) {
            return type_decl(token, false);
        } else if (token->has_type("IfConditional")) {
            return if_decl(token);
        } else if (token->has_type("Method")) {
            return invoke_decl(token);
        } else if (token->has_type("Function")) {
            return function_decl();
        }
    }
    back();
    return parse_expr(0);
}

unique_ptr<expr_base> parser::if_decl(unique_ptr<token>& if_token) {
    // right now, we only implement simple type of if expression
    // ifall, ifnone, ifany are not yet supported

    strict_match("OpenExpr");
    auto logical_expressions = multi_expr_read("Semicolon");
    strict_match("CloseExpr");

    if (next_match("StartBody")) {
        unique_ptr<expr_group> if_expr = read_body();
        unique_ptr<expr_group> else_expr = nullptr;

        if (!isEOF() && next_match("Else")) {
            skip(); // eat 'Else'
            if (next_match("StartBody")) {
                else_expr = read_body();
            } else if (next_match("If")) {
                else_expr = expr_group::singleton(if_decl(next()));
            } else {
                throw runtime_error("Expected body for else statement");
            }
            return make_unique<expr_if>(std::move(logical_expressions), std::move(if_expr), std::move(else_expr));
        }
        return make_unique<expr_if>(std::move(logical_expressions), std::move(if_expr), nullptr);
    } else {
        unique_ptr<expr_base> if_expr = parse_next();
        strict_match("Else");
        unique_ptr<expr_base> else_expr = parse_next();

        return make_unique<expr_inlineif>(std::move(logical_expressions), std::move(if_expr), std::move(else_expr));
    }
}

unique_ptr<expr_func> parser::function_decl() {
    string function_name = strict_match("Identifier")->value;

    vector<unique_ptr<expr_base>> type_args;
    if (peek()->has_type("Colon")) {
        skip(); // eat ':'
        // read function arguments seperated by _
        type_args.emplace_back(type_decl(next(), true));
        while (!isEOF() && peek()->first_type == "With") {
            skip(); // eat '_'
            type_args.emplace_back(type_decl(next(), true));
        }
    }
    auto type_args_group = make_unique<expr_group>(std::move(type_args));

    bool inline_expr = false;
    unique_ptr<expr_group> func_body;
    if (next_match("Assignment")) {
        // inline return expression assignment
        skip();
        func_body = expr_group::singleton(parse_next());
        inline_expr = true;
    } else {
        func_body = read_body();
    }
    return make_unique<expr_func>(function_name, inline_expr, std::move(func_body), std::move(type_args_group));
}

unique_ptr<expr_group> parser::read_body() {
    strict_match("StartBody");
    vector<unique_ptr<expr_base>> func_body;
    while (!isEOF() && !next_match("CloseBody")) {
        func_body.emplace_back(parse_next());
    }
    strict_match("CloseBody");
    return make_unique<expr_group>(std::move(func_body));
}

unique_ptr<expr_invoke> parser::invoke_decl(unique_ptr<token>& method_token) {
    string method_name = method_token->value;
    unique_ptr<expr_group> args = read_invoke_args("With");
    return make_unique<expr_invoke>(method_name, std::move(args));
}

unique_ptr<expr_group> parser::read_invoke_args(const string& type_delimiter) {
    unique_ptr<expr_group> args;
    auto &peek_token = peek();

    if (peek_token->has_type("OpenExpr")) {
        skip(); // eat '('
        args = multi_expr_read(type_delimiter);
        strict_match("CloseExpr");
    } else if (peek_token->has_type("StartBody")) {
        throw runtime_error("Expected '(' and not '{'");
        // maybe we could include new features in future
        // skip(); // eat '{'
        // args = multi_expr_read(type_delimiter);
        // strict_match("CloseBody");
    } else {
        return multi_expr_read(type_delimiter);
    }
    return args;
}

unique_ptr<expr_group> parser::multi_expr_read(const string& type_delimiter) {
    vector<unique_ptr<expr_base>> args;
    args.emplace_back(parse_next());
    while (!isEOF() && peek()->first_type == type_delimiter) {
        skip(); // eat '_'
        args.emplace_back(parse_next());
    }
    return make_unique<expr_group>(std::move(args));
}

unique_ptr<expr_type> parser::type_decl(unique_ptr<token>& class_token, bool simple) {
    strict_match("Kita");

    string class_name = class_token->value;
    string decl_name = strict_match("Identifier")->value;

    if (!simple && next_match("Colon")) {
        // variable declaration
        skip();
        return make_unique<expr_type>(class_name, decl_name, std::move(parse_next()));
    }
    return make_unique<expr_type>(class_name, decl_name, nullptr);
}

unique_ptr<expr_base> parser::parse_expr(int minPrecedence) {
    auto left = read_expr();
    while (!isEOF()) {
        auto &token_operator = peek();

        int precedence = operator_precedence(token_operator);
        if (precedence == -1) {
            // unknown token reached, return
            return left;
        }
        if (precedence >= minPrecedence) {
            skip();
            // * and / are Non-Commute operators, i.e. the order should be preserved
            auto right= token_operator->has_type("NonCommute")
                    ? read_expr()
                    : parse_expr(precedence);
            left = make_unique<expr_binary>(token_operator, std::move(left), std::move(right));
        } else {
            break;
        }
    }
    return left;
}

int parser::operator_precedence(unique_ptr<token> &token_operator) {
    int precedence = -1;
    if (token_operator->has_type("Bitwise")) {
      precedence = 1;
    } else if (token_operator->has_type("LogicalAndOr")) {
        precedence = 2;
    } else if (token_operator->has_type("Equality")) {
        precedence = 3;
    } else if (token_operator->has_type("Relational")) {
        precedence = 4;
    } else if (token_operator->has_type("Binary")) {
        precedence = 5;
    } else if (token_operator->has_type("BinaryPrecede")) {
        precedence = 6;
    }
    return precedence;
}


unique_ptr<expr_base> parser::read_expr() {
    auto &token = next();
    if (token->has_type("Value")) {
        return make_unique<expr_token>(std::move(token));
    } else if (token->has_type("OpenExpr")) {
        auto expr = parse_next();
        strict_match("CloseExpr");
        return expr;
    } else if (token->has_type("StartBody")) {
        back();
        auto inline_body = read_body();
        return inline_body;
    }
    throw runtime_error("Unknown type, not handled '" + token->types_str_repr() + "'");
}

unique_ptr<token>& parser::strict_match(const std::string &type) {
    auto &peek_token = next();
    if (!peek_token->has_type(type)) {
        throw runtime_error("Expected type '" + type + "' but got '" + peek_token->types_str_repr() + "'");
    }
    return peek_token;
}

bool parser::next_match(const string& type) {
    return peek()->has_type(type);
}

void parser::back() {
    if (index == 0) {
        throw runtime_error("Cant go past 0");
    }
    index--;
}

void parser::skip() {
    if (isEOF()) {
        throw runtime_error("Already reached EOF");
    }
    index++;
}

unique_ptr<token>& parser::peek() {
    if (isEOF()) {
        throw runtime_error("Reached EOF");
    }
    return tokens[index];
}

unique_ptr<token>& parser::next() {
    if (isEOF()) {
        throw runtime_error("Reached EOF");
    }
    return tokens[index++];
}

bool parser::isEOF() const {
    return index == size;
}
