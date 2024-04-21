//
// Created by kumaraswamy on 4/13/24.
//

#ifndef KITA_KITA_TYPE_H
#define KITA_KITA_TYPE_H


#include <string>

using namespace std;

class kita_type {

public:
    static const vector<string> Number;
    static const vector<string> String;

    static const vector<string> Identifier;
};

const vector<string> kita_type::Number = {"Number", "Value"};
const vector<string> kita_type::String = {"String", "Value"};

const vector<string> kita_type::Identifier = {"Identifier", "Value"};

#endif //KITA_KITA_TYPE_H
