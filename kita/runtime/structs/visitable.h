//
// Created by kumaraswamy on 4/25/24.
//

#ifndef KITA_VISITABLE_H
#define KITA_VISITABLE_H

#include <functional>
#include "../runtime.h"

using namespace std;

struct visitable {
    function<int()> execute;
};

#endif //KITA_VISITABLE_H
