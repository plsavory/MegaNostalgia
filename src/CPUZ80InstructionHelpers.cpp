#include <iostream>
#include "Memory.h"
#include "CPUZ80.h"
#include "Utils.h"

/**
 * [cpuZ80::ldReg8 Load 8-bit value into destination]
 * @param dest  [Reference to destination location]
 * @param value [Value to be inserted into destination]
 */
void CPUZ80::ldReg8(unsigned char &dest, unsigned char value) {
    dest = value;
    readValue = value;
}

void CPUZ80::ldReg8(unsigned char &dest, unsigned char value, bool modifyFlags) {
    ldReg8(dest, value);

    if (!modifyFlags) {
        return;
    }

    setFlag(Z80CPUFlag::subtractNegative, false);
    setFlag(Z80CPUFlag::halfCarry, false);
    setFlag(Z80CPUFlag::zero, dest == 0);
    setFlag(Z80CPUFlag::sign, Utils::testBit(7, dest));
}

/**
 * [cpuZ80::ldReg16 Load 16-bit value into destination register]
 * @param dest  [Reference to destination location]
 * @param value [Value to be inserted into destination]
 */
void CPUZ80::ldReg16(unsigned short &dest, unsigned short value, bool modifyFlags) {
    dest = value;

    #ifdef DEBUG_VALUES
    readValue = value;
    #endif

    if (!modifyFlags) {
        return;
    }

    setFlag(Z80CPUFlag::halfCarry, false);
    setFlag(Z80CPUFlag::subtractNegative, false);
    setFlag(Z80CPUFlag::overflowParity, iff1 || iff2);
}

void CPUZ80::ldReg16(unsigned short &dest, unsigned short value) {
    ldReg16(dest, value, false);
}

void CPUZ80::addAdc8Bit(unsigned char &dest, unsigned char value, bool withCarry) {
    unsigned char originalValue = dest;

    unsigned char originalValueToAdd = value;

    unsigned short result = dest + value + (withCarry && getFlag(Z80CPUFlag::carry) ? 1 : 0);

    unsigned short carryBits = (originalValue ^ originalValueToAdd ^ result);
    dest = (unsigned char)result;
    setFlag(Z80CPUFlag::zero, dest == 0);
    setFlag(Z80CPUFlag::overflowParity, (originalValue & 0x80) == (originalValueToAdd & 0x80) && (originalValue & 0x80) != (result & 0x80));
    setFlag(Z80CPUFlag::subtractNegative, false);
    setFlag(Z80CPUFlag::halfCarry, carryBits & 0x10);
    setFlag(Z80CPUFlag::carry, carryBits & 0x100);
    setFlag(Z80CPUFlag::sign, Utils::testBit(7, dest));
    handleUndocumentedFlags(dest);
}
/**
 * [cpuZ80::add ADD opcode handler]
 * @param dest  [Destination (Usually register A)]
 * @param value [Value to add with what is stored in dest]
 */
void CPUZ80::add8Bit(unsigned char &dest, unsigned char value) {
    addAdc8Bit(dest, value, false);
}

void CPUZ80::adc8Bit(unsigned char &dest, unsigned char value) {
    addAdc8Bit(dest, value, true);
}

void CPUZ80::addAdc16Bit(unsigned short &dest, unsigned short value, bool withCarry) {
    unsigned short originalValue = dest;

    unsigned short originalValueToAdd = value;

    if (withCarry) {
        value += getFlag(Z80CPUFlag::carry) ? 1 : 0;
    }

    unsigned long result = dest + value;
    dest = result & 0xFFFF;
    setFlag(Z80CPUFlag::subtractNegative, false);
    setFlag(Z80CPUFlag::halfCarry, (originalValue ^ dest ^ originalValueToAdd) & 0x1000);
    setFlag(Z80CPUFlag::carry, result > 0xFFFF);

    if (withCarry) {
        setFlag(Z80CPUFlag::sign, Utils::testBit(15, dest));
        setFlag(Z80CPUFlag::zero, dest == 0);
        setFlag(Z80CPUFlag::overflowParity, (originalValue & 0x8000) == (originalValueToAdd & 0x8000) && (originalValueToAdd & 0x8000) != (dest & 0x8000));
    }

    handleUndocumentedFlags((unsigned char)(dest >> 8));
    gpRegisters[Z80Registers::WZ].whole = originalValue + 1;
}

void CPUZ80::add16Bit(unsigned short &dest, unsigned short value) {
    addAdc16Bit(dest, value, false);
}

