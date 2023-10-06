//
// Created by Peter Savory on 05/10/2023.
//

#include "Memory.h"

Memory::Memory(Cartridge *cartridge) {

    this->cartridge = cartridge;
    for (int i = 0; i < 0xFFFF; i++) {
        m68kRAM[i] = 0;
    }

    for (int i = 0; i < 0x1FFF; i++) {
        z80RAM[i] = 0;
    }
}

unsigned char Memory::z80Read(uint16_t location) {

    if (location <= 0x1FFF) {
        // 0x0000 - 0x1FFF - Z80 RAM
        return z80RAM[location];
    }

    if (location <= 0x3FFF) {
        // 0x2000 - 0x3FFF - Reserved (For what yet, I do not know)
        return 0x0;
    }

    if (location == 0x4000) {
        // TODO YM2612 A0
        return 0x0;
    }

    if (location == 0x4001) {
        // TODO YM2612 D0
        return 0x0;
    }

    if (location == 0x4002) {
        // TODO YM2612 A1
        return 0x0;
    }

    if (location == 0x4003) {
        // TODO YM2612 D1
        return 0x0;
    }

    if (location <= 0x5FFF) {
        // 0x4004 - 0x5FFF - Reserved (For what yet, I do not know)
        return 0x0;
    }

    if (location == 0x6000) {
        // TODO bank register
        return 0x0;
    }

    if (location <= 0x7F10) {
        // 0x6001-0x7F10 - Reserved (For what yet, I do not know)
        return 0x0;
    }

    if (location == 0x7F11) {
        // TODO SN76489 PSG
        return 0x0;
    }

    if (location <= 0x7FFF) {
        // 0x7F12-0x7FFF - Reserved (For what yet, I do not know)
        return 0x0;
    }

    // TODO 0x8000-0xFFFF - M68k memory bank (Need to look into how the bank register works)
    return 0x0;

}

unsigned short Memory::z80Read16Bit(uint16_t location) {
    return z80Read(location) + (z80Read(location+1) << 8);
}

void Memory::z80Write(uint16_t location, unsigned char value) {
    if (location <= 0x1FFF) {
        // 0x0000 - 0x1FFF - Z80 RAM
        z80RAM[location] = value;
        return;
    }

    if (location <= 0x3FFF) {
        // 0x2000 - 0x3FFF - Reserved (For what yet, I do not know)
        return;
    }

    if (location == 0x4000) {
        // TODO YM2612 A0
        return;
    }

    if (location == 0x4001) {
        // TODO YM2612 D0
        return;
    }

    if (location == 0x4002) {
        // TODO YM2612 A1
        return;
    }

    if (location == 0x4003) {
        // TODO YM2612 D1
        return;
    }

    if (location <= 0x5FFF) {
        // 0x4004 - 0x5FFF - Reserved (For what yet, I do not know)
        return;
    }

    if (location == 0x6000) {
        // TODO bank register (Look into how this works)
        return;
    }

    if (location <= 0x7F10) {
        // 0x6001-0x7F10 - Reserved (For what yet, I do not know)
        return;
    }

    if (location == 0x7F11) {
        // TODO SN76489 PSG
        return;
    }

    if (location <= 0x7FFF) {
        // 0x7F12-0x7FFF - Reserved (For what yet, I do not know)
        return;
    }

    // TODO 0x8000-0xFFFF - M68k memory bank (Need to look into how the bank register works)
}

void Memory::z80Write(uint16_t location, unsigned short value) {
    z80Write(location, (unsigned char)(value & 0x00FF));
    z80Write(location+1, (unsigned char)(value >> 8));
}

