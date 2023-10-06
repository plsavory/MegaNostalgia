cmake_minimum_required(VERSION 3.26)
project(MegaNostalgia)

set(CMAKE_CXX_STANDARD 14)
set(EXECUTABLE_NAME "MegaNostalgia")

add_executable(${EXECUTABLE_NAME}
        src/ProjectInfo.h
        src/main.cpp
        src/Emulator.h
        src/Emulator.cpp
        src/Utils.h
        src/Utils.cpp
        src/Exceptions.h
        src/Cartridge.h
        src/Cartridge.cpp
        src/Memory.h
        src/Memory.cpp
        src/CPUM68k.h
        src/CPUM68k.cpp
        src/CPUZ80.h
        src/CPUZ80.cpp
        src/CPUZ80InstructionHelpers.cpp
        src/CPUZ80StandardOpcodeHandlers.cpp
        src/CPUZ80ExtendedOpcodeHandlers.cpp
        src/CPUZ80BitOpcodeHandlers.cpp
        src/CPUZ80IndexOpcodeHandlers.cpp
        src/CPUZ80IndexBitOpcodeHandlers.cpp)