void CPUZ80::adc16Bit(unsigned short &dest, unsigned short value) {
    addAdc16Bit(dest, value, true);
}

void CPUZ80::subSbc8Bit(unsigned char &dest, unsigned char value, bool withCarry) {

    unsigned char originalSubtractValue = value;

    unsigned char originalRegisterValue = dest;
    unsigned short result = dest - value - (withCarry && getFlag(Z80CPUFlag::carry) ? 1 : 0);
    unsigned short carryBits = (originalRegisterValue ^ originalSubtractValue ^ result);
    dest = (unsigned char) result;
    setFlag(Z80CPUFlag::zero, dest == 0);
    setFlag(Z80CPUFlag::subtractNegative, true);
    setFlag(Z80CPUFlag::carry, carryBits & 0x100);
    setFlag(Z80CPUFlag::halfCarry, carryBits & 0x10);
    setFlag(Z80CPUFlag::sign, Utils::testBit(7, dest));
    setFlag(Z80CPUFlag::overflowParity, ((originalRegisterValue & 0x80) != (originalSubtractValue & 0x80)) && ((dest & 0x80) == (originalSubtractValue & 0x80)));
    handleUndocumentedFlags(dest);
}

void CPUZ80::subSbc16Bit(unsigned short &dest, unsigned short value, bool withCarry) {
    unsigned short originalValue = dest;

    if (withCarry) {
        value += getFlag(Z80CPUFlag::carry) ? 1 : 0;
    }

    dest = dest - value;
    unsigned char highResult = dest >> 8;
    setFlag(Z80CPUFlag::zero, dest == 0);
    setFlag(Z80CPUFlag::subtractNegative, true);
    setFlag(Z80CPUFlag::carry, originalValue < value);
    setFlag(Z80CPUFlag::halfCarry, ((originalValue ^ dest ^ value) >> 8) & 0x10);
    setFlag(Z80CPUFlag::sign, Utils::testBit(15, dest));
    setFlag(Z80CPUFlag::overflowParity, ((originalValue & 0x8000) != (value & 0x8000)) && ((dest & 0x8000) == (value & 0x8000)));
    handleUndocumentedFlags(highResult);
    gpRegisters[Z80Registers::WZ].whole = originalValue + 1;
}

/**
 * [cpuZ80::sub sub opcode handler]
 * @param dest  [Destination (Usually register A)]
 * @param value [Value to subtract with what is stored in dest]
 */
void CPUZ80::sub8Bit(unsigned char &dest, unsigned char value) {
    subSbc8Bit(dest, value, false);
}

void CPUZ80::sbc8Bit(unsigned char &dest, unsigned char value) {
    subSbc8Bit(dest, value, true);
}

void CPUZ80::dec8Bit(unsigned char &dest) {
    dest = getDec8BitValue(dest);
}

unsigned char CPUZ80::getDec8BitValue(unsigned char initialValue) {
    unsigned char newValue = initialValue - 1;
    setFlag(Z80CPUFlag::yf, Utils::testBit(5, newValue));
    setFlag(Z80CPUFlag::xf, Utils::testBit(3, newValue));
    setFlag(Z80CPUFlag::zero, newValue == 0);
    setFlag(Z80CPUFlag::overflowParity, newValue == 0x7F);
    setFlag(Z80CPUFlag::subtractNegative, true);
    setFlag(Z80CPUFlag::halfCarry, getHalfCarry(initialValue, newValue));
    setFlag(Z80CPUFlag::sign, Utils::testBit(7, newValue));
    return newValue;
}

void CPUZ80::sbc16Bit(unsigned short &dest, unsigned short value) {
    subSbc16Bit(dest, value, true);
}

void CPUZ80::and8Bit(unsigned char &dest, unsigned char value) {
    dest = dest & value;
    setFlag(Z80CPUFlag::carry, false);
    setFlag(Z80CPUFlag::subtractNegative, false);
    setFlag(Z80CPUFlag::halfCarry, true);
    setFlag(Z80CPUFlag::zero, dest == 0);
    setFlag(Z80CPUFlag::sign, Utils::testBit(7, dest));
    setFlag(Z80CPUFlag::overflowParity, getParity(dest));
    handleUndocumentedFlags(dest);
}

