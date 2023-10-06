//
// Created by Peter Savory on 09/06/2023.
//

#include <iostream>
#include "Memory.h"
#include "CPUZ80.h"
#include "Utils.h"
#include "Exceptions.h"

void CPUZ80::extendedOpcodeHandler0x40() {
    // in b, (c)
    readPortToRegister(gpRegisters[Z80Registers::BC].hi, gpRegisters[Z80Registers::BC].lo);
}

void CPUZ80::extendedOpcodeHandler0x41() {
    // out (c), b
    portOut(gpRegisters[Z80Registers::BC].lo, gpRegisters[Z80Registers::BC].hi);
    cyclesTaken = 12;
}

void CPUZ80::extendedOpcodeHandler0x42() {
    // sbc hl, bc
    sbc16Bit(gpRegisters[Z80Registers::HL].whole, gpRegisters[Z80Registers::BC].whole);
    cyclesTaken = 15;
}

void CPUZ80::extendedOpcodeHandler0x43() {
    // ld (nn), bc
    unsigned short address = build16BitNumber();
    writeMemory(address, gpRegisters[Z80Registers::BC].whole);
    gpRegisters[Z80Registers::WZ].whole = address + 1;
    cyclesTaken = 20;
}

void CPUZ80::extendedOpcodeHandler0x44() {
    // neg
    neg();
    cyclesTaken = 8;
}

void CPUZ80::extendedOpcodeHandler0x45() {
    // retn
    retn();
}

void CPUZ80::extendedOpcodeHandler0x46() {
    // im 0
    setInterruptMode(0);
}

void CPUZ80::extendedOpcodeHandler0x47() {
    // ld i, a
    ldReg8(registerI, gpRegisters[Z80Registers::AF].hi);
    cyclesTaken = 9;
}

void CPUZ80::extendedOpcodeHandler0x48() {
    // in c, (c)
    readPortToRegister(gpRegisters[Z80Registers::BC].lo, gpRegisters[Z80Registers::BC].lo);
}

void CPUZ80::extendedOpcodeHandler0x49() {
    // out (c), c
    portOut(gpRegisters[Z80Registers::BC].lo, gpRegisters[Z80Registers::BC].lo);
    cyclesTaken = 12;
}

void CPUZ80::extendedOpcodeHandler0x4A() {
    // adc hl, bc
    adc16Bit(gpRegisters[Z80Registers::HL].whole, gpRegisters[Z80Registers::BC].whole);
    cyclesTaken = 15;
}

void CPUZ80::extendedOpcodeHandler0x4B() {
    // ld bc, (nn)
    unsigned short address = build16BitNumber();
    ldReg16(gpRegisters[Z80Registers::BC].whole, readMemory16Bit(address), false);
    gpRegisters[Z80Registers::WZ].whole = address + 1;
    cyclesTaken = 20;
}

void CPUZ80::extendedOpcodeHandler0x4D() {
    // reti
    reti();
}

void CPUZ80::extendedOpcodeHandler0x4F() {
    // ld r, a
    ldReg8(registerR, gpRegisters[Z80Registers::AF].hi);
    cyclesTaken = 9;
}

void CPUZ80::extendedOpcodeHandler0x50() {
    // in d, (c)
    readPortToRegister(gpRegisters[Z80Registers::DE].hi, gpRegisters[Z80Registers::BC].lo);
}

void CPUZ80::extendedOpcodeHandler0x51() {
    // out (c), d
    portOut(gpRegisters[Z80Registers::BC].lo, gpRegisters[Z80Registers::DE].hi);
    cyclesTaken = 12;
}

void CPUZ80::extendedOpcodeHandler0x52() {
    // sbc hl, de
    sbc16Bit(gpRegisters[Z80Registers::HL].whole, gpRegisters[Z80Registers::DE].whole);
    cyclesTaken = 15;
}

void CPUZ80::extendedOpcodeHandler0x53() {
    // ld (nn), de
    unsigned short address = build16BitNumber();
    writeMemory(address, gpRegisters[Z80Registers::DE].whole);
    gpRegisters[Z80Registers::WZ].whole = address + 1;
    cyclesTaken = 20;
}

