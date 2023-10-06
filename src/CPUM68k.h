//
// Created by Peter Savory on 05/10/2023.
//

#ifndef MEGANOSTALGIA_CPUM68K_H
#define MEGANOSTALGIA_CPUM68K_H

#include "Memory.h"

enum M68KVectors {
    ResetInitialSSP = 0x0,
    ResetInitialPC = 0x1,
    BusError = 0x2,
    AddressError = 0x3,
    IllegalInstruction = 0x4,
    ZeroDivide = 0x5,
    CHKInstruction = 0x6,
    TRAPVInstruction = 0x7,
    PrivilegeViolation = 0x8,
    Trace = 0x9,
    Line1010Emulator = 0xA,
    Line1111Emulator = 0xB,
    Reserved = 0xC,
    Reserved2 = 0xD,
    FormatError = 0xE,
    UninitializedInterruptVector = 0xF,
    SpuriousInterrupt = 0x18,
    InterruptAutovector = 0x19,
    TRAP = 0x20,
    UserInterruptVectors = 0x40
};

enum M68KGPRegister {
    D0, D1, D2, D3, D4, D5, D6, D7
};

enum M68KAddressRegister {
    A0, A1, A2, A3, A4, A5, A6, SP
};

/**
 *
    From https://www.cs.mcgill.ca/~cs573/fall2002/notes/lec273/lecture9/:
    Bit 0, known as the C bit, is the carry bit. It is set (to 1) whenever the result of an operation generates a carry coming from the most significant bit of the result, and cleared (set to 0) otherwise.
    Bit 1, known as the V bit, is the overflow bit. It is set if an operation results in arithmetic overflow in terms of two's complement arithmetic, i.e. when the result of an operation is too large or too small to be handled by the destination operand.
    Bit 2, known as the Z bit, is the zero bit. It is set when the result of an operation is zero (i.e. all the bits of the result are 0), and is cleared otherwise.
    Bit 3, known as the N bit, is the negative bit. It is set when the result of an operation is negative, and cleared when the result is positive. In other words, the N bit holds the value of the most significant bit of the result of the operation (remember two's complement numbers).
    Bit 4, known as the X bit, is the extend bit. It often has the same value as the C bit. It is used in multiple-precision arithmetic, i.e. arithmetic involving numbers greater than 32 bits in size, to hold the carry.
*/

struct M68KFlags {
    M68KFlags() {
        C = 0;
        V = 1;
        Z = 2;
        N = 3;
        X = 4;
    }

    int C;
    int V;
    int Z;
    int N;
    int X;
};

struct M68KSystemFlags {

    M68KSystemFlags() {
        IM1 = 8;
        IM2 = 9;
        IM3 = 10;
        S = 13;
        T = 15;
    }

    int IM1;
    int IM2;
    int IM3;
    int S;
    int T;
};

class CPUM68k {
public:
    CPUM68k(Memory *memory);

    int execute();

    void reset();

    static unsigned short parseInstructionMask(std::string mask);
private:

    Memory *memory;
    uint32_t programCounter;
    uint32_t supervisorStackPointer;
    uint32_t userStackPointer;
    uint32_t gpRegisters[8]{};
    uint32_t addressRegisters[8]{};
    int cyclesTaken;

    unsigned short statusRegister{};

    unsigned short currentOpcode;

    bool getFlag(int flag);

    void setFlag(int flag, bool value);

    void enterSupervisorMode();

    void enterUserMode();

    bool isInSupervisorMode();

    unsigned char fetchByte();

    unsigned short fetchWord();

    // Memory reading functions to prevent the headache of accidentally using Z80 reads later
    unsigned char readMemory(uint32_t location);
    unsigned short readMemory16Bit(uint32_t location);
    uint32_t readMemory32Bit(uint32_t location);
    void writeMemory(uint32_t location, unsigned char value);
    void writeMemory(uint32_t location, unsigned short value);
    void writeMemory(uint32_t location, uint32_t value);

    // Vectors
    uint32_t vGetResetInitialSSP();
    uint32_t vGetResetInitialPC();
    uint32_t vGetBusError();
    uint32_t vGetAddressError();
    uint32_t vGetIllegalInstruction();
    uint32_t vGetZeroDivide();
    uint32_t vGetCHKInstruction();
    uint32_t vGetTRAPVInstruction();
    uint32_t vGetPrivilegeViolation();
    uint32_t vGetTrace();
    uint32_t vGetLine1010Emulator();
    uint32_t vGetLine1111Emulator();
    uint32_t vGetUninitializedInterruptVector();
    uint32_t vGetInterruptAutovector(int level);
    uint32_t vGetTRAP(int id);
    uint32_t vGetUserInterruptVectors();

    M68KFlags flags;
    M68KSystemFlags systemFlags;

    typedef void (CPUM68k::*OpcodeHandler) ();
    OpcodeHandler opcodeHandlers[0x10000]; // This is going to be one huge table....

    // Jump table construction

    // Opcode handlers
    void invalidOpcodeHandler();


};

#endif //MEGANOSTALGIA_CPUM68K_H
