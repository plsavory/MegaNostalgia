//
// Created by Peter Savory on 05/10/2023.
//

#ifndef MEGANOSTALGIA_MEMORY_H
#define MEGANOSTALGIA_MEMORY_H

#include <cstdint>
#include "Cartridge.h"

class Memory {
public:

    Memory(Cartridge *cartridge);

    unsigned char z80Read(uint16_t location);

    unsigned short z80Read16Bit(uint16_t location);

    unsigned char m68kRead(uint32_t location);

    unsigned short m68kRead16Bit(uint32_t location);

    void m68kWrite(uint32_t location, unsigned char value);

    void m68kWrite(uint32_t location, unsigned short value);

    void z80Write(uint16_t location, unsigned char value);

    void z80Write(uint16_t location, unsigned short value);


private:

    unsigned char z80RAM[0x2000];

    unsigned char m68kRAM[0x10000];

    Cartridge *cartridge;
};

#endif //MEGANOSTALGIA_MEMORY_H
