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
        } else if (token->has_type("IfConditional")) {
            if_decl();
            cout << "Token: " << token->to_string() << endl;
        } else if (token->has_type("Method")) {
            return invoke_decl(token);
        } else if (token->has_type("Function")) {
            return function_decl();
        }
    }
    throw runtime_error("Unknown token type '" + token->to_string() + "'");
}

void parser::if_decl() {
    back();
    for (;;) {
        auto &ifType = peek();
        if (!ifType->has_type("IfConditional")) {
            break;
        }
        skip(); // eat 'ifType'

        strict_match("OpenExpr");
        vector<unique_ptr<expr_base>> args = multi_expr_read("Semicolon");
        strict_match("CloseExpr");

        vector<unique_ptr<expr_base>> ifBody = read_body();

        cout << "Args: " << args[0]->to_string() << endl;
        cout << "Body: " << ifBody[0]->to_string() << endl;
    }
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
    vector<unique_ptr<expr_base>> func_body = read_body();
    return make_unique<expr_func>(function_name, std::move(func_body), std::move(type_args));
}

vector<unique_ptr<expr_base>> parser::read_body() {
    strict_match("StartBody");
    vector<unique_ptr<expr_base>> func_body;
    while (!isEOF() && !peek()->has_type("CloseBody")) {
        func_body.emplace_back(parse_next());
    }
    strict_match("CloseBody");
    return func_body;
}

unique_ptr<expr_invoke> parser::invoke_decl(unique_ptr<token>& method_token) {
    string method_name = method_token->value;
    vector<unique_ptr<expr_base>> args = multi_expr_read("With");
    return make_unique<expr_invoke>(method_name, std::move(args));
}

vector<unique_ptr<expr_base>> parser::multi_expr_read(const string& type_delimiter) {
    vector<unique_ptr<expr_base>> args;
    args.emplace_back(expr_decl());
    while (!isEOF() && peek()->first_type == type_delimiter) {
        skip(); // eat '_'
        args.emplace_back(expr_decl());
    }
    return args;
}

unique_ptr<expr_type> parser::type_decl(unique_ptr<token>& class_token, bool simple) {
    cout << "Class Token " << class_token->to_string() << endl;
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
    auto expr = expr_relational();
    while (!isEOF()) {
        auto &peek_operator = peek();
        if (peek_operator->has_type("Logical")) {
            cout << "Peek Operator " << peek_operator->to_string() << endl;
            skip();
            expr = make_unique<expr_binary>(peek_operator, std::move(expr), expr_relational());
        } else {
            break;
        }
    }
    return expr;
}

unique_ptr<expr_base> parser::expr_relational() {
    auto expr = binary_expr();
    while (!isEOF()) {
        auto &peek_operator = peek();
        if (peek_operator->has_type("LogicalPrecede")) {
            cout << "Peek Operator " << peek_operator->to_string() << endl;
            skip();
            expr = make_unique<expr_binary>(peek_operator, std::move(expr), binary_expr());
        } else {
            break;
        }
    }
    return expr;
}

unique_ptr<expr_base> parser::binary_expr() {
    auto expr = binary_precede_expr();
    while (!isEOF()) {
        auto &peek_operator = peek();
        if (peek_operator->has_type("Binary")) {
            cout << "Peek Operator " << peek_operator->to_string() << endl;
            skip();
            expr = make_unique<expr_binary>(peek_operator, std::move(expr), binary_precede_expr());
        } else {
            break;
        }
    }
    return expr;
}

unique_ptr<expr_base> parser::binary_precede_expr() {
    auto expr = read_expr();
    while (!isEOF()) {
        auto &peek_operator = peek();
        if (peek_operator->has_type("BinaryPrecede")) {
            cout << "Peek Operator " << peek_operator->to_string() << endl;
            skip();
            expr = make_unique<expr_binary>(peek_operator, std::move(expr), read_expr());
        } else {
            break;
        }
    }
    return expr;
}

unique_ptr<expr_base> parser::read_expr() {
    auto &token = next();
    if (token->has_type("Value")) {
        return make_unique<expr_token>(std::move(token));
    } else if (token->has_type("OpenExpr")) {
        auto expr = expr_decl();
        strict_match("CloseExpr");
        return expr;
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

void parser::back() {
    index--;
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
