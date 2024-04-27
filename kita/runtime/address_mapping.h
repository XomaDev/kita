//
// Created by kumaraswamy on 4/27/24.
//

#ifndef KITA_ADDRESS_MAPPING_H
#define KITA_ADDRESS_MAPPING_H


#include <cstdlib>
#include <string>
#include <vector>
#include <memory>

using namespace std;

class address_mapping {
    ulong depth = 0;
    vector<string>* scope_names;
    vector<vector<string> *> frames;
public:
    address_mapping() {
        enter_frame();
    }
    ~address_mapping();

    void enter_frame();
    void exit_frame();

    void declare(const string& name);
    pair<ulong, ulong> dereference(const string& name, bool static_address);
};


#endif //KITA_ADDRESS_MAPPING_H
