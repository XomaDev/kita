//
// Created by kumaraswamy on 4/24/24.
//

#ifndef KITA_FUNC_OBJ_H
#define KITA_FUNC_OBJ_H

#include <string>
#include <utility>
#include <vector>
#include "../bytecode.h"
#include "memory_manager.h"

using namespace std;

class memory_manager;
class visitable;

class func_obj {
    string name;
    u_int8_t args_size;
public:
    ulong visitable_index;

    func_obj(string name, u_int8_t args_size, ulong visitable_index)
                    : name(std::move(name)), args_size(args_size),
                    visitable_index(visitable_index) {
        // constructor initialized
    }

    static void prepare(uint8_t args, memory_manager *memory);
};


#endif //KITA_FUNC_OBJ_H
