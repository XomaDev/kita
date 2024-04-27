//
// Created by kumaraswamy on 4/27/24.
//

#ifndef KITA_NAME_RESOLVER_H
#define KITA_NAME_RESOLVER_H

#include <cstdlib>
#include <string>
#include <vector>
#include "address.h"

using namespace std;

class name_resolver {
    ulong depth = 0;
    vector<string> *names;
    vector<vector<string> *> frames;
public:
    name_resolver() {
        enter_frame();
    }
    void enter_frame();
    void exit_fame();

    void declare(const string& name);
    address resolve(const string& name, bool static_resolution);
};


#endif //KITA_NAME_RESOLVER_H
