#include "device_driver.h"
#include "device_driver_exception.h"

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{
}

int DeviceDriver::read(long address)
{
    int result = static_cast<int>(m_hardware->read(address));
    checkReadPostCondition(result, address);
    return result;
}

void DeviceDriver::checkReadPostCondition(int result, long address)
{
    static constexpr int TEST_READ_COUNT = 4;
    for (int tryCount = 0; tryCount < TEST_READ_COUNT; tryCount++) {
        int testValue = static_cast<int>(m_hardware->read(address));
        if (result == testValue) continue;
        throw ReadFailException("The hardware returns an inconsistent value.\n");
    }
}

void DeviceDriver::write(long address, int data)
{
    checkWritePreCondition(address);
    m_hardware->write(address, static_cast<unsigned char>(data));
}

void DeviceDriver::checkWritePreCondition(long address)
{
    static constexpr unsigned int EMPTY = 0xFF;
    if (read(address) != EMPTY) {
        throw WriteFailException("There's something already written in memory.\n");
    }
}

