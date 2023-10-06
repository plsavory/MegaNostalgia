//
// Created by Peter Savory on 05/10/2023.
//

#ifndef MEGANOSTALGIA_CPUM68K_H
#define MEGANOSTALGIA_CPUM68K_H

#include "Memory.h"

class CPUM68k {
public:
    CPUM68k(Memory *memory);

    int execute();

private:

    Memory *memory;
};

#endif //MEGANOSTALGIA_CPUM68K_H
