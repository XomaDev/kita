//
// Created by kumaraswamy on 4/24/24.
//

#include "func_obj.h"

void func_obj::prepare(uint8_t args, memory_manager *memory) {
    memory->push_frame();
    for (uint8_t i = 0; i < args; i++) {
        // pop() from the last frame, push it to current frame
        memory->relocate_last();
        memory->move_address();
    }
}