void CPUZ80::or8Bit(unsigned char &dest, unsigned char value) {
    dest = dest | value;
    setFlag(Z80CPUFlag::carry, false);
    setFlag(Z80CPUFlag::subtractNegative, false);
    setFlag(Z80CPUFlag::halfCarry, false);
    setFlag(Z80CPUFlag::sign, Utils::testBit(7, dest));
    setFlag(Z80CPUFlag::zero, dest == 0);
    setFlag(Z80CPUFlag::overflowParity, getParity(dest));
    handleUndocumentedFlags(dest);
}

void CPUZ80::setInterruptMode(unsigned char mode) {
    interruptMode = mode;
    cyclesTaken = 8; // All im operations (as far as I know) take 8 cycles
}

/**
 * [CPUZ80::jpCondition Perform a conditional jump]
 * @param condition [The condition which needs to be true]
 * @param location  [Memory location to jump to]
 */
void CPUZ80::jpCondition(JPCondition condition) {

    unsigned short jumpLocation = build16BitNumber();
    gpRegisters[Z80Registers::WZ].whole = jumpLocation;
    cyclesTaken = 10; // TODO: Ensure that this is the same for all conditional jump instructions

    if (!hasMetJumpCondition(condition)) {
        return;
    }

    programCounter = jumpLocation;
}

void CPUZ80::jrCondition(JPCondition condition) {

    signed char offset = signedNB();
    unsigned short address = programCounter + offset;
    gpRegisters[Z80Registers::WZ].whole = address;

    if (!hasMetJumpCondition(condition)) {
        cyclesTaken = 7;
        return;
    }

    programCounter = address;
    cyclesTaken = 12;
    #ifdef DEBUG_VALUES
    readValue = (unsigned char)offset;
    #endif
}

void CPUZ80::retCondition(JPCondition condition) {
    if (!hasMetJumpCondition(condition)) {
        cyclesTaken = 5;
        return;
    }

    cyclesTaken = 11;
    programCounter = popStack16();
    gpRegisters[Z80Registers::WZ].whole = programCounter;
}

void CPUZ80::jr() {
    programCounter += signedNB();
    gpRegisters[Z80Registers::WZ].whole = programCounter;
    cyclesTaken = 12;
}

/**
 * [CPUZ80::jpImm Jumps to the given memory location immediately following the opcode]
 */
void CPUZ80::jpImm() {
    programCounter = build16BitNumber();
    gpRegisters[Z80Registers::WZ].whole = programCounter;
    #ifdef DEBUG_VALUES
    readValue = programCounter;
    #endif
}

/**
 * [CPUZ80::xor XOR value with register A]
 * @param value [description]
 */
void CPUZ80::exclusiveOr(unsigned char &dest, unsigned char value) {
    unsigned char result = gpRegisters[Z80Registers::AF].hi ^ value;
    dest = result;
    setFlag(Z80CPUFlag::carry, false);
    setFlag(Z80CPUFlag::subtractNegative, false);
    setFlag(Z80CPUFlag::halfCarry, false);
    setFlag(Z80CPUFlag::zero, result == 0);
    setFlag(Z80CPUFlag::sign, Utils::testBit(7, dest));
    setFlag(Z80CPUFlag::overflowParity, getParity(result));
    handleUndocumentedFlags(result);
}

void CPUZ80::inc16Bit(unsigned short &target) {
    ++target;
}

void CPUZ80::inc8Bit(unsigned char &target) {
    target = getInc8BitValue(target);
}

unsigned char CPUZ80::getInc8BitValue(unsigned char initialValue) {
    unsigned char newValue = initialValue+1;
    setFlag(Z80CPUFlag::subtractNegative, false);
    setFlag(Z80CPUFlag::overflowParity, newValue == 0x80);
    setFlag(Z80CPUFlag::halfCarry, (initialValue & 0xF) == 0xF);
    setFlag(Z80CPUFlag::sign, Utils::testBit(7, newValue));
    setFlag(Z80CPUFlag::zero, newValue == 0);
    handleUndocumentedFlags(newValue);
    return newValue;
}

void CPUZ80::compare8Bit(unsigned char valueToSubtract) {
    unsigned char aValue = gpRegisters[Z80Registers::AF].hi;
    subSbc8Bit(aValue, valueToSubtract, false);
    handleUndocumentedFlags(valueToSubtract);
    #ifdef DEBUG_VALUES
    readValue = valueToSubtract;
    #endif
}