unsigned char Memory::m68kRead(uint32_t location) {
    // TODO simplify this function when I've determined which parts of memory are write only
    // TODO could also be refactored to be more performant, currently writing it to make the memory map easy to understand
    // TODO some statements further down the chain might need attention, as there are empty/undefined parts of the memory map. Check to ensure I've covered all cases.

    if (location <= 0x3FFFFF) {
        // 0x000000 - 0x3FFFFF: Cartridge ROM/RAM
        return cartridge->read(location);
    }

    if (location >= 0xFF0000 && location <= 0xFFFFFF) {
        // 0xFF0000 - 0xFFFFFF: 68000 RAM - I've put this higher than other cases as this is likely to be accessed often.
        return m68kRAM[location & 0xFFFF];
    }

    if (location <= 0x7FFFFF) {
        // 0x400000 - 0x7FFFFF: Reserved for CD/32X
        return 0x0;
    }

    if (location <= 0x9FFFFF) {
        // 0x800000 - 0x9FFFFF: 32X
        // 0x840000 - 0x85FFFF: 32X Frame Buffer
        // 0x860000 - 0x87FFFF: 32X frame buffer overwrite mode
        // 0x880000 - 0x8FFFFF: 32X cartridge ROM (first 512kB bank only)
        // 0x900000 - 0x9FFFFF: 32X cartridge bankswitched ROM (any 512kB bank, controlled by 32X registers)
        return 0x0;
    }

    if (location <= 0xA0FFFF) {
        // 0xA00000 - 0xA0FFFF: Z80 memory space
        return z80Read((uint16_t)(location & 0xFFFF));
    }

    if (location <= 0xA10001) {
        // 0xA10000 - 0xA10001: Version Register
        return 0x0;
    }

    if (location <= 0xA10003) {
        // 0xA10002 - A10003: Controller 1 Data
        return 0xFF;
    }

    if (location <= 0xA10005) {
        // 0xA10004 - 0xA10005: Controller 2 Data
        return 0xFF;
    }

    if (location <= 0xA10007) {
        // 0xA10006 - 0xA10007: Expansion port data
        return 0x0;
    }

    if (location <= 0xA10009) {
        // 0xA10008 - 0xA10009: Controller 1 control
        return 0x0;
    }

    if (location <= 0xA1000B) {
        // 0xA1000A - 0xA1000B: Controller 2 control
        return 0x0;
    }

    if (location <= 0xA1000D) {
        // 0xA1000C - 0xA1000D: Expansion port control
        return 0x0;
    }

    if (location <= 0xA1000F) {
        // 0xA1000E - A1000F: Controller 1 serial transmit
        return 0x0;
    }

    if (location <= 0xA10011) {
        // 0xA10010 - 0xA10011: Controller 1 serial receive
        return 0x0;
    }

    if (location <= 0xA10013) {
        // 0xA10012 - 0xA10013: Controller 1 serial control
        return 0x0;
    }

    if (location <= 0xA10015) {
        // 0xA10014 - 0xA10015: Controller 2 serial transmit
        return 0x0;
    }

    if (location <= 0xA10017) {
        // 0xA10016 - 0xA10017: Controller 2 serial receive
        return 0x0;
    }

    if (location <= 0xA10019) {
        // 0xA10018 - 0xA10019: Controller 2 serial control
        return 0x0;
    }

    if (location <= 0xA1001B) {
        // 0xA1001A - 0xA1001B: Expansion port serial transmit
        return 0x0;
    }

    if (location <= 0xA1001D) {
        // 0xA1001C - 0xA1001D: Expansion port serial receive
        return 0x0;
    }

    if (location <= 0xA1001F) {
        // 0xA1001E - 0xA1001F: Expansion port serial control
        return 0x0;
    }

    if (location <= 0xA10FFF) {
        // 0xA10020 - A10FFF: Reserved
        return 0x0;
    }

    if (location == 0xA11000) {
        // Memory mode register
        return 0x0;
    }

    if (location == 0xA11001) {
        // TODO should A11001 always return 0? doesn't seem to be present on the memory map.
        return 0x0;
    }

    if (location <= 0xA110FF) {
        // 0xA11002 - 0xA110FF: Reserved
        return 0x0;
    }

    if (location <= 0xA11101) {
        // 0xA11100 - 0xA11101: Z80 bus request
        return 0x0;
    }

    if (location <= 0xA111FF) {
        // 0xA11102 - 0xA111FF: Reserved
        return 0x0;
    }

    if (location <= 0xA11201) {
        // 0xA11200 - 0xA11201: Z80 Reset
        return 0x0;
    }

    if (location <= 0xA12FFF) {
        // 0xA11202 - 0xA12FFF: Reserved
        return 0x0;
    }

    if (location <= 0xA130FF) {
        // 0xA13000 - 0xA130FF: TIME registers; used to send signals to the cartridge
        // 0xA130EC - 0xA130EF: "MARS" when 32X is attached
        // 0xA130F1: SRAM access register
        // 0xA130F3: Bank register for address 0x80000-0xFFFFF
        // 0xA130F5: Bank register for address 0x100000-0x17FFFF
        // 0xA130F7: Bank register for address 0x180000-0x1FFFFF
        // 0xA130F9: Bank register for address 0x200000-0x27FFFF
        // 0xA130FB: Bank register for address 0x280000-0x2FFFFF
        // 0xA130FD: Bank register for address 0x300000-0x37FFFF
        // 0xA130FF: Bank register for address 0x380000-0x3FFFFF
        return 0x0;
    }

    if (location <= 0xA13FFF) {
        // 0xA13100 - 0xA13FFF: Doesn't seem to be defined/used
        return 0x0;
    }
    if (location <= 0xA14003) {
        // 0xA14000 - 0xA14003: TMSS "SEGA"
        return 0x0;
    }

    if (location <= 0xA14100) {
        // 0xA14003 - 0xA14100 - Seems to be unused
        return 0x0;
    }

    if (location == 0xA14101) {
        // 0xA14101: TMSS/cartridge register
        return 0x0;
    }

    if (location <= 0xBFFFFF) {
        // 0xA14102 - 0xBFFFFF: Reserved
        return 0x0;
    }

    if (location <= 0xC00001) {
        // 0xC00000 - 0xC00001: VDP data port
        return 0x0;
    }

    if (location <= 0xC00003) {
        // 0xC00002 - 0xC00003: VDP data port (mirror)
        return 0x0;
    }

    if (location <= 0xC00005) {
        // 0xC00004 - 0xC00005: VDP control port
        return 0x0;
    }

    if (location <= 0xC00007) {
        // 0xC00006 - 0xC00007: VDP control port (mirror)
        return 0x0;
    }

    if (location <= 0xC00009) {
        // 0xC00008 - 0xC00009: VDP H/V counter
        return 0x0;
    }

    if (location <= 0xC0000F) {
        // 0xC0000A - 0xC0000F: VDP H/V counter (mirror)
        return 0x0;
    }

    if (location == 0xC00010) {
        // 0xC00010: Seems to be unused
        return 0x0;
    }
    if (location == 0xC00011) {
        // 0xC00011 - PSG output
        return 0x0;
    }

    if (location == 0xC00012) {
        // 0xC00012: Unused
        return 0x0;
    }

    if (location <= 0xC00017) {
        // 0xC00013 - 0xC00017: PSG output (mirror)
        return 0x0;
    }

    if (location <= 0xC0001B) {
        // 0xC00017 - 0xC0001B: Seems to be unused
        return 0x0;
    }

    if (location <= 0xC0001D) {
        // 0xC0001C - 0xC0001D: Debug register
        return 0x0;
    }

    if (location <= 0xC0001F) {
        // 0xC0001E - 0xC0001F: Debug register (Mirror)
        return 0x0;
    }

    if (location <= 0xFEFFFF) {
        // 0xC00020 - 0xFEFFFF: Reserved
        return 0x0;
    }

    return 0x0; // Handle any undefined cases
}

