//
// Created by Peter Savory on 05/10/2023.
//

#include "Emulator.h"
#include "Exceptions.h"

Emulator::Emulator() {
    cartridge = new Cartridge();
    memory = new Memory(cartridge);
    m68k = new CPUM68k(memory);
    z80 = new CPUZ80(memory);

    // TODO improve the system timings, this is rough for now to get things started.
    masterClockRate = getMasterClockCyclesPerFrame();

}
void Emulator::init(const std::string &romFileName) {
    cartridge->loadROM(romFileName);
    m68k->reset();
    z80->reset(); // TODO turn the Z80 off when we are executing it, the program needs to turn it on itself
}

void Emulator::run() {
    while (true) {
        emulateFrame();
    }
}

void Emulator::emulateFrame() {
    // TODO handle proper console timing - this is all (probably incorrect) guesswork just to get things started.
    uint32_t ticksThisUpdate = 0;

    while (ticksThisUpdate < masterClockRate) {

        // Update 68k
        int m68kCycles = m68k->execute();

        int machineClicks = m68kCycles * 7;
        float neededZ80Cycles = (float)machineClicks / 15;

        // TODO update z80
        float z80Cycles = 0;

        while (z80Cycles < neededZ80Cycles) {
            break;
        }

        // TODO update VDP
        float vdpCycles = (float)machineClicks / 4;

        // TODO update Sound
        int fmCycles = m68kCycles;
        float psgCycles = z80Cycles;

        ticksThisUpdate += machineClicks;
    }
}

uint32_t Emulator::getMasterClockCyclesPerFrame() {
    // 896,040 (NTSC), 1,067,040 (PAL) - From https://segaretro.org/Sega_Mega_Drive/Technical_specifications
    // TODO handle PAL timings
    return 896040;
}