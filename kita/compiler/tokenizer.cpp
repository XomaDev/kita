//
// Created by kumaraswamy on 4/13/24.
//

#include <fstream>
#include <iostream>
#include <sstream>
#include "tokenizer.h"
#include "kita_type.h"

using namespace std;

void tokenizer::load_names() {
    ifstream file(static_names);
    if (!file.is_open()) {
        throw runtime_error("Unable to open file '" + static_names + "'");
    }
    string line;
    for (;;) {
        if (!getline(file, line, ' ')) {
            break;
        }
        string type_match = line;
        if (!getline(file, line)) {
            throw runtime_error("Expected type_name");
        }
        vector<string> types;
        string a_value;
        istringstream types_stream(line);
        while (getline(types_stream, a_value, ':')) {
            types.emplace_back(a_value);
        }
        static_declaration[type_match] = types;
    }
    file.close();
}

vector<unique_ptr<token>> tokenizer::tokenize() {
    while (!isEOF()) {
        scan_tokens();
    }
    for (const auto &token : tokens) {
        cout << token->to_string() << endl;
    }
    return std::move(tokens);
}

void tokenizer::scan_tokens() {
    u_char c = next();
    if (c == ' ' || c == '\n') {
        return;
    }
    // go back a bit and try searching
    back();
    string search_str;
    while (!isEOF() && static_declaration.find(search_str + (char) peek()) != static_declaration.end()) {
        search_str += (char) next();
    }
    if (search_str.empty()) {
        // remember, we have done `back()`!
        if (is_alpha(c)) {
            parse_alpha();
        } else if (is_numeric(c)) {
            parse_numeric();
        } else {
            throw runtime_error("Unknown character '" + string(1, (char) c) + "'");
        }
    } else {
        auto token = new class token(static_declaration[search_str], search_str);
        tokens.emplace_back(token);
    }
}

void tokenizer::parse_alpha() {
    string alpha;
    while (!isEOF() && is_alpha(peek())) {
        alpha += (char) next();
    }
    vector<string> types =
            static_declaration.find(alpha) != static_declaration.end()
            ? static_declaration[alpha]
            : kita_type::Identifier;
    tokens.emplace_back(new class token(types, alpha));
}

void tokenizer::parse_numeric() {
    string numeric;
    while (!isEOF() && is_numeric(peek())) {
        numeric += (char) next();
    }
    tokens.emplace_back(new class token(kita_type::Number, numeric));
}

void tokenizer::back() {
    iter_index--;
}

void tokenizer::skip() {
    iter_index++;
}

u_char tokenizer::next() {
    return source[iter_index++];
}

u_char tokenizer::peek() {
    return source[iter_index];
}

bool tokenizer::isEOF() const {
    return iter_index == source_length;
}

bool tokenizer::is_alpha(u_char c) {
    return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z';
}

bool tokenizer::is_numeric(u_char c) {
    return c >= '0' && c <= '9';
}