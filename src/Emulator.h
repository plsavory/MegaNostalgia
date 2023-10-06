//
// Created by Peter Savory on 05/10/2023.
//

#ifndef MEGANOSTALGIA_EMULATOR_H
#define MEGANOSTALGIA_EMULATOR_H

#include <iostream>
#include "Cartridge.h"
#include "Memory.h"
#include "CPUM68k.h"
#include "CPUZ80.h"

class Emulator {
public:

    Emulator();

    ~Emulator();

    void init(const std::string &romFileName);

    void run();

private:

    Cartridge *cartridge;
    Memory *memory;
    CPUZ80 *z80;
    CPUM68k *m68k;

    void emulateFrame();

    uint32_t getMasterClockCyclesPerFrame();

    uint32_t masterClockRate;
    uint32_t m68kClockRatePerMachineTick;
    uint32_t z80ClockRatePerMachineTick;
};

#endif //MEGANOSTALGIA_EMULATOR_H
