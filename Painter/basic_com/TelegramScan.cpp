//
// Created by asphox on 10/10/18.
//

#include "basic_com/TelegramScan.h"

const std::string& TelegramScan::getCommandType() const
{
    return part[COMMAND_TYPE];
}

const std::string& TelegramScan::getCommand() const
{
    return part[COMMAND];
}

uint16_t TelegramScan::getVersion() const
{
    return std::stoul(part[VERSION],nullptr,16);
}

uint16_t TelegramScan::getDeviceNumber() const
{
    return std::stoul(part[DEVICE_NUMBER],nullptr,16);
}

uint32_t TelegramScan::getSerialNumber() const
{
    return std::stoul(part[SERIAL_NUMBER],nullptr,16);
}

uint16_t  TelegramScan::getDeviceStatus() const
{
    return std::stoul(part[DEVIS_STATUS],nullptr,16)*256+std::stoul(part[DEVIS_STATUS+1],nullptr,16);
}

uint16_t TelegramScan::getTelegramCounter() const
{
    return std::stoul(part[TELEGRAM_COUNTER],nullptr,16);
}

uint16_t TelegramScan::getScanCounter() const
{
    return std::stoul(part[SCAN_COUNTER],nullptr,16);
}

uint32_t TelegramScan::getTimeSinceStartup() const
{
    return std::stoul(part[TIME_SINCE_START],nullptr,16);
}

uint32_t TelegramScan::getTimeTransmission() const
{
    return std::stoul(part[TIME_TRANSMISSION],nullptr,16);
}

uint16_t TelegramScan::getInputStatus()const
{
    return std::stoul(part[INPUT_STATUS],nullptr,16)*256+std::stoul(part[INPUT_STATUS+1],nullptr,16);
}

uint16_t TelegramScan::getOutputStatus()const
{
    return std::stoul(part[OutPUT_STATUS],nullptr,16)*256+std::stoul(part[OutPUT_STATUS+1],nullptr,16);
}

uint32_t TelegramScan::getScanFreq() const
{
    return std::stoul(part[SCAN_FREQ],nullptr,16);
}

uint32_t TelegramScan::getMesFreq() const
{
    return std::stoul(part[MES_FREQ],nullptr,16);
}

uint16_t TelegramScan::getCodeOutput()const
{
    return std::stoul(part[CODE_OUTPUT],nullptr,16)*256+std::stoul(part[CODE_OUTPUT+1],nullptr,16);
}

const std::string& TelegramScan::getNumOutput()const
{
    return part[NUM_OUTPUT];
}


uint32_t TelegramScan::getConvFactor() const
{
    return std::stoul(part[CONV_FACTOR],nullptr,16);
}

uint32_t TelegramScan::getConvOffset() const
{
    return std::stoul(part[CONV_OFFSET],nullptr,16);
}

uint32_t TelegramScan::getStartAngle() const
{
    return std::stoul(part[START_ANGLE],nullptr,16);
}

float TelegramScan::getScaleFactor() const
{
    uint32_t tmp = std::stoul(part[SCALE_FACTOR],nullptr,16);
    std::cout << tmp << std::endl;
    return *(reinterpret_cast<float*>(&tmp));
}

uint16_t TelegramScan::getCount() const
{
    return std::stoul(part[COUNT],nullptr,16);
}