void CPUZ80::ini(bool increment) {
    writeMemory(gpRegisters[Z80Registers::HL].whole, portIn(gpRegisters[Z80Registers::BC].lo));

    gpRegisters[Z80Registers::HL].whole += increment ? 1 : -1;

    setFlag(Z80CPUFlag::subtractNegative, true);
    setFlag(Z80CPUFlag::zero, true);

    cyclesTaken = 16;
    gpRegisters[Z80Registers::WZ].whole = gpRegisters[Z80Registers::BC].whole + (increment ? 1 : -1);
    --gpRegisters[Z80Registers::BC].hi;
}

void CPUZ80::inir(bool increment) {

    ini(increment);

    if (gpRegisters[Z80Registers::BC].hi == 0) {
        return;
    }

    programCounter -= 2;
    cyclesTaken = 21;
}

void CPUZ80::cpi(bool increment) {
    unsigned char value = readMemory(gpRegisters[Z80Registers::HL].whole);
    unsigned char originalRegisterValue = gpRegisters[Z80Registers::AF].hi;

    unsigned char result = originalRegisterValue - value;
    unsigned char carryBits = (originalRegisterValue ^ value ^ result);
    unsigned char finalResult = result;

    setFlag(Z80CPUFlag::sign, Utils::testBit(7, result));
    setFlag(Z80CPUFlag::zero, result == 0);
    setFlag(Z80CPUFlag::halfCarry, carryBits & 0x10);

    if (getFlag(Z80CPUFlag::halfCarry)) {
        finalResult -= 1;
    }

    setFlag(Z80CPUFlag::xf, Utils::testBit(3, finalResult));
    setFlag(Z80CPUFlag::yf, Utils::testBit(1, finalResult));

    --gpRegisters[Z80Registers::BC].whole;
    setFlag(Z80CPUFlag::overflowParity, gpRegisters[Z80Registers::BC].whole != 0);
    setFlag(Z80CPUFlag::subtractNegative, true);
    cyclesTaken = 16;

    gpRegisters[Z80Registers::HL].whole += (increment ? 1 : -1);
    gpRegisters[Z80Registers::WZ].whole += (increment ? 1 : -1);
}

void CPUZ80::cpir(bool increment) {
    cpi(increment);

    if (gpRegisters[Z80Registers::BC].whole == 0) {
        return;
    }

    if (getFlag(Z80CPUFlag::zero)) {
        return;
    }

    cyclesTaken = 21;
    programCounter -= 2;
    gpRegisters[Z80Registers::WZ].whole = programCounter + 1;
}

void CPUZ80::ldi(bool increment) {
    unsigned char valueToWrite = readMemory(gpRegisters[Z80Registers::HL].whole);
    writeMemory(gpRegisters[Z80Registers::DE].whole, valueToWrite);

    unsigned short incrementValue = increment ? 1 : -1;
    gpRegisters[Z80Registers::DE].whole += incrementValue;
    gpRegisters[Z80Registers::HL].whole += incrementValue;

    --gpRegisters[Z80Registers::BC].whole;

    unsigned char n = valueToWrite + gpRegisters[Z80Registers::AF].hi;
    setFlag(Z80CPUFlag::subtractNegative, false);
    setFlag(Z80CPUFlag::halfCarry, false);
    setFlag(Z80CPUFlag::yf, Utils::testBit(1, n));
    setFlag(Z80CPUFlag::xf, Utils::testBit(3, n));
    setFlag(Z80CPUFlag::overflowParity, gpRegisters[Z80Registers::BC].whole != 0);
    cyclesTaken = 16;
}

void CPUZ80::ldir(bool increment) {

    ldi(increment);

    if (gpRegisters[Z80Registers::BC].whole == 0) {
        return;
    }

    programCounter -= 2;
    gpRegisters[Z80Registers::WZ].whole = programCounter + 1;
    cyclesTaken = 21;
}

void CPUZ80::outi(bool increment) {
    unsigned char valueToWrite = readMemory(gpRegisters[Z80Registers::HL].whole);
    portOut(gpRegisters[Z80Registers::BC].lo, valueToWrite);

    if (increment) {
        gpRegisters[Z80Registers::HL].whole += 1;
    } else {
        gpRegisters[Z80Registers::HL].whole -= 1;
    }

    --gpRegisters[Z80Registers::BC].hi;
    setFlag(Z80CPUFlag::yf, false);
    setFlag(Z80CPUFlag::xf, false);
    setFlag(Z80CPUFlag::sign, Utils::testBit(7, gpRegisters[Z80Registers::BC].hi));
    setFlag(Z80CPUFlag::zero, gpRegisters[Z80Registers::BC].hi == 0);

    gpRegisters[Z80Registers::WZ].whole = gpRegisters[Z80Registers::BC].whole + (increment ? 1 : -1);

    unsigned short k = valueToWrite + gpRegisters[Z80Registers::HL].lo;
    setFlag(Z80CPUFlag::subtractNegative, Utils::testBit(7, valueToWrite));
    setFlag(Z80CPUFlag::halfCarry, k > 0xFF);
    setFlag(Z80CPUFlag::carry, k > 0xFF);
    setFlag(Z80CPUFlag::overflowParity, std::bitset<8>((k & 0x7) ^ gpRegisters[Z80Registers::BC].hi).count() % 2 == 0);

    cyclesTaken = 16;
}