unsigned short Memory::m68kRead16Bit(uint32_t location) {
    return (m68kRead(location) << 8) + m68kRead(location+1);
}

void Memory::m68kWrite(uint32_t location, unsigned char value) {
    // TODO simplify this function when I've determined which parts of memory are write only
    // TODO could also be refactored to be more performant, currently writing it to make the memory map easy to understand
    // TODO some statements further down the chain might need attention, as there are empty/undefined parts of the memory map. Check to ensure I've covered all cases.

    if (location <= 0x3FFFFF) {
        // 0x000000 - 0x3FFFFF: Cartridge ROM/RAM
        cartridge->write(location, value);
        return;
    }

    if (location >= 0xFF0000 && location <= 0xFFFFFF) {
        // 0xFF0000 - 0xFFFFFF: 68000 RAM - I've put this higher than other cases as this is likely to be accessed often.
        m68kRAM[location & 0xFFFF] = value;
        return;
    }

    if (location <= 0x7FFFFF) {
        // 0x400000 - 0x7FFFFF: Reserved for CD/32X
        return;
    }

    if (location <= 0x9FFFFF) {
        // 0x800000 - 0x9FFFFF: 32X
        // 0x840000 - 0x85FFFF: 32X Frame Buffer
        // 0x860000 - 0x87FFFF: 32X frame buffer overwrite mode
        // 0x880000 - 0x8FFFFF: 32X cartridge ROM (first 512kB bank only)
        // 0x900000 - 0x9FFFFF: 32X cartridge bankswitched ROM (any 512kB bank, controlled by 32X registers)
        return;
    }

    if (location <= 0xA0FFFF) {
        // 0xA00000 - 0xA0FFFF: Z80 memory space
        z80Write((uint16_t)(location & 0xFFFF), value);
        return;
    }

    if (location <= 0xA10001) {
        // 0xA10000 - 0xA10001: Version Register
        return;
    }

    if (location <= 0xA10003) {
        // 0xA10002 - A10003: Controller 1 Data
        return;
    }

    if (location <= 0xA10005) {
        // 0xA10004 - 0xA10005: Controller 2 Data
        return;
    }

    if (location <= 0xA10007) {
        // 0xA10006 - 0xA10007: Expansion port data
        return;
    }

    if (location <= 0xA10009) {
        // 0xA10008 - 0xA10009: Controller 1 control
        return;
    }

    if (location <= 0xA1000B) {
        // 0xA1000A - 0xA1000B: Controller 2 control
        return;
    }

    if (location <= 0xA1000D) {
        // 0xA1000C - 0xA1000D: Expansion port control
        return;
    }

    if (location <= 0xA1000F) {
        // 0xA1000E - A1000F: Controller 1 serial transmit
        return;
    }

    if (location <= 0xA10011) {
        // 0xA10010 - 0xA10011: Controller 1 serial receive
        return;
    }

    if (location <= 0xA10013) {
        // 0xA10012 - 0xA10013: Controller 1 serial control
        return;
    }

    if (location <= 0xA10015) {
        // 0xA10014 - 0xA10015: Controller 2 serial transmit
        return;
    }

    if (location <= 0xA10017) {
        // 0xA10016 - 0xA10017: Controller 2 serial receive
        return;
    }

    if (location <= 0xA10019) {
        // 0xA10018 - 0xA10019: Controller 2 serial control
        return;
    }

    if (location <= 0xA1001B) {
        // 0xA1001A - 0xA1001B: Expansion port serial transmit
        return;
    }

    if (location <= 0xA1001D) {
        // 0xA1001C - 0xA1001D: Expansion port serial receive
        return;
    }

    if (location <= 0xA1001F) {
        // 0xA1001E - 0xA1001F: Expansion port serial control
        return;
    }

    if (location <= 0xA10FFF) {
        // 0xA10020 - A10FFF: Reserved
        return;
    }

    if (location == 0xA11000) {
        // Memory mode register
        return;
    }

    if (location == 0xA11001) {
        return;
    }

    if (location <= 0xA110FF) {
        // 0xA11002 - 0xA110FF: Reserved
        return;
    }

    if (location <= 0xA11101) {
        // 0xA11100 - 0xA11101: Z80 bus request
        return;
    }

    if (location <= 0xA111FF) {
        // 0xA11102 - 0xA111FF: Reserved
        return;
    }

    if (location <= 0xA11201) {
        // 0xA11200 - 0xA11201: Z80 Reset
        return;
    }

    if (location <= 0xA12FFF) {
        // 0xA11202 - 0xA12FFF: Reserved
        return;
    }

    if (location <= 0xA130FF) {
        // 0xA13000 - 0xA130FF: TIME registers; used to send signals to the cartridge
        // 0xA130EC - 0xA130EF: "MARS" when 32X is attached
        // 0xA130F1: SRAM access register
        // 0xA130F3: Bank register for address 0x80000-0xFFFFF
        // 0xA130F5: Bank register for address 0x100000-0x17FFFF
        // 0xA130F7: Bank register for address 0x180000-0x1FFFFF
        // 0xA130F9: Bank register for address 0x200000-0x27FFFF
        // 0xA130FB: Bank register for address 0x280000-0x2FFFFF
        // 0xA130FD: Bank register for address 0x300000-0x37FFFF
        // 0xA130FF: Bank register for address 0x380000-0x3FFFFF
        return;
    }

    if (location <= 0xA13FFF) {
        // 0xA13100 - 0xA13FFF: Doesn't seem to be defined/used
        return;
    }
    if (location <= 0xA14003) {
        // 0xA14000 - 0xA14003: TMSS "SEGA"
        return;
    }

    if (location <= 0xA14100) {
        // 0xA14003 - 0xA14100 - Seems to be unused
        return;
    }

    if (location == 0xA14101) {
        // 0xA14101: TMSS/cartridge register
        return;
    }

    if (location <= 0xBFFFFF) {
        // 0xA14102 - 0xBFFFFF: Reserved
        return;
    }

    if (location <= 0xC00001) {
        // 0xC00000 - 0xC00001: VDP data port
        return;
    }

    if (location <= 0xC00003) {
        // 0xC00002 - 0xC00003: VDP data port (mirror)
        return;
    }

    if (location <= 0xC00005) {
        // 0xC00004 - 0xC00005: VDP control port
        return;
    }

    if (location <= 0xC00007) {
        // 0xC00006 - 0xC00007: VDP control port (mirror)
        return;
    }

    if (location <= 0xC00009) {
        // 0xC00008 - 0xC00009: VDP H/V counter
        return;
    }

    if (location <= 0xC0000F) {
        // 0xC0000A - 0xC0000F: VDP H/V counter (mirror)
        return;
    }

    if (location == 0xC00010) {
        // 0xC00010: Seems to be unused
        return;
    }
    if (location == 0xC00011) {
        // 0xC00011 - PSG output
        return;
    }

    if (location == 0xC00012) {
        // 0xC00012: Unused
        return;
    }

    if (location <= 0xC00017) {
        // 0xC00013 - 0xC00017: PSG output (mirror)
        return;
    }

    if (location <= 0xC0001B) {
        // 0xC00017 - 0xC0001B: Seems to be unused
        return;
    }

    if (location <= 0xC0001D) {
        // 0xC0001C - 0xC0001D: Debug register
        return;
    }

    if (location <= 0xC0001F) {
        // 0xC0001E - 0xC0001F: Debug register (Mirror)
        return;
    }

    if (location <= 0xFEFFFF) {
        // 0xC00020 - 0xFEFFFF: Reserved
        return;
    }
}

void Memory::m68kWrite(uint32_t location, unsigned short value) {
    m68kWrite(location, (unsigned char)(value >> 8));
    m68kWrite(location + 1, (unsigned char)(value & 0xFF));
}