//
// Created by Peter Savory on 05/10/2023.
//

#ifndef MEGANOSTALGIA_CARTRIDGE_H
#define MEGANOSTALGIA_CARTRIDGE_H

#include "Exceptions.h"

// Largest ROM is supposedly 40megabits
#define MAX_ROM_SIZE 0x2800000

class Cartridge {
public:
    Cartridge();

    void loadROM(const std::string &fileName);

    void printROMInfo();

    unsigned char read(unsigned long location);

    void write(uint32_t location, unsigned char value);

private:

    unsigned char rom[MAX_ROM_SIZE];

    std::string romFilePath;

    std::string systemType;

    std::string copyright;

    std::string gameTitle;

    std::string overseasGameTitle;

    std::string serialNumber;

    std::string supportedDevices;

    std::string regions;

    unsigned short checksum;

    unsigned short calculatedChecksum;

    uint32_t romStartAddress;

    uint32_t romEndAddress;

    uint32_t ramStartAddress;

    uint32_t ramEndAddress;

};

#endif //MEGANOSTALGIA_CARTRIDGE_H
