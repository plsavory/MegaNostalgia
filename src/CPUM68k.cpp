//
// Created by Peter Savory on 05/10/2023.
//

#include "CPUM68k.h"

CPUM68k::CPUM68k(Memory *memory) {
    this->memory = memory;
}

int CPUM68k::execute() {
    throw GeneralException("Emulation code not yet implemented");
    return 0;
}