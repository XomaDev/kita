//
// Created by kumaraswamy on 4/13/24.
//

#include <algorithm>
#include "token.h"

using namespace std;

string token::to_string() {
    return "{kita_type='" + types_str_repr() + "' value='" + value + "'}";
}

string token::types_str_repr() {
    string types_repr = "[";
    for (const string& type : types) {
        types_repr += type + " ";
    }
    types_repr += "]";
    return types_repr;
}

bool token::has_type(const string& match) {
    return find(types.begin(), types.end(), match) != types.end();
}
