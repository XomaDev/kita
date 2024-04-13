//
// Created by kumaraswamy on 4/13/24.
//

#ifndef KITA_KITA_TYPE_H
#define KITA_KITA_TYPE_H


#include <string>

using namespace std;

class kita_type {

public:
    static const string Identifier;
    static const string IntClass;
    static const string Display;

    static const string Number;

    static const string Kita;
    static const string Colon;
    static const string Add;
    static const string Negate;
    static const string Slash;
    static const string Asterisk;
};

const string kita_type::IntClass = "IntClass";
const string kita_type::Identifier = "Identifier";
const string kita_type::Display = "Display";

const string kita_type::Number = "Number";

const string kita_type::Kita = "Kita";
const string kita_type::Colon = "Colon";
const string kita_type::Add = "Add";
const string kita_type::Negate = "Negate";
const string kita_type::Slash = "Slash";
const string kita_type::Asterisk = "Asterisk";

#endif //KITA_KITA_TYPE_H
