//
// Created by kumaraswamy on 4/14/24.
//

#include <iostream>
#include "parser.h"
#include "expr/expr_base.h"
#include "expr/expr_token.h"
#include "expr/expr_binary.h"
#include "expr/expr_type.h"
#include "expr/expr_invoke.h"
#include "expr/expr_func.h"

using namespace std;

void parser::parse() {
    while (!isEOF()) {
        expressions.emplace_back(parse_next());
    }
    for (auto &expr : expressions) {
        cout << expr->to_string() << endl;
    }
}

unique_ptr<expr_base> parser::parse_next() {
    auto &token = next();
    if (!isEOF()) {
        if (token->has_type("Class") && next_match("Kita")) {
            return type_decl(token, false);
        } else if (token->has_type("Method")) {
            return invoke_decl(token);
        } else if (token->has_type("Function")) {
            return function_decl();
        }
    }
    throw runtime_error("Unknown token type '" + token->str_repr() + "'");
}

unique_ptr<expr_func> parser::function_decl() {
    string function_name = strict_match("Identifier")->value;

    vector<unique_ptr<expr_type>> type_args;
    if (peek()->has_type("Colon")) {
        skip(); // eat ':'
        // read function arguments seperated by _
        type_args.emplace_back(type_decl(next(), true));
        while (!isEOF() && peek()->first_type == "With") {
            skip(); // eat '_'
            type_args.emplace_back(type_decl(next(), true));
        }
    }
    strict_match("StartBody");

    vector<unique_ptr<expr_base>> func_body;
    while (!isEOF() && !peek()->has_type("CloseBody")) {
        func_body.emplace_back(parse_next());
    }
    strict_match("CloseBody");
    return make_unique<expr_func>(function_name, std::move(func_body), std::move(type_args));
}

unique_ptr<expr_invoke> parser::invoke_decl(unique_ptr<token>& method_token) {
    string method_name = method_token->value;
    vector<unique_ptr<expr_base>> args = multi_expr_read();
    return make_unique<expr_invoke>(method_name, std::move(args));
}

vector<unique_ptr<expr_base>> parser::multi_expr_read() {
    vector<unique_ptr<expr_base>> args;
    args.emplace_back(expr_decl());
    while (!isEOF() && peek()->first_type == "With") {
        skip(); // eat '_'
        args.emplace_back(expr_decl());
    }
    return args;
}

unique_ptr<expr_type> parser::type_decl(unique_ptr<token>& class_token, bool simple) {
    strict_match("Kita");

    string class_name = class_token->value;
    string decl_name = strict_match("Identifier")->value;

    if (!simple && next_match("Colon")) {
        // variable declaration
        skip();
        return make_unique<expr_type>(class_name, decl_name, std::move(expr_decl()));
    }
    return make_unique<expr_type>(class_name, decl_name, nullptr);
}

unique_ptr<expr_base> parser::expr_decl() {
    auto left = read_expr();
    if (!isEOF()) {
        auto &peek_token = peek();
        if (peek_token->has_type("BinaryOperator")) {
            // high precedence operator
            if (peek_token->has_type("Precede")) {
                skip(); // operator
                auto wrapped_left = make_unique<expr_binary>(peek_token, std::move(left), std::move(read_expr()));
                if (isEOF()) {
                    return wrapped_left;
                }
                return std::move(expr_binary_decl(std::move(wrapped_left)));
            } else {
                // low precedence operator
                return std::move(expr_binary_decl(std::move(left)));
            }
        }
    }
    return std::move(left);
}

unique_ptr<expr_binary> parser::expr_binary_decl(unique_ptr<expr_base> left_expr) {
    auto &bin_operator = next();
    return make_unique<expr_binary>(
                bin_operator, // operator
                std::move(left_expr), // left expr
                std::move(expr_decl()) // right expr
            );
}

unique_ptr<expr_base> parser::read_expr() {
    auto &token = next();
    if (token->has_type("Value")) {
        return make_unique<expr_token>(std::move(token));
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
