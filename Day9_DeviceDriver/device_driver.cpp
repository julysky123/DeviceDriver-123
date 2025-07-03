#include "device_driver.h"
#include "device_driver_exception.h"

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{
}

int DeviceDriver::read(long address)
{
    int firstReadValue = m_hardware->read(address);
    for (int tryCount = 2; tryCount <= MEMORY_READ_COUNT; tryCount++){
        int value = m_hardware->read(address);
        if(isSame(firstReadValue, value)) continue;
        throw ReadFailException("The hardware returns an inconsistent value.\n");
    }
    return firstReadValue;
}

void DeviceDriver::write(long address, int data)
{
    // TODO: implement this method
    m_hardware->write(address, (unsigned char)data);
}

bool DeviceDriver::isSame(char first, char second) {
    return first == second;
}