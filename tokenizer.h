//
// Created by kumaraswamy on 4/13/24.
//

#ifndef KITA_TOKENIZER_H
#define KITA_TOKENIZER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "token.h"

using namespace std;

class tokenizer {
    const string &static_names;

    const string &source;
    int iter_index = 0;
    const int source_length;

    unordered_map<string, string> static_declaration;

    void scan_tokens();

    void parse_alpha();
    void parse_numeric();

    static bool is_alpha(u_char c);
    static bool is_numeric(u_char c);

    void back();
    void skip();
    u_char next();
    u_char peek();
    bool isEOF() const;

public:
    vector<token *> tokens;
    tokenizer(const string &static_names, const string &source)
                    : static_names(static_names), source(source), source_length(source.length()) {
        // constructor initialized
    }
    ~tokenizer() {
        for (auto token: tokens) {
            delete token;
        }
    }
    void load_names();
    void tokenize();
};


#endif //KITA_TOKENIZER_H
