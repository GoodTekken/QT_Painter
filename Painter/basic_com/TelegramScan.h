//
// Created by asphox on 10/10/18.
//

#ifndef TIM561_TELEGRAM_H
#define TIM561_TELEGRAM_H

#include <iostream>
#include <vector>
#include <string>

/**
 * \struct TelegramScan
 * \brief Contains and parse received data
 * \author SOTON "Asphox" Dylan
 * \contact dylan.soton@telecom-sudparis.eu
 */
struct TelegramScan
{
    /**
     * \public
    * \enum ID
    * \brief Position of the information in the received data
    */
    enum ID
    {
        COMMAND_TYPE=0,
        COMMAND=1,
        VERSION=2,

        DEVICE_NUMBER=0,
        SERIAL_NUMBER=1,
        DEVIS_STATUS=2,  //(2-3)
        TELEGRAM_COUNTER=4,
        SCAN_COUNTER=5,
        TIME_SINCE_START=6,
        TIME_TRANSMISSION=7,
        INPUT_STATUS=8,//(8-9)
        OutPUT_STATUS=10,//(10-11)
        RESERVE=12,
        SCAN_FREQ=13,
        MES_FREQ=14,
        CODE_OUTPUT=15,//(15,16)
        NUM_OUTPUT=17,
        CONV_FACTOR = 18,
        CONV_OFFSET =19,
        START_ANGLE =20,
        SCALE_FACTOR=21,
        COUNT =22,  //843-23=820  32B=811
        DATA_1=23
    };

    /**
    * \public
    * \brief Contains all parts of the received message
    */
    std::vector<std::string> part;

    const std::string& getCommandType() const;
    const std::string& getCommand() const;
    uint16_t getVersion() const;
    uint16_t getDeviceNumber() const;
    uint32_t getSerialNumber() const;
    uint8_t  getDeviceStatus() const;
    uint16_t getTelegramCounter() const;
    uint16_t getScanCounter() const;
    uint32_t getTimeSinceStartup() const;
    uint32_t getTimeTransmission() const;
    uint32_t getScanFreq() const;
    uint32_t getMesFreq() const;
    float    getScaleFactor() const;
    uint32_t getStartAngle() const;
};

#endif //TIM561_TELEGRAM_H
