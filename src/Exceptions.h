//
// Created by Peter Savory on 08/10/2022.
//

#ifndef MegaNostalgia_EXCEPTIONS_H
#define MegaNostalgia_EXCEPTIONS_H

#include <fstream>
#include <exception>
#include <stdexcept>
#include <vector>
#include <sstream>
#include "Utils.h"

class GeneralException : public std::exception {
public:

    /**
     * Very basic constructor, probably shouldn't be used outside of quick debugging.
     */
    GeneralException() {
        fullMessage = "No information provided (This is not helpful.)";
        writeToFile();
    };

    /**
     * Simple constructor - To be used when we don't care about displaying the type of error.
     *
     * @param message
     */
    explicit GeneralException(const std::string &message) {
        fullMessage = message;
        writeToFile();
    };

    /**
     * Should be called by exceptions which inherit from this one.
     *
     * @param errorType - The type of error, will usually be the class name of the inheriting exception
     * @param message - The exception message/reason
     */
    GeneralException(const std::string &errorType, const std::string &message) {

        std::vector<std::string> errorStrings = {
                errorType,
                ": ",
                message
        };

        fullMessage = Utils::implodeString(errorStrings);
        writeToFile();
    }

    const char *what() {
        return fullMessage.c_str();
    }

private:
    std::string fullMessage;

    using std::exception::what;

    /**
     * Writes information about the exception to the error log
     */
    void writeToFile() {
        std::ofstream file("errors.log", std::ios::app);

        if (!file) {
            fullMessage.append(" - was not able to write to error log");
            return;
        }

        // TODO: Make the date format customisable
        std::string dateTime = Utils::getSystemDateTime(DateFormat::FORMAT_DATETIME_UK);
        std::stringstream ss;
        ss<<"------------"<<std::endl<<
          "Occurred at: "<<dateTime<<std::endl<<std::endl<<
          fullMessage<<std::endl<<std::endl;

        file<<ss.str();
        file.close();
    }
};

class Z80Exception : public GeneralException {
public:
    explicit Z80Exception(const std::string &message) : GeneralException(std::string("Z80 Exception"),
                                                                             message) {};
};

class M68KException : public GeneralException {
public:
    explicit M68KException(const std::string &message) : GeneralException(std::string("M68K Exception"),
                                                                         message) {};
};

class VDPException : public GeneralException {
public:
    explicit VDPException(const std::string &message) : GeneralException(std::string("Z80 Exception"),
                                                                         message) {};
};

class ConfigurationException : public GeneralException {
public:
    explicit ConfigurationException(const std::string &message) : GeneralException(std::string("Configuration Exception"),
                                                                                message) {};
};

class JSONParserException : public GeneralException {
public:
    explicit JSONParserException(const std::string &message) : GeneralException(std::string("JSON Parser Exception"),
                                                                         message) {};
};

class IOException : public GeneralException {
public:
    explicit IOException(const std::string &message) : GeneralException(std::string("IO Exception"),
                                                                                message) {};
};

#endif //MegaNostalgia_EXCEPTIONS_H