void CPUZ80::otir(bool increment) {

    outi(increment);

    if (gpRegisters[Z80Registers::BC].hi == 0) {
        return;
    }

    programCounter -= 2;
    cyclesTaken = 21;
}

void CPUZ80::call(unsigned short location, bool conditionMet) {

    #ifdef DEBUG_VALUES
    readValue = location;
    #endif

    gpRegisters[Z80Registers::WZ].whole = location;

    if (!conditionMet) {
        cyclesTaken = 10;
        return;
    }

    pushStack(programCounter);
    programCounter = location;
    cyclesTaken = 17;
}

void CPUZ80::call(unsigned short location) {
    call(location, true);
}

void CPUZ80::callCondition(JPCondition condition) {
    call(build16BitNumber(), hasMetJumpCondition(condition));
}

void CPUZ80::rst(unsigned short location) {
    pushStack(programCounter);
    programCounter = location;
    gpRegisters[Z80Registers::WZ].whole = programCounter;
    cyclesTaken = 11;

    #ifdef DEBUG_VALUES
    readValue = programCounter;
    #endif
}

void CPUZ80::store(unsigned short location, unsigned char hi, unsigned char lo) {
    writeMemory(location, lo);
    writeMemory(location+1, hi);
}

void CPUZ80::djnz() {

    signed char offset = signedNB();
    gpRegisters[Z80Registers::WZ].whole = programCounter + offset;

    if (--gpRegisters[Z80Registers::BC].hi == 0) {
        cyclesTaken = 7;
        return;
    }

    cyclesTaken = 13;
    programCounter += offset;
}

void CPUZ80::dec16Bit(unsigned short &target) {
    --target;
    // TODO handle any flag changes that are required, so far it seems like this instruction never does though.
}

unsigned char CPUZ80::shiftLeft(unsigned char dest, Z80ShiftBitToCopy lowBitCopyMode, bool updateAllFlags) {
    bool previousCarryFlagValue = getFlag(Z80CPUFlag::carry);
    bool bit7 = Utils::testBit(7, dest);

    setFlag(Z80CPUFlag::carry, bit7);

    bool copyBitValue = false;

    switch (lowBitCopyMode) {
        case Z80ShiftBitToCopy::copyOutgoingValue:
            copyBitValue = bit7;
            break;
        case Z80ShiftBitToCopy::copyCarryFlag:
            copyBitValue = previousCarryFlagValue;
            break;
        case Z80ShiftBitToCopy::preserve:
            copyBitValue = Utils::testBit(0, dest);
        case Z80ShiftBitToCopy::copyZero:
            break;
        case Z80ShiftBitToCopy::copyOne:
            copyBitValue = true;
            break;
    }

    dest <<= 1;

    Utils::setBit(0, copyBitValue, dest);

    setFlag(Z80CPUFlag::subtractNegative, false);
    setFlag(Z80CPUFlag::halfCarry, false);
    handleUndocumentedFlags(dest);

    if (updateAllFlags) {
        setFlag(Z80CPUFlag::sign, Utils::testBit(7, dest));
        setFlag(Z80CPUFlag::zero, dest == 0);
        setFlag(Z80CPUFlag::overflowParity, getParity(dest));
    }
    return dest;
}

unsigned char CPUZ80::rlc(unsigned char dest) {
    // 8-bit rotation to the left. The bit leaving on the left is copied into the carry, and to bit 0.
    return shiftLeft(dest, Z80ShiftBitToCopy::copyOutgoingValue, true);
}

void CPUZ80::rlca() {
    // Performs RLC A much quicker, and modifies the flags differently.
    gpRegisters[Z80Registers::AF].hi = shiftLeft(gpRegisters[Z80Registers::AF].hi, Z80ShiftBitToCopy::copyOutgoingValue, false);
}

