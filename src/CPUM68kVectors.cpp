//
// Created by Peter Savory on 06/10/2023.
//

#include "CPUM68k.h"

uint32_t CPUM68k::vGetResetInitialSSP() {
    return readMemory32Bit(0x0);
}

uint32_t CPUM68k::vGetResetInitialPC() {
    return readMemory32Bit(0x4);
}

uint32_t CPUM68k::vGetBusError() {
    return readMemory32Bit(0x8);
}

uint32_t CPUM68k::vGetAddressError() {
    return readMemory32Bit(0xC);
}

uint32_t CPUM68k::vGetIllegalInstruction() {
    return readMemory32Bit(0x10);
}

uint32_t CPUM68k::vGetZeroDivide() {
    return readMemory32Bit(0x14);
}

uint32_t CPUM68k::vGetCHKInstruction() {
    return readMemory32Bit(0x18);
}

uint32_t CPUM68k::vGetTRAPVInstruction() {
    return readMemory32Bit(0x1C);
}

uint32_t CPUM68k::vGetPrivilegeViolation() {
    return readMemory32Bit(0x20);
}

uint32_t CPUM68k::vGetTrace() {
    return readMemory32Bit(0x24);
}

uint32_t CPUM68k::vGetLine1010Emulator() {
    return readMemory32Bit(0x28);
}

uint32_t CPUM68k::vGetLine1111Emulator() {
    return readMemory32Bit(0x2C);
}

uint32_t CPUM68k::vGetUninitializedInterruptVector() {
    return readMemory32Bit(0x3C);
}

uint32_t CPUM68k::vGetInterruptAutovector(int level) {

    if (level < 1 || level > 7) {
        throw M68KException(Utils::implodeString({"Unsupported interrupt autovector '", std::to_string(level), "'"}));
    }

    level -= 1;

    return readMemory32Bit(0x64 + (level * 4));
}

uint32_t CPUM68k::vGetTRAP(int id) {
    if (id < 1 || id > 15) {
        throw M68KException(Utils::implodeString({"Unsupported TRAP '", std::to_string(id), "'"}));
    }

    id -= 1;

    return readMemory32Bit(0x80 + (id * 4));
}