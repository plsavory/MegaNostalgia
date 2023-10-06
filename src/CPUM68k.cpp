//
// Created by Peter Savory on 05/10/2023.
//

#include "CPUM68k.h"

CPUM68k::CPUM68k(Memory *memory) {
    this->memory = memory;

    programCounter = 0x0;
    statusRegister = 0x0;
    cyclesTaken = 0;

    for (int i = 0; i < 8; i++) {
        gpRegisters[i] = 0;
        addressRegisters[i] = 0;
    }

    for (int i = 0; i <= 0xFFFF; i++) {
        opcodeHandlers[i] = &CPUM68k::invalidOpcodeHandler;
    }
}

void CPUM68k::reset() {

    programCounter = vGetResetInitialPC();
    supervisorStackPointer = vGetResetInitialSSP();
    userStackPointer = 0x0;
    statusRegister = 0x0;

    for (int i = 0; i < 8; i++) {
        gpRegisters[i] = 0;
        addressRegisters[i] = 0;
    }

    enterSupervisorMode();
}

int CPUM68k::execute() {
    cyclesTaken = 0;

    unsigned short opcode = fetchWord();
    currentOpcode = opcode; // Used for display reasons
    (this->*opcodeHandlers[opcode])();
    return 0;
}

bool CPUM68k::getFlag(int flag) {
    return Utils::testBit(flag, statusRegister);
}

void CPUM68k::setFlag(int flag, bool value) {
    Utils::setBit(flag, value, statusRegister);
}

unsigned char CPUM68k::readMemory(uint32_t location) {
    return memory->m68kRead(location);
}

unsigned short CPUM68k::readMemory16Bit(uint32_t location) {
    return memory->m68kRead16Bit(location);
}

uint32_t CPUM68k::readMemory32Bit(uint32_t location) {
    return memory->m68kRead32Bit(location);
}

void CPUM68k::writeMemory(uint32_t location, unsigned char value) {
    memory->m68kWrite(location, value);
}

void CPUM68k::writeMemory(uint32_t location, unsigned short value) {
    memory->m68kWrite(location, value);
}

void CPUM68k::writeMemory(uint32_t location, uint32_t value) {
    memory->m68kWrite(location, value);
}

bool CPUM68k::isInSupervisorMode() {
    return Utils::testBit(systemFlags.S, statusRegister);
}

void CPUM68k::enterSupervisorMode() {

    if (isInSupervisorMode()) {
        return;
    }

    Utils::setBit(systemFlags.S, true, statusRegister);
    userStackPointer = addressRegisters[M68KAddressRegister::SP];
    gpRegisters[M68KAddressRegister::SP] = supervisorStackPointer;
}

void CPUM68k::enterUserMode() {

    if (!isInSupervisorMode()) {
        return;
    }

    Utils::setBit(systemFlags.S, false, statusRegister);
    supervisorStackPointer = addressRegisters[M68KAddressRegister::SP];
    gpRegisters[M68KAddressRegister::SP] = userStackPointer;
}

unsigned char CPUM68k::fetchByte() {
    return readMemory(programCounter++);
}

unsigned short CPUM68k::fetchWord() {
    return (fetchByte() << 8) + fetchByte();
}