unsigned char CPUZ80::rl(unsigned char dest) {
    // 9-bit rotation to the left. the register's bits are shifted left. The carry value is put into 0th bit of the register, and the leaving 7th bit is put into the carry.
    return shiftLeft(dest, Z80ShiftBitToCopy::copyCarryFlag, true);
}

void CPUZ80::rla() {
    // Performs an RL A, but is much faster and S, Z, and P/V flags are preserved.
    gpRegisters[Z80Registers::AF].hi = shiftLeft(gpRegisters[Z80Registers::AF].hi, Z80ShiftBitToCopy::copyCarryFlag, false);
}

unsigned char CPUZ80::sla(unsigned char dest) {
    // Shift all bits left, copy a 0 onto the low bit
    return shiftLeft(dest, Z80ShiftBitToCopy::copyZero, true);
}

unsigned char CPUZ80::sll(unsigned char dest) {
    // An "undocumented" instruction. Functions like sla, except a 1 is inserted into the low bit.
    return shiftLeft(dest, Z80ShiftBitToCopy::copyOne, true);
}

unsigned char CPUZ80::shiftRight(unsigned char dest, Z80ShiftBitToCopy highBitCopyMode, bool updateAllFlags) {
    bool previousCarryFlagValue = getFlag(Z80CPUFlag::carry);
    bool bit0 = Utils::testBit(0, dest);

    setFlag(Z80CPUFlag::carry, bit0);

    bool copyBitValue = false;

    switch (highBitCopyMode) {
        case Z80ShiftBitToCopy::copyOutgoingValue:
            copyBitValue = bit0;
            break;
        case Z80ShiftBitToCopy::copyCarryFlag:
            copyBitValue = previousCarryFlagValue;
            break;
        case Z80ShiftBitToCopy::preserve:
            copyBitValue = Utils::testBit(7, dest);
            break;
        case Z80ShiftBitToCopy::copyZero:
            break;
        case Z80ShiftBitToCopy::copyOne:
            copyBitValue = true;
            break;
    }

    dest >>= 1;

    Utils::setBit(7, copyBitValue, dest);

    setFlag(Z80CPUFlag::subtractNegative, false);
    setFlag(Z80CPUFlag::halfCarry, false);
    handleUndocumentedFlags(dest);

    if (updateAllFlags) {
        setFlag(Z80CPUFlag::sign, Utils::testBit(7, dest));
        setFlag(Z80CPUFlag::zero, dest == 0);
        setFlag(Z80CPUFlag::overflowParity, getParity(dest));
    }

    return dest;
}

unsigned char CPUZ80::rrc(unsigned char dest) {
    // 8-bit rotation to the right. the bit leaving on the right is copied into the carry, and into bit 7.
    return shiftRight(dest, Z80ShiftBitToCopy::copyOutgoingValue, true);
}

void CPUZ80::rrca() {
    // Performs RRC A faster and modifies the flags differently.
    gpRegisters[Z80Registers::AF].hi = shiftRight(gpRegisters[Z80Registers::AF].hi, Z80ShiftBitToCopy::copyOutgoingValue, false);
}

unsigned char CPUZ80::rr(unsigned char dest) {
    // 9-bit rotation to the right. The carry is copied into bit 7, and the bit leaving on the right is copied into the carry.
    return shiftRight(dest, Z80ShiftBitToCopy::copyCarryFlag, true);
}

void CPUZ80::rra() {
    // Performs RR A faster, and modifies the flags differently.
    gpRegisters[Z80Registers::AF].hi = shiftRight(gpRegisters[Z80Registers::AF].hi, Z80ShiftBitToCopy::copyCarryFlag, false);
}

unsigned char CPUZ80::sra(unsigned char dest) {
    // Arithmetic shift right 1 bit, bit 0 goes to carry flag, bit 7 remains unchanged.
    return shiftRight(dest, Z80ShiftBitToCopy::preserve, true);
}

unsigned char CPUZ80::srl(unsigned char dest) {
    // Like SRA, except a 0 is put into bit 7. The bits are all shifted right, with bit 0 put into the carry flag.
    return shiftRight(dest, Z80ShiftBitToCopy::copyZero, true);
}

void CPUZ80::exchange8Bit(unsigned char &register1, unsigned char &register2) {
    unsigned char originalRegister1 = register1;
    register1 = register2;
    register2 = originalRegister1;
}

void CPUZ80::exchange16Bit(unsigned short &register1, unsigned short &register2) {
    unsigned short originalRegister1 = register1;
    register1 = register2;
    register2 = originalRegister1;

}

