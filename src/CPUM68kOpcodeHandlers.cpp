//
// Created by Peter Savory on 06/10/2023.
//

#include "CPUM68k.h"
#include "Exceptions.h"

void CPUM68k::invalidOpcodeHandler() {
    // TODO this should probably call the unknown opcode vector
    throw M68KException(Utils::implodeString({"Unknown opcode ", Utils::formatHexNumber(currentOpcode)}));
}