void CPUZ80::extendedOpcodeHandler0x56() {
    // im 1
    setInterruptMode(1);
}

void CPUZ80::extendedOpcodeHandler0x57() {
    // ld a, i
    ldReg8(gpRegisters[Z80Registers::AF].hi, registerI, true);
    cyclesTaken = 9;
}

void CPUZ80::extendedOpcodeHandler0x58() {
    // in e, (c)
    readPortToRegister(gpRegisters[Z80Registers::DE].lo, gpRegisters[Z80Registers::BC].lo);
    cyclesTaken = 12;
}

void CPUZ80::extendedOpcodeHandler0x59() {
    // out (c), e
    portOut(gpRegisters[Z80Registers::BC].lo, gpRegisters[Z80Registers::DE].lo);
    cyclesTaken = 12;
}

void CPUZ80::extendedOpcodeHandler0x5A() {
    // adc hl, de
    adc16Bit(gpRegisters[Z80Registers::HL].whole, gpRegisters[Z80Registers::DE].whole);
    cyclesTaken = 15;
}

void CPUZ80::extendedOpcodeHandler0x5B() {
    // ld de, (nn)
    unsigned short address = build16BitNumber();
    ldReg16(gpRegisters[Z80Registers::DE].whole, readMemory16Bit(address));
    gpRegisters[Z80Registers::WZ].whole = address + 1;
    cyclesTaken = 20;
}

void CPUZ80::extendedOpcodeHandler0x5E() {
    setInterruptMode(2);
}

void CPUZ80::extendedOpcodeHandler0x5F() {
    // ld a, r
    ldReg8(gpRegisters[Z80Registers::AF].hi, registerR, true);
    cyclesTaken = 9;
}

void CPUZ80::extendedOpcodeHandler0x60() {
    // in h, (c)
    readPortToRegister(gpRegisters[Z80Registers::HL].hi, gpRegisters[Z80Registers::BC].lo);
    cyclesTaken = 12;
}

void CPUZ80::extendedOpcodeHandler0x61() {
    // out (c), h
    portOut(gpRegisters[Z80Registers::BC].lo, gpRegisters[Z80Registers::HL].hi);
    cyclesTaken = 12;
}

void CPUZ80::extendedOpcodeHandler0x62() {
    // sbc hl, hl
    sbc16Bit(gpRegisters[Z80Registers::HL].whole, gpRegisters[Z80Registers::HL].whole);
    cyclesTaken = 15;
}

void CPUZ80::extendedOpcodeHandler0x63() {
    // ld (nn), hl
    unsigned short address = build16BitNumber();
    writeMemory(address, gpRegisters[Z80Registers::HL].whole);
    gpRegisters[Z80Registers::WZ].whole = address + 1;
    cyclesTaken = 20;
}

void CPUZ80::extendedOpcodeHandler0x67() {
    // rrd
    rrd(gpRegisters[Z80Registers::AF].hi);
}

void CPUZ80::extendedOpcodeHandler0x68() {
    // in l, (c)
    readPortToRegister(gpRegisters[Z80Registers::HL].lo, gpRegisters[Z80Registers::BC].lo);
    cyclesTaken = 12;
}

void CPUZ80::extendedOpcodeHandler0x69() {
    // out (c), l
    portOut(gpRegisters[Z80Registers::BC].lo, gpRegisters[Z80Registers::HL].lo);
    cyclesTaken = 12;
}

void CPUZ80::extendedOpcodeHandler0x6A() {
    // adc hl, hl
    adc16Bit(gpRegisters[Z80Registers::HL].whole, gpRegisters[Z80Registers::HL].whole);
    cyclesTaken = 15;
}

void CPUZ80::extendedOpcodeHandler0x6B() {
    // ld hl, (nn)
    unsigned short address = build16BitNumber();
    ldReg16(gpRegisters[Z80Registers::HL].whole, readMemory16Bit(address));
    gpRegisters[Z80Registers::WZ].whole = address + 1;
    cyclesTaken = 20;
}