void CPUZ80::daa(unsigned char &dest) {

    bool subtract = getFlag(Z80CPUFlag::subtractNegative);

    unsigned char diff = 0;

    if (((dest & 0x0F) > 0x09) || getFlag(Z80CPUFlag::halfCarry)) {
        diff += 0x6;
    }

    if ((dest > 0x99) || getFlag(Z80CPUFlag::carry)) {
        diff += 0x60;
        setFlag(Z80CPUFlag::carry, true);
    }

    if (subtract && !getFlag(Z80CPUFlag::halfCarry)) {
        setFlag(Z80CPUFlag::halfCarry, false);
    } else {
        if (subtract && getFlag(Z80CPUFlag::halfCarry)) {
            setFlag(Z80CPUFlag::halfCarry, (dest & 0xF) < 6);
        } else {
            setFlag(Z80CPUFlag::halfCarry, (dest & 0xF) >= 0xA);
        }
    }

    if (subtract) {
        dest -= diff;
    } else {
        dest += diff;
    }

    setFlag(Z80CPUFlag::overflowParity, getParity(dest));
    setFlag(Z80CPUFlag::zero, dest == 0);
    setFlag(Z80CPUFlag::sign, Utils::testBit(7, dest));
    handleUndocumentedFlags(dest);
}

void CPUZ80::cpl(unsigned char &dest) {
    dest = ~dest;
    setFlag(Z80CPUFlag::halfCarry, true);
    setFlag(Z80CPUFlag::subtractNegative, true);
    handleUndocumentedFlags(dest);
}

void CPUZ80::exStack(unsigned short &dest) {
    unsigned short originalRegisterValue = dest;
    dest = popStack16();
    pushStack(originalRegisterValue);
    gpRegisters[Z80Registers::WZ].whole = dest;
}

void CPUZ80::readPortToRegister(unsigned char &dest, unsigned char portAddress) {

    dest = portIn(portAddress);

    #ifdef DEBUG_VALUES
        ioPortAddress = portAddress;
        readValue = dest;
    #endif

    setFlag(Z80CPUFlag::halfCarry, false);
    setFlag(Z80CPUFlag::subtractNegative, false);
    setFlag(Z80CPUFlag::overflowParity, getParity(dest));
    cyclesTaken = 12;
}

void CPUZ80::retn() {
    programCounter = popStack16();
    iff1 = iff2;
    cyclesTaken = 14;
}

void CPUZ80::reti() {
    programCounter = popStack16();
    gpRegisters[Z80Registers::WZ].whole = programCounter;
    cyclesTaken = 14;
}

void CPUZ80::rrd(unsigned char &dest) {
    /**
     * Moves the lower four bits of A (0-3) into the upper four bits of (HL) (4-7); moves the upper four bits of (HL) (4-7)
     * into the lower four bits of (HL) (0-3); moves the lower four bits of (HL) (0-3) into the lower four bits of A (0-3).
     */
    unsigned char memoryCurrentValue = readMemory(gpRegisters[Z80Registers::HL].whole);
    unsigned char previousRegisterLowerNibble = dest & 0x0F;
    dest = (gpRegisters[Z80Registers::AF].hi & 0xF0) + (memoryCurrentValue & 0x0F);
    writeMemory(gpRegisters[Z80Registers::HL].whole, (unsigned char)((previousRegisterLowerNibble << 4) + (memoryCurrentValue >> 4)));
    setFlag(Z80CPUFlag::subtractNegative, false);
    setFlag(Z80CPUFlag::overflowParity, getParity(dest));
    setFlag(Z80CPUFlag::halfCarry, false);
    setFlag(Z80CPUFlag::zero, dest == 0);
    setFlag(Z80CPUFlag::sign, Utils::testBit(7, dest));
    handleUndocumentedFlags(dest);
    gpRegisters[Z80Registers::WZ].whole = gpRegisters[Z80Registers::HL].whole + 1; // TODO should this be the value of HL before or after the operation? documentation doesn't say
    cyclesTaken = 18;
}

