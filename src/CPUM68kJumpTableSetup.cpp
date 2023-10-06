//
// Created by Peter Savory on 06/10/2023.
//

#include "CPUM68k.h"
#include <string>

unsigned short CPUM68k::parseInstructionMask(std::string mask) {
    std::string::iterator i;

    unsigned short sum = 0;

    for (i = mask.begin(); i != mask.end();
         i++) {
        if (*i == ' ') {
            continue; // Do nothing, empty space.
        }

        if (*i == '-' || *i == '0') {
            sum <<= 1;
            continue;
        }

        if (*i == '1') {
            sum <<= 1;
            sum += 1;
        }
    }

    return sum;
}