void CPUZ80::extendedOpcodeHandler0x6F() {
    // rld
    rld(gpRegisters[Z80Registers::AF].hi);
}

void CPUZ80::extendedOpcodeHandler0x70() {
    // in (c)
    portIn(gpRegisters[Z80Registers::BC].lo);
    cyclesTaken = 12;
}

void CPUZ80::extendedOpcodeHandler0x71() {
    // out (c), 0
    portOut(gpRegisters[Z80Registers::BC].lo, 0);
    cyclesTaken = 12;
}

void CPUZ80::extendedOpcodeHandler0x72() {
    // sbc hl, sp
    sbc16Bit(gpRegisters[Z80Registers::HL].whole, stackPointer);
    cyclesTaken = 15;
}

void CPUZ80::extendedOpcodeHandler0x73() {
    // ld (nn), sp
    unsigned short address = build16BitNumber();
    writeMemory(address, stackPointer);
    gpRegisters[Z80Registers::WZ].whole = address + 1;
    cyclesTaken = 20;
}

void CPUZ80::extendedOpcodeHandler0x78() {
    // in a, (c)
    readPortToRegister(gpRegisters[Z80Registers::AF].hi, gpRegisters[Z80Registers::BC].lo);
    gpRegisters[Z80Registers::WZ].whole = gpRegisters[Z80Registers::BC].whole + 1;
    cyclesTaken = 12;
}

void CPUZ80::extendedOpcodeHandler0x79() {
    // out (c), a
    portOut(gpRegisters[Z80Registers::BC].lo, gpRegisters[Z80Registers::AF].hi);
    gpRegisters[Z80Registers::WZ].whole = gpRegisters[Z80Registers::BC].whole + 1;
    cyclesTaken = 12;
}

void CPUZ80::extendedOpcodeHandler0x7A() {
    // adc hl, sp
    adc16Bit(gpRegisters[Z80Registers::HL].whole, stackPointer);
    cyclesTaken = 15;
}

void CPUZ80::extendedOpcodeHandler0x7B() {
    // ld sp, (nn)
    unsigned short address = build16BitNumber();
    ldReg16(stackPointer, readMemory16Bit(address));
    gpRegisters[Z80Registers::WZ].whole = address + 1;
    cyclesTaken = 20;
}

void CPUZ80::extendedOpcodeHandler0xA0() {
    // ldi
    ldi(true);
}

void CPUZ80::extendedOpcodeHandler0xA1() {
    // cpi
    cpi(true);
}

void CPUZ80::extendedOpcodeHandler0xA2() {
    // ini
    ini(true);
}

void CPUZ80::extendedOpcodeHandler0xA3() {
    // outi
    outi(true);
}

void CPUZ80::extendedOpcodeHandler0xA8() {
    // ldd
    ldi(false);
}

void CPUZ80::extendedOpcodeHandler0xA9() {
    // cpd
    cpi(false);
}

void CPUZ80::extendedOpcodeHandler0xAA() {
    ini(false);
}

void CPUZ80::extendedOpcodeHandler0xAB() {
    outi(false);
}

void CPUZ80::extendedOpcodeHandler0xB0() {
    // ldir
    ldir(true);
}

void CPUZ80::extendedOpcodeHandler0xB1() {
    // cpir
    cpir(true);
}

void CPUZ80::extendedOpcodeHandler0xB2() {
    // inir
    inir(true);
}

void CPUZ80::extendedOpcodeHandler0xB3() {
    // otir
    otir(true);
}

void CPUZ80::extendedOpcodeHandler0xB8() {
    // lddr
    ldir(false);
}

void CPUZ80::extendedOpcodeHandler0xB9() {
    // cpdr
    cpir(false);
}

void CPUZ80::extendedOpcodeHandler0xBA() {
    // indr
    inir(false);
}

void CPUZ80::extendedOpcodeHandler0xBB() {
    // otdr
    otir(false);
}