void CPUZ80::rld(unsigned char &dest) {
    /**
     * Moves the lower four bits of HL (0-3) into the upper four bits of (HL) (4-7); moves the upper four bits of (HL) (4-7)
     * into the lower four bits of A (0-3); moves the lower four bits of A (0-3) into the lower four bits of (HL) (0-3).
     */
    unsigned char memoryCurrentValue = readMemory(gpRegisters[Z80Registers::HL].whole);
    unsigned char previousRegisterLowerNibble = dest & 0x0F;
    dest = (gpRegisters[Z80Registers::AF].hi & 0xF0) + ((memoryCurrentValue & 0xF0) >> 4);
    writeMemory(gpRegisters[Z80Registers::HL].whole, (unsigned char)(((memoryCurrentValue & 0x0F) << 4) + previousRegisterLowerNibble));
    setFlag(Z80CPUFlag::subtractNegative, false);
    setFlag(Z80CPUFlag::overflowParity, getParity(dest));
    setFlag(Z80CPUFlag::halfCarry, false);
    setFlag(Z80CPUFlag::zero, dest == 0);
    setFlag(Z80CPUFlag::sign, Utils::testBit(7, dest));
    handleUndocumentedFlags(dest);
    gpRegisters[Z80Registers::WZ].whole = gpRegisters[Z80Registers::HL].whole + 1;
    cyclesTaken = 18;
}

inline void CPUZ80::bitLogic(unsigned char bitNumber, unsigned char value) {
    bool bitIsSet = Utils::testBit(bitNumber, value);
    setFlag(Z80CPUFlag::sign, bitNumber == 7 && bitIsSet);
    setFlag(Z80CPUFlag::zero, !bitIsSet);
    setFlag(Z80CPUFlag::halfCarry, true);
    setFlag(Z80CPUFlag::overflowParity, !bitIsSet);
    setFlag(Z80CPUFlag::subtractNegative, false);
}

void CPUZ80::bit(unsigned char bitNumber, unsigned char value) {
    bitLogic(bitNumber, value);
    handleUndocumentedFlags(value);
}

void CPUZ80::indexedBit(unsigned char bitNumber, unsigned char value) {
    bitLogic(bitNumber, value);
    unsigned char indexedAddressHi = indexedAddressForCurrentOpcode >> 8;
    setFlag(Z80CPUFlag::yf, Utils::testBit(5, indexedAddressHi));
    setFlag(Z80CPUFlag::xf, Utils::testBit(3, indexedAddressHi));
}

void CPUZ80::hlBit(unsigned char bitNumber) {
    bitLogic(bitNumber, readMemory(gpRegisters[Z80Registers::HL].whole));
    handleUndocumentedFlags(gpRegisters[Z80Registers::WZ].hi);
}

unsigned char CPUZ80::res(unsigned char bitNumber, unsigned char value) {
    Utils::setBit(bitNumber, false, value);
    return value;
}

unsigned char CPUZ80::set(unsigned char bitNumber, unsigned char value) {
    Utils::setBit(bitNumber, true, value);
    return value;
}

unsigned char CPUZ80::portIn(unsigned char port) {
    return 0xFF;
}

void CPUZ80::portOut(unsigned char port, unsigned char value) {
    return;
}

void CPUZ80::writeMemory(unsigned short location, unsigned char value) {
    memory->z80Write(location, value);

    #ifdef DEBUG_VALUES
    readValue = value;
    memoryAddress = location;
    #endif
}

void CPUZ80::writeMemory(unsigned short location, unsigned short value) {
    memory->z80Write(location, value);

    #ifdef DEBUG_VALUES
    readValue = value;
    memoryAddress = location;
    #endif
}

unsigned char CPUZ80::readMemory(unsigned short location) {
    // TODO this wrapper function has been created for debugging purposes to get console output - refactor later.
    unsigned char value = memory->z80Read(location);
    #ifdef DEBUG_VALUES
    readValue = value;
    memoryAddress = location;
    #endif
    return value;
}

unsigned short CPUZ80::readMemory16Bit(unsigned short location) {
    // TODO this wrapper function has been created for debugging purposes to get console output - refactor later.
    unsigned short value = memory->z80Read16Bit(location);
    #ifdef DEBUG_VALUES
        readValue = value;
        memoryAddress = location;
    #endif
    return value;
}

void CPUZ80::ccf() {
    setFlag(Z80CPUFlag::halfCarry, getFlag(Z80CPUFlag::carry));
    setFlag(Z80CPUFlag::carry, !getFlag(Z80CPUFlag::carry));
    setFlag(Z80CPUFlag::subtractNegative, false);

    // Set XF/YF flags accordingly
    gpRegisters[Z80Registers::AF].lo |= gpRegisters[Z80Registers::AF].hi & 0x28;
}

void CPUZ80::neg() {
    unsigned char value = 0;
    sub8Bit(value, gpRegisters[Z80Registers::AF].hi);
    gpRegisters[Z80Registers::AF].hi = value;
}