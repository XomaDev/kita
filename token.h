//
// Created by kumaraswamy on 4/13/24.
//

#ifndef KITA_TOKEN_H
#define KITA_TOKEN_H


#include <string>
#include <utility>

using namespace std;

class token {
public:
    const string type;
    const string value;

    token(string type, string value) : type(std::move(type)), value(std::move(value)) {
        // constructor initialized
    }
    string str_repr();
};


#endif //KITA_TOKEN_H
