//
// Created by Peter Savory on 05/10/2023.
//


#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <iterator>
#include "Cartridge.h"
#include <regex>
#include "Exceptions.h"

Cartridge::Cartridge() {
    for (int i = 0; i < MAX_ROM_SIZE; i++) {
        rom[i] = 0x0;
    }
}

void Cartridge::loadROM(const std::string &fileName) {

    romFilePath = fileName;

    // Determine the size of the ROM file
    struct stat fileStat{};
    int ROMSize;

    int fileStatus = stat(fileName.c_str(), &fileStat);

    // Do some standard checks on the file to decide on whether it is kosher or not (This is not 100%, but we at least should check obvious stuff)
    if (fileStatus != 0) {
        // File does not exist
        throw GeneralException(Utils::implodeString({"Unable to load file '", romFilePath, "': File does not exist."}));
    }

    // Figure out the size of the ROM
    ROMSize = fileStat.st_size;

    if (ROMSize > MAX_ROM_SIZE) {
        throw GeneralException(Utils::implodeString({"Unable to load file '", romFilePath, "': File is too large (", std::to_string(ROMSize), " byes) max: ", std::to_string(MAX_ROM_SIZE), " bytes"}));
    }

    if (ROMSize < 0x200) {
        // TODO decide on a better limit than this, this is to prevent a crash while reading the header
        throw GeneralException(Utils::implodeString({"ROM file is too small, could not possibly be a valid ROM"}));
    }

    // Fill temp storage with data from ROM file
    typedef std::istream_iterator<unsigned char> istream_iterator;

    std::ifstream romStream(fileName, std::ios::binary); // file stream for reading file contents
    std::vector<unsigned char> tempStorage; // temporary storage for data - will be copied into cartridge struct later

    romStream >> std::noskipws; // Do not skip white space, we want every single character of the file.
    std::copy(istream_iterator(romStream), istream_iterator(),
              std::back_inserter(tempStorage)); // Copy the contents of the file into the temporary storage vector

    // Get the system type
    for (int i = 0; i < 16; i++) {
        systemType += (signed char)tempStorage[0x100 + i];
    }

    // TODO throw an error if an incorrect system type has been found - might want to find another way to handle valid ROM detection in the future as this will make non-TMSS games fail to work

    // Get the copyright/release date
    for (int i = 0; i < 16; i++) {
        copyright += (signed char)tempStorage[0x110 + i];
    }

    // Get the game name
    for (int i = 0; i < 48; i++) {
        gameTitle += (signed char)tempStorage[0x120 + i];
    }

    // Get the alternative/overseas game name
    for (int i = 0; i < 48; i++) {
        overseasGameTitle += (signed char)tempStorage[0x150 + i];
    }

    for (int i = 0; i < 14; i++) {
        serialNumber += (signed char)tempStorage[0x180 + i];
    }

    // Get the checksum from the ROM header and then calculate it
    // TODO might want to complain if they differ
    checksum = (tempStorage[0x18E] << 8) + tempStorage[0x18F];

    calculatedChecksum = 0;

    for (int i = 0x200; i < ROMSize; i+=2) {
        calculatedChecksum += (tempStorage[i] << 8) + tempStorage[i+1];
    }

    for (int i = 0; i < 16; i++) {
        // TODO come up with a better way to represent these for detection while emulating. Perhaps set booleans for them each.
        supportedDevices += (signed char)tempStorage[0x190 + i];
    }

    romStartAddress = 0;
    romEndAddress = 0;

    for (int i = 0; i < 4; i++) {
        romStartAddress += (tempStorage[0x1A0 + i] << ((3-i) * 8));
    }

    for (int i = 0; i < 4; i++) {
        romEndAddress += (tempStorage[0x1A4 + i] << ((3-i) * 8));
    }


    ramStartAddress = 0;
    ramEndAddress = 0;

    for (int i = 0; i < 4; i++) {
        ramStartAddress += (tempStorage[0x1A8 + i] << ((3-i) * 8));
    }

    for (int i = 0; i < 4; i++) {
        ramEndAddress += (tempStorage[0x1AC + i] << ((3-i) * 8));
    }

    // TODO Determine whether the cartridge has extra RAM or SRAM

    // TODO add handling for when a ROM has the newer style of region information
    for (int i = 0; i < 3; i++) {
        regions += (signed char)tempStorage[0x1F0 + i];
    }

    if (ROMSize < (romEndAddress + 1)) {
        throw IOException(Utils::implodeString({"Unable to load file '", romFilePath, "': Unexpected end of file"}));
    }

    if (ROMSize > (romEndAddress + 1)) {
        std::cout<<"Warning: ROM file larger than expected"<<std::endl;
    }

    // Copy the temporary storage into the cartridge ROM
    for (uint32_t i = 0; i <= romEndAddress; i++) {
        rom[i] = tempStorage[i];
    }

    // Print ROM information
    printROMInfo();

}

void Cartridge::printROMInfo() {
    std::cout<<"Detected ROM information: "<<std::endl;
    std::cout<<"System Type: "<<systemType<<std::endl;
    std::cout<<"Title: "<<gameTitle<<std::endl;
    std::cout<<"Alt. Title: "<<overseasGameTitle<<std::endl;
    std::cout<<"Copyright: "<<copyright<<std::endl;
    std::cout<<"Region: "<<regions<<std::endl;
    std::cout<<"Serial Number: "<<serialNumber<<std::endl;
    std::cout<<"Checksum: 0x" << std::hex << checksum << " (0x" << calculatedChecksum << " calculated)"<<std::endl;
    std::cout<<"Supported Devices: "<<supportedDevices<<std::endl;
    std::cout<<"ROM Address Range: 0x"<<std::hex<<romStartAddress << "-" << "0x" << romEndAddress<<std::endl;
    std::cout<<"RAM Address Range: 0x"<<std::hex<<ramStartAddress << "-" << "0x" << ramEndAddress<<std::endl;
}

unsigned char Cartridge::read(unsigned long location) {
    // TODO add mapper handling code for the few games that apparently require it.
    return rom[location];
}

void Cartridge::write(uint32_t location, unsigned char value) {
    // TODO handle writing to SRAM
}